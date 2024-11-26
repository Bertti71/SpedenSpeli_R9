#include "display.h"
#include <arduino.h>

// 7 segment display numbers
uint8_t segmentCodes[10] = {
    0b00111111, // Number 0
    0b00000110, // Number 1
    0b01011011, // Number 2
    0b01001111, // Number 3
    0b01100110, // Number 4
    0b01101101, // Number 5
    0b01111101, // Number 6
    0b00000111, // Number 7
    0b01111111, // Number 8
    0b01101111  // Number 9
};


void initializeDisplay(void)
{
    pinMode(DATA_PIN, OUTPUT); // Arduino pin 8  = serial to parallel component serialInput
    pinMode(CLOCK_PIN, OUTPUT); // Arduino pin 11 = serial to parallel component shiftClock
    pinMode(LATCH_PIN, OUTPUT); // Arduino pin 10 = serial to parallel component latchClock
  	pinMode(OE_PIN, OUTPUT); // Arduino pin 9  = serial to parallel component outEnable
  	pinMode(CLEAR_PIN, OUTPUT); // Arduino pin 12 = serial to parallel component reset
  
  	digitalWrite(OE_PIN, LOW); // Setting the Output Enable Pin to LOW as per the datasheet's instructions
  	digitalWrite(CLEAR_PIN, HIGH); // Setting the Shift Register Clear Pin to HIGH as per the datasheet's instructions
}


void writeByte(uint8_t number,bool last)
{
    // If last is true, latch is off first
    if (last) {
        digitalWrite(LATCH_PIN, LOW);
    }
    
    // Shifting 8 bits, bit by bit
    for (int i = 7; i >= 0; i--) {
        digitalWrite(CLOCK_PIN, LOW); // Making sure Clock begins on low
        digitalWrite(DATA_PIN, (number & (1 << i)) ? HIGH : LOW); // Sending a singular bit
        digitalWrite(CLOCK_PIN, HIGH); // Clock high
    }

    // Latch on, if last is true
    if (last) {
        digitalWrite(LATCH_PIN, HIGH);
    }
}


void writeHighAndLowNumber(uint8_t tens,uint8_t ones)
{
// Checks if values are within limits
    if (tens < 10 && ones < 10) {
        writeByte(segmentCodes[tens], false); // Display tens
        writeByte(segmentCodes[ones], true); // Display ones
    }
}

void showResult(byte result)
{
  if (result <= 99) { // Checks that the result is within set limits
    uint8_t tens = result / 10; // Counts tens
    uint8_t ones = result % 10; // Counts ones
    writeHighAndLowNumber(tens,ones); //Displays the result
  } else { // If the result is outside of limits, displays an error 00
    writeHighAndLowNumber(0,0);
    }
}