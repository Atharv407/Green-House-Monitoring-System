#include <DHT.h>
#define dhtPin 2
#define dhtType DHT22
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <SoftwareSerial.h>
#define fan_pin 4
#define pump_pin 7

#define seaLevelPressure_hPa 1013.25

Adafruit_BMP085 bmp;

const int soil_sensor_pin = A0;
const int ldr_sensor_pin = A1;
DHT dht(dhtPin, dhtType);
void setup() {
  Serial.begin(115200);
  // pinMode(ledpin, OUTPUT);
  pinMode(fan_pin, OUTPUT);
  pinMode(pump_pin, OUTPUT);
  dht.begin();
  // while (!bmp.begin()) {
	//   Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  //   delay(2000);
  // }
}

float ktfilter(float z) {
    static const float F = 1;
    static const float H = 1;
    static float P = 0;
    static float K = 0;
    static float Q = 1;
    static float x = 0;
    static float R = 40;

    // prediction step
    P = F*P + Q;
    x = F*x;

    // update step
    float y = z -H*x;
    float S = H*P*H + R;
    K = (P*H)/(S);
    x = x + K*(y);
    P = (1-K*H)*P;
    return x;
}
float khfilter(float z) {
    static const float F = 1;
    static const float H = 1;
    static float P = 0;
    static float K = 0;
    static float Q = 1;
    static float x = 0;
    static float R = 40;

    // prediction step
    P = F*P + Q;
    x = F*x;

    // update step
    float y = z -H*x;
    float S = H*P*H + R;
    K = (P*H)/(S);
    x = x + K*(y);
    P = (1-K*H)*P;
    return x;
}
float kmfilter(float z) {
    static const float F = 1;
    static const float H = 1;
    static float P = 0;
    static float K = 0;
    static float Q = 1;
    static float x = 0;
    static float R = 40;

    // prediction step
    P = F*P + Q;
    x = F*x;

    // update step
    float y = z -H*x;
    float S = H*P*H + R;
    K = (P*H)/(S);
    x = x + K*(y);
    P = (1-K*H)*P;
    return x;
}
float klfilter(float z) {
    static const float F = 1;
    static const float H = 1;
    static float P = 0;
    static float K = 0;
    static float Q = 1;
    static float x = 0;
    static float R = 40;

    // prediction step
    P = F*P + Q;
    x = F*x;

    // update step
    float y = z -H*x;
    float S = H*P*H + R;
    K = (P*H)/(S);
    x = x + K*(y);
    P = (1-K*H)*P;
    return x;
}
float kafilter(float z) {
    static const float F = 1;
    static const float H = 1;
    static float P = 0;
    static float K = 0;
    static float Q = 1;
    static float x = 0;
    static float R = 40;

    // prediction step
    P = F*P + Q;
    x = F*x;

    // update step
    float y = z -H*x;
    float S = H*P*H + R;
    K = (P*H)/(S);
    x = x + K*(y);
    P = (1-K*H)*P;
    return x;
}
float kpfilter(float z) {
    static const float F = 1;
    static const float H = 1;
    static float P = 0;
    static float K = 0;
    static float Q = 1;
    static float x = 0;
    static float R = 40;

    // prediction step
    P = F*P + Q;
    x = F*x;

    // update step
    float y = z -H*x;
    float S = H*P*H + R;
    K = (P*H)/(S);
    x = x + K*(y);
    P = (1-K*H)*P;
    return x;
}

void loop() {
  // Serial.println(digitalRead(ledpin));
  
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  if(isnan(temp) || isnan(hum)){
    Serial.println("No data Available");
  }
  temp = ktfilter(temp);
  hum = khfilter(hum);
  Serial.print(temp);
  Serial.print(",");
  Serial.print(hum);
  Serial.print(",");

  float moisture_percentage;
  int sensor_analog1, sensor_analog2;
  sensor_analog1 = analogRead(soil_sensor_pin);
  moisture_percentage = ( 100 - ( (sensor_analog1/1023.00) * 100 ) );
  moisture_percentage = kmfilter(moisture_percentage);
  Serial.print(moisture_percentage);
  Serial.print(",");

  sensor_analog2 = analogRead(ldr_sensor_pin);
  int lightIntensity = 100 - map(sensor_analog2, 0, 300, 0, 100);
  lightIntensity = analogRead(ldr_sensor_pin);
  Serial.println();
  Serial.println(lightIntensity);
  Serial.println();
  // if (analogRead(ledpin) == HIGH ) {
  //   Serial.println(696969);
    
  //   digitalWrite(ledpin, LOW);
  // }
  // else{
  //   Serial.println(420);
  //   digitalWrite(ledpin, HIGH);
  // }
  lightIntensity = klfilter(lightIntensity);
  digitalWrite(fan_pin, LOW);
  digitalWrite(pump_pin, HIGH);
  Serial.print(100-lightIntensity);
  Serial.print(",");

  float pressure = 100;
  pressure = kpfilter(pressure);
  Serial.print(pressure);
  Serial.print(",");
  
  float altitude = 100;
  altitude = kafilter(altitude);
  Serial.print(altitude);

  Serial.println();

  delay(1000);
}