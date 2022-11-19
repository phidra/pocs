import asyncio

from aio_pika import connect
from aio_pika.abc import AbstractIncomingMessage


async def on_message(message: AbstractIncomingMessage) -> None:
    """
    on_message doesn't necessarily have to be defined as async.
    Here it is to show that it's possible.
    """
    print(" [x] Received message %r" % message)
    print("Message body is: %r" % message.body)

    print("Before sleep!")
    await asyncio.sleep(1)  # Represents async I/O operations
    print("After sleep!")

    print("")
    print(" [*] Waiting for messages. To exit press CTRL+C")


async def main() -> None:
    # Perform connection
    connection = await connect("amqp://guest:guest@localhost/")
    async with connection:
        # Creating a channel
        channel = await connection.channel()

        # Declaring queue
        queue = await channel.declare_queue("hello")

        # Start listening the queue with name 'hello'
        await queue.consume(on_message, no_ack=True)

        print(" [*] Waiting for messages. To exit press CTRL+C")
        await asyncio.Future()


if __name__ == "__main__":
    description = """
    CE QUE FAIT LA POC :
        c'est l'exemple d'introduction de la doc aio_pika :
            https://aio-pika.readthedocs.io/en/8.2.5/rabbitmq-tutorial/1-introduction.html
        on lance un receiver en tâche de fond (qui affiche les paramètres du message et son body)
        on lance un sender en foreground
        on voit les deux communiquer

    NOTE :
        il faut un broker RMQ sur la machine :
            systemctl start rabbitmq-server
    """
    print(description)
    asyncio.run(main())
