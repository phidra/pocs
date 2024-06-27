#include <sqlite3.h>

#include <filesystem>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::pair;
using std::string;
using std::vector;

void print_poc_description() {
    cout << R"DELIM(
CE QUE MONTRE CETTE POC = les bases pour utiliser la lib sqlite3 pour créer/lire une DB.

- ce qu'il faut pour créer et supprimer une DB
- ce qu'il faut pour passer une requête qui ne retourne pas de résultats
- ce qu'il faut pour passer une requête qui retourne des résultats

C'est un peu lourd, mais quand on appelle sqlite3_exec, pour lire les résultats renvoyés par la requête (par exemple
ce que renvoie une SELECT * FROM mytable), il faut passer une callback qui sera appelée par sqlite3.
Comment on peut passer un argument custom à cette callback, on peut lui faire (comme dans cette POC) remplir un
vector de strings avec les résultats.
Même si on la passe, la callback ne sera pas appelée par sqlite3 pour des requêtes qui ne renvoient pas de résultats,
comme CREATE TABLE.
Attention que la callback est là pour lire les DONNÉES retournées par une requête de type SELECT, mais que même sans
callback, il faut vérifier le return-code pour savoir s'il y a des erreurs.

)DELIM";
    cout << endl;
}

struct DbWrapper {
    DbWrapper(std::filesystem::path db_path) : db{nullptr} {
        int has_error = sqlite3_open(db_path.c_str(), &db);
        if (has_error) {
            std::ostringstream oss;
            oss << "ERROR> can't open database: " << sqlite3_errmsg(db) << "\n";
            throw std::runtime_error(oss.str());
        }
    }
    DbWrapper(DbWrapper&& other) {
        db = other.db;
        other.db = nullptr;
    }
    ~DbWrapper() {
        if (db) {
            sqlite3_close(db);
        }
    }
    sqlite3* db;
};

void make_non_returning_query(sqlite3* db, string sql_query) {
    char* zErrMsg = 0;
    int return_code = sqlite3_exec(db, sql_query.c_str(), nullptr, nullptr, &zErrMsg);
    if (return_code != SQLITE_OK) {
        string error{sqlite3_errmsg(db)};
        sqlite3_free(zErrMsg);
        throw std::runtime_error(error);
    }
}

using QueryResults = vector<vector<pair<string, string>>>;

static int read_results_callback(void* results_as_void, int argc, char** argv, char** azColName) {
    QueryResults& results = *(static_cast<QueryResults*>(results_as_void));

    vector<pair<string, string>> this_line_results;
    for (int col_index = 0; col_index < argc; col_index++) {
        this_line_results.push_back({string{azColName[col_index]}, string{argv[col_index] ? argv[col_index] : "NULL"}});
    }

    results.push_back(this_line_results);
    return 0;
}

QueryResults make_returning_query(sqlite3* db, string sql_query) {
    char* zErrMsg = 0;
    QueryResults to_return{};
    int return_code = sqlite3_exec(db, sql_query.c_str(), read_results_callback, &to_return, &zErrMsg);
    if (return_code != SQLITE_OK) {
        string error{sqlite3_errmsg(db)};
        sqlite3_free(zErrMsg);
        throw std::runtime_error(error);
    }
    return to_return;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    DbWrapper wrapper("NOGIT_skywalkers.db");

    vector<string> populate_statements{
        string("CREATE TABLE persons (name VARCHAR(256), age SMALLINT)"),
        string("INSERT INTO persons (name, age) values ('Luke', 40)"),
        string("INSERT INTO persons (name, age) values ('Leia', 40)"),
    };
    for (auto statement : populate_statements) {
        make_non_returning_query(wrapper.db, statement.c_str());
    }

    auto lines = make_returning_query(wrapper.db, "SELECT * FROM persons");
    cout << "Content of table :" << endl;
    for (auto line : lines) {
        for (auto const& [col_name, col_value] : line) {
            cout << "    " << col_name << " = " << col_value << endl;
        }
        cout << "    ----" << endl;
    }
    cout << endl;

    make_non_returning_query(wrapper.db, "DELETE FROM persons");
    make_non_returning_query(wrapper.db, "DROP TABLE persons");

    return 0;
}
