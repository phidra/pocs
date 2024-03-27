#include <google/protobuf/util/json_util.h>

#include <iostream>

#include "addressbook.pb.h"

using namespace std;

int main(void) {
    cout << "BEFORE SERIALIZATION" << endl;

    GOOGLE_PROTOBUF_VERIFY_VERSION;
    tutorial::AddressBook address_book;

    auto new_person_ptr = address_book.add_people();
    new_person_ptr->set_id(42);
    *new_person_ptr->mutable_name() = "Luke Skywalker";
    new_person_ptr->set_email("luke@etoilenoire.com");

    tutorial::Person::PhoneNumber* phone1 = new_person_ptr->add_phones();
    phone1->set_number("01.23.45.67.89");
    phone1->set_type(tutorial::Person::HOME);

    tutorial::Person::PhoneNumber* phone2 = new_person_ptr->add_phones();
    phone2->set_number("00.11.22.33.44");
    phone2->set_type(tutorial::Person::WORK);

    cout << "AFTER SERIALIZATION" << endl;

    string json_string;
    google::protobuf::util::MessageToJsonString(address_book, &json_string);
    cout << "JSON STRING = " << endl;
    cout << json_string << endl;

    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}
