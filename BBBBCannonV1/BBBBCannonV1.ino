#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <WiFi.h>
#include <WebServer.h>

// AP credentials
const char* ap_ssid = "INADAZE BBBB Turret";

// Motor B (adjusted for your setup)
int motor2Pin1 = 25;
int motor2Pin2 = 26;
int enable2Pin = 27;

// Servo channels
int servoPanChannel = 0;
int servoTiltChannel = 1;
int servoTriggerChannel = 2;

// Laser pin
int laserPin = 32;

// PWM properties
int dutyCycle = 255; // Max duty cycle for full power
int pwmFreq = 60; // Servo frequency

// Initial servo positions
int panAngle = 90;
int tiltAngle = 85; // Starting at 85 degrees
int triggerAngle = 90;

// Laser state
bool laserState = false;

// Create PWM servo driver
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Web server running on port 80
WebServer server(80);

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize the motor pins as outputs
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);

  // Initialize the laser pin as output
  pinMode(laserPin, OUTPUT);
  digitalWrite(laserPin, LOW); // Ensure laser is off initially

  // Initialize PWM driver
  pwm.begin();
  pwm.setPWMFreq(pwmFreq);

  // Set initial servo positions
  setServoAngle(servoPanChannel, panAngle);
  setServoAngle(servoTiltChannel, tiltAngle);
  setServoAngle(servoTriggerChannel, triggerAngle);

  // Setup Access Point
  WiFi.softAP(ap_ssid);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Define web server routes
  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/stop", handleStop);
  server.on("/laserToggle", handleLaserToggle);
  server.on("/servoPanLeft", handleServoPanLeft);
  server.on("/servoPanRight", handleServoPanRight);
  server.on("/servoTiltUp", handleServoTiltUp);
  server.on("/servoTiltDown", handleServoTiltDown);
  server.on("/trigger", handleTrigger);
  server.on("/zero", handleZero);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Handle client requests
  server.handleClient();
}

void handleRoot() {
  String html = "<!DOCTYPE html><html lang='en'><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; display: flex; justify-content: center; align-items: center; height: 100vh; background-color: #e0e0e0; margin: 0; }";
  html += ".container { text-align: center; }";
  html += "h1 { font-size: 2rem; color: #333; margin-bottom: 20px; }";
  html += ".status { font-size: 1.5rem; color: #28a745; margin-bottom: 20px; }";
  html += "button { width: 100%; max-width: 300px; height: 60px; font-size: 1.5rem; border: none; border-radius: 30px; background: #e0e0e0; box-shadow: 20px 20px 60px #bebebe, -20px -20px 60px #ffffff; cursor: pointer; transition: all 0.3s ease; margin-bottom: 10px; }";
  html += "button:active { box-shadow: inset 20px 20px 60px #bebebe, inset -20px -20px 60px #ffffff; }";
  html += ".button-group { display: flex; flex-direction: column; align-items: center; }";
  html += ".button-row { display: flex; justify-content: center; gap: 10px; }";
  html += "</style>";
  html += "</head><body>";
  html += "<div class='container'>";
  html += "<h1>INADAZE BBBB Turret</h1>";
  html += "<div class='status' id='status'>System Active</div>";
  html += "<div class='button-group'>";
  html += "<button onclick='armSystem()'>ARM SYSTEM</button>";
  html += "<button onclick='stopSystem()'>STOP!</button>";
  html += "<button onclick='laserToggle()'>TOGGLE LASER</button>";
  html += "<div class='button-row'>";
  html += "<button onclick='servoTiltUp()'>UP</button>";
  html += "</div>";
  html += "<div class='button-row'>";
  html += "<button onclick='servoPanLeft()'>LEFT</button>";
  html += "<button onclick='servoPanRight()'>RIGHT</button>";
  html += "</div>";
  html += "<div class='button-row'>";
  html += "<button onclick='servoTiltDown()'>DOWN</button>";
  html += "</div>";
  html += "<button onclick='trigger()'>TRIGGER</button>";
  html += "<button onclick='zeroServos()'>ZERO</button>";
  html += "<div class='status'>Pan Angle: <span id='panAngle'>" + String(panAngle) + "</span></div>";
  html += "<div class='status'>Tilt Angle: <span id='tiltAngle'>" + String(tiltAngle) + "</span></div>";
  html += "</div>";
  html += "</div>";
  html += "<script>";
  html += "function armSystem() {";
  html += "  fetch('/arm').then(response => {";
  html += "    if (response.ok) {";
  html += "      document.getElementById('status').textContent = 'Turret Armed';";
  html += "      alert('System Armed');";
  html += "    } else {";
  html += "      alert('Failed to arm system');";
  html += "    }";
  html += "  });";
  html += "}";
  html += "function stopSystem() {";
  html += "  fetch('/stop').then(response => {";
  html += "    if (response.ok) {";
  html += "      document.getElementById('status').textContent = 'System Active';";
  html += "      alert('System Stopped');";
  html += "    } else {";
  html += "      alert('Failed to stop system');";
  html += "    }";
  html += "  });";
  html += "}";
  html += "function laserToggle() {";
  html += "  fetch('/laserToggle').then(response => response.json()).then(data => {";
  html += "    document.getElementById('status').textContent = data.laserState ? 'Laser ON' : 'Laser OFF';";
  html += "  });";
  html += "}";
  html += "function servoPanLeft() {";
  html += "  fetch('/servoPanLeft').then(response => response.json()).then(data => {";
  html += "    document.getElementById('panAngle').textContent = data.panAngle;";
  html += "  });";
  html += "}";
  html += "function servoPanRight() {";
  html += "  fetch('/servoPanRight').then(response => response.json()).then(data => {";
  html += "    document.getElementById('panAngle').textContent = data.panAngle;";
  html += "  });";
  html += "}";
  html += "function servoTiltUp() {";
  html += "  fetch('/servoTiltUp').then(response => response.json()).then(data => {";
  html += "    document.getElementById('tiltAngle').textContent = data.tiltAngle;";
  html += "  });";
  html += "}";
  html += "function servoTiltDown() {";
  html += "  fetch('/servoTiltDown').then(response => response.json()).then(data => {";
  html += "    document.getElementById('tiltAngle').textContent = data.tiltAngle;";
  html += "  });";
  html += "}";
  html += "function trigger() {";
  html += "  fetch('/trigger');";
  html += "}";
  html += "function zeroServos() {";
  html += "  fetch('/zero').then(response => {";
  html += "    if (response.ok) {";
  html += "      document.getElementById('panAngle').textContent = '90';";
  html += "      document.getElementById('tiltAngle').textContent = '85';"; // Starting at 85 degrees
  html += "    }";
  html += "  });";
  html += "}";
  html += "</script>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleArm() {
  Serial.println("System Armed");
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  analogWrite(enable2Pin, dutyCycle);
  laserState = true;
  digitalWrite(laserPin, HIGH);
  server.send(200, "text/plain", "OK");
}

