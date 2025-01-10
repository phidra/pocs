from confluent_kafka import Producer


def delivery_report(err, msg):
    if err is not None:
        print(f"Erreur lors de la publication du message : {err}")
    else:
        print(f"Message publié avec succès : {msg.topic()} [{msg.partition()}] at offset {msg.offset()}")


def publish_message(bootstrap_servers, topic, message):
    producer = Producer({"bootstrap.servers": bootstrap_servers})

    producer.produce(topic, message.encode("utf-8"), callback=delivery_report)

    producer.flush()


bootstrap_servers = "localhost:9092"
topic = "TARTIFLETTE"
message = "Hello, Kafka ! This is a message from a PYTHON producer !"

publish_message(bootstrap_servers, topic, message)
