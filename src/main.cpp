#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h" 

const int ledPin = 2;

void setup() { 
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);
    
    Serial.print("Connecting to WiFi");
    WiFi.begin(SSID, PASS);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }
    Serial.println(" Connected!");
}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(ENDPOINT);
        int httpResponseCode = http.GET();
        
        if (httpResponseCode == 200) {
            String payload = http.getString();
            Serial.println("Received JSON:");
            Serial.println(payload);
            
            // Parse JSON response
            JsonDocument doc;
            DeserializationError error = deserializeJson(doc, payload);
            
            if (!error) {
                float rainfall = doc["rainfall"];
                Serial.print("Rainfall: ");
                Serial.println(rainfall);
                
                digitalWrite(ledPin, rainfall > 0 ? HIGH : LOW);
            } else {
                Serial.println("Failed to parse JSON");
            }
        } else {
            Serial.print("HTTP Request failed with code: ");
            Serial.println(httpResponseCode);
        }
        
        http.end();
    } else {
        Serial.println("WiFi Disconnected!");
    }
    
    delay(5000); // Wait 5 seconds before next request
}
