#include "LiquidCrystal_I2C.h"
#include "RudderController.h"
#include "EngineMonitor.h"

int ledPin = 13;
bool ledPin13State = HIGH;
int ledPin13StateTime = 0;
unsigned long oldTime;

int pinCoolantWaterFlow = 0;
int addrCoolantWaterTemp = 0;
int pinOilPress = 0;
int addrOilTemp = 0;

int ledSBPin = 7;
int ledCenterPin = 8;
int ledBBPin = 9;

int analogPin = 0;


bool rudderUpdated = false;


RudderController rudderController(analogPin);
EngineMonitor EngineMonitor(pinCoolantWaterFlow, addrCoolantWaterTemp, pinOilPress, addrOilTemp);
// LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3); //, POSITIVE); 
LiquidCrystal_I2C lcd(0x27, 16, 2); // addres, cols, rows


void(* resetFunc) (void) = 0; //declare reset function @ address 0

void setup() {
  Serial.begin(9600);

/*
  lcd.begin(20, 4);// Configuramos el numero de columnas y filas del LCD.
  lcd.clear();     
  lcd.begin(16,2);
  lcd.setCursor(0,0);  
  lcd.print("RUDDER INDICATOR");
  lcd.setCursor(6,1);
  lcd.print("Stcklfsch/Wndrlb");
*/

  rudderController.begin(); // todo: pass lcd?

  pinMode(ledPin, OUTPUT);

  delay(666);
}



void loop() {
  rudderUpdated = rudderController.update_state();
  if( rudderUpdated ) 
  { 
    rudderController.print_state();
    delay(50);
  }
  else
  {
    delay(500);
  }

  ledPin13State = !ledPin13State;
  digitalWrite(ledPin, ledPin13State);


}
