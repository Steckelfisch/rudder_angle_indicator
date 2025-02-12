#include "Arduino.h"
#include "RudderController.h"



RudderController::RudderController(int pinRudder)
{
  _rudderPin = pinRudder;
}

void RudderController::begin()
{
  Serial.print("Rudder Initial Startup ");
  _rudder_angle_pointer_initialized = false;
  raw = -1; // from 0 to 1023
  prev_raw = -1;
  _measure_sensitivity = 10; // 1 = high, 100 = quite low 1023 = no sensitivity at all
  // als gevoeligheid 10 is, worden de laatste en eerste 9 'units' niet gepakt. (of daar is dan een kans toe.)
  // de Vrudder min en max worden gezet bij elke mogeljke uitbreiding van de bandbreedte (gevoeligheid =  1)
  Vin = 5;
  Vrudder = 0;
  Vrudder_min = Vin; // will move to 0
  Vrudder_max = 0;  // will move to Vin
  Vdelta_max = 0;
  Vrudder_center = 0;
  Serial.print("..");
  Vminimal_delta_range = 4.200; // minimal Vdelta_max for rudderangle pointer to work
  Vdelta_narrow_range_valid = 0.020; // sensitivity of rudder angle measurement (center)
  Vdelta_medium_range_valid = 0.090; // sensitivity of rudder angle measurement (max-sb, max-bb)
  Vdelta_wide_range_valid   = 0.150; // sensitivity of rudder angle measurement (max-sb, max-bb)

  max_bb_shown = false;
  bb_shown = false;
  max_sb_shown = false;
  sb_shown = false;
  rudder_center_shown= false;
  Serial.println(" Done");
}

void RudderController::print_state()
{
  Serial.println("");
  Serial.println("======= Rudder State =====");
  Serial.print("raw: ");  Serial.println(raw, DEC);
  Serial.print("Vrudder: "); Serial.println(Vrudder, 4);
  Serial.print("Vrudder_min: "); Serial.println(Vrudder_min, 4);
  Serial.print("Vrudder_max: "); Serial.println(Vrudder_max, 4);
  Serial.print("Vdelta_max: "); Serial.println(Vdelta_max, 4);
  // Serial.print("Vminimal_delta_range: "); Serial.println(Vminimal_delta_range, 4);
  Serial.print("Vrudder_center: "); Serial.println(Vrudder_center, 4);
}



/**
 * 
 * Maximale uitslagen worden continue aangepast indien nodig
 * Dit is dan eens soort van dynamische initialisatie
 * Voor vertrek moet dan ook het roer helemaal heen en weer geweest zijn. 
 * 
 * return: true when updated,
           false otherwise
 */
bool RudderController::update_state()
{
  raw = analogRead(_rudderPin);
  // Serial.print("raw: ");  Serial.println(raw, DEC);
  if (abs(raw-prev_raw) > 1)
  { // Er is _een_ uitslagverschil waargenomen.
    // dit zit nu op '1' (maximale gevoeligheid) zodat we altijd de maximale update krijgen
    // hiermee worden de mini en maxi uitersten bepaald
    Vrudder = (raw*Vin) / 1024.0;
    // (her)bepaal maximale uitslag SB
    setRudderMaxSB(Vrudder);
    // (her)bepaal maximale uitslag BB
    setRudderMaxBB(Vrudder);
  }

  if (abs(raw-prev_raw) > _measure_sensitivity)
  { // minimale uitslagverschil is waargenomen 
    Vrudder = (raw*Vin) / 1024.0;

    // als nog niet geinitialiseerd, en minimale meetbereik is nu wel gevonden,
    // geef aan dat dat roeraanwijzer nu wel geinitialiseerd
    if (_rudder_angle_pointer_initialized == false and
        Vdelta_max > Vminimal_delta_range)
    {
      // print_state();
      _rudder_angle_pointer_initialized = true;
    }


    if (_rudder_angle_pointer_initialized)
    {
      // test voor middenstand
      if(!checkRudderDC()) 
      {
        // not in Dead Center. check SB or BB
        if(Vrudder<Vrudder_center) {
           // test voor Bakboord
           checkRudderSB();
        }
        else
        {
         // test voor Stuurboord
         checkRudderBB();
        }
      }
    } 
    else
    {
      signalNotInitialized();
    }
    
    prev_raw = raw;
    return true; // updated
  }
  return false; // not updated
}


