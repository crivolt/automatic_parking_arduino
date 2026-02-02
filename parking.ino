#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


Servo Servo1;
Servo Servo2;

// Sensore 1
#define trigPin1 6
#define echoPin1 7

// Sensore 2
#define trigPin2 5
#define echoPin2 8

// Stati sbarra
enum State { DOWN, GOING_UP, WAITING, GOING_DOWN };

State state1 = DOWN;
State state2 = DOWN;

int pos1 = 25;
int pos2 = 55;
bool stateled2= false;
bool stateled1= false;

// Led
#define greenLed 2
#define redLed 3
#define led1 4
#define led2 9

int accensione;
int spegnimento;



unsigned long waitStart1 = 0;
unsigned long waitStart2 = 0;



// =============== PARCHEGGIO ===============
int capacity = 2;      // totale posti disponibili
int carsInside = 1;     // auto attualmente nel parcheggio
int freeSlots = 0;      // posti liberi
// ==========================================

LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD I2C 16x2

int tolleranza = 10;

int delayMov = 50;    // tempo tra un movimento e l’altro
unsigned long lastMove1 = 0;
unsigned long lastMove2 = 0;

long readUltrasonic(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(5);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long duration = pulseIn(echo, HIGH, 25000); // timeout di sicurezza
  return duration / 58;
}

// Caso di lettura non corretta del sensore
bool stableDetect1(long cm) {
  static int count1 = 0;
  if (cm > 0 && cm < tolleranza) count1++;
  else count1 = 0;
  return count1 >= 2;
}

bool stableDetect2(long cm) {
  static int count2 = 0;
  if (cm > 0 && cm < tolleranza) count2++;
  else count2 = 0;
  return count2 >= 2;
}


bool objectDetected(long cm) {
  if (cm == 0) return false;   // 0 significa nessuna eco → nessun oggetto
  return (cm < tolleranza);
}
void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Free slots:");
  lcd.setCursor(0, 1);
  lcd.print(freeSlots);
}

void setup() {
  Serial.begin(9600);

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);


  Servo1.attach(12);
  Servo2.attach(11);
  Servo1.write(pos1);
  Servo2.write(pos2);
 

  freeSlots = capacity - carsInside; // inizializza
  lcd.init();      // Inizializza il display
  lcd.backlight(); // Accende la retroilluminazione
  updateLCD();

  
}

void loop() {

  long cm1 = readUltrasonic(trigPin1, echoPin1);
  long cm2 = readUltrasonic(trigPin2, echoPin2);

  unsigned long now = millis();
  if (freeSlots != 0) {
    digitalWrite(greenLed, HIGH);
    digitalWrite(redLed, LOW);
  } 
  else{
    digitalWrite(greenLed, LOW);
    digitalWrite(redLed, HIGH);
  }


  // ------------------------ SBARRA 1 --------------------------
  switch (state1) {

    case DOWN: // Abbassata completamente

    if (stableDetect1(cm1) && freeSlots > 0) {
       state1 = GOING_UP;
    }

      break;

    case GOING_UP:
      if (now - lastMove1 > delayMov) {
        lastMove1 = now;
        pos1 += 3;
        digitalWrite(led1, stateled1);
        stateled1 = !stateled1;
        if (pos1 >= 100) {
          pos1 = 100;
          digitalWrite(led1, LOW);
          state1 = WAITING;
        }
        Servo1.write(pos1);
      }
      break;

    case WAITING:
      if (cm1 >= tolleranza) {
        waitStart1 = now;
        state1 = GOING_DOWN;
      }
      break;

    case GOING_DOWN:
      if (now - waitStart1 > 3000) {
        lastMove1 = now;
        pos1 -= 3;
        digitalWrite(led1, stateled1);
        stateled1 = !stateled1;
        if (pos1 <= 25) {
          pos1 = 25;
          state1 = DOWN;
          digitalWrite(led1, LOW);
          carsInside++;
          freeSlots = capacity - carsInside;
          updateLCD();
        }
        Servo1.write(pos1);
      }
      break;
  }

  // ------------------------ SBARRA 2 --------------------------
  switch (state2) {

    case DOWN:
      if (stableDetect2(cm2) && freeSlots < 2) {
          state2 = GOING_UP;
      }

      break;

    case GOING_UP:
      if (now - lastMove2 > delayMov) {
        lastMove2 = now;
        pos2 += 3;
        digitalWrite(led2, stateled2);
        stateled2 = !stateled2;
        if (pos2 >= 155) {
          pos2 = 155;
          digitalWrite(led2, LOW);
          state2 = WAITING;
          
        }
        Servo2.write(pos2);
      }
      break;

    case WAITING:
      if (cm2 >= tolleranza) {
        waitStart2 = now;
        state2 = GOING_DOWN;
      }
      break;

    case GOING_DOWN:
      if (now - waitStart2 > 3000) {
        lastMove2 = now;
        pos2 -= 3;
        digitalWrite(led2, stateled2);
        stateled2 = !stateled2;
        if (pos2 <= 55) {
          pos2 = 55;
          digitalWrite(led2, LOW);
          state2 = DOWN;
          carsInside--;
          freeSlots = capacity - carsInside;
          updateLCD();
        }
        Servo2.write(pos2);
      }
      break;
  }\

  // Stampa diagnostica
  Serial.print("Cm1: "); Serial.print(cm1);
  Serial.print("\t Pos1: "); Serial.print(pos1);
  Serial.print("\t State1: "); Serial.print(state1);

  Serial.print("   |   Cm2: "); Serial.print(cm2);
  Serial.print("\t Pos2: "); Serial.print(pos2);
  Serial.print("\t State2: "); Serial.println(state2);
}








