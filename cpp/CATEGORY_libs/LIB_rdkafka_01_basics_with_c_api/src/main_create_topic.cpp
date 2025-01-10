#include <librdkafka/rdkafkacpp.h>

#include <iostream>
#include <string>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Erreur : on attend le nom du topic en premier argument." << std::endl;
        std::cerr << "USAGE :  " << argv[0] << "  <topic>" << std::endl;
        return 1;
    }
    std::string brokers = "localhost:9093";  // Adresse du broker Kafka
    std::string topic = argv[1];             // Nom du topic à créer
    std::string errstr;

    // Configuration du producteur Kafka
    RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    conf->set("bootstrap.servers", brokers, errstr);

    // Création du producteur Kafka
    RdKafka::Producer* producer = RdKafka::Producer::create(conf, errstr);
    if (!producer) {
        std::cerr << "Erreur lors de la création du producteur Kafka : " << errstr << std::endl;
        delete conf;
        return 1;
    }

    // Configuration du topic
    RdKafka::Conf* topicConf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
    topicConf->set("request.required.acks", "1", errstr);

    // Création du topic
    RdKafka::Topic* rdkafka_topic = RdKafka::Topic::create(producer, topic, topicConf, errstr);
    if (!rdkafka_topic) {
        std::cerr << "Erreur lors de la création du topic Kafka : " << errstr << std::endl;
        delete producer;
        delete conf;
        delete topicConf;
        return 1;
    }

    std::cout << "Le topic '" << topic << "' a été créé avec succès." << std::endl;

    // Nettoyage des ressources
    delete rdkafka_topic;
    delete producer;
    delete conf;
    delete topicConf;

    return 0;
}
