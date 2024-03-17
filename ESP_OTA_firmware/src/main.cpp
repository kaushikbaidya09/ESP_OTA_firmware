#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <Hash.h>
#include <LittleFS.h>
#include <WebSocketsServer.h>


String old_value, value;

ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

char html_template[] PROGMEM = R"=====(
<html lang="en">
   <head>
      <meta charset="utf-8">
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <title>ESP WS Test</title>
      <script>
        socket = new WebSocket("ws:/" + "/" + location.host + ":81");
        socket.onopen = function(e) {  console.log("[socket] socket.onopen "); };
        socket.onerror = function(e) {  console.log("[socket] socket.onerror "); };
        socket.onmessage = function(e) {  
            console.log("[socket] " + e.data + e.data);
            document.getElementById("mrdiy_value1").innerHTML = e.data;
            document.getElementById("mrdiy_value2").innerHTML = e.data;
        };
      </script>
   </head>
   <body style="max-width:400px;margin: auto;font-family:Arial, Helvetica, sans-serif;text-align:center">
      <div><h1><br />My Value1</h1></div>
      <div><p id="mrdiy_value1" style="font-size:100px;margin:0"></p></div>
      <div><h1><br />My Value2</h1></div>
      <div><p id="mrdiy_value2" style="font-size:100px;margin:0"></p></div>
   </body>
</html>
)=====";

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload,
                    size_t length) {

  switch (type) {
  case WStype_DISCONNECTED:
    Serial.printf("[%u] Disconnected!\n", num);
    break;

  case WStype_CONNECTED: {
    IPAddress ip = webSocket.remoteIP(num);
    Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0],
                  ip[1], ip[2], ip[3], payload);
    // send message to client
    webSocket.sendTXT(num, "0");
  } break;

  case WStype_TEXT:
    Serial.printf("[%u] get Text: %s\n", num, payload);
    // send message to client
    // webSocket.sendTXT(num, "message here");
    // send data to all connected clients
    // webSocket.broadcastTXT("message here");
    break;

  case WStype_BIN:
    Serial.printf("[%u] get binary length: %u\n", num, length);
    hexdump(payload, length);
    // send message to client
    // webSocket.sendBIN(num, payload, length);
    break;
  }
}

void handleMain() {
  // server.send_P(200, "text/html", html_template );
  server.send(200, "text/html", (LittleFS.open("/test.html", "r").readString()) );
}
void handleNotFound() {
  server.send(404, "text/html", "<html><body><p>404 Error</p></body></html>");
}

void setup() {

  Serial.begin(9600);
  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
  delay(1000);

  WiFiMulti.addAP("Lakshmi 5th floor", "8247096672");
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }

  Serial.println(WiFi.localIP());

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  server.on("/", handleMain);

  server.onNotFound(handleNotFound);
  server.begin();
}

uint32_t currentMillis = 0;
uint32_t previousMills = 0;
uint32_t counter = 0;
void loop() {
  currentMillis = millis();
  if ((currentMillis - previousMills) >= 100) {
    previousMills = currentMillis;
    counter++;
    old_value = (String)(counter - 100);
    value = (String)counter;
    webSocket.broadcastTXT(old_value);
    webSocket.broadcastTXT(value);
  }
  webSocket.loop();
  server.handleClient();
  delay(50);
}
