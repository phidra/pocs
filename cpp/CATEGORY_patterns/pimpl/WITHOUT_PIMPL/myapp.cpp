#include "myapp.h"

MyApp::MyApp() : repo_connection{lib::connect()} {}
MyApp::~MyApp() {
    lib::disconnect(repo_connection);
}

int MyApp::get_age(std::string first_name, std::string last_name) {
    auto [personal_info, phone_number, address, company, facebook_data] =
        lib::get_all_about_this_person(repo_connection, first_name, last_name);
    return personal_info.age;
}
