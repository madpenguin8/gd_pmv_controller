//
//  Arduino Projects
//
//  Created by Michael Diehl on 3/3/13.
//
// This sketch requires the ModbusMaster library found at http://playground.arduino.cc/Code/ModbusMaster
// Directions for installing the library can also be found there.
//
// This sketch allows you to control the position of an PMV (Precision Mixing valve) found on Gardner Denver VS Series
// air compressors. The valve is controlled via modbus by writing to holding register 0 and the device ID is 159. The
// register holds a value from 0-900 representing tenths of a degree. Communications are RS-485 at 19200 baud, 8N1.
//
// I tested this with the RS-485 sheild from CuteDigi
// http://www.cutedigi.com/arduino-shields/rs485-shield-for-arduino.html
// I also tested it with using a DS3696 RS-485 line transceiver 8-pin DIP. This sketch uses Digital Out 2 to drive the
// RE-DE pin 2. The pin is pulled HIGH before writing to the register with a 2 millisecond delay before pulling it LOW
// after writing. I'm not sure if we need the delay but I've found you need it to allow the UART to clear out before
// putting the transceiver back in receive. It works this way so I'm keeping it.
//
// Input is a 10k potentiometer as a voltage divider with the wiper wired to Analog In pin A1.
//
// 

#include <ModbusMaster.h>

// Slave address of PMV
#define pmvSlaveAddress 159

// instantiate ModbusMaster object as slave ID 159
ModbusMaster node(pmvSlaveAddress);

// Use analog input pin A1 for potentiometer input.
const int inputPin = A1;

// Timer interval for sending the requested position every 500 milliseconds
const long interval = 500;

// Previous uptime value for timer.
long previousMillis = 0;

// We scale the input of the 10bit ADC for the range of the valve
const float degreesPerBit = (900.0 / 1023.0);

// Our register data for the valve
uint16_t pmvPosition = 0;

void setup()
{
    // initialize Modbus communication baud rate
    node.begin(19200);

    // Setup pin 2 as RTS out.
    DDRD |= B00000100;
    DDRB |= B00100000;
}

void loop()
{
    // Get the current uptime
    unsigned long currentMillis = millis();

    // Reset previousMillis in the event of a rollover.
    if(previousMillis > currentMillis){
        previousMillis = 0;
    }

    // Check the timer
    if(currentMillis - previousMillis > interval){
        previousMillis = millis();
        commandValvePosition();
    }
}

void commandValvePosition()
{
    // Set RS485 shifter to transmit
    PORTD |= B00000100;
    // Blink LED on DO13
    PORTB |= B00100000;

    // Calculate the position of the valve based on our input
    pmvPosition = (analogRead(inputPin) * degreesPerBit);

    // Write to the valve register
    node.writeSingleRegister(0, pmvPosition);

    // Possible delay to clear out buffers.
    delay(2);
    // Return RS485 shifter to receive
    PORTD &= B11111011;
    // Blink LED on DO13
    PORTB &= B11011111;
}

