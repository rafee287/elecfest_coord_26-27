#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include <MPU6050_light.h>

// Your network credentials
const char* ssid = "ssid";
const char* password = "passwd";

MPU6050 mpu(Wire);
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

unsigned long sensorTimer = 0;
unsigned long cleanupTimer = 0;

// HTML & JavaScript Dashboard with Accelerometer elements added
const char index_html[] PROGMEM = R"rawhtml(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 MPU6050 Custom Scaling</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { font-family: Arial; text-align: center; background: #f4f4f9; margin: 0; padding: 20px; }
        h2 { color: #333; margin-bottom: 5px; }
        h3 { color: #555; margin-top: 25px; border-bottom: 2px solid #ddd; padding-bottom: 5px; }
        .card-container { display: flex; justify-content: center; gap: 20px; flex-wrap: wrap; margin-top: 15px; }
        .card { background: white; padding: 20px; border-radius: 10px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); min-width: 150px; }
        .card h4 { margin: 0 0 10px 0; color: #666; }
        .value { font-size: 2rem; font-weight: bold; }
        .gyro-val { color: #28a745; }
        .accel-val { color: #007bff; }
    </style>
</head>
<body>
    <h2>ESP32 MPU6050 Telemetry Dashboard</h2>
    
    <h3>Rotational Angles (Degrees)</h3>
    <div class="card-container">
        <div class="card"><h4>Scaled Roll (X)</h4><div id="roll" class="value gyro-val">0.00&deg;</div></div>
        <div class="card"><h4>Scaled Pitch (Y)</h4><div id="pitch" class="value gyro-val">0.00&deg;</div></div>
        <div class="card"><h4>Yaw (Z)</h4><div id="yaw" class="value gyro-val">0.00&deg;</div></div>
    </div>

    <h3>Acceleration Forces (g)</h3>
    <div class="card-container">
        <div class="card"><h4>Accel X</h4><div id="ax" class="value accel-val">0.00g</div></div>
        <div class="card"><h4>Accel Y</h4><div id="ay" class="value accel-val">0.00g</div></div>
        <div class="card"><h4>Accel Z</h4><div id="az" class="value accel-val">0.00g</div></div>
    </div>

    <script>
        var gateway = `ws://${window.location.hostname}/ws`;
        var websocket = new WebSocket(gateway);
        websocket.onmessage = function(event) {
            var data = JSON.parse(event.data);
            
            // Updating Angle Displays
            document.getElementById('roll').innerHTML = data.roll + "&deg;";
            document.getElementById('pitch').innerHTML = data.pitch + "&deg;";
            document.getElementById('yaw').innerHTML = data.yaw + "&deg;";
            
            // Added: Updating Acceleration Displays
            document.getElementById('ax').innerHTML = data.ax + "g";
            document.getElementById('ay').innerHTML = data.ay + "g";
            document.getElementById('az').innerHTML = data.az + "g";
        };
    </script>
</body>
</html>
)rawhtml";

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {}

void setup() {
  Serial.begin(115200);
  Wire.begin();
     
  if (mpu.begin() != 0) { 
    Serial.println("MPU6050 find failed!");
    while(1) { delay(10); } 
  }
  
  delay(1000);
  mpu.calcOffsets(true, true);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  ws.onEvent(onEvent);
  server.addHandler(&ws);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
  server.begin();
}

void loop() {
  mpu.update();

  if (millis() - cleanupTimer > 1000) {
    ws.cleanupClients();
    cleanupTimer = millis();
  }

  if (millis() - sensorTimer > 50) {
    sensorTimer = millis();

    float rawX = mpu.getAngleX();
    float rawY = mpu.getAngleY();
    float rawZ = mpu.getAngleZ();

    float scaledX = rawX * 3.0;
    float scaledY = rawY * 3.21428;

    float accX = mpu.getAccX();
    float accY = mpu.getAccY();
    float accZ = mpu.getAccZ();

    String jsonString;
    jsonString.reserve(150); 
    jsonString = "{\"roll\":\""  + String(scaledY, 2) + 
                 "\",\"pitch\":\"" + String(scaledX, 2) + 
                 "\",\"yaw\":\""   + String(rawZ, 2) + 
                 "\",\"ax\":\""    + String(accX, 2) + 
                 "\",\"ay\":\""    + String(accY, 2) + 
                 "\",\"az\":\""    + String(accZ, 2) + "\"}";
                             
    if (ws.count() > 0) {
      ws.textAll(jsonString);
    }
  }
}
