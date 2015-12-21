### PowerSensor_wTemp

This is a combination of
[PowerSensor](https://github.com/karlduino/PowerSensor) and
[TempHumid_WebPanel](https://github.com/karlduino/TempHumid_WebPanel).

The aim of this project is (1) to sense my sump pump losing power and send
a email/text, and (2) to get temperature and humidity readings from a
[RHT003 temperature/humidity sensor](https://www.sparkfun.com/products/10167)
sensor (equivalent to [DHT22 at Adafruit](https://www.adafruit.com/products/385))
and make that available on a local web site.

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

The RHT003 is connected as follows:

- Pin 1 (on left) to 5V
- Pin 2 to arduino pin 12
- Pin 4 (on right) to Ground
- 10k resisitor between pins 1 and 2
