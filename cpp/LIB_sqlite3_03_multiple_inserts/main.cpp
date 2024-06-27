#include <sqlite3.h>

#include <chrono>
#include <filesystem>
#include <iostream>
#include <vector>

void print_poc_description() {
    std::cout << R"DELIM(
CE QUE MONTRE CETTE POC = l'insertion de nombreuses lignes dans une DB sqlite.

IMPORTANT = on peut enchaîner plusieurs sqlite3_exec("INSERT INTO") sans s'embêter à réutiliser un prepared_statement.
En revanche, il est CRUCIAL de wrapper ses statements dans une TRANSACTION !

Chiffres sur mon vieux PC :
- si j'insère en me contentant d'enchaîner les sqlite3_exec("INSERT INTO ...") séquentiellement
- l'insertion de 100 lignes SANS transaction prend ~5.5s.
- l'insertion de 100.000 lignes AVEC transaction prend ~600 ms.

CONCLUSION : "oublier" de wrapper ses insert dans une transaction est donc des millions de fois plus lent...
Note : cette surlenteur provient d'une transaction implicite à chaque INSERT, désactivé si on explicite la transaction :
https://www.sqlite.org/lang_transaction.html

PERFS> ce post a plein de tweaks pour améliorer les perfs de sqlite3 en insertion :
    https://stackoverflow.com/questions/1711631/improve-insert-per-second-performance-of-sqlite
    --------------------------------------------------------------------------------
    (par exemple, réutiliser un statement en le bindant avec d'autres valeurs sera encore plus rapide que de construire
    des strings ad-hoc stockant la requête SQL, mais je ne m'embête pas avec ça pour cette POC)

)DELIM";
    std::cout << std::endl;
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

void make_non_returning_query(sqlite3* db, std::string sql_query) {
    char* zErrMsg = 0;
    int return_code = sqlite3_exec(db, sql_query.c_str(), nullptr, nullptr, &zErrMsg);
    if (return_code != SQLITE_OK) {
        std::string error{sqlite3_errmsg(db)};
        sqlite3_free(zErrMsg);
        throw std::runtime_error(error);
    }
}

int main(int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    if (argc < 2) {
        std::cout << "ERROR> missing database name" << std::endl;
        return 1;
    }

    std::string database_name(argv[1]);
    std::cout << "Using database = " << database_name << std::endl;
    DbWrapper wrapper(database_name);

    make_non_returning_query(wrapper.db, "DROP TABLE IF EXISTS persons");
    make_non_returning_query(wrapper.db, "CREATE TABLE persons (name TEXT, age SMALLINT)");

    auto before = std::chrono::steady_clock::now();

    make_non_returning_query(wrapper.db, "BEGIN TRANSACTION;");  // <----- ATTENTION : ceci est primordial !!!
    constexpr const size_t max = 100 * 1000;
    for (size_t idx = 0; idx < max; ++idx) {
        make_non_returning_query(wrapper.db,
                                 std::string("INSERT INTO persons (name, age) values ('name") + std::to_string(idx) +
                                     "', " + std::to_string(idx) + ");");
    }
    make_non_returning_query(wrapper.db, "COMMIT;");

    auto after = std::chrono::steady_clock::now();
    auto measured_ms = std::chrono::duration_cast<std::chrono::milliseconds>(after - before).count();
    std::cout << "Insertion of " << max << " rows took : " << measured_ms << " ms" << std::endl;

    return 0;
}
