// Binghamton University, EECE 287 Sophomore Design
// Final Project #
// Instructor: Meghana Jain

//Group: 47
//Student Names: Anthony Kish, Connor Meybohm

#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include "zumo_drivers.h"

// define the pin for button A with the left motor direction and speed
#define A_BUTTON_LOCATION 3
#define LEFT_MOTOR_DIRECTION 2

// define the pin for button C with the right motor direction and speed
#define C_BUTTON_LOCATION 0
#define RIGHT_MOTOR_DIRECTION 1

// define PWM parameters
#define MOTOR_SPEED_INCREMENT 5
#define MOTOR_SPEED_PWM_TOP 100
#define MOTOR_SPEED_PWM_BOTTOM 0
#define RIGHT_MOTOR_PWM_LOCATION 5  //Port B
#define LEFT_MOTOR_PWM_LOCATION 6  //Port B
#define B_BUTTON_LOCATION 5  //Port D
#define DUTY_CYCLE_DEFAULT 40

///////////// configure motors and buttons below ///////////////
void configure_A_button() {

	DDRB &= ~(1<< A_BUTTON_LOCATION);
	PORTB |= (1<< A_BUTTON_LOCATION);

}

//configure DDR and pull resistors of Button B
void configure_B_button()
{	
	DDRD &= ~(1 << B_BUTTON_LOCATION);
	PORTD |= (1 << B_BUTTON_LOCATION);     
}

void configure_right_motor() {

	DDRB &= ~(1<< RIGHT_MOTOR_DIRECTION);
	PORTB &= (1<< RIGHT_MOTOR_DIRECTION);
	DDRB &= ~(1 << RIGHT_MOTOR_PWM_LOCATION);
}

void configure_left_motor() {

	DDRB &= ~(1<< LEFT_MOTOR_DIRECTION);
	PORTB &= (1<< LEFT_MOTOR_DIRECTION);
	DDRB &= ~(1 << LEFT_MOTOR_PWM_LOCATION);
}

//configure the pwm ports of motors
void motors_pwm_high_right()
{
	PORTB |=(1 << RIGHT_MOTOR_PWM_LOCATION);
}

void motors_pwm_high_left()
{
	PORTB |= (1 << LEFT_MOTOR_PWM_LOCATION);
}
void motors_pwm_low_right()
{
	PORTB &= ~(1 << RIGHT_MOTOR_PWM_LOCATION);
}

void motors_pwm_low_left()
{
	PORTB &= ~(1 << LEFT_MOTOR_PWM_LOCATION) ;
}

///////////// detecting button a is pressed ///////////////
_Bool button_A_is_pressed() {
	
	unsigned int button_A_pressed;
	unsigned int last_button_A_state = (PINB & (1 << A_BUTTON_LOCATION));


	//Pulser for button B
		if (button_a_is_down() != last_button_A_state)
		{
			if (button_a_is_down() == 0)
				button_A_pressed = 1;
			last_button_A_state = (button_a_is_down());
		}
		else
			button_A_pressed = 0;

}

_Bool button_A_is_released() {

	return !button_A_is_pressed();

}

///////////// right motor direction change below //////////
void right_motor_foward() {

	PORTB &= (1<< RIGHT_MOTOR_DIRECTION);

}

void right_motor_backwards() {

	PORTB |= ~(1<< RIGHT_MOTOR_DIRECTION);

}

void toggle_right_motor_direction() {

	PORTB ^= (1<< RIGHT_MOTOR_DIRECTION);

}

///////////// left motor direction change below //////////
void left_motor_foward() {

	PORTB &= (1<< LEFT_MOTOR_DIRECTION);

}

//switched portb functions for left motor foward and backwards for testing purposes

void left_motor_backwards() {

	PORTB |= ~(1<< LEFT_MOTOR_DIRECTION);

}

void toggle_left_motor_direction() {

	PORTB ^= (1<< LEFT_MOTOR_DIRECTION);

}

/*
void configure_yellow_led() {
	DDRC |= (1<<YELLOW_LED_LOCATION);
}

void turn_on_yellow_led() {
	PORTC |= (1<<YELLOW_LED_LOCATION);
}

void turn_off_yellow_led() {
	PORTC &= ~(1<<YELLOW_LED_LOCATION);
}

void toggle_yellow_led() {
	PORTC ^= (1<<YELLOW_LED_LOCATION);
}
*/

