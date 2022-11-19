import asyncio

from aio_pika import Message, connect


async def main() -> None:
    # Perform connection
    connection = await connect("amqp://guest:guest@localhost/")

    async with connection:
        # NDM : un channel est une "connexion logique" au sein de la "vraie" connexion TCP.
        # cf. https://www.rabbitmq.com/channels.html
        #       connections are multiplexed with channels that can be thought of as
        #       "lightweight connections that share a single TCP connection".
        channel = await connection.channel()

        queue = await channel.declare_queue("hello")

        await channel.default_exchange.publish(
            Message(b"Hello World!"),
            routing_key=queue.name,
        )

        print(" [x] Sent 'Hello World!'")


if __name__ == "__main__":
    asyncio.run(main())
