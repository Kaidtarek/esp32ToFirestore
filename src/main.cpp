
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "F11Pro";
const char* password = "1144114411";

const char* firestoreHost = "firestore.googleapis.com";
const int firestorePort = 443;
const char* firestoreProjectID = "customesp32";
const char* firestoreCollectionID = "parkData";
const char* firestoreAPIKey = "AIzaSyAGopwhyX6dQKfpB3AV-6hNkaL7xb8hSoU";

void sendDataToFirestore(); // Function prototype declaration

void setup() {
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  sendDataToFirestore();
}

void loop() {
  // Nothing to do here for a simple one-time data upload
}

void sendDataToFirestore() {
  HTTPClient http;
  
  // Construct Firestore URL
  String url = "https://" + String(firestoreHost) + "/v1/projects/" + String(firestoreProjectID) + "/databases/(default)/documents/" + String(firestoreCollectionID) + "?key=" + String(firestoreAPIKey);

  // Construct Firestore JSON data
  String data = "{\"fields\": {\"sensorValue\": {\"integerValue\": 123}}}";

  // Send POST request to Firestore
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(data);
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}
