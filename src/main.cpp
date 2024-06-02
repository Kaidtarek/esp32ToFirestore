#include <WiFi.h>
#include <HTTPClient.h>
#include <map>

const char *ssid = "F11Pro";
const char *password = "1144114411";
const char *firestoreHost = "firestore.googleapis.com";
const int firestorePort = 443;
const char *firestoreProjectID = "customesp32";
const char *firestoreCollectionID = "parkData";
const char *firestoreDocumentID = "UnivPark"; 
const char *firestoreAPIKey = "AIzaSyAGopwhyX6dQKfpB3AV-6hNkaL7xb8hSoU";

void sendDataToFirestore(); 

struct MyData
{
  int key;
  bool value;
};

MyData myData[] = {
  {1, true},
  {2, false},
  {3, true},
  {4, false},
  {5, true},
  {6, false},
};

void setup()
{
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  sendDataToFirestore();
}

void loop()
{
}

void sendDataToFirestore()
{
  HTTPClient http;

  // Construct Firestore URL with document ID
  String url = "https://" + String(firestoreHost) + "/v1/projects/" + String(firestoreProjectID) + "/databases/(default)/documents/" + String(firestoreCollectionID) + "/" + String(firestoreDocumentID) + "?key=" + String(firestoreAPIKey);
  
  // Construct Firestore JSON data with map data
  String data = "{\"fields\": {\"parkData\": {\"mapValue\": {\"fields\": {";

  int myDataSize = sizeof(myData) / sizeof(myData[0]);
  for (int i = 0; i < myDataSize; i++) {
    data += "\"" + String(myData[i].key) + "\": {\"stringValue\": \"" + (myData[i].value ? "true" : "false") + "\"}";
    if (i < myDataSize - 1) {
      data += ",";
    }
  }

  data += "}}}}}";

  Serial.println("JSON Payload:");
  Serial.println(data);

  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.PATCH(data); 

  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String response = http.getString();
    Serial.println(response);
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  
  http.end();
}
