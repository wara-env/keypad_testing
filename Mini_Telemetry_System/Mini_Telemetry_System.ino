#include <ESP8266WiFi.h>
#include <DHT.h>

// =========================
// WIFI
// =========================
const char* ssid = "jmk48";
const char* password = "kingnasir";

WiFiServer server(80);

// =========================
// PIN SENSOR
// =========================
#define DHTPIN D5
#define DHTTYPE DHT11
#define LDR_PIN A0

// =========================
// DHT OBJECT
// =========================
DHT dht(DHTPIN, DHTTYPE);

// =========================
// SETUP
// =========================
void setup() {

  Serial.begin(9600);

  dht.begin();

  Serial.println();
  Serial.println("================================");
  Serial.println("     MINI TELEMETRY SYSTEM");
  Serial.println("================================");
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
  Serial.println();
}


// =========================
// LOOP
// =========================
void loop() {

  WiFiClient client = server.available();

  if (!client) {
    return;
  }

  Serial.println("Client terhubung!");

  while (!client.available()) {
    delay(1);
  }

  // =========================
  // BACA REQUEST
  // =========================
  String request = client.readStringUntil('\r');

  Serial.println("Request:");
  Serial.println(request);


  // =====================================================
  // REQUEST DATA SENSOR
  // =====================================================

if (request.indexOf("GET /data") >= 0) {

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  int light = analogRead(LDR_PIN);

  String json = "{";
  json += "\"temperature\":";
  json += isnan(temperature) ? "null" : String(temperature);
  json += ",";
  json += "\"humidity\":";
  json += isnan(humidity) ? "null" : String(humidity);
  json += ",";
  json += "\"light\":";
  json += String(light);
  json += "}";

  Serial.print("JSON: ");
  Serial.println(json);

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(json.length());
  client.println("Connection: close");
  client.println();

  client.print(json);

  client.flush();
  delay(50);
  client.stop();

  Serial.println("JSON berhasil dikirim!");

  return;
}


  // =====================================================
  // REQUEST HALAMAN UTAMA
  // =====================================================

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();


  // =========================
  // HTML
  // =========================

  client.println("<!DOCTYPE html>");
  client.println("<html>");

  client.println("<head>");
  client.println("<meta charset='UTF-8'>");
  client.println("<title>ESP8266 Telemetry</title>");

  client.println("<style>");

  client.println("body {");
  client.println("font-family: Arial;");
  client.println("text-align: center;");
  client.println("margin-top: 50px;");
  client.println("}");

  client.println(".data {");
  client.println("font-size: 24px;");
  client.println("margin: 15px;");
  client.println("}");

  client.println("</style>");

  client.println("</head>");


  // =========================
  // BODY
  // =========================

  client.println("<body>");

  client.println("<h1>ESP8266 Telemetry</h1>");

  client.println("<h2>Status: ONLINE</h2>");

  client.println("<div class='data'>");
  client.println("Temperature: <span id='temperature'>--</span> &deg;C");
  client.println("</div>");

  client.println("<div class='data'>");
  client.println("Humidity: <span id='humidity'>--</span> %");
  client.println("</div>");

  client.println("<div class='data'>");
  client.println("Light (LDR): <span id='light'>--</span>");
  client.println("</div>");

  client.println("<p>IP Address: ");
  client.print(WiFi.localIP());
  client.println("</p>");


  // =====================================================
  // JAVASCRIPT
  // =====================================================

  client.println("<script>");

  client.println("function updateData() {");

  client.println("fetch('/data')");
  
  client.println(".then(response => response.json())");

  client.println(".then(data => {");

  client.println("document.getElementById('temperature').textContent = data.temperature;");

  client.println("document.getElementById('humidity').textContent = data.humidity;");

  client.println("document.getElementById('light').textContent = data.light;");

  client.println("})");

  client.println(".catch(error => {");

  client.println("console.log('Error:', error);");

  client.println("});");

  client.println("}");

  // Jalankan pertama kali
  client.println("updateData();");

  // Update setiap 2 detik
  client.println("setInterval(updateData, 2000);");

  client.println("</script>");


  client.println("</body>");
  client.println("</html>");


  delay(10);

  client.stop();

  Serial.println("Halaman web dikirim!");
}