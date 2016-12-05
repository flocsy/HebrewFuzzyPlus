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
        console.log("open()");
        liveconfig.websocket = new WebSocket(url);
        console.log("open readyState", liveconfig.websocket.readyState);
        console.log("open()2");
        liveconfig.websocket.onopen = function(open) {
            console.log("WebSocket opened", JSON.stringify(open));
            console.log("onopen readyState", liveconfig.websocket.readyState);
        };
        console.log("open()3");
        liveconfig.websocket.onmessage = function(message) {
            console.log("onmessage()", JSON.stringify(message));
            var attr = JSON.parse(message.data);
            onChange(attr.id, attr.value);
            console.log("onmessage readyState", liveconfig.websocket.readyState);
        };
        console.log("open()4");
        liveconfig.websocket.onerror = function(error) {
            console.log("onerror()", JSON.stringify(error));
            console.log("onerror readyState", liveconfig.websocket.readyState);
        };
        console.log("open()5");
        liveconfig.websocket.onclose = function(close, c2, c3) {
            console.log("WebSocket closed", close, JSON.stringify(close), close.wasClean, close.reason, c2, c3);
            console.log("onclose readyState", liveconfig.websocket.readyState);
            if (close.wasClean) {
                // liveconfig.clearTimer();
            } else {
                console.log("onclose will reopen in 1000ms");
                setTimeout(function () {
                    console.log("Reopening WebSocket");
                    open();
                }, 1000);
            }
        };
        console.log("open()6");
    };
    open();
    // var pollReadyState = function(){
    //     console.log("readyState", liveconfig.websocket.readyState);
    //     liveconfig.pollReadyStateTimer = setTimeout(pollReadyState, 1000);
    // };
    // pollReadyState();
};
// liveconfig.clearTimer = function() {
//     if (liveconfig.pollReadyStateTimer) {
//         clearTimeout(liveconfig.pollReadyStateTimer);
//     }
// };
liveconfig.disconnect = function() {
    console.log("disconnect()");
    // liveconfig.clearTimer();
    if (liveconfig.websocket) {
        try {
            liveconfig.websocket.close(4200, 'disconnect');
        } catch(e) {
            console.log("Couldn't close websocket", e);
        }
    }
};
