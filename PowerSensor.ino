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
static int redPin = 7;
static int greenPin = 8;

// private info [made generic here]
byte mac[] = { 0x00, 0xC1, 0x4E, 0xF2, 0xDA, 0x30 }; // local MAC
char email_auth[] = "encoded_email_and_password";
char from_email[] = "<email_addr@yahoo.com>";
char to_email[] = "<another_email@gmail.com>";
// end of private info

EthernetClient client;
byte server[] = { 63, 250, 193, 228 }; // Mail server address  (smtp.mail.yahoo.com)

unsigned long lastTimeSent, currentTime;
static unsigned long halfhour = 1800000;
static unsigned int MAX_NUM_EMAILS = 5;
int numberSent = 0;

void setup() {
  pinMode(inputPin, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);  

  Ethernet.begin(mac);
  Serial.begin(9600);

  lastTimeSent = millis();
  Serial.println("Ready.");
}

void loop() {
 if(digitalRead(inputPin) == HIGH) {
   digitalWrite(greenPin, HIGH);
   digitalWrite(redPin, LOW);
 }
 else {
   currentTime = millis();
   digitalWrite(greenPin, LOW);
   digitalWrite(redPin, HIGH);
  
   if(numberSent==0 || (currentTime > lastTimeSent + halfhour &&
                        numberSent < MAX_NUM_EMAILS)) {
     numberSent++;
     lastTimeSent = millis();
     Serial.print("Sending email. ");
     Serial.println(numberSent);
     send_email();
   }

 }
}


void send_email()
{
  Serial.println("Connecting...");
  if(client.connect(server, 25)) {
 
    Serial.println("Connected.");
    delay(1000);
    Serial.println("Sending...");

    client.println("HELO smpt.mail.yahoo.com");
    delay(50);
    client.print("AUTH PLAIN ");
    client.println(email_auth);
    delay(50);
    client.print("mail from:");
    client.println(from_email);
    client.print("rcpt to:");
    client.println(to_email);
    delay(50);
    client.println("data");
    client.print("The sump pump is down. (");
    client.print(numberSent);
    client.println(")");
    client.println(".");
    delay(50);

    Serial.println("Disconnecting...");
    client.println("quit");
    delay(1000);
    Serial.println("Done.");
    client.stop();
  } else {
    Serial.println("Connection failed.");
  }
}
