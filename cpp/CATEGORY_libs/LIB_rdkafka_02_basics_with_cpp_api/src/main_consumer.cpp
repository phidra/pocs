#include <librdkafka/rdkafkacpp.h>

#include <csignal>
#include <iostream>
#include <string>

static volatile sig_atomic_t run = 1;
static bool exit_eof = false;

static void sigterm([[maybe_unused]] int sig) {
    run = 0;
}

void msg_consume(RdKafka::Message* message) {
    RdKafka::Headers const* headers;

    switch (message->err()) {
        case RdKafka::ERR__TIMED_OUT:
            break;

        case RdKafka::ERR_NO_ERROR:
            /* Real message */
            std::cout << "\nRead msg at offset " << message->offset() << std::endl;
            if (message->key()) {
                std::cout << "Key: " << *message->key() << std::endl;
            }
            headers = message->headers();
            if (headers) {
                std::vector<RdKafka::Headers::Header> hdrs = headers->get_all();
                for (size_t i = 0; i < hdrs.size(); i++) {
                    const RdKafka::Headers::Header hdr = hdrs[i];

                    if (hdr.value() != NULL)
                        printf(" Header: %s = \"%.*s\"\n",
                               hdr.key().c_str(),
                               (int)hdr.value_size(),
                               (char const*)hdr.value());
                    else
                        printf(" Header:  %s = NULL\n", hdr.key().c_str());
                }
            }
            printf("%.*s\n", static_cast<int>(message->len()), static_cast<char const*>(message->payload()));
            break;

        case RdKafka::ERR__PARTITION_EOF:
            /* Last message */
            if (exit_eof) {
                run = 0;
            }
            break;

        case RdKafka::ERR__UNKNOWN_TOPIC:
        case RdKafka::ERR__UNKNOWN_PARTITION:
            std::cerr << "Consume failed: " << message->errstr() << std::endl;
            run = 0;
            break;

        default:
            /* Errors */
            std::cerr << "Consume failed: " << message->errstr() << std::endl;
            run = 0;
    }

    std::cout << "<Ctrl+C> suivi de <Entrée> pour arrêter le consumer" << std::endl;
}

int main(int argc, char** argv) {
    std::string brokers = "localhost";
    std::string errstr;
    if (argc < 2) {
        std::cerr << "ERROR : missing topic_str" << std::endl;
        exit(1);
    }
    std::string topic_str = argv[1];
    // int32_t partition = RdKafka::Topic::PARTITION_UA;
    int32_t partition = 0;  // NDM : ça n'a pas l'air de marcher avec PARTITION_UA...
    int64_t start_offset = RdKafka::Topic::OFFSET_BEGINNING;

    /*
     * Create configuration objects
     */
    RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    RdKafka::Conf* tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

    /*
     * Set configuration properties
     */
    conf->set("metadata.broker.list", brokers, errstr);

    signal(SIGINT, sigterm);
    signal(SIGTERM, sigterm);

    conf->set("enable.partition.eof", "true", errstr);

    /*
     * Create consumer using accumulated global configuration.
     */
    RdKafka::Consumer* consumer = RdKafka::Consumer::create(conf, errstr);
    if (!consumer) {
        std::cerr << "Failed to create consumer: " << errstr << std::endl;
        exit(1);
    }

    std::cout << "% Created consumer " << consumer->name() << std::endl;

    /*
     * Create topic handle.
     */
    RdKafka::Topic* topic = RdKafka::Topic::create(consumer, topic_str, tconf, errstr);
    if (!topic) {
        std::cerr << "Failed to create topic: " << errstr << std::endl;
        exit(1);
    }

    /*
     * Start consumer for topic+partition at start offset
     */
    RdKafka::ErrorCode resp = consumer->start(topic, partition, start_offset);
    if (resp != RdKafka::ERR_NO_ERROR) {
        std::cerr << "Failed to start consumer: " << RdKafka::err2str(resp) << std::endl;
        exit(1);
    }

    /*
     * Consume messages
     */
    while (run) {
        int timeout_ms = 1000;
        RdKafka::Message* msg = consumer->consume(topic, partition, timeout_ms);
        msg_consume(msg);
        delete msg;
        int no_timeout = 0;
        consumer->poll(no_timeout);
    }

    /*
     * Stop consumer
     */
    consumer->stop(topic, partition);

    int poll_timeout_ms = 1000;
    consumer->poll(poll_timeout_ms);

    delete topic;
    delete consumer;

    delete conf;
    delete tconf;

    /*
     * Wait for RdKafka to decommission.
     * This is not strictly needed (when check outq_len() above), but
     * allows RdKafka to clean up all its resources before the application
     * exits so that memory profilers such as valgrind wont complain about
     * memory leaks.
     */
    int wait_destroyed_timeout_ms = 5000;
    RdKafka::wait_destroyed(wait_destroyed_timeout_ms);

    return 0;
}
