#!/usr/bin/env python3

import sys

import addressbook_pb2
from google.protobuf import json_format
from google.protobuf import descriptor


def display(person):
    print(f"Person #{person.id}")
    print(f"    Name = '{person.name}'")
    if person.HasField('email'):
        print(f"    email = {person.email}")
        print(f"    nb of phones = {len(person.phones)}")
    for phone_number in person.phones:
        if phone_number.type == addressbook_pb2.Person.PhoneType.MOBILE:
            print(f"         (mobile) {phone_number.number}")
        elif phone_number.type == addressbook_pb2.Person.PhoneType.HOME:
            print(f"         (home)   {phone_number.number}")
        elif phone_number.type == addressbook_pb2.Person.PhoneType.WORK:
            print(f"         (work)   {phone_number.number}")


def display_json(person):
    json_string = json_format.MessageToJson(person)
    print(json_string)


def display_structure(obj):
    # see https://googleapis.dev/python/protobuf/latest/google/protobuf/descriptor.html#google.protobuf.descriptor.FieldDescriptor  # noqa
    LABEL_TO_STR = {
        descriptor.FieldDescriptor.LABEL_OPTIONAL: "optional",
        descriptor.FieldDescriptor.LABEL_REQUIRED: "required",
        descriptor.FieldDescriptor.LABEL_REPEATED: "repeated",
    }

    TYPE_TO_STR = {
        descriptor.FieldDescriptor.TYPE_BOOL: "bool",
        descriptor.FieldDescriptor.TYPE_BYTES: "bytes",
        descriptor.FieldDescriptor.TYPE_DOUBLE: "double",
        descriptor.FieldDescriptor.TYPE_ENUM: "enum",
        descriptor.FieldDescriptor.TYPE_FIXED32: "fixed32",
        descriptor.FieldDescriptor.TYPE_FIXED64: "fixed64",
        descriptor.FieldDescriptor.TYPE_FLOAT: "float",
        descriptor.FieldDescriptor.TYPE_GROUP: "group",
        descriptor.FieldDescriptor.TYPE_INT32: "int32",
        descriptor.FieldDescriptor.TYPE_INT64: "int64",
        descriptor.FieldDescriptor.TYPE_MESSAGE: "message",
        descriptor.FieldDescriptor.TYPE_SFIXED32: "sfixed32",
        descriptor.FieldDescriptor.TYPE_SFIXED64: "sfixed64",
        descriptor.FieldDescriptor.TYPE_SINT32: "sint32",
        descriptor.FieldDescriptor.TYPE_SINT64: "sint64",
        descriptor.FieldDescriptor.TYPE_STRING: "string",
        descriptor.FieldDescriptor.TYPE_UINT32: "uint32",
        descriptor.FieldDescriptor.TYPE_UINT64: "uint64",
    }
    for field_descriptor, field_value in obj.ListFields():
        print("FIELD :")
        print(f"    NAME  = '{field_descriptor.name}'")
        print(f"    TAG   = {field_descriptor.number}")
        print(f"    TYPE  = <{TYPE_TO_STR[field_descriptor.type]}>")
        print(f"    LABEL = <{LABEL_TO_STR[field_descriptor.label]}>")
        print(f"    VALUE = {field_value}")


def read_db(input_file: str):

    # an empty address book :
    my_super_book = addressbook_pb2.AddressBook()

    # unserializing file into this object :
    with open(input_file, "rb") as f:
        my_super_book.ParseFromString(f.read())

    print("CONTENT :")
    for person in my_super_book.people:
        display(person)
    print("")

    # WOW, we can display as json :
    print("FOR THE LULZ, CONTENT AS JSON : ")
    for person in my_super_book.people:
        display_json(person)
    print("")

    # we can inspect an object :
    print("Introspection : what is the structure of the first person in the book :")
    first_person = my_super_book.people[0]
    display_structure(first_person)
    print("")


if __name__ == "__main__":
    input_file = sys.argv[1]
    print("READER  unserializing from {}".format(input_file))
    read_db(input_file)
    print("READER  done")