int main(){
	//configure ports
	configure_A_button();
	configure_B_button();
	//configure_yellow_led();
	configure_zumo();
	configure_left_motor();
	configure_right_motor();
	//configure OLED
	oled_clear();
	
	unsigned int oled_counter = 0;
	
	unsigned int motor_speed_pwm_counter = 0;
	unsigned int duty_cycle_left = 0;
	unsigned int duty_cycle_right = 0;
	
	unsigned int button_A_pressed;
	unsigned int last_button_A_state = (PINB & (1 << A_BUTTON_LOCATION));
	unsigned int button_B_pressed;
	unsigned int last_button_B_state = (PINB & (1 << B_BUTTON_LOCATION));
	unsigned int button_C_pressed;
	unsigned int last_button_C_state = (PINB & (1 << C_BUTTON_LOCATION));
	
	uint8_t oled_row = 0;
	uint8_t oled_column = 0;
	oled_set_cursor( oled_row, oled_column);
	oled_put_hex(oled_counter >> 4);
	oled_put_hex(oled_counter);
	
	uint32_t right_quad_value;
	uint32_t left_quad_value;
	uint32_t left_quad_turn_value;
	uint32_t left_quad_difference_one = 0;
	uint32_t right_quad_difference_two = 0;
	uint32_t right_quad_x_value;

	
	signed int x_coordinate = 0;
	signed int y_coordinate = 0;
	signed int direction_set_x = 0;
	signed int direction_x_check = 0;
	signed int direction_set_y = 0;
	signed int direction_y_check = 0;
	
	bool turn_complete = 0;
	bool x_direction_complete = 0;
	unsigned int y_direction_complete = 0;

	left_motor_foward();
	right_motor_foward();

while(1){	
	right_quad_value = get_right_quadrature_counter();
	left_quad_value = get_left_quadrature_counter();
	
	//Change motor direction
	if(button_A_pressed == 1)
	{
		oled_set_cursor( oled_row, oled_column);

		oled_counter = oled_counter - 1;
		
		
		//
		if(oled_row == 0)
		{
			direction_set_y = direction_set_y - 1;
			direction_y_check = direction_set_y;
		}
		else if(oled_row == 2)
		{
			direction_set_x = direction_set_x - 1;
			direction_x_check = direction_set_x;
		}
		
		if (oled_counter == 11111111)
			oled_counter = 0;
			//oled_clear();
			oled_put_hex(oled_counter >> 4);
			oled_put_hex(oled_counter);
			
		//checks to see if direction_y_check is negative. if it is negative then it will make the motors go backwards, positive will go foward
		if(direction_y_check<0)
		{
			right_motor_backwards();
			left_motor_backwards();
		}
		else
		{
			right_motor_foward();
			left_motor_foward();
		}

	}
	
	//Motor Speed PWM Counter
	
		motor_speed_pwm_counter = motor_speed_pwm_counter + 1;
		if (motor_speed_pwm_counter >= MOTOR_SPEED_PWM_TOP)
			motor_speed_pwm_counter = MOTOR_SPEED_PWM_BOTTOM;
		
		//PWM for motor speed 
		if (motor_speed_pwm_counter < duty_cycle_right)
			motors_pwm_high_right();
		else
			motors_pwm_low_right();
		
		if (motor_speed_pwm_counter < duty_cycle_left)
			motors_pwm_high_left();
		else
			motors_pwm_low_left();
		
		//Pulser for button A
		if (button_a_is_down() != last_button_A_state)
		{
			if (button_a_is_down() == 0)
				button_A_pressed = 1;
			last_button_A_state = (button_a_is_down());
		}
		else
			button_A_pressed = 0;
		
		//Pulser for button B
		if (button_b_is_down() != last_button_B_state)
		{
			if (button_b_is_down() == 0)
				button_B_pressed = 1;
			last_button_B_state = (button_b_is_down());
		}
		else
			button_B_pressed = 0;
		
		//Pulser for button C
		if (button_c_is_down() != last_button_C_state)
		{
			if (button_c_is_down() == 0)
				button_C_pressed = 1;
			last_button_C_state = (button_c_is_down());
		}
		else
			button_C_pressed = 0;
		
		
		
		if(left_quad_value == 200*y_coordinate && oled_row == 5 && y_direction_complete == 0)
		{
			duty_cycle_left = 0;
			duty_cycle_right = 0;
			
			//left_quad_value = 0;
			
			left_motor_foward();
			right_motor_foward();
			
			y_direction_complete = 1;

		}
		
		
		//B button actions
		
		if (button_B_pressed == 1)
		{	
			oled_put_space();
			oled_counter = 0;
			
			oled_row = oled_row + 2;
		}
		
		/*
		if (button_B_pressed == 1)
		{
			if (duty_cycle_left < MOTOR_SPEED_PWM_TOP)
				duty_cycle_left = duty_cycle_left + MOTOR_SPEED_INCREMENT;
			else
				duty_cycle_left = MOTOR_SPEED_PWM_TOP;
		}
		*/
		
		//duty cycle increment based on button C being pressed
		if (button_C_pressed == 1)
		{
			oled_set_cursor( oled_row, oled_column);
			
			oled_counter = oled_counter + 1;
			
			if(oled_row == 0)
			{
				direction_set_y = direction_set_y + 1;
				direction_y_check = direction_set_y;
			}
				else if(oled_row == 2)
				{
					direction_set_x = direction_set_x + 1;
					direction_x_check = direction_set_x;
				}
			
			if (oled_counter == 11111111)
				oled_counter = 0;
			//oled_clear();
			oled_put_hex(oled_counter >> 4);
			oled_put_hex(oled_counter);
			
			//checks to see if direction_y_check is negative. if it is negative then it will make the motors go backwards, positive will go foward
			if(direction_y_check<0)
			{
				right_motor_backwards();
				left_motor_backwards();
			}
			else
			{
				right_motor_foward();
				left_motor_foward();
			}
		
		}
		
		
		if(oled_row == 0)
			{
				if(direction_set_y<0)
				{
					y_coordinate = direction_set_y*-1;
				}
				else
				y_coordinate = direction_set_y;
			}
		else if(oled_row == 2)
			{
				if(direction_set_x<0)
				{
					x_coordinate = direction_set_x*-1;
				}
				else
				x_coordinate = direction_set_x;
			}
		//activates motors when button b is pressed for the second time 
		else if(oled_row == 4)
		{
			duty_cycle_left = 40;
			duty_cycle_right = 40;
			oled_row = 5;
		}
		
		//x coordinate turn start
		if ((direction_x_check < 0) && (y_direction_complete == 1))
		//turn robot left
		{
			left_quad_difference_one = get_left_quadrature_counter();
			toggle_left_motor_direction();
			duty_cycle_left = DUTY_CYCLE_DEFAULT;
			duty_cycle_right = DUTY_CYCLE_DEFAULT;
			y_direction_complete = 2;
		}
		else if ((direction_x_check > 0) && (y_direction_complete == 1))
		//turn robot right
		{
			left_quad_difference_one = get_left_quadrature_counter();
			toggle_right_motor_direction();
			duty_cycle_left = DUTY_CYCLE_DEFAULT;
			duty_cycle_right = DUTY_CYCLE_DEFAULT;
			y_direction_complete = 2;
		}
		
		if(y_direction_complete == 2)
		{
			left_quad_turn_value = get_left_quadrature_counter() - left_quad_difference_one;
		}

		/*
		if(left_quad_value == (left_quad_value + 1) && y_direction_complete == 2)
		{
			left_quad_turn_value = left_quad_turn_value + 1;
		}
		*/
	
		//finish turn, turn motors off and reset left_quad_value
		if ((left_quad_turn_value >= 685) && (y_direction_complete == 2))
		{
			right_quad_difference_two = get_right_quadrature_counter();
			
			duty_cycle_left = 0;
			duty_cycle_right = 0;			
			
			
			if(direction_x_check < 0)
			{
				toggle_left_motor_direction();
			}
			else if(direction_x_check > 0)
			{
				toggle_right_motor_direction();
			}
						
			y_direction_complete = 3;
		}

		
	
		
		//starts moving the bot towards x coordinate
		if (y_direction_complete == 3)
		{
			duty_cycle_left = DUTY_CYCLE_DEFAULT;
			duty_cycle_right = DUTY_CYCLE_DEFAULT;
			y_direction_complete = 4;
		}
		
		if(y_direction_complete == 4)
		{
			right_quad_x_value = get_right_quadrature_counter() - right_quad_difference_two;
		}

		//stops the bot when it reaches x coordinate
		if ((right_quad_x_value >= 200*x_coordinate) && (y_direction_complete == 4))
		{
			duty_cycle_left = 0;
			duty_cycle_right = 0;
			y_direction_complete = 5;
		}
		
		
		_delay_us(5); //creates a short delay
		
	}
	return 0;
}

