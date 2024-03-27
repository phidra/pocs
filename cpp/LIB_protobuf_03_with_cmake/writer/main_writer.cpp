#include <iostream>
#include <fstream>

#include "addressbook.pb.h"
#include <google/protobuf/util/json_util.h>

using namespace std;

void add_empty_person(tutorial::AddressBook& book) {
    book.add_people();
}

void add_stub_person(tutorial::AddressBook& book) {
    auto new_person_ptr = book.add_people();
    new_person_ptr->set_id(42);
    *new_person_ptr->mutable_name() = "Luke Skywalker";
    new_person_ptr->set_email("luke@etoilenoire.com");

    tutorial::Person::PhoneNumber* phone1 = new_person_ptr->add_phones();
    phone1->set_number("01.23.45.67.89");
    phone1->set_type(tutorial::Person::HOME);

    tutorial::Person::PhoneNumber* phone2 = new_person_ptr->add_phones();
    phone2->set_number("00.11.22.33.44");
    phone2->set_type(tutorial::Person::WORK);
}

int main(int argc, char* argv[]) {
    string filename = argv[1];
    cout << "serializing to : " << filename << endl;
    ofstream out(filename);

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    tutorial::AddressBook book;
    add_stub_person(book);
    add_empty_person(book);
    book.SerializeToOstream(&out);

    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}
