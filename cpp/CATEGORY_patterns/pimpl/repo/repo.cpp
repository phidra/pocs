#include "repo.h"

namespace lib {

constexpr char const MALE = 'M';

DataRepoConnection connect() {
    // stub
    return {};
}
std::tuple<PersonalInfo, PhoneNumber, Address, Company, FacebookData>
get_all_about_this_person(DataRepoConnection& connection, std::string first_name, std::string last_name) {
    (void)connection;
    // stub
    return {{first_name, last_name, 42, MALE, {}}, {}, {}, {}, {}};
}
void disconnect(DataRepoConnection& connection) {
    (void)connection;
}

}  // namespace lib
