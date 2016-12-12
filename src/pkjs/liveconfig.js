const CONFIG = require('./configuration');
var liveconfig = module.exports;

/**
 * Connects to the liveconfig session
 *
 * @param uuid Your app's UUID
 * @param onChange The function to call when a value is changed.
 */
liveconfig.connect = function(uuid, onChange) {
    if (!uuid) {
        throw new Error("Liveconfig requires a UUID!");
    }
    const url = CONFIG.LIVECONFIG_URL + uuid + "/" + Pebble.getWatchToken();
    console.log("connect(): opening socket: " + url);
    var open = function() {
        liveconfig.websocket = new WebSocket(url);
        liveconfig.websocket.onopen = function(open) {
            console.log("WebSocket opened", JSON.stringify(open));
        };
        liveconfig.websocket.onmessage = function(message) {
            console.log("onmessage()", JSON.stringify(message));
            var attr = JSON.parse(message.data);
            onChange(attr.id, attr.value);
        };
        liveconfig.websocket.onerror = function(error) {
            console.log("onerror()", JSON.stringify(error));
        };
        liveconfig.websocket.onclose = function(close, c2, c3) {
            console.log("WebSocket closed", close, JSON.stringify(close), close.wasClean, close.reason, c2, c3);
            if (!close.wasClean) {
                console.log("onclose will reopen in 1000ms");
                setTimeout(function () {
                    console.log("Reopening WebSocket");
                    open();
                }, 1000);
            }
        };
    };
    open();
};
liveconfig.disconnect = function() {
    console.log("disconnect()");
    if (liveconfig.websocket) {
        try {
            liveconfig.websocket.close(4200, 'disconnect');
        } catch(e) {
            console.log("Couldn't close websocket", e);
        }
    }
};
