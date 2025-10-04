#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// === CONFIG WIFI ===
const char* ssid = "TonSSID";
const char* password = "TonMotDePasse";

// === PELTIER ===
const int peltierPin = 12;  // GPIO PWM
int pwmValue = 0;

// === DS18B20 ===
#define ONE_WIRE_BUS 13
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float tempC = 0;

// === SERVEUR WEB ===
WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  // PWM Peltier
  ledcAttachPin(peltierPin, 0);
  ledcSetup(0, 1000, 10);
  ledcWrite(0, pwmValue);

  // DS18B20
  sensors.begin();

  // Connexion Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connexion au Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnecté!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  String req = "";
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      req += c;
      if (c == '\n') break;
    }
  }

  // Lire paramètre PWM depuis URL
  if (req.indexOf("GET /?pwm=") >= 0) {
    int idx = req.indexOf("pwm=") + 4;
    int idx2 = req.indexOf(" ", idx);
    String pwmStr = req.substring(idx, idx2);
    pwmValue = pwmStr.toInt();
    pwmValue = constrain(pwmValue, 0, 1023);
    ledcWrite(0, pwmValue);
  } 
  else if (req.indexOf("GET /?action=up") >= 0) {
    pwmValue = min(pwmValue + 50, 1023);
    ledcWrite(0, pwmValue);
  } 
  else if (req.indexOf("GET /?action=down") >= 0) {
    pwmValue = max(pwmValue - 50, 0);
    ledcWrite(0, pwmValue);
  }

  // Lire température
  sensors.requestTemperatures();
  tempC = sensors.getTempCByIndex(0);

  // Générer page HTML moderne
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta http-equiv="refresh" content="5">
<title>ESP32 Peltier Dashboard</title>
<style>
  body { font-family: Arial; background:#111; color:#0f0; text-align:center; margin:0; }
  h1 { color:#0ff; margin:20px 0; }
  .temp { font-size: 2em; color:#0ff; margin:10px 0; }
  .slider { width: 60%; margin:20px auto; }
  button { padding:10px 20px; margin:5px; background:#222; color:#0f0; border:1px solid #0f0; cursor:pointer; font-size:1em; }
  canvas { background:#000; display:block; margin:20px auto; border:1px solid #0f0; }
</style>
</head>
<body>
<h1>ESP32 Peltier Controller</h1>
<p class="temp">Température: )rawliteral" + String(tempC) + R"rawliteral( °C</p>
<p>Puissance PWM: )rawliteral" + String(pwmValue) + R"rawliteral(/1023</p>

<form action="/" method="get">
<input type="range" name="pwm" min="0" max="1023" value=")rawliteral" + String(pwmValue) + R"rawliteral(" class="slider" onchange="this.form.submit()"><br>
<button type="submit" name="action" value="up">+</button>
<button type="submit" name="action" value="down">-</button>
</form>

<canvas id="pwmGraph" width="600" height="200"></canvas>

<script>
let canvas = document.getElementById('pwmGraph');
let ctx = canvas.getContext('2d');
ctx.clearRect(0,0,600,200);
ctx.fillStyle = '#0f0';
ctx.fillRect(0, 200 - )rawliteral" + String(pwmValue*0.195) + R"rawliteral(", 600, 5);
</script>
</body>
</html>
)rawliteral";

  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();
  client.println(html);
  client.stop();
}
