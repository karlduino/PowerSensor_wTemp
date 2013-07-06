/**
 PowerSensor
 
 Sensor power on pin 2
 Power on: light led on pin 13 (green)
 Power off: light led on pin 12 (red)
**/

static int inputPin = 2;
static int redPin = 12;
static int greenPin = 13;

void setup() {
  pinMode(inputPin, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);  
}

void loop() {
 if(digitalRead(inputPin) == HIGH) {
   digitalWrite(greenPin, HIGH);
   digitalWrite(redPin, LOW);
 }
 else {
   digitalWrite(greenPin, LOW);
   digitalWrite(redPin, HIGH);
 }
 delay(500);
}

