#include <iostream>
#include <fstream>

#include "addressbook.pb.h"
#include <google/protobuf/util/json_util.h>


using namespace std;

void add_empty_person(tutorial::AddressBook& book) {
    book.add_people();
}

void display_person(tutorial::Person const& person) {
    cout << "Person #" << person.id() << endl;
    cout << "    Name = '" << person.name() << "'" << endl;
    if (person.has_email()) {
        cout << "    email = " << person.email() << endl;
    }
    cout << "    nb of phones = " << person.phones_size() << endl;
    for (int j = 0; j != person.phones_size(); ++j) {
        const tutorial::Person::PhoneNumber& phone_number = person.phones(j);
        switch (phone_number.type()) {
            case tutorial::Person::MOBILE:
                cout << "        (mobile) " << phone_number.number() << endl;
                break;
            case tutorial::Person::HOME:
                cout << "        (home) " << phone_number.number() << endl;
                break;
            case tutorial::Person::WORK:
                cout << "        (work) " << phone_number.number() << endl;
                break;
        }
    }
}

int main(int argc, char* argv[]) {
    string filename = argv[1];
    cout << "unserializing from : " << filename << endl;
    ifstream in(filename);

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    tutorial::AddressBook book;
    book.ParseFromIstream(&in);

    cout << "Nombre de personnes dans le book : " << book.people_size() << endl;


    for (int i = 0; i < book.people_size(); i++) {
        const tutorial::Person& person = book.people(i);
        display_person(person);
    }

    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}
