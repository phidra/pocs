#include <librdkafka/rdkafkacpp.h>

#include <iostream>
#include <string>

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    std::string errstr;

    RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    [[maybe_unused]] RdKafka::Conf* tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

    std::list<std::string>* global_conf_dump = conf->dump();
    std::cout << "# Global config" << std::endl;
    for (std::list<std::string>::iterator it = global_conf_dump->begin(); it != global_conf_dump->end();) {
        std::cout << *it << " = ";
        it++;
        std::cout << *it << std::endl;
        it++;
    }
    std::cout << std::endl;

    std::list<std::string>* topic_conf_dump = conf->dump();
    std::cout << "# Topic config" << std::endl;
    for (std::list<std::string>::iterator it = topic_conf_dump->begin(); it != topic_conf_dump->end();) {
        std::cout << *it << " = ";
        it++;
        std::cout << *it << std::endl;
        it++;
    }
    std::cout << std::endl;

    // cf. https://docs.confluent.io/platform/current/installation/configuration/topic-configs.html#retention-ms

    // auto tconf_result = tconf->set("retention.ms", "3000", errstr);
    // if (tconf_result != RdKafka::Conf::CONF_OK) {
    //     std::cerr << "UNABLE TO SET tconf retention.ms :" << errstr << std::endl;
    //     exit(1);
    // }
    // auto conf_result = conf->set("retention.ms", "3000", errstr);
    // if (conf_result != RdKafka::Conf::CONF_OK) {
    //     std::cerr << "UNABLE TO SET conf retention.ms !" << std::endl;
    //     exit(1);
    // }

    return 0;
}
