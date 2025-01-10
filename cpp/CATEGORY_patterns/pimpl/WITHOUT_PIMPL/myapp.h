#pragma once

#include <string>

// IMPORTANT : la dépendance à la lib est publique !
// (et sans pimpl, on n'a pas le choix, car MyApp a besoin d'utiliser lib::DataRepoConnection comme membre...)
#include "repo.h"

struct MyApp {
    MyApp();   // nécessaire pour setup la connexion
    ~MyApp();  // nécessaire pour destroy la connexion

    int get_age(std::string first_name, std::string last_name);

   private:
    // on veut utiliser la même connexion pour tous les appels aux méthodes de la classe
    // il nous faut donc un membre qui stocke la connexion
    lib::DataRepoConnection repo_connection;
};
