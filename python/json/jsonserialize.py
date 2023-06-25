#!/usr/bin/env python
# coding: utf-8

from collections import namedtuple
import datetime
import json

# Position du problème = TODO
# État de la POC = ok pour traiter des types à part.
# Problème tout de même : les namedtuples sont considérés comme tuple.

# Avec uniquement des types json-serializables :
regular_data = [
        u"This is a unicode string",
        "This is a regular string",
        42,
        3.14,
        None,
        ]
encoded = json.dumps(regular_data)
decoded = json.loads(encoded)

print "=========== Types json-serializables :"
print regular_data
print encoded
print decoded
print regular_data == decoded
print ""


# avec des types non-json serializables :
Point = namedtuple("Point", ["x", "y"])
now = datetime.datetime.now()

noregular_data = [
        Point(0, 0),
        Point(3, 4),
        u"This is a unicode string",
        "This is a regular string",
        42,
        3.14,
        None,
        now,
        ]
def encode_special(obj):
    if isinstance(obj, datetime.datetime):
        return {
                "special_object_id": "datetime.datetime",
                "value": obj.isoformat()
                }
    if isinstance(obj, Point):
        return {
                "special_object_id": "Point",
                "value": {"x": obj.x, "y": obj.y}
                }
    raise TypeError("Unencodable object type = {}".format(type(obj).__name__))

def decode_special(obj):
    assert "special_object_id" in obj.keys()
    id_ = obj["special_object_id"]
    value = obj["value"]
    if id_ == "datetime.datetime":
        return datetime.datetime.strptime(value, "%Y-%m-%dT%H:%M:%S.%f")
    elif id_ == "Point":
        return Point(x=value["x"], y=value["y"])
    else:
        raise TypeError("Undecodable special_object_id : {}".format(id_))

encoded = json.dumps(noregular_data, default=encode_special)
decoded = json.loads(encoded, object_hook=decode_special)

print "=========== Types NON json-serializables :"
print noregular_data
print encoded
print decoded
print noregular_data == decoded
print ""





print "=========== Nouvelle façon de faire :"
from json import JSONEncoder


class MyEncoder(JSONEncoder):
    def iterencode(self, obj, _one_shot=False):
        if isinstance(obj, datetime.datetime):
            value_dict = {
                    "special_object_id": "datetime.datetime",
                    "value": obj.isoformat()
                    }
            return JSONEncoder.iterencode(self, value_dict, _one_shot)
        elif isinstance(obj, Point):
            value_dict = {
                    "special_object_id": "Point",
                    "value": {"x": obj.x, "y": obj.y}
                    }
            return JSONEncoder.iterencode(self, value_dict, _one_shot)
        else:
            return JSONEncoder.iterencode(self, obj, _one_shot)

#     def encode(self, obj):
#         print "=================== PASSAGE LA =============== pour obj", obj
#         if isinstance(obj, Point):
#             print "POINT !!!!!!!!"
#             value_dict = {
#                     "special_object_id": "Point",
#                     "value": {"x": obj.x, "y": obj.y}
#                     }
#             return JSONEncoder.encode(self, value_dict)
#         else:
#             print "ELSE !!!!!!!!"
#             return JSONEncoder.encode(self, obj)

#     def default(self, obj):
#         print "=================== PASSAGE ICI =============== pour obj", obj
#         if isinstance(obj, datetime.datetime):
#             print "DATETIME !!!!!!!!"
#             value_dict = {
#                     "special_object_id": "datetime.datetime",
#                     "value": obj.isoformat()
#                     }
#             return JSONEncoder.encode(self, value_dict)
#         elif isinstance(obj, Point):
#             print "POINT !!!!!!!!"
#             value_dict = {
#                     "special_object_id": "Point",
#                     "value": {"x": obj.x, "y": obj.y}
#                     }
#             return JSONEncoder.encode(self, value_dict)
#         else:
#             print "ELSE !!!!!!!!"
#             return JSONEncoder.encode(self, obj)
    # def encode(self, obj):
    #     print "============= ICI ===============, obj = ", obj
    #     if isinstance(obj, datetime.datetime):
    #         value_dict = {
    #                 "special_object_id": "datetime.datetime",
    #                 "value": obj.isoformat()
    #                 }
    #         return JSONEncoder.encode_dict(self, value_dict)
    #     elif isinstance(obj, Point):
    #         value_dict = {
    #                 "special_object_id": "Point",
    #                 "value": {"x": obj.x, "y": obj.y}
    #                 }
    #         return JSONEncoder.encode_dict(self, value_dict)
    #     else:
    #         return JSONEncoder.encode(self, obj)

enc = MyEncoder()
encoded = enc.encode(noregular_data)
# encoded = enc.encode({"coucou": "coco", "pouet": [1, 2 ,3], "hoho": {"youpi": [{"hihi": "haha"}, 24]}})
print encoded
