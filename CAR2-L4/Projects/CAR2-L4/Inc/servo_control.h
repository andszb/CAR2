/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SERVO_CONTROL_H
#define __SERVO_CONTROL_H

float pd_control(int8_t line_position_tmp);
void turn_servo(int8_t line_position_tmp);

#endif /* __SERVO_CONTROL_H */
