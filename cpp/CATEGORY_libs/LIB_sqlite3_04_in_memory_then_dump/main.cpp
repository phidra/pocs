#include <sqlite3.h>

#include <chrono>
#include <filesystem>
#include <iostream>
#include <vector>

void print_poc_description() {
    std::cout << R"DELIM(
CE QUE MONTRE CETTE POC = l'utilisation d'une DB in-memory + son dump sur disque.

TL;DR :
    passer le filename ":memory:" ouvre une DB en mémoire
    il existe des fonctions sqlite3_backup_init/backup_step/backup_finish pour cloner une DB dans une autre

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

void dump_to_disk(sqlite3* db, std::filesystem::path outfile) {
    // see https://www.sqlite.org/c3ref/backup_finish.html  and  https://www.sqlite.org/backup.html
    sqlite3* on_disk_db;
    sqlite3_open(outfile.c_str(), &on_disk_db);
    auto src_db_name = "main";
    auto dst_db_name = "main";
    sqlite3_backup* backup = sqlite3_backup_init(on_disk_db, dst_db_name, db, src_db_name);
    sqlite3_backup_step(backup, -1);
    sqlite3_backup_finish(backup);
    sqlite3_close(on_disk_db);
}

int main(int argc, char** argv) {
    print_poc_description();

    if (argc < 2) {
        std::cout << "ERROR> missing database name" << std::endl;
        return 1;
    }
    std::string database_name(argv[1]);

    DbWrapper wrapper(":memory:");  // pour le moment, la DB est en mémoire

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

    // on a maintenant 100k lignes dans notre DB in-memory, on va la dumper sur le disque :
    std::cout << "Dumping in-memory DB to : " << database_name << std::endl;
    dump_to_disk(wrapper.db, database_name);

    return 0;
}
