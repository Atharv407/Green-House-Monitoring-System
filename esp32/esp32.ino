#include <WiFi.h>
#include <HTTPClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const String ssid = "P7F";
const String password = "iiitbbsr";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const String serverName = "http://api.thingspeak.com/update";
String apiKey = "AXYQR2JICGS60SC5";

const char index_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>
<body>
    <h1>Served</h1>
</body>
</html>
)rawliteral";

void notifyClients() {
  ws.textAll("Walter White is Heisenberg");
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    // Serial.println(data)
    data[len] = 0;
    if (strcmp((char*)data, "Kanye") == 0) {
      // ledState = !ledState;
      Serial.println(0);
      notifyClients();
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String& var){
  Serial.println(var);
  // if(var == "STATE"){
  //   if (1){
  //     return "ON";
  //   }
  //   else{
  //     return "OFF";
  //   }
  // }
  return String();
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  // Serial.println("Connecting to " + ssid);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  initWebSocket();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  server.begin();
}

// void loop() {
//   if (Serial.available()) {

//     String data = Serial.readStringUntil('\n');
//     int firstComma = data.indexOf(',');
//     int secondComma = data.indexOf(',', firstComma + 1);
//     int thirdComma = data.indexOf(',', secondComma + 1);
//     int fourthComma = data.indexOf(',', thirdComma + 1);
//     int fifthComma = data.indexOf(',', fourthComma + 1);

//     if(firstComma != -1 && secondComma != -1 && thirdComma != -1 && fourthComma != -1 && fifthComma != -1){

//       float t = data.substring(0, firstComma).toFloat();
//       float h = data.substring(firstComma + 1, secondComma).toFloat();
//       float m = data.substring(secondComma + 1, thirdComma).toFloat();
//       float l = data.substring(thirdComma + 1, fourthComma).toFloat();
//       float pressure = data.substring(fourthComma + 1, fifthComma).toFloat();
//       float alt = data.substring(fifthComma + 1).toFloat();

      
//       // String httpRequestData = "api_key=" + apiKey + "&field1=" + String(t) + "&field2=" + String(h);
//       String httpRequestData = "api_key=" + apiKey + "&field1=" + String(t) + "&field2=" + String(h) + "&field3=" + String(m) + "&field4=" + String(l) + "&field5=" + String(pressure) + "&field6=" + String(alt);

//       WiFiClient client;
//       HTTPClient http;
//       http.begin(client, serverName);
//       http.addHeader("Content-Type", "application/x-www-form-urlencoded");    
//       int httpResponseCode = http.POST(httpRequestData);
//       http.end();
//       // delay(2000);s
//     }
//   }
// }

void loop() {
  ws.cleanupClients();
  // digitalWrite(ledPin, ledState);
}