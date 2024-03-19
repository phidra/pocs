#include "myapp.h"

// IMPORTANT : grâce à pimpl, la dépendance à la lib est privée dans le cpp.
#include "repo.h"

struct MyApp::PImpl {
    PImpl() : repo_connection{lib::connect()} {}
    ~PImpl() { lib::disconnect(repo_connection); }
    lib::DataRepoConnection repo_connection;
};

MyApp::MyApp() : pimpl{new MyApp::PImpl()} {}
MyApp::~MyApp() {}

int MyApp::get_age(std::string first_name, std::string last_name) {
    auto [personal_info, phone_number, address, company, facebook_data] =
        lib::get_all_about_this_person(pimpl->repo_connection, first_name, last_name);
    return personal_info.age;
}
