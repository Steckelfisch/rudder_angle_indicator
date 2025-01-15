/*
  RudderController.h - Library for displaying Rudder status.
  Rename To RudderMonitor.h
  Created by Gert-Jan Braas
  Released into the public domain.

  The controller is self-priming. 
  full-priming is reached when max-sb and max-bb have been aquired.
  
*/
#ifndef RudderController_h
#define RudderController_h

#include "Arduino.h"

// #define RELAY_ON 1
// #define RELAY_OFF 0

// single LED rudder indicator
// not_initialized = fast_flash
// angle_max_sb = flash_flash_off
// angle_center = slow_flash
// angle_max_sb = flash_off_off
//
// or multicolor led
// puls_max_rood_groen voor niet geinitialiseerd
// puls_max_rood BBmax
// puls_max_groen SBmax
// max_rood_groen (oranje) CENTER
// pwm_rood voor BB
// pwm_groen voor SB

class RudderController
{
  public:
    RudderController(int pinRudder);
    void begin(void);
    void print_state();
    bool update_state();
    
  private:
    int _analogPin = 0;
    int raw;
    int prev_raw;
    int _measure_sensitivity; // 1 = high, 100 = quite low. This is to counter ubersensitive measuring 
    int Vin;// = 5;
    float Vrudder;// = 0;
    float Vrudder_min;// = Vin;
    float Vrudder_max;// = 0;
    float Vdelta_max;// = 0;
    float Vrudder_center;// = 0;
    //float volts;// = 0;
    bool _rudder_angle_pointer_initialized;// = false;
    float Vminimal_delta_range;// = 1.200; // minimal Vdelta_max for rudderangle pointer to work
    float Vdelta_narrow_range_valid;// = 0.020; // sensitivity of rudder angle measurement (center)
    float Vdelta_medium_range_valid;// = 0.090; // sensitivity of rudder angle measurement (max-sb,max-bb)
    float Vdelta_wide_range_valid;//   = 0.150; // sensitivity of rudder angle measurement (max-sb,max-bb)
    
    bool max_bb_shown;// = false;
    bool bb_shown;// = false;
    bool max_sb_shown;// = false;
    bool sb_shown;// = false;
    bool rudder_center_shown;// = false;
    
    bool rudderInRange(float Vref, float delta_v_range_valid);
    void setRudderMaxBB(float Vrudder);
    void setRudderMaxSB(float Vrudder);
    bool checkRudderDC();
    void checkRudderBB();
    void checkRudderSB();
    void signalNotInitialized();
};

#endif
