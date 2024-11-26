#include "buttons.h"
 
volatile int pinPressed = 0;

 
void initButtonsAndButtonInterrupts(void){
    for (int i = 2;i<6;i++){
    pinMode(i, INPUT_PULLUP);   //pinnit 2-5 input_pullup -tilaan
  }
 
PCICR = 0x04;
PCMSK2 = B00111100;
}
 
ISR(PCINT2_vect){
  for (int i = 2; i < 6; i++){
    byte luettu = digitalRead(i);
    while(digitalRead(i)==LOW)
    {
      //
    }
    if (luettu == LOW){
      pinPressed = i;
    }
  }
}