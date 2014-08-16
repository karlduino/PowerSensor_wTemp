/**
 PowerSensor
 
 Sense power on pin 2
 Power on: green led on pin A2
 Power off: red led on pin A5

 If power goes off, send an email (using gmail account and temboo service)
 Following this sketch: https://temboo.com/arduino/yun/send-an-email
**/

#include <SPI.h>
#include <Ethernet.h>

#define DEBUG

static int inputPin = 2;
static int redPin = A5;
static int greenPin = A2;

#include <Bridge.h>
#include <Temboo.h>
#include "TembooAccount.h" // contains Temboo account information

unsigned long lastTimeSent, currentTime;
int numberSent = 0;
bool lastSaidOff = false;
static unsigned long TIME_BETWEEN_TEXTS = 600000; // 10 minutes
static unsigned int MAX_NUM_EMAILS = 8;
static unsigned int STARTUP_DELAY = 60000; // one minute
static unsigned int DELAY_BEFORE_TEXT = 5000; // 5 seconds

void setup() {
  pinMode(inputPin, INPUT);
  digitalWrite(inputPin, HIGH);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);  

#ifdef DEBUG
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Starting up.");
#endif  
  
  flash_leds(5, 250);
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, HIGH);  
  delay(STARTUP_DELAY);
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);  

#ifdef DEBUG
  Serial.println("Done with startup.");
#endif  

  send_email("Sump ready", "Sensor ready.");
  flash_leds(5, 250);
}

void loop() {
 if(digitalRead(inputPin) == HIGH) {
   digitalWrite(greenPin, HIGH);
   digitalWrite(redPin, LOW);

   if(numberSent < MAX_NUM_EMAILS &&
      (currentTime > lastTimeSent + TIME_BETWEEN_TEXTS ||
       lastSaidOff)) {
#ifdef DEBUG
     Serial.print("Sending email (power is on). ");
     Serial.println(numberSent);
#endif
     numberSent++;
     lastTimeSent = millis();
     lastSaidOff = false;
     send_email("Sump working", "The sump pump is working again.");
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
#ifdef DEBUG
       Serial.print("Sending email (power is off). ");
       Serial.println(numberSent);
#endif
       numberSent++;
       lastTimeSent = millis();
       lastSaidOff = true;
       send_email("Sump down", "The sump pump is DOWN!");
     }
   }

 }
}


void send_email(char subject[], char message[])
{
  bool success = false;

  while (!success) {

#ifdef DEBUG    
    Serial.println("Running SendAnEmail...");
#endif

    TembooChoreo SendEmailChoreo;

    // invoke the Temboo client
    // NOTE that the client must be reinvoked, and repopulated with
    // appropriate arguments, each time its run() method is called.
    SendEmailChoreo.begin();
    
    // set Temboo account credentials
    SendEmailChoreo.setAccountName(TEMBOO_ACCOUNT);
    SendEmailChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    SendEmailChoreo.setAppKey(TEMBOO_APP_KEY);

    // identify the Temboo Library choreo to run (Google > Gmail > SendEmail)
    SendEmailChoreo.setChoreo("/Library/Google/Gmail/SendEmail");

    // set the required choreo inputs
    // see https://www.temboo.com/library/Library/Google/Gmail/SendEmail/ 
    // for complete details about the inputs for this Choreo

    // the first input is your Gmail email address
    SendEmailChoreo.addInput("Username", GMAIL_USER_NAME);
    // next is your Gmail password.
    SendEmailChoreo.addInput("Password", GMAIL_PASSWORD);
    // who to send the email to
    SendEmailChoreo.addInput("ToAddress", TO_EMAIL_ADDRESS);
    // then a subject line
    SendEmailChoreo.addInput("Subject", subject);

     // next comes the message body, the main content of the email   
    SendEmailChoreo.addInput("MessageBody", message);

    // tell the Choreo to run and wait for the results. The 
    // return code (returnCode) will tell us whether the Temboo client 
    // was able to send our request to the Temboo servers
    unsigned int returnCode = SendEmailChoreo.run();

    // a return code of zero (0) means everything worked
    if (returnCode == 0) {
#ifdef DEBUG      
        Serial.println("Success! Email sent!");
#endif        
        success = true;
    } else {
      // a non-zero return code means there was an error
      // read and print the error message
      while (SendEmailChoreo.available()) {
#ifdef DEBUG      
        char c = SendEmailChoreo.read();
        Serial.print(c);
#endif
      }
    } 
    SendEmailChoreo.close();

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
