// Aliohjelma sytyttämään LED määritellyssä pinnissä
void setLed(byte pin) {
  pinMode(pin, OUTPUT);  // Asetetaan pinni ulostuloksi
  digitalWrite(pin, HIGH);  // Sytytetään ledi
}
 
 
// Aliohjelma sammuttamaan LED määritellyssä pinnissä
void clearLed(byte pin) {
  pinMode(pin, OUTPUT);  // Varmistetaan, että pinni on ulostulo
  digitalWrite(pin, LOW);  // Sammutetaan ledi
}
 
 
// aliohjelma joka sytyttää kaikki ledit yhtä aikaa
 
void setAllLeds(){
 
  setLed(A2);
  setLed(A3);
  setLed(A4);
  setLed(A5);
}
 
 
// Aliohjelma joka sammuttaa kaikki ledit
void clearAllLeds() {
  // Määritellään käytettävät LED-pinnit taulukossa
  byte ledPins[] = {A2, A3, A4, A5};  // taulukko pinneille mitä käytetään
  for (byte i = 0; i < sizeof(ledPins); i++) {
    clearLed(ledPins[i]);  // Sammuttaa kaikki pinneissä olevat ledit
  }
}
 
