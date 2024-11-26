#include <LiquidCrystal.h>
#include <Arduino.h>
#include "display.h"
#include "leds.h"
#include "buttons.h"


//
volatile bool buttonPressedFlag = false;
bool gameActive = false; // Seuraa onko peli käynnissä
bool playerError = false; // Seuraa onko pelaaja tehnyt virheen

// määrittää lcd-näytön pinnit
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// pelilogiikan muuttujat
int currentLevel = 1; 
int playerPosition = 0; // pelaajan sijainti sequencen aikana
int sequenceLength = 1; 

// taulukot mihin tallentuu arvotut numerot ja käyttäjän inputit
int randomNumbers[100]; 
int userNumbers[100];   
// seuraa onko pelaajan inputit samat kuin pelin arpomat numerot
bool sequenceCorrect = true;

// Timerin muuttujat
unsigned long gameInterval = 1000;  
unsigned long previousMillis = 0;
int successfulRounds = 0;

// keskeytytspalvelija 
ISR(TIMER1_COMPA_vect) {
  
  if (gameActive) {
    if (millis() - previousMillis >= gameInterval) {
      previousMillis = millis();
      
    }
  }
}

// functio generoi random numerosarjan
void generateRun() {
  for (int i = 0; i < 100; i++) {
    randomNumbers[i] = random(0, 4);  // generoi random numerosarjan 0-3
  }
}

// functio jolla näytetään numerosarja pelaajalle 
void displaySequence() {
  for (int i = 0; i < sequenceLength; i++) {
    setLed(A2 + randomNumbers[i]);
    delay(500);
    clearLed(A2 + randomNumbers[i]);
    delay(250);
  }
}

// functio joka tarkastaa käyttäjän inputin
bool checkUserInput() {
  if (pinPressed != 0) {                // jos napin painallus havaitaan
    int buttonIndex = pinPressed - 2;  
    pinPressed = 0; 

    if (buttonIndex != randomNumbers[playerPosition]) {
      return false;  // palauttaa falsen jos input ei vastaa numerosarjaa
    }

    userNumbers[playerPosition] = buttonIndex; // tallentaa pelaajan inputit 
    playerPosition++;                  // liikkuu seuraavaan sijaintiin
    return true;
  }
  return false;        // palauttaa falsen jos käyttäjä ei paina mitään nappia
}

// Functio joka vahvistaa vastaako käyttäjän input numerosarjaa
bool checkRun() {
  for (int i = 0; i < playerPosition; i++) {
    if (userNumbers[i] != randomNumbers[i]) {
      return false; // palauttaa falsen jos pelaajan input ei vastaa numero sarjaa
    }
  }
  return true;
}

// functio joka tekee lcd-näytölle printtaamisesta helpompaa 
void lcdPrintMessage(String line1, String line2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

// functio joka nopeuttaa peliä onnistuneiden kierrosten mukaan
void increaseSpeed() {
  successfulRounds++;
  if (successfulRounds % 10 == 0) {   //joka kymmenes onnistunut kierros pelin nopeus nousee
    gameInterval = gameInterval - (gameInterval * 0.1);  // pienentää aikaväliä 10%
  }
}

// functio joka ajetaan pelin alkaessa
void startGame() {
  lcdPrintMessage("Game is Starting", "");
  setAllLeds();
  delay(1500);
  lcd.clear();
  clearAllLeds();

  currentLevel = 1;
  playerPosition = 0;
  sequenceLength = 1;
  generateRun();   // generoi uuden random numerosarjan
  playerError = false;
  successfulRounds = 0;
  showResult(currentLevel); // näyttää käyttäjän pisteet
}

// functio joka ajetaan pelin pysähtyessä
void stopGame() {
  lcdPrintMessage("Game Over!", "");
  gameActive = false;

  setAllLeds();
  delay(1000);
  lcdPrintMessage("Press any button", "to start");
  clearAllLeds();

  showResult(0);
}

// functio jolla tarkastetaan pelin tila ja käsitellään käyttäjän errorit
void checkGame() {
  if (playerPosition == sequenceLength) {
    if (checkRun()) {    // jos käyttäjän inputit vastaa numerosarjaa peli jatkuu seuraavalle tasolle ja numerosarja kasvaa
      currentLevel++;
      sequenceLength++;
      playerPosition = 0;
      successfulRounds++;

      increaseSpeed();
      lcdPrintMessage("Level Up!", "Level: " + String(currentLevel));
      delay(1000);
      lcd.clear();
      clearAllLeds();
      showResult(currentLevel);
      displaySequence();
      
    } else {
      playerError = true; // asettaa virhelipun jos input on virheellinen
      stopGame();  // pysäyttää pelin
    }
  }
}

// tarkastaa onko mikää nappi painettuna
bool checkAnyButtonPressed() {
  return pinPressed != 0;
}

void setup() {
  lcd.begin(16, 2);
  lcdPrintMessage("Press any button", "to start");

  // kutsutaan aliohjelmat
  initButtonsAndButtonInterrupts();
  interrupts();
  initializeDisplay();
  randomSeed(analogRead(0));

  clearAllLeds();
  showResult(0);

  // määrittää timerin keskeytyksiä varten
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 15624;  
  TCCR1B |= (1 << WGM12);  
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  TIMSK1 |= (1 << OCIE1A);  
  interrupts();
}

void loop() {
  if (!gameActive && checkAnyButtonPressed()) {
    startGame();
    gameActive = true;
    displaySequence();
  }

  if (gameActive) {
    checkGame();
    if (playerError) {
      stopGame();
    } else if (buttonPressedFlag) {
      buttonPressedFlag = false;
      if (!checkUserInput()) {
        playerError = true;
      }
    }
  }
}
