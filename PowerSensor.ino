/**
 PowerSensor
 
 Sense power on pin 2
 Power on: light led on pin 8 (green)
 Power off: light led on pin 7 (red)

 If power goes off, send an email (using telnet to yahoo account)
 Following this post from Rob Glinka: http://bit.ly/12PivcA
**/

#include <SPI.h>
#include <Ethernet.h>

static int inputPin = 2;
static int redPin = A5;
static int greenPin = A2;

// private info [made generic here]
byte mac[] = { 0x00, 0xC1, 0x4E, 0xF2, 0xDA, 0x30 }; // local MAC
char from_email[] = "<email_addr@yahoo.com>";
char to_email[] = "<another_email@gmail.com>";
byte server[] = {63, 250, 193, 228 }; // Mail server address  (smtp.mail.yahoo.com)
// end of private info

EthernetClient client;

unsigned long lastTimeSent, currentTime;
int numberSent = 0;
bool lastSaidOff = false;
static unsigned long TIME_BETWEEN_TEXTS = 1800000; // half hour
static unsigned int MAX_NUM_EMAILS = 8;
static unsigned int DELAY_BEFORE_TEXT = 1000; // one second
static unsigned int CONNECTION_DELAY = 1000; // delay before connecting and after email
static unsigned int DELAY_BETWEEN_COMMANDS = 50; // delay between commands to yahoo

void setup() {
  pinMode(inputPin, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);  

  Ethernet.begin(mac);
  Serial.begin(9600);

  lastTimeSent = millis();
  Serial.println("Ready.");

  flash_leds(5, 250);
  send_email("Sensor ready.");
  flash_leds(5, 250);
}

void loop() {
 if(digitalRead(inputPin) == HIGH) {
   digitalWrite(greenPin, HIGH);
   digitalWrite(redPin, LOW);

   if(numberSent < MAX_NUM_EMAILS &&
      (currentTime > lastTimeSent + TIME_BETWEEN_TEXTS ||
       lastSaidOff)) {
     numberSent++;
     lastTimeSent = millis();
     Serial.print("Sending email (power is on). ");
     Serial.println(numberSent);
     lastSaidOff = false;
     send_email("The sump pump is working again.");
   }
 }
 else {
   delay(DELAY_BEFORE_TEXT);

   if(digitalRead(inputPin) == LOW) { // not a false alarm
     currentTime = millis();
     digitalWrite(greenPin, LOW);
     digitalWrite(redPin, HIGH);
  
     if(numberSent==0 || !lastSaidOff ||
        (currentTime > lastTimeSent + TIME_BETWEEN_TEXTS &&
         numberSent < MAX_NUM_EMAILS)) {
       numberSent++;
       lastTimeSent = millis();
       Serial.print("Sending email (power is off). ");
       Serial.println(numberSent);
       lastSaidOff = true;
       send_email("The sump pump is DOWN!");
     }
   }

 }
}


void send_email(char message[])
{
  Serial.println("Connecting...");
  if(client.connect(server, 25)) {
 
    Serial.println("Connected.");
    delay(CONNECTION_DELAY);
    Serial.println("Sending...");

    client.println("HELO smtp");
    delay(DELAY_BETWEEN_COMMANDS);
    client.print("mail from:");
    client.println(from_email);
    client.print("rcpt to:");
    client.println(to_email);
    delay(DELAY_BETWEEN_COMMANDS);
    client.println("data");
    client.print("[msg ");
    client.print(numberSent);
    client.print("]: ");
    client.println(message);
    client.println(".");
    delay(DELAY_BETWEEN_COMMANDS);

    Serial.println("Disconnecting...");
    client.println("quit");
    delay(CONNECTION_DELAY);
    Serial.println("Done.");
    client.stop();
  } else {
    Serial.println("Connection failed.");
  }
}

void flash_leds(int n_times, long delay_amount)
{
  for(int i=0; i<n_times; i++) {
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, HIGH);

    delay(delay_amount);

    digitalWrite(greenPin, LOW);
    digitalWrite(redPin, LOW);

    delay(delay_amount);
  }
}