bool  RudderController::rudderInRange(float Vref, float delta_v_range_valid)
{
  float delta_v_range = Vrudder - Vref;
  Serial.print(" rudderInRange delta_v_range: "); Serial.print(abs(delta_v_range), 4); 
  Serial.print(" delta_v_range_valid: "); Serial.print(delta_v_range_valid, 4);
  if (abs(delta_v_range) <= delta_v_range_valid)
  { 
    Serial.println(" :true");
    return true; // 0
  }
  Serial.println(" :false");
  return false;  // Vdelta_range
}


/**
 * Als uistlag bakboorder is dan de vorige bakboordse,
 * Dan zet de nieuwe bakboordse.
 *     bereken nieuwe midden
 */
void  RudderController::setRudderMaxBB(float Vrudder) {
    if (Vrudder > Vrudder_max)
    {
      Vrudder_max = Vrudder;
      Vdelta_max = Vrudder_max - Vrudder_min;
      Vrudder_center = Vrudder_min + (Vdelta_max / 2);
    }
}

/**
 * Als uistlag stuurboorder is dan de vorige stuurboordse,
 * Dan zet de nieuwe stuurboordse
 *     bereken nieuwe midden
 */
void  RudderController::setRudderMaxSB(float Vrudder) {
    if (Vrudder < Vrudder_min)
    {
      Vrudder_min = Vrudder;
      Vdelta_max = Vrudder_max - Vrudder_min;
      Vrudder_center = Vrudder_min + (Vdelta_max / 2);
    }

}

void  RudderController::checkRudderBB() {
      Serial.print("checkRudderBB ");
      if (rudderInRange(Vrudder_min, Vdelta_wide_range_valid))
      {
        if (max_bb_shown == false)
        {
          bb_shown = false;
          max_bb_shown = true;
          //Serial.print(" max_bb On. "); 
          //Serial.print(" bb Off. "); 
          //Serial.print(" Vrudder: "); Serial.println(Vrudder, 6);
        }
        return;
      }

      max_bb_shown = false;
      bb_shown = true;
      //Serial.print("max_bb Off. ");
      //Serial.print("bb On. "); 
      //Serial.print(" Vrudder: "); Serial.println(Vrudder, 6);
}


void  RudderController::checkRudderSB() {
      Serial.print("checkRudderSB ");
      if (rudderInRange(Vrudder_max, Vdelta_wide_range_valid))
      {
        if (max_sb_shown == false)
        {
          sb_shown = false;
          max_sb_shown = true;
          //Serial.print("max_sb On. "); Serial.print(" Vrudder: "); Serial.println(Vrudder, 4);
        }
        return;
      }
      max_sb_shown = false;
      sb_shown = true;
      //Serial.print("max_sb Off. "); Serial.print(" Vrudder: "); Serial.println(Vrudder, 4);
      //Serial.print("sb On. "); Serial.print(" Vrudder: "); Serial.println(Vrudder, 6);

}

bool  RudderController::checkRudderDC() { //DeadCenter
      Serial.print("checkRudderDC ");
      if (rudderInRange(Vrudder_center, Vdelta_medium_range_valid))
      {
        if (rudder_center_shown == false)
        {
          // Serial.print("Vrudder_center On. "); Serial.print(" Vrudder: "); Serial.println(Vrudder, 4);
          rudder_center_shown = true;
        }
        return true;
      } 
      // Serial.print("Vrudder_center Off. "); Serial.print(" Vrudder: "); Serial.println(Vrudder, 4);
      rudder_center_shown = false;
      return false;
}


void  RudderController::signalNotInitialized(){
      Serial.println("");
      Serial.println("======= Rudder Controller Not Initialized =====");
      // todo: flash all Rudder Pointer LED's
      Serial.println("All LED's On");
      delay(50);
      Serial.println("All LED's Off");
}
