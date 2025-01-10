#include <sqlite3.h>

#include <filesystem>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;

void print_poc_description() {
    cout << R"DELIM(
CE QUE MONTRE CETTE POC = l'utilisation de sqlite3_prepare/bind/step/finalize en remplacement de sqlite3_exec.

On montre notamment comment préparer un statement + binder ses placeholders.
On montre aussi comment RÉUTILISER le même prepared statement avec différents bindings pour deux requêtes différentes.

Autre avantage du binding = il fait l'escaping des caractères spéciaux (' pour un TEXT)

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

void make_non_returning_query(sqlite3* db, std::string sql_query) {
    char* zErrMsg = 0;
    int return_code = sqlite3_exec(db, sql_query.c_str(), nullptr, nullptr, &zErrMsg);
    if (return_code != SQLITE_OK) {
        std::string error{sqlite3_errmsg(db)};
        sqlite3_free(zErrMsg);
        throw std::runtime_error(error);
    }
}

void panic_if_unexpected(int return_code, int expected_return_code, sqlite3* db) {
    if (return_code != expected_return_code) {
        std::string error{sqlite3_errmsg(db)};
        throw std::runtime_error(error);
    }
}

void make_prepared_statement(sqlite3_stmt* prepared_statement, sqlite3* db, std::string name, int age) {
    std::cout << "About to bind name placeholder to : " << name << std::endl;
    auto index_of_name_placeholder = 1;
    auto return_code_bind1 =
        sqlite3_bind_text(prepared_statement, index_of_name_placeholder, name.c_str(), -1, SQLITE_STATIC);
    panic_if_unexpected(return_code_bind1, SQLITE_OK, db);

    // on peut aussi laisser sqlite retrouver l'index du placeholder :
    auto index_of_age_placeholder = sqlite3_bind_parameter_index(prepared_statement, ":age");
    std::cout << "About to bind age placeholder to : " << age << std::endl;
    auto return_code_bind2 = sqlite3_bind_int(prepared_statement, index_of_age_placeholder, age);
    panic_if_unexpected(return_code_bind2, SQLITE_OK, db);

    auto return_code_step = sqlite3_step(prepared_statement);
    panic_if_unexpected(return_code_step, SQLITE_DONE, db);
}

int main(int argc, char** argv) {
    print_poc_description();

    if (argc < 2) {
        std::cout << "ERROR> missing database name" << std::endl;
        return 1;
    }

    std::string database_name(argv[1]);
    std::cout << "Using database = " << database_name << std::endl;
    DbWrapper wrapper(database_name);

    make_non_returning_query(wrapper.db, "DROP TABLE persons");
    make_non_returning_query(wrapper.db, "CREATE TABLE persons (name TEXT, age SMALLINT)");

    // cf. https://www.sqlite.org/c3ref/prepare.html
    // int sqlite3_prepare_v2(
    //   sqlite3 *db,            /* Database handle */
    //   const char *zSql,       /* SQL statement, UTF-8 encoded */
    //   int nByte,              /* Maximum length of zSql in bytes. */
    //   sqlite3_stmt **ppStmt,  /* OUT: Statement handle */
    //   const char **pzTail     /* OUT: Pointer to unused portion of zSql */
    // );
    std::string statement = "INSERT INTO persons (name, age) values (:name, :age)";
    // index des placeholders :                                      ^1     ^2
    // le statement a des "trous" = des placeholders qui commencent par ':'
    // chaque placeholder est identifié par son index, celui du plus à gauche est '1'
    // cf. https://www.sqlite.org/c3ref/bind_blob.html

    sqlite3_stmt* prepared_statement = nullptr;
    auto return_code_prepare =
        sqlite3_prepare_v2(wrapper.db, statement.c_str(), statement.size() + 1, &prepared_statement, nullptr);
    panic_if_unexpected(return_code_prepare, SQLITE_OK, wrapper.db);

    // première insertion :
    make_prepared_statement(prepared_statement, wrapper.db, "Luke", 42);

    // Ici, on réutilise la même prepared statement pour une DEUXIÈME insertion :
    sqlite3_reset(prepared_statement);
    sqlite3_clear_bindings(prepared_statement);

    // deuxième insertion :
    make_prepared_statement(prepared_statement, wrapper.db, "An'akin", 62);

    // finalement, on détruit le statement :
    auto return_code_finalize = sqlite3_finalize(prepared_statement);
    panic_if_unexpected(return_code_finalize, SQLITE_OK, wrapper.db);

    return 0;
}
