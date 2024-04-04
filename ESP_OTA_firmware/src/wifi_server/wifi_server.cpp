#include "../main.h"

#define AP_MODE

/* Global Variable Instances */
ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
bool ws_connected = false;
JsonDocument JSON_variable;


/*************************************************************************************************/
/* WebSocket Event Callback Function                                                             */
/*************************************************************************************************/
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
            String ID = "{ \"id\" : " + (String)(num + 1) + " }";
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
    server.send(200, "text/html", (LittleFS.open("/test.html", "r").readString()) );
}
void handleCli() {
    server.send(200, "text/html", (LittleFS.open("/com.html", "r").readString()) );
}
void handleNotFound() {
    server.send(404, "text/html", "<html><body><h1>404 Page Not Found<h1></body></html>");
}

void server_init(void) {
    /* Start Web Server & Establish WebSocket Communication */
    server.on("/", handleMain);               /* Host Web Page Requested */
    server.on("/cli", handleCli);            /* Host Web Page Requested */
    server.onNotFound(handleNotFound);        /* Page Handler if page not found */
    server.begin();                           /* Start Web Server */
    webSocket.begin();                        /* Begin Web Socket Communication */
    webSocket.onEvent(webSocketEvent);        /* Web Socket Event Handler */

}

void wifi_server(void) {
    webSocket.loop();
    server.handleClient();
}

void wifi_scan(void)
{

}

void wifi_init(void) {
    /* List Of WiFi Credentials, Connected to when ever available */
    WiFi.mode(WIFI_STA);
    WiFi.mode(WIFI_AP);
    WiFiMulti.addAP("KBs GT 2 Pro", "24681355");
    WiFiMulti.addAP("Lakshmi 5th floor", "8247096672");

        /* WiFi.scanNetworks will return the number of networks found */
    Serial.println("Scanning for available networks . . . ");
    int n = WiFi.scanNetworks();
    if (n == 0) {
        Serial.println("no networks found");
    } 
    else {
        Serial.print(n);
        Serial.println(" Networks found.");
        for (int i = 0; i < n; ++i) {
        // Print SSID and RSSI for each network found
        Serial.print("\t");
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(WiFi.SSID(i));
        Serial.print(" (");
        Serial.print(WiFi.RSSI(i));
        Serial.print(")");
        Serial.println((WiFi.encryptionType(i) == AUTH_OPEN)?" ":"*");
        delay(10);
        }
    }

    // Set static IP address
    IPAddress ip(192, 168, 1, 95); // Your desired static IP address
    IPAddress gateway(192, 168, 1, 1); // Your gateway/router IP address
    IPAddress subnet(255, 255, 255, 0); // Subnet mask

    WiFi.config(ip, gateway, subnet);

    /* Connect to Strongest Available Network */
    Serial.print("Connecting . . . ");
    while (WiFiMulti.run(10000) != WL_CONNECTED) {
        Serial.print(". ");
        delay(100);
    }

    /* Print Connected Network Info */
    Serial.print("\nConnected to ");
    Serial.print(WiFi.SSID());
    Serial.print(" ");
    Serial.println(WiFi.RSSI());
    Serial.print("IP : ");
    Serial.println(WiFi.localIP());           /* Print WiFi IP_Address to Serial Monitor */

}