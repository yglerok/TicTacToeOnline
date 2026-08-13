#include "GameManager.h"
#include "ConnectionManager.h"
#include "Database.h"

#include "nlohmann/json.hpp"

std::string statusToStr(Game::Status status)
{
    std::string str;
    switch (status)
    {
    case Game::Status::Waiting:
        str = "Waiting";
        break;
    case Game::Status::Playing:
        str = "Playing";
        break;
    case Game::Status::Finished:
        str = "Finished";
        break;
    default:
        break;
    }
    return str;
}

int main()
{
    // Game game(0, 1);
    // game.play();

    httplib::Server srv;

    // srv.Post("/", [](const httplib::Request& req, httplib::Response& res){
    //     std::string body = req.body;
    //     std::cout << "Recieved: " << body << std::endl;

    //     res.set_content("Recieved: " + body, "text/plain");
    // });

    srv.set_mount_point("/", "./public");
    // srv.Get("/", [](const httplib::Request &, httplib::Response &res) {
    //     res.set_content("", "text/html");
    // });

    srv.Post("/button-click", [](const httplib::Request &req, httplib::Response &res)
             {
        std::string body = req.body;
        std::cout << "[http] Button clicked! Message: " << body << std::endl;

        res.set_content("Button click processed! Recieved: " + body, "text/plain"); });

    srv.Get("/hi", [](const httplib::Request &, httplib::Response &res)
            { res.set_content("Hello, world!", "text/plain"); });

    // std::unordered_map<httplib::ws::WebSocket*, uint32_t> gameByWs;
    srv.WebSocket("/ws", [](const httplib::Request &req, httplib::ws::WebSocket &ws) {

        nlohmann::json j = {
                                {"action", "getName"}
                            };
                            ws.send(j.dump());

        uint32_t playerId = ConnectionManager::getInstance()->addConnection(&ws); 

        uint32_t gameId = GameManager::getInstance()->findWaitingGame();
        if (gameId == 0) {
            // Waiting game not found -> creating new
            gameId = GameManager::getInstance()->createNewGame(playerId);

            nlohmann::json j = {
                {"action", "gameCreated"},
                {"gameId", gameId},
                {"playerId", playerId}
            };
            ConnectionManager::getInstance()->sendMessage(playerId, j.dump());

        } else {
            // Waiting game found -> join it -> start it
            GameManager::getInstance()->joinGame(gameId, playerId);

            nlohmann::json j = {
                {"action", "gameJoined"},
                {"gameId", gameId},
                {"playerId", playerId}
            };
            ConnectionManager::getInstance()->sendMessage(playerId, j.dump());

            auto players = GameManager::getInstance()->getGame(gameId)->getPlayersIds();
            uint32_t currentPlayer = GameManager::getInstance()->getGame(gameId)->getCurrentPlayerId();
            j = {
                {"action", "gameStarted"},
                {"opponentId", players.second},
                {"playerSymbol", "X"},
                {"currentPlayerId", currentPlayer}
            };
            ConnectionManager::getInstance()->sendMessage(players.first, j.dump());
            j = {
                {"action", "gameStarted"},
                {"opponentId", players.first},
                {"playerSymbol", "O"},
                {"currentPlayerId", currentPlayer}
            };
            ConnectionManager::getInstance()->sendMessage(players.second, j.dump());
        }

        // Send opponent info for both

        //ws.send("Connected to game (id=" + std::to_string(gameId) + ")");

        std::string playerName;
        std::string msg;
        while (ws.read(msg)) {
            // std::cout << "[ws] id = " << playerId << " : " << msg << std::endl;
            // ws.send("echo: " + msg);

            try {
                nlohmann::json data = nlohmann::json::parse(msg);
                if (data["action"] == "makeMove") {
                    uint8_t index = data["index"];
                    auto game = GameManager::getInstance()->getGame(gameId);
                    if (playerId == game->getCurrentPlayerId()) {
                        std::cout << "Player (id=" << playerId << ") trying to make a move (" << index / 3 << 
                        ", " << index % 3 << ")" << std::endl;
                        if (!game->makeMove(index / 3, index % 3)) {
                            std::cout << "Invalid move" << std::endl;
                            nlohmann::json error {
                                {"action", "error"},
                                {"type", "not valid move"}
                            };
                            ws.send(error.dump());
                            continue;
                        }
                    } else {
                        nlohmann::json error {
                            {"action", "error"},
                            {"type", "not your turn"}
                        };
                        ws.send(error.dump());
                        continue;
                    }

                    // Check for a win
                    bool isDraw = false;
                    uint32_t winnerId = 0;
                    if (game->checkWin(game->getCurrentPlayerId())) {
                        std::cout << "Player (id=" << game->getCurrentPlayerId() << ") wins!" << std::endl;
                        winnerId = game->getCurrentPlayerId();
                    } else if (game->isBoardFull()) {
                        std::cout << "It's a draw" << std::endl;
                        isDraw = true;
                    } else {
                        std::cout << "Default move" << std::endl;
                        game->switchPlayers();
                    }

                    nlohmann::json j = {
                        {"action", "gameState"},
                        {"gameId", gameId},
                        {"board", game->getBoard()},
                        {"currentPlayerId", game->getCurrentPlayerId()},
                        {"gameStatus", statusToStr(game->getStatus())},
                        {"winner", winnerId},
                        {"draw", isDraw}
                    };

                    auto players = GameManager::getInstance()->getGame(gameId)->getPlayersIds();
                    ConnectionManager::getInstance()->sendMessage(players.first, j.dump());
                    ConnectionManager::getInstance()->sendMessage(players.second, j.dump());

                    if (isDraw || winnerId > 0) {
                        try {
                            Database::getInstance()->saveResult(
                                ConnectionManager::getInstance()->getUserByPlayer(players.first).value(), 
                                ConnectionManager::getInstance()->getUserByPlayer(players.second).value(),
                                ConnectionManager::getInstance()->getUserByPlayer(winnerId)
                            );
                        } catch(const std::exception& e) {
                            std::cerr << e.what() << '\n';
                        }
                    }
                    

                } else if (data["action"] == "sendName") {
                    playerName = data["name"];
                    
                    try {
                        uint32_t userId = Database::getInstance()->findOrCreateUser(playerName);
                        ConnectionManager::getInstance()->addUserByPlayer(playerId, userId);
                    } catch (const std::exception& e) {
                        std::cerr << "[Database] " << e.what() << std::endl;
                    }
                }
            } catch (const std::exception& e) {
                nlohmann::json error {
                    {"action", "error"},
                    {"type", e.what()}
                };
                ws.send(error.dump());
            }
            
        }        

        ConnectionManager::getInstance()->deleteConnection(playerId);
        GameManager::getInstance()->disconnectPlayer(gameId, playerId); 
    });

    std::cout << "server is running on http://127.0.0.1:8080" << std::endl;
    srv.listen("0.0.0.0", 8080);

    return 0;
}