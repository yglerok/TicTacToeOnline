class EventEmitter {
    constructor() {
        this.listeners = {};
    }

    on(eventName, callback) {
        if (!this.listeners[eventName])
            this.listeners[eventName] = [];
        this.listeners[eventName].push(callback);
    }

    emit(eventName, data) {
        if (!this.listeners[eventName]) return;
        
        for (const callback of this.listeners[eventName]) {
            callback(data);
        }
    }
}

export default EventEmitter;