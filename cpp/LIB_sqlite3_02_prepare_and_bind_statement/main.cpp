#include <sqlite3.h>

#include <filesystem>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;

void print_poc_description() {
    cout << R"DELIM(
CE QUE MONTRE CETTE POC = l'utilisation de sqlite3_prepare/bind/step/finalize en remplacement de sqlite3_exec.

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

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    DbWrapper wrapper("NOGIT_skywalkers.db");

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
    std::string statement = "INSERT INTO persons (name, age) values ('Luke', 40)";
    sqlite3_stmt* prepared_statement = nullptr;
    auto return_code_prepare =
        sqlite3_prepare_v2(wrapper.db, statement.c_str(), statement.size() + 1, &prepared_statement, nullptr);
    panic_if_unexpected(return_code_prepare, SQLITE_OK, wrapper.db);

    auto return_code_step = sqlite3_step(prepared_statement);
    panic_if_unexpected(return_code_step, SQLITE_DONE, wrapper.db);

    auto return_code_finalize = sqlite3_finalize(prepared_statement);
    panic_if_unexpected(return_code_finalize, SQLITE_OK, wrapper.db);

    return 0;
}
