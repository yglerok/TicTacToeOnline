import Connection from "./Connection.js";
import BoardView from "./BoardView.js";
import InfoPanelView from "./InfoPanelView.js";

class App {
    constructor() {
        this.screen = "in-game"; // "lobby" | "in-room" | "in-game"
        this.playerId = null;
        this.opponentId = null;
        this.currentId = null;
        this.gameId = null;
        this.symbol = null;

        this.connection = new Connection("ws://127.0.0.1:8080/ws");
        this.board = new BoardView(document.getElementById('boardId'));
        this.board.onMoveAttempt = (index) => this.handleMoveAttempt(index);
        this.panel = new InfoPanelView(document.getElementById('youPanel'),
            document.getElementById('opponentPanel'), document.getElementById('resultBanner'));

        this.registerListeners();
    }

    registerListeners() {
        this.connection.on("gameCreated", (data) => this.handleGameCreated(data));
        this.connection.on("gameJoined", (data) => this.handleGameJoined(data));
        this.connection.on("gameStarted", (data) => this.handleGameStarted(data));
        this.connection.on("gameState", (data) => this.handleGameState(data));
        this.connection.on("error", (data) => { });
    }

    handleMoveAttempt(index) {
        this.connection.send({ action: "makeMove", index: index });
    }

    handleGameCreated(data) {
        this.gameId = data.gameId;
        this.playerId = data.playerId;
    }

    handleGameJoined(data) {
        this.gameId = data.gameId;
        this.playerId = data.playerId;
    }

    handleGameStarted(data) {
        this.opponentId = data.opponentId;
        this.currentId = data.currentPlayerId;
        this.symbol = data.playerSymbol;

        this.panel.setupToggleElement(this.playerId, this.currentId, this.symbol);
        this.board.lock(this.currentId == this.playerId);
    }

    handleGameState(data) {
        const viewModel = { ...data, playerId: this.playerId, symbol: this.symbol }
        this.board.render(viewModel);

        if (data.draw || data.winner != 0) {
            this.board.lock(false);
            
            this.panel.showResult(data.draw, data.winner, this.playerId);
            return;
        }
        
        this.panel.render(viewModel);
    }
}

new App();