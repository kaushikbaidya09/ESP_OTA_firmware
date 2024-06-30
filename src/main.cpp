#include "main.h"

// void checkForUpdates() {
//     // Make a HTTP request to GitHub API to get latest release information
//     HTTPClient http;
//     http.begin(repositoryURL);
//     int httpResponseCode = http.GET();
    
//     if (httpResponseCode == HTTP_CODE_OK) {
//         String payload = http.getString();
//         http.end();

//         // Parse JSON response
//         DynamicJsonDocument jsonBuffer(1024);
//         deserializeJson(jsonBuffer, payload);

//         const char* latestVersion = jsonBuffer["tag_name"];
//         if (strcmp(latestVersion, firmwareVersion) > 0) {
//             Serial.println("New firmware version available!");

//             // Download the new firmware binary
//             char firmwareDownloadURL[256];
//             sprintf(firmwareDownloadURL, firmwareURL, latestVersion);
//             Serial.print("Downloading firmware from: ");
//             Serial.println(firmwareDownloadURL);

//             t_httpUpdate_return ret = ESPhttpUpdate.update(firmwareDownloadURL);
//             if (ret == HTTP_UPDATE_OK) {
//                 Serial.println("Firmware update successful!");
//                 // Reset or perform necessary actions after successful update

//                 // Update file system data
//                 updateFileSystem();
//             } else {
//                 Serial.print("Firmware update failed, error: ");
//                 Serial.println(ESPhttpUpdate.getLastError());
//             }
//         } else {
//             Serial.println("Firmware is up to date!");
//         }
//     } else {
//         Serial.print("Failed to connect to GitHub API, error: ");
//         Serial.println(httpResponseCode);
//     }
// }

// void updateFileSystem() {
//     // Make a HTTP request to download the file
//     HTTPClient http;
//     http.begin(fileURL);
//     int httpResponseCode = http.GET();
    
//     if (httpResponseCode == HTTP_CODE_OK) {
//         File file = LittleFS.open("/data.txt", "w");
//         if (!file) {
//             Serial.println("Failed to open file for writing");
//             return;
//         }
        
//         String payload = http.getString();
//         file.print(payload);
//         file.close();
        
//         Serial.println("File system data updated successfully!");
//     } else {
//         Serial.print("Failed to download file, error: ");
//         Serial.println(httpResponseCode);
//     }
// }

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
