#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// Define RX and TX pins for the GPS module
#define RXPin 4
#define TXPin 3

// Create SoftwareSerial for GPS communication
SoftwareSerial gpsSerial(SerialR, TXPin);

// Create an instance of TinyGPS++
TinyGPSPlus gps;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  Serial.println("Initializing GPS Module...");
  
  // Initialize GPS Serial communication
  gpsSerial.begin(115200);
}

void loop() {
  // Process incoming GPS data
  while (gpsSerial.available() > 0) {
    Serial.print("GPS serial Available");
    char c = gpsSerial.read();
    if (gps.encode(c)) {
      // If valid GPS data is available, display location
      if (gps.location.isValid()) {
        Serial.print("Latitude: ");
        Serial.print(gps.location.lat(), 6); // Latitude with 6 decimal places
        Serial.print(" Longitude: ");
        Serial.println(gps.location.lng(), 6); // Longitude with 6 decimal places
      } else {
        Serial.println("Waiting for valid GPS signal...");
      }
    }
  }

  // Add a short delay for stability
  delay(1000);
}