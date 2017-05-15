#ifndef ARMS_CONTROLLER_H
#define ARMS_CONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "scara/scara.h"
#include "hand/hand.h"
#include "cvra_arm_motors.h"

#define ARMS_FREQUENCY 50

extern scara_t left_arm;
extern scara_t right_arm;

extern hand_t left_hand;
extern hand_t right_hand;

/* Initialize arms */
void arms_init(void);

/* Auto index a motor given its name */
float arms_motor_auto_index(const char* motor_name, int motor_dir, float motor_speed);

/* Run arms controller thread */
void arms_controller_start(void);

/* Index a whole bunch of motors simultaneously */
void arms_auto_index(cvra_arm_motor_t** motors, float* motor_speeds, size_t num_motors);

/* Index a whole bunch of motors simultaneously */
void arms_wrist_auto_index(cvra_arm_wrist_t** wrists, float* heading_speeds, float* pitch_speeds, size_t num_wrists);

#ifdef __cplusplus
}
#endif
#endif
