import EventEmitter from "./EventEmitter.js";

class Connection extends EventEmitter {
    constructor(url) {
        super();

        this.socket = new WebSocket(url);

        this.socket.onmessage = (event) => {
            console.log(`[message] Get data from server: ${event.data}`);
            const data = JSON.parse(event.data);
            this.emit(data.action, data)
        };

        this.socket.onopen = () => {
            console.log("[open] Connection established");
        }

        this.socket.onclose = (event) => {
            if (event.wasClean) {
                console.log(`[close] Connection close clean (code=${event.code}, reason=${event.reason})`);
            } else {
                console.log(`[close] Connection terminated`);
            }
        }

        this.socket.onerror = (event) => {
            console.error(`[error]`);
        }
    }

    send(obj) {
        var msg = JSON.stringify(obj);
        this.socket.send(msg);
    }
}

export default Connection;