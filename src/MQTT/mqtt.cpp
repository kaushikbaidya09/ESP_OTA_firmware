#include "../main.h"

/* MQTT Broker */
// const char *mqtt_broker = "dfe88d08.ala.us-east-1.emqxsl.com"; /* broker address */
const char *mqtt_broker = "c3f81831.ala.asia-southeast1.emqxsl.com"; /* broker address */
const char *mqtt_username = "KB_test";      /* username for authentication */
const char *mqtt_password = "KB9520";       /* password for authentication */
const int mqtt_port = 8883;                 /* port of MQTT over TCP */
const char *client_id = "KBs_Device_01";    /* Clint Name */
const char *mqtt_topic_pub_01 = "ESP_tx";    /* Publish topic name */
const char *mqtt_topic_sub_01 = "ESP_rx";    /* Subscribe topic name */
String message = "";

/* init wifi client */
WiFiClientSecure espClient;
PubSubClient client(espClient);

/*
  The common fingerprints of EMQX broker, for reference only.
  If you are not using EMQX Cloud Serverless or public EMQX broker,
  you need to calculate the sha1 fingerprint of your server certificate
  and update the 'fingerprint' variable below.
*/
// 1. fingerprint of public emqx broker. Host: broker.emqx.io
// const char* fingerprint = "B6 C6 FF 82 C6 59 09 BB D6 39 80 7F E7 BC 10 C9 19 C8 21 8E";
// 2. fingerprint of EMQX Cloud Serverless. Host: *.emqxsl.com
const char* fingerprint = "42:AE:D8:A3:42:F1:C4:1F:CD:64:9C:D7:4B:A1:EE:5B:5E:D7:E2:B5";
// 3. fingerprint of EMQX Cloud Serverless. Host: *.emqxsl.cn
// const char* fingerprint = "7E:52:D3:84:48:3C:5A:9F:A4:39:9A:8B:27:01:B1:F8:C6:AD:D4:47";

// void callback(char* topic, byte* payload, unsigned int length);
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    message = "";
    for (unsigned int i = 0; i < length; i++) {
        message = "" + ((String) payload[i]);
        Serial.print((char) payload[i]);
    }
    Serial.println();
    Serial.print("Variable Massage: ");
    Serial.println(message);
    Serial.println("-----------------------");
}

void mqtt_init(void) {
    /* connecting to a mqtt broker */
    espClient.setFingerprint(fingerprint);
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    while (!client.connected()) {
        Serial.printf("The client %s connects to the mqtt broker\n", client_id);
        if (client.connect(client_id, mqtt_username, mqtt_password)) {
            Serial.println("Connected to MQTT broker.");
            client.subscribe(mqtt_topic_sub_01);
        } else {
            Serial.print("Failed to connect to MQTT broker, rc=");
            Serial.print(client.state());
            Serial.println(" Retrying in 5 seconds.");
            delay(5000);
        }
    }
    client.publish(mqtt_topic_pub_01, "Hi EMQX I'm KB9520");

}