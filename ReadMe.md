### PowerSensor

The aim of this project is to sense my sump pump losing power and send
a email/text.

- I connected a 5V wall adapter to pin 2, with a 10k pull-down
  resistor
- A green LED is connected to pin A2, lit when power is on.
- A red LED is connected to pin A5, lit when power is off.
- Currently using an [Arduino Y&uacute;] and sending email from a
  gmail account using
  [this sketch](https://temboo.com/arduino/yun/send-an-email) at
  [Temboo](https://temboo.com), which also requires a Temboo account.

I'm powering the arduino with a UPS. But, of course, if my web
connection goes down (my cable modem or router), this won't work.
