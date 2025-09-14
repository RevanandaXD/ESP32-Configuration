#include <WiFi.h>   // Library WiFi untuk ESP32

const char* ssid     = "Full 24 Jam";  // SSID WiFi
const char* password = "12345678";       // Password WiFi

int LED1 = 2; // 
WiFiServer server(80); // Server berjalan di port 80

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, LOW);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  Serial.println("Server started");

  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.println("New client connected");
  while (!client.available()) {
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  int value = LOW;
  if (request.indexOf("/LED=ON") != -1) {
    digitalWrite(LED1, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1) {
    digitalWrite(LED1, LOW);
    value = LOW;
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); // wajib ada
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.print("LED pin is now: ");
  if (value == HIGH) {
    client.print("On");
  } else {
    client.print("Off");
  }

  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"><button>Turn On</button></a>");
  client.println("<a href=\"/LED=OFF\"><button>Turn Off</button></a><br />");
  client.println("</html>");

  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
}
