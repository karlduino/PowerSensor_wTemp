### PowerSensor

The aim of this project is to sense my sump pump losing power and send
a email/text.

- I connected a 5V wall adapter to pin 2, with a 10k pull-down
  resistor
- A green LED is connected to pin 8, lit when power is on.
- A red LED is connected to pin 7, lit when power is off.
- Using an ethernet shield, send an email if the power on pin 2 goes
  off.  I'm using telnet with a yahoo account, following
  [this post](http://bit.ly/12PivcA) from Rob Glinka.


My plan is to power the arduino with a UPS. But, of course, if my web
connection goes down (my cable modem or router), this won't work.

       
