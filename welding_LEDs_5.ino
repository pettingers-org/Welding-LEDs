//  - Produces random flashes to simulate arc welding for modeling.
//  - ledL and ledR are intended to be indirect (e.g. bounced off an internal building wall)
//  and should be high-intensity, cool white, lensed LEDs. Welding site 1 and 2, respectively.
//  - ledW is intended to work in conjunction with ledG and should be micro (sub-milimeter) LEDs.
//  - ledW should be a cool white LED and ledG should be a red LED.  The red LED simulates the
//  glow of hot steel from "welding" with ledW.  ledW and ledG are exclusive OR in operation.
//  - ledW and ledG comprise welding site 3.

int randflash1;             //welding site 1 on
int randflash2;             //welding site 2 on
int randflashW;             //welding site 3 on
int randpause1;             //welding site 1 off
int randpause2;             //welding site 2 off
int randpauseW;             //welding site 3 off
int bigpause1;              //welding site 1 or 2 multi-second pause
int bigpausecoin;           //welding site 1 or 2 coin flip for big pause

int ledW = 9;               // digital pin for welding site 3 white LED
int ledG = 10;              // PWM pin for welding site 3 red LED
int ledL = 11;              // digital pin for welding site 1 LED
int ledR = 12;              // digital pin for welding site 2 LED
int shortest = 8;           // shortest (ms) of on-time
int longest = 100;          // longest (ms) of on-time
int darkshort = 2;          // shortest (ms) of off-time
int darklong = 30;          // longest (ms) of off-time
int bigpause = 2800;        // maximum time for long pause
int pausecheck = 3800;      // maximum time between long pauses
int glowcheck = 3000;       // minimum time between site 3 long pauses
int glow = 4500;            // delay white ledG is simulating glow
int fadeValue = 120;        // PWM starting value

unsigned long previousMillis1;
unsigned long previousMillis2;
unsigned long previousMillis3;
unsigned long previousMillisW;
unsigned long previousMillisG;

void setup() {
  pinMode(ledL, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledW, OUTPUT);
  pinMode(ledG, OUTPUT);
  
  previousMillis1 = millis();
  previousMillis2 = millis();
  previousMillis3 = millis();
  previousMillisW = millis();
  previousMillisG = millis();
  randflash1 = random(shortest, longest);
  randflash2 = random(shortest, longest);
  randflashW = random(shortest, longest);
  randpause1 = random(darkshort, darklong);
  randpause2 = random(darkshort, darklong);
  randpauseW = random(darkshort, darklong);
  bigpause1 = random(darklong, bigpause);
  glowcheck = random(8000, 12000);
  glow = random(4500, 8500);
  bigpausecoin = random(1, 3);

  digitalWrite(ledL, HIGH);
  digitalWrite(ledR, HIGH);
  digitalWrite(ledW, HIGH);
  digitalWrite(ledG, LOW);
}

void flash1() {
  // Non-blocking subroutine for welding site 1
  
  unsigned long currentMillis1 = millis();

  if (currentMillis1 - previousMillis1 >= randflash1 && digitalRead(ledL) == HIGH) {
    digitalWrite(ledL, LOW);
    previousMillis1 = currentMillis1;
    randflash1 = random(shortest, longest);
  }     //end if randflash1

  if (currentMillis1 - previousMillis1 >= randpause1 && digitalRead(ledL) == LOW) {
      previousMillis1 = currentMillis1;
      randpause1 = random(darkshort, darklong);
      digitalWrite(ledL, HIGH);
  }     //end if randpause1
  
}       //end flash1


void flash2() {
  // Non-blocking subroutine for welding site 2
    
  unsigned long currentMillis2 = millis();

  if (currentMillis2 - previousMillis2 >= randflash2 && digitalRead(ledR) == HIGH) {
    digitalWrite(ledR, LOW);
    previousMillis2 = currentMillis2;
    randflash2 = random(shortest, longest);
  }     //end if randflash2

  if (currentMillis2 - previousMillis2 >= randpause2 && digitalRead(ledR) == LOW) {
      previousMillis2 = currentMillis2;
      randpause2 = random(darkshort, darklong);
      digitalWrite(ledR, HIGH);
  }     //end if randpause2
  
}       //end flash2

void flashwg() {
  // Non-blocking subroutine for welding site 3 (no glow done here)  
  
  unsigned long currentMillisW = millis();

  if (currentMillisW - previousMillisW >= randflashW && digitalRead(ledW) == HIGH) {
    digitalWrite(ledW, LOW);
    previousMillisW = currentMillisW;
    randflashW = random(shortest, longest);
  }     //end if randflashW

  if (currentMillisW - previousMillisW >= randpauseW && digitalRead(ledW) == LOW) {
      previousMillisW = currentMillisW;
      randpauseW = random(darkshort, darklong);
      digitalWrite(ledW, HIGH);
  }     //end if randpauseW
  
}       //end flashwg


void loop() {

  unsigned long currentMillis3 = millis();
  unsigned long currentMillisG = millis();

  flash1();
  flash2();
  if (currentMillisG - previousMillisG < glowcheck) {flashwg();}  // only flash site 3 if ledG not active
  
  if (currentMillis3 - previousMillis3 >= pausecheck && bigpausecoin == 1) {
    // coin flip has selected site 1 so we randomly start a random length long pause
    randpause1=randpause1+bigpause1;
    previousMillis3 = currentMillis3;
    bigpause1 = random(darklong, bigpause);
    bigpausecoin = random(1, 3);
  } //end if

  if (currentMillis3 - previousMillis3 >= pausecheck && bigpausecoin == 2) {
    // coin flip has selected site 2 so we randomly start a random length long pause
    randpause2=randpause2+bigpause1;
    previousMillis3 = currentMillis3;
    bigpause1 = random(darklong, bigpause);
    bigpausecoin = random(1, 3);
  } //end if

  if (currentMillisG - previousMillisG >= glowcheck) {
    // time for a long pause on site 3 and glow using ledG
    analogWrite(ledG, fadeValue);                         // PWM for ledG to produce glowing fade as welding stops
    if (fadeValue >= 90) {fadeValue = fadeValue - 2;}
    if (fadeValue < 90 && fadeValue >=1) {fadeValue = fadeValue - 1;}
      
    if (fadeValue >=1) {delay(20);}
    digitalWrite(ledW, LOW);          // keep site 3, ledW off while glowing ledG 
    
    if (currentMillisG - previousMillisG >= glow + glowcheck) {
      // done glowing, reset everything and go back to flashing ledW at site 3
      fadeValue = 120;
      previousMillisG = currentMillisG;
      glowcheck = random(8000, 12000);
      glow = random(4500, 8500);
    
    } //end if done glowing
  } //end if  


}     //end loop
