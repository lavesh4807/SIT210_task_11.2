#include "HX711.h"
#include "DHT.h"
#include <WiFiNINA.h>


char ssid[] = "Lavi";
char password[] = "Laveshgarg";
char server[] = "192.168.156.191";  
int port = 8000;

const int DT_PIN = 6;   
const int SCK_PIN = 7;  
const int ECG_LOP_PIN = 4; 
const int ECG_LON_PIN = 11;
const int MQ135_PIN = A1;   
const int DHT_PIN = 2;      
const float REQUIRED_HUMIDITY = 60.0;
const float MIN_TEMPERATURE = 32.5;  
const float MAX_TEMPERATURE = 37.5;  

HX711 scale;
DHT dht(DHT_PIN, DHT11);

void setup() {
   // Connect to Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    delay(10000); // Attempt to connect for 10 seconds
  }
  Serial.println("Connected to WiFi");
  Serial.begin(9600);
  scale.begin(DT_PIN, SCK_PIN);
  dht.begin();
  
  Serial.println("Baby Incubator System Initialized.");
  delay(2000);  // Give some time for initialization
}

void loop() {
  // Weight Measurement
  if (scale.is_ready()) {
    long weight = scale.get_units(10); // Average the reading over 10 samples for stability
    Serial.print("Weight: ");
    Serial.print(weight);
    Serial.println(" grams");
  } else {
    Serial.println("Error reading from HX711. Check your connections.");
  }

  // ECG Measurement
  int ecgValueLOP = analogRead(ECG_LOP_PIN);
  int ecgValueLON = analogRead(ECG_LON_PIN);
  Serial.print("ECG LO+ Value: ");
  Serial.print(ecgValueLOP);
  Serial.print("\tECG LO- Value: ");
  Serial.println(ecgValueLON);

  // Gas Measurement
  int mq135Value = analogRead(MQ135_PIN);
  Serial.print("MQ135 Value: ");
  Serial.println(mq135Value);

  // Temperature and Humidity Measurement
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C\tHumidity: ");
  Serial.print(humidity);
  Serial.println(" %");
 // Create the request string
  String data = "temperature=" + String(temperature) +
                "&humidity=" + String(humidity) +
                "&weight=" + String(scale.get_units(10)) + // You may adjust the number of samples
                "&ecg_lop=" + String(analogRead(ECG_LOP_PIN)) +
                "&ecg_lon=" + String(analogRead(ECG_LON_PIN)) +
                "&mq135=" + String(analogRead(MQ135_PIN));

  // Send the data to the local server
  if (sendDataToServer(data)) {
    Serial.println("Data sent successfully.");
  } else {
    Serial.println("Failed to send data.");
  }

  delay(1000); // Delay for a second, adjust as needed
}

bool sendDataToServer(String data) {
  WiFiClient client;
    if (sendDataToServer(data, temperature, humidity)) {
    Serial.println("Data sent successfully.");
  } else {
    Serial.println("Failed to send data.");
  }

  delay(1000); // Delay for a second, adjust as needed
}

bool sendDataToServer(String data, float temperature, float humidity) {
  WiFiClient client;
  
  // Connect to the server
  if (client.connect(server, port)) {
    Serial.println("Connected to server.");

    // Make an HTTP POST request
    client.println("POST /your-endpoint HTTP/1.1");
    client.println("Host: " + String(server));
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Content-Length: " + String(data.length()));
    client.println();
    client.println(data);

    delay(1000); // Allow time for the server to process the request
  // Read the server's response
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }

    // Disconnect from the server
    client.stop();
    Serial.println("\nDisconnected from server.");
    return true;
  } else {
    Serial.println("Connection to server failed.");
    return false;
  }

 if (humidity < REQUIRED_HUMIDITY) {
    Serial.println("Humidity below required level. Activating Humidifier.");
  } else {
    Serial.println("Humidity at or above required level. Deactivating Humidifier.");
  }
// Temperature Control Logic
  if (temperature < MIN_TEMPERATURE) {
    Serial.println("Temperature below required level. Activating Heater.");
  } else if (temperature > MAX_TEMPERATURE) {
    Serial.println("Temperature above required level. Activating Cooler/Fan.");
  } else {
    Serial.println("Temperature within required range. Deactivating Heater and Cooler/Fan.");
  }
  delay(1000); // Delay for a second, adjust as needed
}
