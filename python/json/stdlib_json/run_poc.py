#!/usr/bin/env python
# coding: utf-8
########################################################################################################################
#
# Cette POC montre l'utilisation du module json :
#   https://docs.python.org/2/library/json.html
#
# Parmi les types de base :
#   - seuls les sets ne sont pas directement serializable
#   - les tuples sont sérialisés en Array (et donc déserializés en list, et non en tuple).
#   - les bytestr ont l'air d'être toutes décodées en unicode.
#
# TODO : faire des tests sur la serialization/deserialization des date/time/datetime.
#
########################################################################################################################

import json
from pprint import pprint
import sys ; sys.dont_write_bytecode = True

########################################################################################################################

class MyClass:
    def __init__(self):
        self.a = "a"
        self.b = "b"
        self.c = "c"

data = {
        'bytestr': "Paris",
        'unicode': u"Cæn",
        'int': 42,
        'float': 3.1416,
        'None': None,
        'True': True,
        'False': False,
        'list': [42, 43, "quarante-quatre"],
        'dict': {"fils": "Luke", "fille": "Leia", "neveu": None},
        }

print("PYTHON DATA TO ENCODED :")
pprint(data)

print("")
print("JSON DATA :")
encoded = json.dumps(data)
print(encoded)

print("")
print("DECODED PYTHON DATA :")
decoded = json.loads(encoded)
pprint(decoded)

print("")
print("decoded == encoded ?")
print(decoded == encoded)
for key in data:
    print("\tdata[{key:7}] == decoded[{key:7}] ? {result}".format(key=key, result=(data[key] == decoded[key])))

########################################################################################################################
# Special cases :
########################################################################################################################

# Tuples are encoded as Arrays (thus decoded as lists) :
who = ("Link", "Zelda", "Ganondorf")
print("")
print("TUPLE ENCODING+DECODING == LIST")
pprint(who)
pprint(json.loads(json.dumps(who)))

# Set are not directly json-serializable :
print("")
print("ERRORS :")
try:
    json.dumps(set([1,2,3]))
except TypeError, e:
    print("\t{}".format(e))

# Objects are not directly json-serializable :
try:
    json.dumps(MyClass())
except TypeError, e:
    print("\t{}".format(e))

########################################################################################################################
# Fin du fichier.
########################################################################################################################
