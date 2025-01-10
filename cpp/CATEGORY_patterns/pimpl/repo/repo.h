#pragma once

#include <string>
#include <tuple>
#include <vector>

namespace lib {

struct FacebookData {};
struct Address {};
struct PhoneNumber {};
struct Company {};
struct PersonalInfo {
    std::string first_name;
    std::string last_name;
    int age;
    char sex;
    std::vector<PersonalInfo> family;
};

struct DataRepoConnection {};

// La librairie permet de récupérer beaucoup de choses depuis un repo distant :
DataRepoConnection connect();
std::tuple<PersonalInfo, PhoneNumber, Address, Company, FacebookData>
get_all_about_this_person(DataRepoConnection& connection, std::string first_name, std::string last_name);
void disconnect(DataRepoConnection& connection);

// Il faut envisager qu'on ait des dépendances complexes
// P.ex. une dépendance vers une lib genre curl pour retrouver les infos.
// Ou encore une dépendance au SDK AWS pour récupérer un fichier sur S3.

}  // namespace lib
