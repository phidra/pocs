#!/usr/bin/env python3

import addressbook_pb2

# NOTES :
# the code doesn't seem to comply to usual QA tools (linter, etc.)
# I had to "pip install protobuf"

person = addressbook_pb2.Person()
person.id = 1234
person.name = "John Doe"
person.email = "jdoe@example.com"
phone = person.phones.add()
phone.number = "555-4321"
phone.type = addressbook_pb2.Person.HOME

# showing that fields are constrained by what is defined in the .proto file :
try:
    person.inexisting_field = 42
    raise ValueError("ERROR : we shouldn't reach this line, we should have raised an AttributeError")
except AttributeError:
    print("OK : this line proves that I can only use the fields defined in the .proto file")

try:
    person.id = "this is a string while the field expects an int"
    raise ValueError("ERROR : we shouldn't reach this line, we should have raised an TypeError")
except TypeError:
    print("OK : this line proves that I can only use the types defined in the .proto file")

print("ALL is OK")
