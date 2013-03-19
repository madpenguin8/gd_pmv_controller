
This sketch requires the ModbusMaster library found at http://playground.arduino.cc/Code/ModbusMaster
Directions for installing the library can also be found there.

This sketch allows you to control the position of an PMV (Precision Mixing valve) found on Gardner Denver VS Series
air compressors. The valve is controlled via modbus by writing to holding register 0 and the device ID is 159. The
register holds a value from 0-900 representing tenths of a degree. Communications are RS-485 at 19200 baud, 8N1.

I tested this with the RS-485 sheild from CuteDigi
http://www.cutedigi.com/arduino-shields/rs485-shield-for-arduino.html
I also tested it with using a DS3696 RS-485 line transceiver 8-pin DIP. This sketch uses Digital Out 2 to drive the
RE-DE pin 2. The pin is pulled HIGH before writing to the register with a 2 millisecond delay before pulling it LOW
after writing. I'm not sure if we need the delay but I've found you need it to allow the UART to clear out before
putting the transceiver back in receive. It works this way so I'm keeping it.

Input is a 10k potentiometer as a voltage divider with the wiper wired to Analog In pin A1.
