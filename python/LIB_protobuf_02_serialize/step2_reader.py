#!/usr/bin/env python3

import sys

import addressbook_pb2
from google.protobuf import json_format


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

    print("FOR THE LULZ, CONTENT AS JSON : ")
    for person in my_super_book.people:
        display_json(person)
    print("")


if __name__ == "__main__":
    input_file = sys.argv[1]
    print("READER  unserializing from {}".format(input_file))
    read_db(input_file)
    print("READER  done")
