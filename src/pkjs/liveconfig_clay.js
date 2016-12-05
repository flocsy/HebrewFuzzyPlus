/**
 * Created by Gavriel Fleischer on 2016-11-24.
 */
module.exports = function(minified) {
    var clayConfig = this;
    // var _ = minified._;
    // var $ = minified.$;
    // var HTML = minified.HTML;

    clayConfig.on( clayConfig.EVENTS.AFTER_BUILD, function() {
        var uuid = clayConfig.meta.userData.uuid;
        var watchtoken = clayConfig.meta.watchToken;
        var connection;
        if (uuid && watchtoken) {
            var open = function() {
                console.log("open()");
                connection = new WebSocket(clayConfig.meta.userData.liveconfigServerUrl + uuid + "/" + watchtoken);
                connection.onopen = function (open) {
                    console.log("onopen()", open);
                };
                connection.onerror = function(error) {
                    console.log("onerror()", JSON.stringify(error));
                };
                connection.onclose = function (close) {
                    console.log("onclose()", close);
                    setTimeout(function(){
                        console.log("onclose() + timeout: Reopening WebSocket");
                        open();
                    }, 1000);
                };
            };
            open();
            clayConfig.getAllItems().forEach(function (item) {
                item.on('change', function () {
                    var item = this;
                    console.log("custom_clay.js:change item:", item, "messageKey:", item.messageKey, "val:", item.get());
                    connection.send(JSON.stringify({
                        id: item.messageKey,
                        value: item.get()
                    }));
                });
            });
        }
    });
};
