#include "LiquidCrystal_I2C.h"
#include "RudderController.h"
#include "EngineMonitor.h"

#define ONE_WIRE_BUS 2

int ledPin = 13;
bool ledPin13State = HIGH;
int ledPin13StateTime = 0;
unsigned long oldTime;


// Create a new instance of the oneWire class to communicate with any OneWire device:
OneWire oneWire(ONE_WIRE_BUS);
// Pass the oneWire reference to DallasTemperature library:
DallasTemperature sensors(&oneWire);


int pinCoolantWaterFlow = 0;
DeviceAddress addrCoolantWaterTemp; // = { 0x28, 0x35, 0xD9, 0x06, 0x0C, 0x00, 0x00, 0x07 };
                              
int pinOilPress = 0;
DeviceAddress addrOilTemp; // = { 0x28, 0xFF, 0x7F, 0x72, 0x50, 0x17, 0x04, 0x7D };

int ledSBPin = 7;
int ledCenterPin = 8;
int ledBBPin = 9;

int rudderanglePin = A0;


bool rudderUpdated = false;


RudderController rudderController(rudderanglePin);
EngineMonitor engineMonitor; //pinCoolantWaterFlow, addrCoolantWaterTemp, pinOilPress, addrOilTemp);
// LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3); //, POSITIVE); 
LiquidCrystal_I2C lcd(0x27, 16, 2); // addres, cols, rows


void(* resetFunc) (void) = 0; //declare reset function @ address 0

void setup() {
  Serial.begin(115200);

/*
  lcd.begin(20, 4);// Configuramos el numero de columnas y filas del LCD.
  lcd.clear();     
  lcd.begin(16,2);
  lcd.setCursor(0,0);  
  lcd.print("RUDDER INDICATOR");
  lcd.setCursor(6,1);
  lcd.print("Stcklfsch/Wndrlb");
*/
  //rudderController.begin(); // todo: pass lcd?
  engineMonitor.init(sensors, pinCoolantWaterFlow, addrCoolantWaterTemp, pinOilPress, addrOilTemp);
  //engineMonitor.init(pinCoolantWaterFlow, addrCoolantWaterTemp, pinOilPress, addrOilTemp);

  engineMonitor.begin();

  pinMode(ledPin, OUTPUT);

  delay(666);
}



void loop() {
  if(rudderanglePin) 
  {
    //rudderUpdated = rudderController.update_state();
    if( rudderUpdated ) 
    { 
      // rudderController.print_state();
      delay(50);
    }
  }
  else
  {
    delay(500);
  }

  engineMonitor.update_state();
  engineMonitor.print_state();


  ledPin13State = !ledPin13State;
  digitalWrite(ledPin, ledPin13State);


}
