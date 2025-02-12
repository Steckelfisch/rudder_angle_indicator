
#include "Arduino.h"
#include "EngineMonitor.h"


// Create a new instance of the oneWire class to communicate with any OneWire device:
// OneWire oneWire(ONE_WIRE_BUS);
// Pass the oneWire reference to DallasTemperature library:
// DallasTemperature sensors(&oneWire);
// variable to hold device addresses
DeviceAddress thermometer;

WaterFlow coolantWaterFlow; // WATER_FLOW_PIN, 65);

void count()
{
  coolantWaterFlow.pulseCount();
}

void printAddress(DeviceAddress deviceAddress)
{ 
  for (uint8_t i = 0; i < 8; i++)
  {
    Serial.print("0x");
    if (deviceAddress[i] < 0x10) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
    if (i < 7) Serial.print(", ");
  }
  Serial.println("");
}

EngineMonitor::EngineMonitor()
{
}

//void EngineMonitor::init(int pinCoolantWaterFlow, int addrCoolantWaterTemp, int pinOilPress, int addrOilTemp)
void EngineMonitor::init(DallasTemperature _sensors, int pinCoolantWaterFlow, int addrCoolantWaterTemp, int pinOilPress, int addrOilTemp)
{
  sensors = _sensors;
  _pinCoolantWaterFlow = pinCoolantWaterFlow;
  _addrCoolantWaterTemp = addrCoolantWaterTemp;
  _pinOilPress = pinOilPress; 
  _addrOilTemp = addrOilTemp;
  _deviceCount = 0;
}

EngineMonitor::EngineMonitor(DallasTemperature _sensors, int pinCoolantWaterFlow, int addrCoolantWaterTemp, int pinOilPress, int addrOilTemp)
{
  sensors = _sensors;
  _pinCoolantWaterFlow = pinCoolantWaterFlow;
  _addrCoolantWaterTemp = addrCoolantWaterTemp;
  _pinOilPress = pinOilPress; 
  _addrOilTemp = addrOilTemp;
  _deviceCount = 0;
  

  //_flowCount = 0;
  // coolantWaterFlow
}


void EngineMonitor::begin()
{
  if (!_pinCoolantWaterFlow)
  {
    coolantWaterFlow.init(_pinCoolantWaterFlow, 45);
    coolantWaterFlow.begin(count);
    
    // pinMode(_pinCoolantWaterFlow), INPUT);
    // digitalWrite(_pinCoolantWaterFlow), INPUT_PULLUP);  
    // attachInterrupt(digitalPinToInterrupt(_pinCoolantWaterFlow), handleFlowIntHandle, FALLING);
  }

  // if (_addrCoolantWaterTemp != 0) || (_addrOilTemp != 0)
  //if (_addrCoolantWaterTemp==0 && _addrOilTemp==0)
  //{
    sensors.begin();
    // Locate the devices on the bus:
    Serial.println("Locating devices...");
    Serial.print("Found ");
    _deviceCount = sensors.getDeviceCount();
    Serial.print(_deviceCount);
    Serial.println(" devices");

    Serial.println("Printing addresses...");
    for (int i = 0;  i < _deviceCount;  i++)
    {
      Serial.print("Sensor ");
      Serial.print(i+1);
      Serial.print(" : ");
      sensors.getAddress(thermometer, i);
      printAddress(thermometer);
      sensors.setResolution(i, 9);

    }
  //}
  // report parasite power requirements
	Serial.print("Parasite power is: "); 
	if (sensors.isParasitePowerMode()) Serial.println("ON");
	else Serial.println("OFF");

}

bool EngineMonitor::update_state()
{
  if (!_pinCoolantWaterFlow)
  {
    // coolantWaterFlow.pulseCount();
  }

  //if (_addrCoolantWaterTemp||_addrOilTemp)
  //{
  sensors.requestTemperatures();
  // delay(1000);
  //}

  
  float tempC = sensors.getTempCByIndex(0);
  if(tempC == DEVICE_DISCONNECTED_C) 
  {
		Serial.println("Error: Could not read temperature data");
		return;
	} 
  else 
  {
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.println("°C");
  }
  
  //if (!_addrCoolantWaterTemp) 
  //{
    Serial.print(sensors.getResolution(1));
    Serial.print(" Koelwater Temp: ");
    // Serial.print(sensors.getTempC(_addrCoolantWaterTemp));
    float tmp1 = sensors.getTempCByIndex(1);
    Serial.print(tmp1);
    Serial.println("°C ");
  //}

  //if (!_addrOilTemp)
  //{
    Serial.print("Olie Temp: ");
    // Serial.print(sensors.getTempC(_addrCoolantWaterTemp));
    Serial.print(sensors.getTempCByIndex(0));
    Serial.println("°C ");
  //}
  return false;
}

void EngineMonitor::print_state()
{
}
