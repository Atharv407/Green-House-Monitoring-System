#include <WiFi.h>
#include <HTTPClient.h>

const String ssid = "PJ";
const String password = "pj091018";

const String serverName = "http://api.thingspeak.com/update";
String apiKey = "AXYQR2JICGS60SC5";

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  // Serial.println("Connecting to " + ssid);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    // Serial.println(".");
  }
  // Serial.println("");
  // Serial.print("Connected to WiFi network with IP Address: ");
}

void loop() {
  if (Serial.available()) {

    String data = Serial.readStringUntil('\n');
    int firstComma = data.indexOf(',');
    int secondComma = data.indexOf(',', firstComma + 1);
    int thirdComma = data.indexOf(',', secondComma + 1);
    int fourthComma = data.indexOf(',', thirdComma + 1);
    int fifthComma = data.indexOf(',', fourthComma + 1);

    if(firstComma != -1 && secondComma != -1 && thirdComma != -1 && fourthComma != -1 && fifthComma != -1){

      float t = data.substring(0, firstComma).toFloat();
      float h = data.substring(firstComma + 1, secondComma).toFloat();
      float m = data.substring(secondComma + 1, thirdComma).toFloat();
      float l = data.substring(thirdComma + 1, fourthComma).toFloat();
      float pressure = data.substring(fourthComma + 1, fifthComma).toFloat();
      float alt = data.substring(fifthComma + 1).toFloat();

      
      // String httpRequestData = "api_key=" + apiKey + "&field1=" + String(t) + "&field2=" + String(h);
      String httpRequestData = "api_key=" + apiKey + "&field1=" + String(t) + "&field2=" + String(h) + "&field3=" + String(m) + "&field4=" + String(l) + "&field5=" + String(pressure) + "&field6=" + String(alt);

      WiFiClient client;
      HTTPClient http;
      http.begin(client, serverName);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");    
      int httpResponseCode = http.POST(httpRequestData);
      http.end();
      // delay(2000);s
    }
  }
}
