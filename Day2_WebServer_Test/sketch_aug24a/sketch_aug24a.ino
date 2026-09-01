#include <ESP8266WiFi.h>

const char* ssid = "jmk48";
const char* password = "kingnasir";

WiFiServer server(80);

void setup() {
  Serial.begin(9600);

  Serial.println();
  Serial.println("Menghubungkan ke Wi-Fi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi terhubung!");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();

  Serial.println("Web server dimulai!");
}

void loop() {
  WiFiClient client = server.available();

  if (!client) {
    return;
  }

  Serial.println("Client terhubung!");

  // Tunggu request dari browser
  while (!client.available()) {
    delay(1);
  }

  // Baca request pertama
  String request = client.readStringUntil('\r');

  Serial.println("Request:");
  Serial.println(request);

  // HTTP response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();

  // HTML
  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<head>");
  client.println("<meta charset='UTF-8'>");
  client.println("<title>ESP8266 Telemetry</title>");
  client.println("</head>");

  client.println("<body>");
  client.println("<h1>ESP8266 Telemetry</h1>");

  client.println("<p>Status: ONLINE</p>");

  client.print("<p>IP Address: ");
  client.print(WiFi.localIP());
  client.println("</p>");

  client.print("<p>Uptime: ");
  client.print(millis() / 1000);
  client.println(" detik</p>");

  client.println("</body>");
  client.println("</html>");

  delay(10);

  client.stop();

  Serial.println("Client disconnected!");
}