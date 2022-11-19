import asyncio
import logging

from aio_pika import Message, connect
from aio_pika.abc import AbstractIncomingMessage


def fib(n: int) -> int:
    if n == 0:
        return 0
    elif n == 1:
        return 1
    else:
        return fib(n - 1) + fib(n - 2)


async def main() -> None:
    # Perform connection
    connection = await connect("amqp://guest:guest@localhost/")

    # Creating a channel
    channel = await connection.channel()
    exchange = channel.default_exchange

    # Declaring queue
    queue = await channel.declare_queue("rpc_queue")

    print(" [x] Awaiting RPC requests")

    # Start listening the queue with name 'hello'
    # NDM : pas hyper-clair ce que sont queue.iterator et message.process, mais je vais supposer que c'est la façon
    #       canonique d'itérer sur les messages en attente...
    async with queue.iterator() as qiterator:
        message: AbstractIncomingMessage
        async for message in qiterator:
            try:
                async with message.process(requeue=False):
                    assert message.reply_to is not None

                    n = int(message.body.decode())

                    print(f" [.] fib({n})")
                    response = str(fib(n)).encode()

                    # NDM : le serveur renvoie la réponse sur la queue passée en "reply_to" du message entrant, et sette
                    #       le "correlation_id" de sa réponse à celle du message entrant.
                    await exchange.publish(
                        Message(
                            body=response,
                            correlation_id=message.correlation_id,
                        ),
                        routing_key=message.reply_to,
                    )
                    print("Request complete")
            except Exception:
                logging.exception("Processing error for message %r", message)


if __name__ == "__main__":
    description = """
    CE QUE FAIT LA POC :
        c'est l'exemple de RPC de la doc aio_pika :
            https://aio-pika.readthedocs.io/en/8.2.5/rabbitmq-tutorial/6-rpc.html#summary

    FONCTIONNEMENT DU SERVEUR :
        le serveur se contente d'écouter tous les messages (=requêtes) sur une queue, de calculer fib(n), et de publier
        sa réponse sur la queue indiquée par le message (message.reply_to) et en associant le correlation_id indiqué par
        le message (message.correlation_id).

    FONCTIONNEMENT DU CLIENT :
        le fait d'utiliser un client (plutôt qu'un code one-shot) sert principalement à mutualiser la queue de réponse
        à l'initialisation du client, outre la connexion au serveur, le client crée sa queue de réponse, et l'écoute, en
        associant à cette queue de réponse une callback 'on_response' (c'est non-bloquant)

        pour chaque requête qu'il a envoyée au serveur, le client a créé une future qui est en attente de la réponse
        le client maintient donc une liste des futures en attente de réponse, en les identifiant par leur correlation_id

        la callback 'on_response' parse le correlation_id de la réponse, retrouve la future matchant ce correlation_id,
        et la débloque en lui settant le résultat reçu

        plus tard, asyncio reprendra donc l'exécution de 'await call(...)' pour renvoyer le résultat...

    NOTE :
        il faut un broker RMQ sur la machine :
            systemctl start rabbitmq-server
    """
    print(description)
    asyncio.run(main())
