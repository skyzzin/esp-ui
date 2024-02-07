#include "funcs.h"
#include <ArduinoJson.h>



void pins(ESP8266WebServer* server){
    String data = server->arg("plain");
    Serial.println(" ");

    

    const size_t capacity = JSON_OBJECT_SIZE(5) + 30; 
    DynamicJsonDocument doc(capacity);

    DeserializationError error = deserializeJson(doc, data.c_str());
    if (error) {
        Serial.print(F("Falha ao analisar o JSON: "));
        Serial.println(error.c_str());
        return;
    }

    String pinName = doc["pinName"].as<String>();
    int pinValue = doc["pinValue"].as<int>(); //numero do pino
    String state = doc["state"];
    String output = doc["output"];
    int value = doc["value"].as<int>();

    String response = "Pino: " + pinName + "nPin" + value + " Estado: " + state + " Tipo De Saida: " + output; 

    Serial.println(response.c_str());

    pinMode(value, (output == "INPUT") ? INPUT : OUTPUT);
    digitalWrite(value, (state == "HIGH") ? HIGH : LOW);
  
    server->send(200,"texto",response.c_str());
}