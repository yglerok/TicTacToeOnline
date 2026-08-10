async function btnClickHandler() {
    // let text = 'Button clicked at ' + new Date().toLocaleTimeString();
    //let text = new Date().toLocaleTimeString() + ' ' + document.getElementById('text-to-send').value;
    try {
        let response = await fetch('/button-click', {
            method: 'POST',
            headers: {
                'Content-Type': 'text/plain'
            },
            body: text
        });
        if (response.ok) {
            let result = await response.text();
            console.log('Server response:', result);
        } else {
            console.error('Server error:', response.status);
        }
    } catch (error) {
        console.error('Fetch error:', error);
    }
}

// const button = document.querySelector('#btn');
// button.addEventListener('click', btnClickHandler);

let socket = new WebSocket("ws://127.0.0.1:8080/ws");
socket.onopen = function (e) {
    console.log("[open] Connection established");
    //socket.send("Connection established");
}

let gameId = 0;
let playerId = 0;
let opponentId = 0;
let currentId = 0;
let symbol = "";

socket.onmessage = function (event) {
    console.log(`[message] Get data from server: ${event.data}`);

    let data = JSON.parse(event.data);

    if (data.action == "gameCreated" || data.action == "gameJoined") {
        gameId = data.gameId;
        playerId = data.playerId;
    } else if (data.action == "gameStarted") {
        opponentId = data.opponentId;
        currentId = data.currentPlayerId;
        symbol = data.playerSymbol;

        setupToggleElement();

        let panels = [document.getElementById('youPanel'), document.getElementById('opponentPanel')];
        for (var i = 0; i < panels.length; i++)
            if (panels[i].classList.contains("active")) {
                // panels[i].classList.remove("active");
                document.getElementById('boardId').classList.add("locked");
            } else {
                // panels[i].classList.add("active");
                document.getElementById('boardId').classList.remove("locked");
            }
    } else if (data.action == "gameState") {
        switchTurn();
    }

};

socket.onclose = function (event) {
    if (event.wasClean) {
        console.log(`[close] Connection close clean (code=${event.code}, reason=${event.reason})`);
    } else {
        console.log(`[close] Connection terminated`);
    }
}

socket.onerror = function (error) {
    console.error(`[error]`);
};

// const wsbutton = document.querySelector('#wsbtn');
// wsbutton.addEventListener('click', function () {
//     let indx = document.getElementById('move-indx').value;
//     let move = {
//         action: "makeMove",
//         // gameId: ,
//         // playerId: ,
//         index: Number(indx)
//     };
//     console.log(`Sent:` + JSON.stringify(move));
//     socket.send(JSON.stringify(move));
// });

function setupToggleElement() {
    if (symbol == "O") {
        document.getElementById('youSymbol').innerHTML = "O";
        document.getElementById('opponentSymbol').innerHTML = "X";

        let root = document.querySelector(':root');
        root.style.setProperty('--you', 'var(--color-o)');
        root.style.setProperty('--you-text', 'var(--o-text)');
        root.style.setProperty('--opponent', 'var(--color-x)');
        root.style.setProperty('--opponent-text', 'var(--x-text)');
    } else {
        document.getElementById('opponentSymbol').innerHTML = "O";
    }

    document.getElementById('opponentLabel').innerHTML = "Opponent";
    document.getElementById('opponentPanel').classList.remove('waiting');

    if (currentId != playerId) {
        document.getElementById('youPanel').classList.remove('active');
        document.getElementById('opponentPanel').classList.add('active');
    }
}

function switchTurn() {
    let panels = [document.getElementById('youPanel'), document.getElementById('opponentPanel')];
    for (var i = 0; i < panels.length; i++)
        if (panels[i].classList.contains("active")) {
            panels[i].classList.remove("active");
            document.getElementById('boardId').classList.add("locked");
        } else {
            panels[i].classList.add("active");
            document.getElementById('boardId').classList.remove("locked");
        }
}

var cells = document.getElementsByClassName('cell');

for (var i = 0; i < cells.length; i++) {
    cells[i].addEventListener('click', onclickCell);
}

function onclickCell() {
    if (document.getElementById('boardId').classList.contains('locked'))
        return;
    this.innerHTML = symbol;
    if (symbol == "O")
        this.style.color = "var(--color-o)";
    else {
        this.style.color = "var(--color-x)";
    }
}