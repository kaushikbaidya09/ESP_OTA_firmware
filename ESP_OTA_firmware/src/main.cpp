#include <Arduino.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

// Allocate the JSON document
JsonDocument JSON_variable;
String value;

/* Global Variable Instances */
bool ws_connected = false;      /* Websocket Connected Flag */

ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    switch (type) {
        case WStype_DISCONNECTED:
            ws_connected = false;
            Serial.printf("[%u] Disconnected!\n", num);
            break;

        case WStype_CONNECTED: {
            ws_connected = true;
            IPAddress ip = webSocket.remoteIP(num);
            Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
            // webSocket.sendTXT(num, "0");    /* send message to client */
            String ID = "{ \"id\" : " + (String)num + " }";
            webSocket.sendTXT(num, ID);    /* send message to client */
        } break;

        case WStype_TEXT:
            Serial.printf("[%u] get Text: %s\n", num, payload);
            // webSocket.sendTXT(0, payload);    /* send message to client */
            webSocket.broadcastTXT(payload);
            break;

        case WStype_BIN:
            Serial.printf("[%u] get binary length: %u\n", num, length);
            hexdump(payload, length);
            break;
        case WStype_ERROR:
            break;
        case WStype_FRAGMENT_TEXT_START:
            break;
        case WStype_FRAGMENT_BIN_START:
            break;
        case WStype_FRAGMENT:
            break;
        case WStype_FRAGMENT_FIN:
            break;
        case WStype_PING:
            break;
        case WStype_PONG:
            break;
    }
}

void handleMain() {
    server.send(200, "text/html", (LittleFS.open("/com.html", "r").readString()) );
}
void handleNotFound() {
    server.send(404, "text/html", "<html><body><p>404 Error</p></body></html>");
}

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

    deserializeJson(JSON_variable, " ");                              /* Deserialize String into JSON Format (Used as to Clear JSON Object) */
    JSON_variable["msg"] = "Massage will be displayed here!";         /* Add Massage to JSON object */
    JSON_variable["val"] = 0;                                         /* Add Key Value Pair to JSON Object */
    JSON_variable["val2"] = (int)(JSON_variable["val"]) * 1000;       /* Update JSON Object Key Value Pair */
    serializeJson(JSON_variable, Serial); Serial.println(" ");        /* Serialize JSON Object in one single line */
    serializeJsonPretty(JSON_variable, Serial); Serial.println(" ");  /* Serialize JSON Object in Formated String */

    /* List Of WiFi Credentials, Connected to when ever available */
    WiFiMulti.addAP("Lakshmi 5th floor", "8247096672");
    Serial.print("Connecting to Wifi.");
    while (WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }

    Serial.print("\nConnected. IP ");
    Serial.println(WiFi.localIP());           /* Print WiFi IP_Address to Serial Monitor */
    webSocket.begin();                        /* Begin Web Socket Communication */
    webSocket.onEvent(webSocketEvent);        /* Web Socket Event Handler */
    server.on("/", handleMain);               /* Host Web Page Requested */
    server.onNotFound(handleNotFound);        /* Page Handler if page not found */
    server.begin();                           /* Start Web Server */
}

uint32_t currentMillis = 0;
uint32_t previousMills = 0;
uint32_t counter = 0;
void loop() {
    // delay(1000);
    // // value = "{\"msg\" : \"Hello World\", \"val\" : 10 }";
    // if(ws_connected){
    //     JSON_variable["val"] = (int)(JSON_variable["val"]) + 1;
    //     serializeJson(JSON_variable, value);
    //     webSocket.broadcastTXT(value);
    //     // Serial.println(value);
    // }
    webSocket.loop();
    server.handleClient();
}
