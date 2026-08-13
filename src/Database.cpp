#include "Database.h"

Database* Database::instance = nullptr;

uint32_t Database::findOrCreateUser(const std::string &name)
{
    std::lock_guard lock(mx);

    pqxx::work tx{cx};

    pqxx::result r = tx.exec_params("SELECT id FROM users WHERE name = $1", name);

    if (r.empty())
    {
        r = tx.exec_params("INSERT INTO users (name) VALUES ($1) RETURNING id", name);
    }

    tx.commit();

    return r[0]["id"].as<uint32_t>();
}

void Database::saveResult(uint32_t xPlayer, uint32_t oPlayer, std::optional<uint32_t> winner)
{
    std::lock_guard lock(mx);

    pqxx::work tx{cx};

    tx.exec_params("INSERT INTO game_results (x_player_id, o_player_id, winner_id) VALUES ($1, $2, $3)", xPlayer, oPlayer, winner);

    tx.commit();
}

std::string Database::getEnvOrDefault(const char *name, const char *fallback)
{
    const char* env = std::getenv(name);
    return env ? std::string(env) : std::string(fallback);
}

std::string Database::buildCxStr()
{
    return "host=" + getEnvOrDefault("DB_HOST", "localhost") + 
            " port=" + getEnvOrDefault("DB_PORT", "5432") + 
            " dbname=" + getEnvOrDefault("DB_NAME", "tictactoe_db") + 
            " user=" + getEnvOrDefault("DB_USER", "tictactoe") + 
            " password=" + getEnvOrDefault("DB_PASSWORD", "devpassword");
}
