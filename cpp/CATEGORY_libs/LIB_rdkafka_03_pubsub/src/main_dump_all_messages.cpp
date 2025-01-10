#include <librdkafka/rdkafkacpp.h>

#include <csignal>
#include <iostream>
#include <string>

static volatile sig_atomic_t run = 1;

static void sigterm([[maybe_unused]] int sig) {
    run = 0;
}

void display_message(RdKafka::Message& message) {
    std::cout << "[MSG offset=" << message.offset() << "] " << std::endl;
    ;
    if (message.key()) {
        std::cout << "    KEY = " << *message.key() << std::endl;
    }
    RdKafka::Headers const* headers = message.headers();
    if (headers) {
        std::vector<RdKafka::Headers::Header> headers_vec = headers->get_all();
        for (RdKafka::Headers::Header const& header : headers_vec) {
            auto header_value = (header.value() ? static_cast<char const*>(header.value()) : "NO VALUE");
            std::cout << "    HEADER = " << header.key() << " -> " << header_value << std::endl;
        }
    }
    // le payload contient le message suivi des headers -> il faut limiter la string aux N premiers caractères :
    auto message_value = std::string(static_cast<char const*>(message.payload()), static_cast<int>(message.len()));
    std::cout << "    MESSAGE = " << message_value << std::endl;
}

bool msg_consume(RdKafka::Message* message) {
    // Le seul cas où on arrête de consommer les messages, c'est s'il n'y en a plus :
    if (message->err() == RdKafka::ERR__PARTITION_EOF) {
        return true;
    }

    // On n'a pas reçu de réponse dans le temps imparti (mais ça ne veut pas dire qu'il n'y a plus de messages) :
    if (message->err() == RdKafka::ERR__TIMED_OUT) {
        return false;
    }

    // On a reçu un message :
    if (message->err() == RdKafka::ERR_NO_ERROR) {
        display_message(*message);
        return false;
    }

    // Tous les autres cas sont des erreurs :
    std::cerr << "ERROR> Consume failed: " << message->errstr() << std::endl;
    exit(1);
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

    tconf->set("retention.ms", "3000", errstr);

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
    while (run) {
        int timeout_ms = 100;
        RdKafka::Message* msg = consumer->consume(topic, partition, timeout_ms);
        bool should_stop = msg_consume(msg);
        if (should_stop) {
            // ce consumer est dédié à la lecture des messages existants -> dès qu'il n'y en a plus, on arrête
            run = 0;
            std::cout << std::endl;
            std::cout << "*** No more messages -> exiting... ***" << std::endl;
            std::cout << "*** No more messages -> exiting... ***" << std::endl;
            std::cout << "*** No more messages -> exiting... ***" << std::endl;
        }
        delete msg;
        int no_timeout = 0;
        consumer->poll(no_timeout);
    }

    consumer->stop(topic, partition);

    int poll_timeout_ms = 100;
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
    int wait_destroyed_timeout_ms = 100;
    RdKafka::wait_destroyed(wait_destroyed_timeout_ms);

    return 0;
}
