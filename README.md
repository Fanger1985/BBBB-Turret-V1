INADAZE BBBB Turret by Bruce Fanger - All INADAZE - BB1-zero BB Cannon/Turret http://www.thefanger.com http://www.allinadaze.com

Overview
Welcome to the INADAZE BBBB Turret project! This badass turret system is equipped with a laser, pan and tilt servos, and a trigger mechanism, all controlled via an ESP32 with a web-based control panel. Whether you're aiming to protect your turf or just have some fun, this project has you covered.

Features
Pan and Tilt Control: Precisely control the pan and tilt servos to adjust the turret's aim.
Laser Control: Toggle the laser on and off, with automatic activation when the system is armed.
Trigger Mechanism: Activate the trigger servo to simulate firing.
Zeroing Function: Reset the servos to their original positions for easy calibration.
Web-based Control Panel: Conveniently control all turret functions from your web browser.
Access Point Mode: Host your own network for easy setup and control.
GPIO Pin Map
Here's the pin configuration for connecting the components to the ESP32:

Motor B:
motor2Pin1 -> GPIO 25
motor2Pin2 -> GPIO 26
enable2Pin -> GPIO 27
Servo Controller (PCA9685):
VCC -> 3.3V or 5V (depending on your PCA9685)
GND -> GND
SDA -> GPIO 21
SCL -> GPIO 22
Servos:
servoPanChannel -> PCA9685 Channel 0
servoTiltChannel -> PCA9685 Channel 1
servoTriggerChannel -> PCA9685 Channel 2
Laser:
laserPin -> GPIO 32




Install Dependencies:

Make sure you have the following libraries installed in your Arduino IDE:
Adafruit PWM Servo Driver
WiFi
WebServer
Wire
Upload the Sketch:

Open inadaze_bbbb_turret.ino in the Arduino IDE.
Select your ESP32 board and the correct port.
Upload the sketch to your ESP32.
Usage
Power Up:

Connect the ESP32 to a power source.
The ESP32 will create an access point named INADAZE BBBB Turret.
Connect to the Access Point:

Open your Wi-Fi settings and connect to INADAZE BBBB Turret.
Access the Control Panel:

Open a web browser and navigate to the access point's IP address (typically 192.168.4.1).
Control the Turret:

Use the web-based control panel to pan, tilt, toggle the laser, trigger, and zero the servos.
Control Panel Functions
ARM SYSTEM: Arms the turret and activates the laser.
STOP!: Disarms the turret and deactivates the laser.
TOGGLE LASER: Manually toggle the laser on or off.
UP: Tilt the turret up.
DOWN: Tilt the turret down.
LEFT: Pan the turret left.
RIGHT: Pan the turret right.
TRIGGER: Activate the trigger servo.
ZERO: Reset the servos to their original positions.
Initial Servo Positions
Pan: 90 degrees
Tilt: 85 degrees (default start)
Trigger: 90 degrees
