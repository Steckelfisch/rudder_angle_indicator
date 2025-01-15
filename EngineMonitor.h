/*
  EngineMonitor.h - Library for displaying (Diesel) Engine Status.
  Created by Gert-Jan Braas
  Released into the public domain.

  
*/
#ifndef EngineMonitor_h
#define EngineMonitor_h

#include "Arduino.h"
// Include the required Arduino libraries:
#include "OneWire.h"
#include "DallasTemperature.h"

// Define to which pin of the Arduino the 1-Wire bus is connected:
#define ONE_WIRE_BUS 2

// https://github.com/Tobiyouth/WaterFlow

class EngineMonitor
{
  public:
    EngineMonitor(int pinCoolantWaterFlow, int addrCoolantWaterTemp,
                  int pinOilPress, int addrOilTemp);
    void begin(void);
    void print_state();
    bool update_state();
    
  private:
    int _deviceCount;
    // int _flowCount;
    // void handleFlowIntHandle();
    int _pinCoolantWaterFlow;
    int _addrCoolantWaterTemp; 
    int _pinOilPress;
    int _addrOilTemp;

};

#endif
