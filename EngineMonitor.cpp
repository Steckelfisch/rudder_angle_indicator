
#include "Arduino.h"
#include "EngineMonitor.h"


// Create a new instance of the oneWire class to communicate with any OneWire device:
OneWire oneWire(ONE_WIRE_BUS);

// Pass the oneWire reference to DallasTemperature library:
DallasTemperature sensors(&oneWire);

int _flowCount = 0;
void handleFlowIntHandle()
{
  _flowCount++;
}

EngineMonitor::EngineMonitor(int pinCoolantWaterFlow, int addrCoolantWaterTemp, int pinOilPress, int addrOilTemp)
{
  _pinCoolantWaterFlow = pinCoolantWaterFlow;
  _addrCoolantWaterTemp = addrCoolantWaterTemp;
  _pinOilPress = pinOilPress; 
  _addrOilTemp = addrOilTemp;
  _deviceCount = 0;
  //_flowCount = 0;
}


void EngineMonitor::begin()
{
  if (!_pinCoolantWaterFlow)
  {
    attachInterrupt(digitalPinToInterrupt(_pinCoolantWaterFlow), handleFlowIntHandle, CHANGE);
  }

  // if (_addrCoolantWaterTemp != 0) || (_addrOilTemp != 0)
  if (!_addrCoolantWaterTemp || !_addrOilTemp)
  {
    sensors.begin();
    // Locate the devices on the bus:
    Serial.println("Locating devices...");
    Serial.print("Found ");
    _deviceCount = sensors.getDeviceCount();
    Serial.print(_deviceCount);
    Serial.println(" devices");
  }
}

bool EngineMonitor::update_state()
{
  return false;
}

void EngineMonitor::print_state()
{
}
