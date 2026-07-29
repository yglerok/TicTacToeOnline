#include "GameManager.h"
#include "ConnectionManager.h"

#include <iostream>

GameManager* gameManager = nullptr;
bool end = false;

// void processInput(int input) 
// {
//     switch (input)
//         {
//         case 1: {
//             uint32_t id1, id2;
//             std::cout << "Enter players id: ";
//             std::cin >> id1 >> id2;
//             gameManager->createNewGame(id1, id2);
//             break;
//         }
//         case 2: {
//             uint32_t id;
//             std::cout << "Enter game id: ";
//             std::cin >> id;
//             gameManager->daleteFinishedGame(id);
//             break;
//         }
//         case 3: {
//             const auto& games = gameManager->getAllGames();
//             std::cout << "id\t xId\t oId" << std::endl;
//             for (const auto& [id, game] : games) {
//                 std::cout << id << "\t" << game->getPlayersIds().first << "\t" << game->getPlayersIds().second << std::endl;
//             }
//             break;
//         }
//         case 4:
//             end = true;
//             break;
//         default:
//             break;
//         }
// }

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

    srv.Post("/button-click", [](const httplib::Request& req, httplib::Response& res) {
        std::string body = req.body;
        std::cout << "[http] Button clicked! Message: " << body << std::endl;

        res.set_content("Button click processed! Recieved: " + body, "text/plain");
    });

    srv.Get("/hi", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("Hello, world!", "text/plain");
    });

    //std::unordered_map<httplib::ws::WebSocket*, uint32_t> gameByWs;
    srv.WebSocket("/ws", [](const httplib::Request& req, httplib::ws::WebSocket &ws) {
        uint32_t playerId = ConnectionManager::getInstance()->addConnection(&ws); 

        uint32_t gameId = GameManager::getInstance()->findWaitingGame();
        if (gameId == 0) {
            // Waiting game not found -> creating new
            gameId = GameManager::getInstance()->createNewGame(playerId);
        } else {
            // Waiting game found -> join it -> start it
            GameManager::getInstance()->joinGame(gameId, playerId);
        }

        ws.send("Connected to game (id=" + std::to_string(gameId) + ")");

        std::string msg;
        while (ws.read(msg)) {
            std::cout << "[ws] id = " << playerId << " : " << msg << std::endl;
            ws.send("echo: " + msg);
        }

        // Think about game end
        // delete connection -> disconnect player from game -> check if game finished -> delete game
        ConnectionManager::getInstance()->deleteConnection(playerId);
        GameManager::getInstance()->disconnectPlayer(gameId, playerId);
    });

    std::cout << "server is running on http://127.0.0.1:8080" << std::endl;
    srv.listen("127.0.0.1", 8080);

    // gameManager = GameManager::getInstance();

    // while (!end) {
    //     std::cout << "==========" << std::endl;
    //     std::cout << "1 - add new game\n2 - delete game\n3 - show list\n4 - exit" << std::endl;
    //     std::cout << "==========" << std::endl;
    //     int input;
    //     std::cin >> input;
    //     processInput(input);
    // }

    return 0;
}