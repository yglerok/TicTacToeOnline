#pragma once 

#include <pqxx/pqxx>

#include <iostream>
#include <mutex>
#include <string>
#include <optional>
#include <cstdlib>

class Database {
public:
    static Database* getInstance() {
        if (instance == nullptr) {
            instance = new Database();
        }
        return instance;
    }

    uint32_t findOrCreateUser(const std::string& name);
    void saveResult(uint32_t xPlayer, uint32_t oPlayer, std::optional<uint32_t> winner);

private:
    static Database* instance; 
    std::mutex mx;
    pqxx::connection cx;

    static std::string getEnvOrDefault(const char* name, const char* fallback);
    static std::string buildCxStr();

    Database() : cx(buildCxStr()) {}
    ~Database() = default;
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
};