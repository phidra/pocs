#!/usr/bin/env python3

import sys

import addressbook_pb2


def add_empty_person(my_super_book):
    my_super_book.people.add()


def add_stub_person(my_super_book):
    # "add()" pushes a new empty object in the vector, and returns it :
    new_person = my_super_book.people.add()

    # we can now fill this new object :
    new_person.id = 1
    new_person.name = "Luke"
    new_person.email = "luke@darkside.com"

    # similarly, we can define some phones :
    phone1 = new_person.phones.add()
    phone1.number = "01.23.45.67.89"
    phone1.type = addressbook_pb2.Person.HOME
    phone2 = new_person.phones.add()
    phone2.number = "11.22.33.44.55"
    phone2.type = addressbook_pb2.Person.MOBILE


def write_db(output_file: str):
    # an empty address book :
    my_super_book = addressbook_pb2.AddressBook()

    add_stub_person(my_super_book)
    add_empty_person(my_super_book)

    # here, our address book contains one object, we dump it :
    with open(output_file, "wb") as f:
        f.write(my_super_book.SerializeToString())


if __name__ == "__main__":
    output_file = sys.argv[1]
    print("WRITER  serializing an object to {}".format(output_file))
    write_db(output_file)
    print("WRITER  done")
