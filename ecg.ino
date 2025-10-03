#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define SENSOR 36

const char* ssid = "CHENAB";//"IITRPR";
const char* password = "44zMf3QqdU&KC3Mv";//"V#6qF?pyM!bQ$%NX";

const char* url = "http://simpop.org/ecg/savedata.php";


unsigned long sessnum;

String httpResponseData;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  pinMode(SENSOR, INPUT);
  sessnum = millis();
  httpResponseData = "sessnum=" + String(sessnum) + "&sessdata=";
}

int queryCnt = 0;

void loop() {
  // Serial.print(analogRead(SENSOR));
  if(queryCnt < 2){
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("WiFi Connected");
      if((digitalRead(10) == 1)||(digitalRead(11) == 1)){
        Serial.println('!');
      } else {
        httpResponseData += String(analogRead(SENSOR))+",";
      }

      if (httpResponseData.length() > 1000) {
        Serial.println(httpResponseData);


        HTTPClient http;
        WiFiClient client;

        http.begin(client, url);

        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        int httpResponseCode = http.POST(httpResponseData);

        String payload = http.getString();
        if (payload.indexOf("SUCCESS") == -1) {
          Serial.println(payload);
        } else {
          queryCnt++;
          Serial.println("queries "+String(queryCnt));
        }
        http.end();
        httpResponseData = "sessnum=" + String(sessnum) + "&sessdata=";
      }
    } else {
      Serial.println("WiFi Disconnected");
    }
  } else {
    Serial.println("Done");
  }
  delay(1);
}
