// servor.h

#ifndef SERVOR_H
#define SERVOR_H

#include <stdio.h> 
#include "hardware/pwm.h" 
#include "hardware/clocks.h"
#include "hardware/gpio.h"

#define PWM_PERIOD_MS 20


#define SERVO_PIN 28 

#define SERVO_MIN_PULSE_US 500
#define SERVO_MAX_PULSE_US 2500

// Protótipos das funções
void servo_init();
void servo_set_angle(uint angle);

#endif // SERVOR_H