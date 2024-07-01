#include "main.hpp"

/*************************************************************************************************/
/* Application Entry Function                                                                    */
/*************************************************************************************************/
void setup() {

    /* Initialize the Debug Print Serial Port */
    Serial.begin(9600);

    Serial.println("\n| |/ /                  | |    (_)| |   ( )     \\ \\        / /(_)|  ____|(_)   / ____|| |     |_   _|");
    Serial.println("| ' /  __ _  _   _  ___ | |__   _ | | __|/ ___   \\ \\  /\\  / /  _ | |__    _   | |     | |       | |  ");
    Serial.println("|  <  / _` || | | |/ __|| '_ \\ | || |/ /  / __|   \\ \\/  \\/ /  | ||  __|  | |  | |     | |       | |  ");
    Serial.println("| . \\| (_| || |_| |\\__ \\| | | || ||   <   \\__ \\    \\  /\\  /   | || |     | |  | |____ | |____  _| |_ ");
    Serial.println("|_|\\_\\\\__,_| \\__,_||___/|_| |_||_||_|\\_\\  |___/     \\/  \\/    |_||_|     |_|   \\_____||______||_____|\n");

    /* Initialize SPI Flash File System, Used to Store web server (HTML + CSS + JS) */
    if (!LittleFS.begin()) {
        Serial.println("An Error has occurred while mounting LittleFS");
        return;
    }
    delay(1000);

    wifi_init();
    server_init();
    // mqtt_init();
}

/*************************************************************************************************/
/* Application Main Loop                                                                         */
/*************************************************************************************************/
void loop() {
    // String payload;
    // deserializeJson(JSON_variable, " ");                              /* Deserialize String into JSON Format (Used as to Clear JSON Object) */
    // JSON_variable["user"] = "ESP_Serial";                             /* Add Massage to JSON object */
    // JSON_variable["msg"] = "Massage will be displayed here!";         /* Add Key Value Pair to JSON Object */
    // serializeJson(JSON_variable, payload); // Serial.println(" ");        /* Serialize JSON Object in one single line */
    // serializeJsonPretty(JSON_variable, Serial); Serial.println(" ");  /* Serialize JSON Object in Formated String */
    //webSocket.broadcastTXT(payload);

    wifi_server();
}