void handleStop() {
  Serial.println("System Stopped");
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  analogWrite(enable2Pin, 0);
  laserState = false;
  digitalWrite(laserPin, LOW);
  server.send(200, "text/plain", "OK");
}

void handleLaserToggle() {
  laserState = !laserState;
  digitalWrite(laserPin, laserState ? HIGH : LOW);
  server.send(200, "application/json", "{\"laserState\":" + String(laserState) + "}");
}

void handleServoPanLeft() {
  panAngle += 10; // Increase angle for left
  if (panAngle > 180) panAngle = 180;
  setServoAngle(servoPanChannel, panAngle);
  server.send(200, "application/json", "{\"panAngle\":" + String(panAngle) + "}");
}

void handleServoPanRight() {
  panAngle -= 10; // Decrease angle for right
  if (panAngle < 0) panAngle = 0;
  setServoAngle(servoPanChannel, panAngle);
  server.send(200, "application/json", "{\"panAngle\":" + String(panAngle) + "}");
}

void handleServoTiltUp() {
  tiltAngle += 10; // Increase angle for up
  if (tiltAngle > 160) tiltAngle = 160;
  setServoAngle(servoTiltChannel, tiltAngle);
  server.send(200, "application/json", "{\"tiltAngle\":" + String(tiltAngle) + "}");
}

void handleServoTiltDown() {
  tiltAngle -= 10; // Decrease angle for down
  if (tiltAngle < 70) tiltAngle = 70;
  setServoAngle(servoTiltChannel, tiltAngle);
  server.send(200, "application/json", "{\"tiltAngle\":" + String(tiltAngle) + "}");
}

void handleTrigger() {
  Serial.println("Trigger Activated");
  setServoAngle(servoTriggerChannel, 140);
  delay(500);
  setServoAngle(servoTriggerChannel, 90);
  server.send(200, "text/plain", "OK");
}

void handleZero() {
  panAngle = 90;
  tiltAngle = 85; // Starting at 85 degrees
  setServoAngle(servoPanChannel, panAngle);
  setServoAngle(servoTiltChannel, tiltAngle);
  server.send(200, "text/plain", "OK");
}

void setServoAngle(uint8_t channel, int angle) {
  uint16_t pulseLength = map(angle, 0, 180, 122, 615); // Adjust these values if necessary
  pwm.setPWM(channel, 0, pulseLength);
}
