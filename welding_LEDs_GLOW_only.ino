//  - Produces random flashes to simulate arc welding for modeling.
//  - ledW is intended to work in conjunction with ledG and should be micro (sub-milimeter) LEDs.
//  - ledW should be a cool white LED and ledG should be a red LED.  The red LED simulates the
//  glow of hot steel from "welding" with ledW.  ledW and ledG are exclusive OR in operation.
//  - ledW and ledG comprise welding site 3.

int randflashW;             //welding site 3 on
int randpauseW;             //welding site 3 off


int ledW = 9;               // digital pin for welding site 3 white LED
int ledG = 10;              // PWM pin for welding site 3 red LED
int shortest = 8;           // shortest (ms) of on-time
int longest = 100;          // longest (ms) of on-time
int darkshort = 2;          // shortest (ms) of off-time
int darklong = 30;          // longest (ms) of off-time
int bigpause = 2800;        // maximum time for long pause
int pausecheck = 3800;      // maximum time between long pauses
int glowcheck = 3000;       // minimum time between site 3 long pauses
int glow = 4500;            // delay white ledG is simulating glow
int fadeValue = 120;        // PWM starting value

unsigned long previousMillis3;
unsigned long previousMillisW;
unsigned long previousMillisG;

void setup() {

  pinMode(ledW, OUTPUT);
  pinMode(ledG, OUTPUT);
  
  previousMillis3 = millis();
  previousMillisW = millis();
  previousMillisG = millis();
  randflashW = random(shortest, longest);
  randpauseW = random(darkshort, darklong);
  glowcheck = random(8000, 12000);
  glow = random(4500, 8500);

  digitalWrite(ledW, HIGH);
  digitalWrite(ledG, LOW);
}

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

  if (currentMillisG - previousMillisG < glowcheck) {flashwg();}  // only flash site 3 if ledG not active
  
  if (currentMillisG - previousMillisG >= glowcheck) {
    // time for a long pause on site 3 and glow using ledG
    analogWrite(ledG, fadeValue);                         // PWM for ledG to produce glowing fade as welding stops
    if (fadeValue >= 70) {fadeValue = fadeValue - 2;}
    if (fadeValue < 70 && fadeValue >=1) {fadeValue = fadeValue - 1;}
      
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
