#include <LiquidCrystal.h>
#include <Arduino.h>
#include "display.h"
#include "leds.h"
#include "buttons.h"

// LCD-näytön pinnit
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Pelin muuttujat
int randomNumbers[100]; // Satunnaisesti luotu numerosarja
int userNumbers[100];   // Pelaajan syöttämät numerot
int currentLevel = 1;   // Pelaajan taso
int playerPosition = 0; // Pelaajan sijainti sarjassa
int sequenceLength = 1; // Nykyisen sarjan pituus

volatile bool buttonPressedFlag = false; // Keskeytyslippu
bool gameActive = false; // Onko peli käynnissä?

// Funktio, joka generoi satunnaisen numerosarjan
void generateSequence() {
  Serial.println("generoidaan numerosarja");
  for (int i = 0; i < 100; i++) {
    randomNumbers[i] = random(0, 4); // Numerot välillä 0-3
    Serial.print(randomNumbers[i]);
    Serial.print(" ");
  }
  Serial.println();
}

// Funktio, joka näyttää pelaajalle numerosarjan
void showSequence() {
  
  Serial.print(sequenceLength);
  
  for (int i = 0; i < sequenceLength; i++) {
    Serial.print("LED ");
    Serial.println(A2 + randomNumbers[i]);
    setLed(A2 + randomNumbers[i]); 
    delay(500);
    clearLed(A2 + randomNumbers[i]); 
  }
}

// Funktio, joka tarkistaa pelaajan syötteen
bool checkUserInput() {
  if (pinPressed != 0) {  // Jos nappia painettiin
    int buttonIndex = pinPressed - 2; // Selvitetään napin indeksi
    pinPressed = 0; // Nollataan pinPressed

    Serial.print("nappia painettu: ");
    Serial.println(buttonIndex);

    if (buttonIndex != randomNumbers[playerPosition]) {
      Serial.println("Väärä syöte!");
      return false; // Pelaajan syöte ei täsmää sarjan kanssa
    }

    Serial.println("oikea syöte");
    userNumbers[playerPosition] = buttonIndex; // Tallennetaan pelaajan syöte
    playerPosition++; // Siirrytään seuraavaan sijaintiin
    return true;
  }
  return false; // Ei syötettä
}


void startGame() {
  Serial.println("peli alkaa");
  lcd.clear();
  lcd.print("game is starting");
  delay(1000);

  // Nollataan pelin muuttujat
  currentLevel = 1;
  sequenceLength = 1;
  playerPosition = 0;
  gameActive = true;
  generateSequence(); // Generoidaan uusi numerosarja
  showResult(currentLevel); // Näytetään nykyinen taso
  lcd.clear();
  lcd.print("Taso: 1");
  showSequence(); 
}


void gameOver() {
  Serial.println("Game over!");
  gameActive = false;
  lcd.clear();
  lcd.print("Game over!");
  delay(2000);
  lcd.clear();
  lcd.print("Press to");
  lcd.setCursor(0, 1);
  lcd.print("Start");
  showResult(0); // Näytetään tulos 0
}

// Funktio, joka tarkistaa pelin tilan ja etenee seuraavalle tasolle
void updateGame() {
  if (playerPosition == sequenceLength) { // Pelaaja on suorittanut sarjan
    Serial.println("joujou");
    currentLevel++; // Nostetaan tasoa
    sequenceLength++; // Kasvatetaan sarjan pituutta
    playerPosition = 0; // Nollataan pelaajan sijainti

    lcd.clear();
    lcd.print("Next Level");
    delay(1000);
    lcd.clear();
    lcd.print("Taso: " + String(currentLevel));
    showResult(currentLevel); // Päivitetään taso näytölle
    showSequence(); // Näytetään uusi sarja
  }
}


void setup() {
  Serial.begin(9600); 
  Serial.println("Game is starting");

  lcd.begin(16, 2);
  lcd.print("press to ");
  lcd.setCursor(0, 1);
  lcd.print("start!");

  initButtonsAndButtonInterrupts(); 
  initializeDisplay();              
  randomSeed(analogRead(0));        
  clearAllLeds();                   
  showResult(0);                    
  Serial.println("jejea");
}


void loop() {
  if (!gameActive) { 
    if (pinPressed != 0) {
      Serial.println("press to start");
      startGame();
    }
  } else { 
    if (!checkUserInput()) { 
      Serial.println("game over");
      gameOver();
    } else {
      updateGame(); 
    }
  }
}
