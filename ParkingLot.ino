#include <Servo.h>
#include <LiquidCrystal.h>
//Pin locations
const int SERVO_pin = 7;
const int IR_pinIn = 6;
const int IR_pinOut = 5; 

const int RS = 8;
const int EN = 9;
const int D4 = 10;
const int D5 = 11;
const int D6 = 12;
const int D7 = 13;
 
//LCD Display
/* The circuit:
 * RS -> digital pin
 * E -> digital pin
 * D4 -> digital pin
 * D5 -> digital pin
 * D6 -> digital pin
 * D7 -> digital pin
 * Anode -> 5v
 * Cathode -> ground
 * R/W pin -> ground
 * VSS -> ground
 * VCC -> 5v
 * V0 -> variable output of 10k resistor/potentiometer, 5v:ground */
LiquidCrystal Screen (RS, EN, D4, D5, D6, D7); 
const int column = 16;
const int row = 2;
void numToScreen() //Output to LCD
{
  Screen.setCursor(0,0);
  Screen.print("Max Spots:");
  Screen.print(maxParkingSpots);
  
  Screen.setCursor(0,1);
  Screen.print("Open Spots:");
  Screen.print(currentParkingSpots);
}

//Servo
Servo MyGate;
const int GATE_OPEN = 90; //In degrees
const int GATE_CLOSE = 0; //In degrees
const int maxParkingSpots = 4; //Adjustable
int currentParkingSpots = 0; 

void moveGate(int &position)
{
  if (position <= GATE_OPEN && position >= GATE_CLOSE)
  {
  MyGate.write(position);
  delay(15);
  }
}
void activateGate() //Opens the gate, waits, then closes the gate
{
  int position = 0;    // Current position in degrees
  for (position = GATE_CLOSE; position <= GATE_OPEN; position += 1) 
    moveGate(position);
  delay(2500); //time between up and down again, adjustable
  for (position = GATE_OPEN; position >= GATE_CLOSE; position -= 1) 
    moveGate(position);    
}
 
bool isLotFull()
{
      if (currentParkingSpots >= maxParkingSpots) 
        return true;
      else
        return false;
}

void checkInIR()
{
  if (digitalRead(IR_pinIn) == LOW && !isLotFull())
  {
    activateGate();
    ++currentParkingSpots;
  }
}
void checkOutIR()
{
  if (digitalRead(IR_pinOut) == LOW) //no full check because cars need to be able to come out regardless of capacity
  {
    activateGate();
    if (currentParkingSpots > 0)
      --currentParkingSpots;
  }
}
 
void setup() //Run once
{
  //IR Sensors
  pinMode(IR_pinIn,INPUT);
  pinMode(IR_pinOut,INPUT);
  Serial.begin(9600);
  
  MyGate.attach(SERVO_pin);
  MyGate.write(GATE_CLOSE);

  Screen.begin(column, row);
}
 
void loop() 
{
    checkInIR();
    checkOutIR();
    numToScreen();
    delay(100);
}
