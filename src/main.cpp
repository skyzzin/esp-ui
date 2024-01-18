#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <string>
#include "funcs.h"

using namespace std;

#define LED 2
#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15
#define D9 3
#define D10 1

ESP8266WebServer server(80);

const char* ssid = "FR2N_2.4G";
const char* password = "982327979";

void setup() {
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    server.on("/",[](){
      html(&server);
    });

    server.on("/pins",HTTP_POST,[](){
      pins(&server);
      
    });

    server.begin();
}

void loop() {
  server.handleClient();
}


