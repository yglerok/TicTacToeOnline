async function btnClickHandler() {
            // let text = 'Button clicked at ' + new Date().toLocaleTimeString();
    let text = new Date().toLocaleTimeString() + ' ' + document.getElementById('text-to-send').value;
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

const button = document.querySelector('#btn');
button.addEventListener('click', btnClickHandler);

let socket = new WebSocket("ws://127.0.0.1:8080/ws");
socket.onopen = function(e) {
    console.log("[open] Connection established");
    //socket.send("Connection established");
}

let gameId = 0;
let playerId = 0;
socket.onmessage = function(event) {
    console.log(`[message] Get data from server: ${event.data}`);        

};

socket.onclose = function(event) {
    if (event.wasClean) {
        console.log(`[close] Connection close clean (code=${event.code}, reason=${event.reason})`);
    } else {
        console.log(`[close] Connection terminated`);
    }
}

socket.onerror = function(error) {
    console.error(`[error]`);
};

const wsbutton = document.querySelector('#wsbtn');
wsbutton.addEventListener('click', function(){
    let indx = document.getElementById('move-indx').value;
    let move = {
        action: "makeMove",
        // gameId: ,
        // playerId: ,
        index: Number(indx)
    };
    console.log(`Sent:` + JSON.stringify(move));
    socket.send(JSON.stringify(move));
});