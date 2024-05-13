#include <librdkafka/rdkafkacpp.h>

#include <csignal>
#include <iostream>
#include <string>

static volatile sig_atomic_t run = 1;

static void sigterm([[maybe_unused]] int sig) {
    run = 0;
}

class ExampleDeliveryReportCb : public RdKafka::DeliveryReportCb {
   public:
    void dr_cb(RdKafka::Message& message) {
        std::string status_name;
        switch (message.status()) {
            case RdKafka::Message::MSG_STATUS_NOT_PERSISTED:
                status_name = "NotPersisted";
                break;
            case RdKafka::Message::MSG_STATUS_POSSIBLY_PERSISTED:
                status_name = "PossiblyPersisted";
                break;
            case RdKafka::Message::MSG_STATUS_PERSISTED:
                status_name = "Persisted";
                break;
            default:
                status_name = "Unknown?";
                break;
        }
        std::cout << "Message delivery for (" << message.len() << " bytes): " << status_name << ": " << message.errstr()
                  << std::endl;
        if (message.key())
            std::cout << "Key: " << *(message.key()) << ";" << std::endl;
    }
};

class ExampleEventCb : public RdKafka::EventCb {
   public:
    void event_cb(RdKafka::Event& event) {
        switch (event.type()) {
            case RdKafka::Event::EVENT_ERROR:
                if (event.fatal()) {
                    std::cerr << "FATAL ";
                    run = 0;
                }
                std::cerr << "ERROR (" << RdKafka::err2str(event.err()) << "): " << event.str() << std::endl;
                break;

            case RdKafka::Event::EVENT_STATS:
                std::cerr << "\"STATS\": " << event.str() << std::endl;
                break;

            case RdKafka::Event::EVENT_LOG:
                fprintf(stderr, "LOG-%i-%s: %s\n", event.severity(), event.fac().c_str(), event.str().c_str());
                break;

            default:
                std::cerr << "EVENT " << event.type() << " (" << RdKafka::err2str(event.err()) << "): " << event.str()
                          << std::endl;
                break;
        }
    }
};

int main(int argc, char** argv) {
    std::string brokers = "localhost";
    std::string errstr;
    if (argc < 3) {
        std::cerr << "USAGE :" << std::endl;
        std::cerr << "    " << argv[0] << "  <TOPIC>    <LINE1>   <LINE2>   ..." << std::endl;
        std::cerr << "EXAMPLE :" << std::endl;
        std::cerr << "    " << argv[0] << "  FROMAGES   reblochon   camembert" << std::endl;
        exit(1);
    }

    // parsing des arguments :
    std::string topic_str = argv[1];
    constexpr const size_t nb_params_before_lines = 2;
    auto nb_lines = argc - nb_params_before_lines;
    std::vector<std::string> lines_to_send;

    for (size_t line_index = 0; line_index < nb_lines; ++line_index) {
        lines_to_send.emplace_back(argv[line_index + nb_params_before_lines]);
    }

    for (auto line_to_send : lines_to_send) {
        std::cout << "SENDING LINE = " << line_to_send << std::endl;
    }
    int32_t partition = RdKafka::Topic::PARTITION_UA;

    // Create configuration objects
    RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    RdKafka::Conf* tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

    // Set configuration properties
    conf->set("metadata.broker.list", brokers, errstr);

    ExampleEventCb ex_event_cb;
    conf->set("event_cb", &ex_event_cb, errstr);

    signal(SIGINT, sigterm);
    signal(SIGTERM, sigterm);

    ExampleDeliveryReportCb ex_dr_cb;

    // Set delivery report callback :
    conf->set("dr_cb", &ex_dr_cb, errstr);
    conf->set("default_topic_conf", tconf, errstr);

    // Create producer using accumulated global configuration :
    RdKafka::Producer* producer = RdKafka::Producer::create(conf, errstr);
    if (!producer) {
        std::cerr << "Failed to create producer: " << errstr << std::endl;
        exit(1);
    }

    std::cout << "% Created producer " << producer->name() << std::endl;

    // produce messages to broker :
    for (auto line_to_send : lines_to_send) {
        RdKafka::Headers* headers = RdKafka::Headers::create();
        headers->add("my header", "header value");
        headers->add("other header", "yes");

        std::string key = "DAKEY";
        RdKafka::ErrorCode resp = producer->produce(topic_str,
                                                    partition,
                                                    RdKafka::Producer::RK_MSG_COPY,  // copy payload

                                                    // value du message :
                                                    const_cast<char*>(line_to_send.c_str()),
                                                    line_to_send.size(),

                                                    // key du message :
                                                    key.c_str(),
                                                    key.size(),

                                                    // timestamp (defaults to now)
                                                    0,

                                                    // headers :
                                                    headers,
                                                    // Per-message opaque value passed to delivery report
                                                    NULL);
        if (resp != RdKafka::ERR_NO_ERROR) {
            std::cerr << "% Produce failed: " << RdKafka::err2str(resp) << std::endl;
            delete headers;  // note :  headers are automatically deleted on produce() success.
        } else {
            std::cerr << "% Produced message (" << line_to_send.size() << " bytes)" << std::endl;
        }

        producer->poll(0);
    }
    run = 1;

    while (run && producer->outq_len() > 0) {
        // NDM : ici, je suppose qu'on attends que le producer aie bien envoyé tous ses messages enqueués
        std::cerr << "Waiting for " << producer->outq_len() << std::endl;
        producer->poll(1000);
    }

    delete producer;
    delete conf;
    delete tconf;

    /*
     * Wait for RdKafka to decommission.
     * This is not strictly needed (when check outq_len() above), but
     * allows RdKafka to clean up all its resources before the application
     * exits so that memory profilers such as valgrind wont complain about
     * memory leaks.
     */
    RdKafka::wait_destroyed(5000);

    return 0;
}
