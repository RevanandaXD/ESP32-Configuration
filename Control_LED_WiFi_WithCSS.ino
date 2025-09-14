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
  client.println("<html lang=\"en\">");
  client.println("<head>");
  client.println("<meta charset=\"UTF-8\">");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
  client.println("<title>ESP Web Server</title>");
  client.println("<link href=\"https://fonts.googleapis.com/css2?family=Poppins:wght@400;600&display=swap\" rel=\"stylesheet\">"); 
  client.println("<style>");
  client.println("body { font-family: 'Poppins', sans-serif; text-align: center; background-color: #f4f4f9; margin: 0; padding: 20px; }");
  client.println("h1 { color: #333; margin-bottom: 5px; font-size: 3rem}");
  client.println("p { font-size: 14px; margin-bottom: 15px; }");
  client.println("button { padding: 5px 17px; font-size: 0.7rem; border-radius: 3px; cursor: pointer; font-family: 'Poppins', sans-serif; font-weight: 600; transition: all 200ms ease; }");
  client.println(".on { background-color: transparent; border: 1.5px solid #1fd655; color: #1fd655;} ");
  client.println(".on:hover { background-color: #1fd655; color: #fff;}");
  client.println(".off { background-color: transparent; color: #e60000; border: 1.5px solid #e60000} ");
  client.println(".off:hover { background-color: #e60000; color: #fff}");
  client.println("</style>");
  client.println("</head>");

  client.println("<body>");
  client.println("<h1>ESP LED Control</h1>");
  client.print("<p>LED pin is now: <strong>");
  if (value == HIGH) {
    client.print("ON");
  } else {
    client.print("OFF");
  }
  client.println("</strong></p>");
  client.println("<a href=\"/LED=ON\"><button class=\"on\">Turn On</button></a>");
  client.println("<a href=\"/LED=OFF\"><button class=\"off\">Turn Off</button></a>");
  client.println("</body>");
  client.println("</html>");


  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
}



