#ifndef BUTTONS_H
#define BUTTONS_H
 
#include <Arduino.h>
 
extern volatile int pinPressed;   // kertoo muille tiedostoille, että muuttujalle pinPressed on valittu paikka muistista, eikä sitä saa alustaa uudelleen

void initButtonsAndButtonInterrupts();
 
#endif