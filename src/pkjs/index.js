const CONFIG = require('./configuration');
const DEBUG = CONFIG.DEBUG;
const Analytics = require('./pebble-ga');
const PACKAGE = require('./package.json');
const MESSAGE_KEYS = require('message_keys');
const MESSAGE_IDS = {};
Object.keys(MESSAGE_KEYS).forEach(function(key){MESSAGE_IDS[MESSAGE_KEYS[key]] = key});
const liveconfig = require('./liveconfig');
const Clay = require('pebble-clay');
const liveconfigClay = require('./liveconfig_clay');
const clayConfig = require('./config');
const clay = new Clay(clayConfig, liveconfigClay, {autoHandleEvents: false, userData: {
    uuid: PACKAGE.pebble.uuid,
    liveconfigServerUrl: CONFIG.LIVECONFIG_URL
}});
var ga;

function sendDictionaryToPebble( dictionary ) {
    console.log("sendDictionaryToPebble");

    Pebble.sendAppMessage( dictionary,
        function(e) {
            if (DEBUG) console.log( "index.js: sendDictionaryToPebble(): Message sent to Pebble successfully. " + JSON.stringify( dictionary ) );
        },
        function(e) {
            if (DEBUG) console.log( "index.js: sendDictionaryToPebble(): Error sending message to Pebble. " + JSON.stringify( e ) );
            ga.trackEvent('error', 'config.sendAppMessage');
        });
}

function sendConfig() {
    // var dictionary = {};

    // Object.keys(MESSAGE_KEYS).forEach(function(key){
    //     var item = MESSAGE_KEYS[key];
    //     var item_str = localStorage.getItem(item);
    //     if (DEBUG) console.log( "index.js: sendConfig() getItem(" + item + "): key:" + key + " item_str:" + item_str);
    //     dictionary[item] = item_str ? JSON.parse(item_str) : undefined;
    //     if (DEBUG) console.log( "index.js: sendConfig(): " + key + "/" + item + ": " + JSON.stringify(dictionary[item]) );
    // });

    var settingsStorage = {};
    try {
        settingsStorage = JSON.parse(localStorage.getItem('clay-settings')) || {};
    } catch (e) {
        console.error("sendConfig", e.toString(), localStorage.getItem('clay-settings'));
    }

    console.log("sendConfig getItem clay-settings", JSON.stringify(settingsStorage));
    sendDictionaryToPebble( settingsStorage );
}

Pebble.addEventListener('ready', function(e) {
    console.log('JavaScript app ready and running!');
    ga = new Analytics(CONFIG.GA_UA, PACKAGE.name, PACKAGE.version);
    ga.trackEvent('app', 'start');
    sendConfig();
});

Pebble.addEventListener('showConfiguration', function(e) {
    liveconfig.disconnect();
    /*
     * Creates an instance on the liveconfig server based off of a unique id for the user.
     * The configuration settings will be passed via this connection and updated in realtime
     *
     * The first parameter is your app's UUID. In the future this will not be required
     *
     * The second parameter is a call back function to call when a value is changed
     */
    liveconfig.connect(PACKAGE.pebble.uuid, function(id, value) {
        var config = {};
        config[id] = value;
        var dict = Clay.prepareSettingsForAppMessage(config);
        console.log("liveconfig.onChange", id, value, JSON.stringify(config), "dict", JSON.stringify(dict));
        Object.keys(dict).forEach(function(item){
            var key = MESSAGE_IDS[item];
            // localStorage.setItem(item, JSON.stringify(dict[item]));
            clay.setSettings(key, dict[item]);
            if (DEBUG) console.log( "index.js: showConfiguration: setItem(" + item + ", " + dict[item] + ")");
            ga.trackEvent('liveconfig.' + key, dict[item]);
        });
        Pebble.sendAppMessage(dict);
    });

    Pebble.openURL(clay.generateUrl());

    ga.trackScreen('settings');
    ga.trackEvent('config', 'open');
});

Pebble.addEventListener('webviewclosed', function(e) {
    if (e && !e.response) {
        return;
    }

    // Get the keys and values from each config item
    var dict = clay.getSettings(e.response);
    if (DEBUG) console.log( "index.js/webviewclosed: " + JSON.stringify( dict) );

    Object.keys(dict).forEach(function(item){
//        localStorage.setItem(item, JSON.stringify(dict[item]));
//        if (DEBUG) console.log( "index.js: webviewclosed: setItem(" + item + ", " + JSON.stringify(dict[item]) + ")");
        ga.trackEvent('config.' + MESSAGE_IDS[item], dict[item]);
    });
    sendConfig();
    ga.trackEvent('config', 'close');
    liveconfig.disconnect();
});
