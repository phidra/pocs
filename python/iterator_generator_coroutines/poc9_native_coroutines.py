#!/usr/bin/env python3
# coding: utf-8

import asyncio
import aiohttp

"""
Initialement, je voulais faire l'équivalent des POCs précédentes, en utilisant les native-coroutines d'asyncio à la place des
generator-based coroutines. Par exemple, je voulais faire une POC où j'alternais le flot d'exécution entre l'appelant et la coroutine.

En pratique, ce n'est pas possible : les coroutines d'asyncio NE SONT PAS des coroutines génériques !
Ce sont des coroutines SPÉCIALISÉES pour faire de l'IO, au sein d'une event-loop (rôle géré par l'appelant), chargée notamment de gérer
QUAND réveiller la coroutine : suite à un select positif, ou à un dépassement de timeout.

Ainsi, les native-coroutiens sont en fait des io-coroutines, et je NE PEUX PAS les utiliser pour alterner le flot d'exécution.
Il faut voir le await non pas en tant que moyen de rendre la main (comme yield), mais comme un moyen "d'attendre" sans bloquer
que l'opération awaitée soit prête à être exécutée.

Du coup, cette POC se contente de donner un court exemple d'utilsiation d'asyncio.
"""

fra_url = "https://fr.wikipedia.org/wiki/France"
bel_url = "https://fr.wikipedia.org/wiki/Belgique"
lux_url = "https://fr.wikipedia.org/wiki/Luxembourg"


def parse_title(body):
    idx_open = body.find("<title>")
    idx_close = body.find("</title>")
    if idx_open < 0 or idx_close < 0:
        raise ValueError("unable to find <title> tags")
    return body[idx_open+7:idx_close]


async def fetch_title(session, url):
    async with session.get(url) as response:
        body = await response.text()
        return parse_title(body)


async def main():
    async with aiohttp.ClientSession() as session:
        fra_title, bel_title, lux_title = await asyncio.gather(
            fetch_title(session, fra_url),
            fetch_title(session, bel_url),
            fetch_title(session, lux_url),
        )
        print("FRANCE     = ", fra_title)
        print("BELGIQUE   = ", bel_title)
        print("LUXEMBOURG = ", lux_title)


if __name__ == '__main__':
    loop = asyncio.get_event_loop()
    loop.run_until_complete(main())
