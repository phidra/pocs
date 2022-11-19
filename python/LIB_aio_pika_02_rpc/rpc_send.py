import asyncio
import uuid
from typing import MutableMapping
import sys

from aio_pika import Message, connect
from aio_pika.abc import (
    AbstractChannel,
    AbstractConnection,
    AbstractIncomingMessage,
    AbstractQueue,
)


class FibonacciRpcClient:
    connection: AbstractConnection
    channel: AbstractChannel
    callback_queue: AbstractQueue
    loop: asyncio.AbstractEventLoop

    def __init__(self) -> None:
        self.futures: MutableMapping[str, asyncio.Future] = {}
        self.loop = asyncio.get_running_loop()

    async def connect(self) -> "FibonacciRpcClient":
        self.connection = await connect(
            "amqp://guest:guest@localhost/",
            loop=self.loop,
        )
        self.channel = await self.connection.channel()
        self.callback_queue = await self.channel.declare_queue(exclusive=True)
        await self.callback_queue.consume(self.on_response)

        # NDM : cette étape est l'initialisation du client. On crée une queue anonyme (je suppose que c'est RabbitMQ qui
        #       calcule tout seul un nom de queue libre), cette queue anonyme sera utilisée comme unique queue de
        #       réponse de toutes les requêtes envoyées par ce client.
        #       (c'est surtout à ça que sert la classe RpcClient = à partager la queue de réponse)
        #       Une fois la queue créée, on lui adjoint la callback "on_response", qui "débloque" les futures (en les
        #       identifiant avec le "correlation_id" du message) et leur sette le résultat reçu du serveur.
        #       Plus tard, la future ayant été débloquée, l'IO-loop d'asyncio va pourvoir poursuivre l'exécution de la
        #       fonction 'call' ci-dessous.

        return self

    def on_response(self, message: AbstractIncomingMessage) -> None:
        if message.correlation_id is None:
            print(f"Bad message {message!r}")
            return

        future: asyncio.Future = self.futures.pop(message.correlation_id)
        future.set_result(message.body)

    async def call(self, n: int) -> int:
        correlation_id = str(uuid.uuid4())
        future = self.loop.create_future()

        self.futures[correlation_id] = future

        await self.channel.default_exchange.publish(
            Message(
                str(n).encode(),
                content_type="text/plain",
                correlation_id=correlation_id,
                reply_to=self.callback_queue.name,
            ),
            routing_key="rpc_queue",
        )

        # Cette future sera "bloquée" (en attente de réponse), jusqu'à ce que 'on_response' débloque cette future en
        # particulier, en l'identifiant avec le correlation_id du message.
        return int(await future)


async def main() -> None:
    fibonacci_rpc = await FibonacciRpcClient().connect()
    print(" [x] Requesting fib(30)")
    try:
        requested_fibo = int(sys.argv[1])
    except IndexError:
        requested_fibo = 30
    response = await fibonacci_rpc.call(requested_fibo)
    print(f" [.] Got {response!r}")


if __name__ == "__main__":
    asyncio.run(main())
