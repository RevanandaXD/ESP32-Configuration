#include <WiFi.h>  

const char* ssid     = "Full 24 Jam"; // Ganti SSID Sesuai dengan Hostpot/Wifi Yang Anda ingim Gunakan
const char* password = "12345678"; // Ganti Password sesuai hospot/wifi

WiFiServer server(80);

// Sesuaikan dengan PIN Yang Anda Gunakan
#define LED1 2 
#define LED2 16
#define LED3 5
#define LED4 19

void setup() {
  Serial.begin(115200);
  delay(10);

  // Set pin LED sebagai output
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  // Matikan semua LED di awal
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);

  // Koneksi WiFi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  server.begin();
  Serial.println("Server started");

  // Tampilkan alamat IP
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop() {
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

  // Kontrol LED berdasarkan request
  if (request.indexOf("/light1on") > 0)  digitalWrite(LED1, HIGH);
  if (request.indexOf("/light1off") > 0) digitalWrite(LED1, LOW);

  if (request.indexOf("/light2on") > 0)  digitalWrite(LED2, HIGH);
  if (request.indexOf("/light2off") > 0) digitalWrite(LED2, LOW);

  if (request.indexOf("/light3on") > 0)  digitalWrite(LED3, HIGH);
  if (request.indexOf("/light3off") > 0) digitalWrite(LED3, LOW);

  if (request.indexOf("/light4on") > 0)  digitalWrite(LED4, HIGH);
  if (request.indexOf("/light4off") > 0) digitalWrite(LED4, LOW);

  // Kirim respon HTML ke browser
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head><title>ESP32 Device Control</title></head>");
  client.println("<body bgcolor='#f7e6ec'>");

  client.println("<h2><center>ESP32 Electrical Device Control</center></h2>");
  client.println("<hr/>");

  // Kontrol Device 1
  client.println("<p>Device 1: ");
  client.println("<a href='/light1on'><button>Turn On</button></a>");
  client.println("<a href='/light1off'><button>Turn Off</button></a></p>");

  // Kontrol Device 2
  client.println("<p>Device 2: ");
  client.println("<a href='/light2on'><button>Turn On</button></a>");
  client.println("<a href='/light2off'><button>Turn Off</button></a></p>");

  // Kontrol Device 3
  client.println("<p>Device 3: ");
  client.println("<a href='/light3on'><button>Turn On</button></a>");
  client.println("<a href='/light3off'><button>Turn Off</button></a></p>");

  // Kontrol Device 4
  client.println("<p>Device 4: ");
  client.println("<a href='/light4on'><button>Turn On</button></a>");
  client.println("<a href='/light4off'><button>Turn Off</button></a></p>");

  // Tabel status
  client.println("<hr/><center><table border='1' cellpadding='10'>");
  client.println("<tr>");
  client.print("<td>Light 1: "); client.print(digitalRead(LED1) ? "ON" : "OFF"); client.println("</td>");
  client.print("<td>Light 2: "); client.print(digitalRead(LED2) ? "ON" : "OFF"); client.println("</td>");
  client.println("</tr><tr>");
  client.print("<td>Light 3: "); client.print(digitalRead(LED3) ? "ON" : "OFF"); client.println("</td>");
  client.print("<td>Light 4: "); client.print(digitalRead(LED4) ? "ON" : "OFF"); client.println("</td>");
  client.println("</tr>");
  client.println("</table></center>");

  client.println("</body></html>");

  delay(1);
  Serial.println("Client disconnected\n");
}

