/*
 * @file main.c
 *
 * @brief Main source code for the Smart Door Lock program.
 *
 * This file contains the main entry point and function definitions for the Smart Door Lock program.
 * It interfaces with the PMOD ENC Rotary Encoder (Replaces KeyPad), and RGB LED of the
 * Tiva C Series TM4C123G LaunchPad. The RGB LED will be used to indicate whether the
 * lock is in a locked or unlocked position.
 *
 * It interfaces with the following:
 *	- EduBase Board LEDs (LED0 - LED3)
 *	- EduBase Board 16x2 Liquid Crystal Display (LCD)
 *  - PMOD ENC Module (Rotary Encoder) - this replaces the need for a KeyPad
 *
 * To verify the pinout of the user LED, refer to the Tiva C Series TM4C123G LaunchPad User's Guide
 * Link: https://www.ti.com/lit/pdf/spmu296
 *
 * @note For more information regarding the LCD, refer to the HD44780 LCD Controller Datasheet.
 * Link: https://www.sparkfun.com/datasheets/LCD/HD44780.pdf
 *
 * @author Ashraf Abdelgawad
 */

#include "TM4C123GH6PM.h"
#include "SysTick_Delay.h"
#include "EduBase_LCD.h"
#include "PMOD_ENC.h"
#include "Timer_0A_Interrupt.h"
#include "GPIO.h"

#define MENU_MAX_COUNT 0
#define PASSWORD_MAX_COUNT 9

static uint8_t state = 0;
static uint8_t last_state = 0;
static uint8_t pmod_enc_btn_pressed = 0;
static int prev_main_menu_counter = 0xFF;
static int main_menu_counter = 0;
static int password_counter = 0;
static uint8_t position = 0;

static uint8_t entered_code[4] = {};
static const uint8_t correct_code[4] = {2, 3, 4, 5};

/**
 * @brief Reads the state of the PMOD ENC module every 1 ms.
 *
 * The PMOD_ENC_Task function is called when the Timer 0A module triggers a periodic interrupt
 * every 1 ms. It reads the state of the PMOD ENC module. It sets the pmod_enc_btn_pressed flag
 * if the button is pressed. In addition, it increments or decrements the global variable, main_menu_counter,
 * depending on the direction of the rotary encoder's rotation. This is used to indicate the active item on the LCD menu.
 *
 * @param None
 * @return None
 */
void PMOD_ENC_Task(void);

/**
 * @brief Displays the main menu items on the LCD based on the value of main_menu_state.
 *
 * Determines the menu item that is currently selected
 * based on the global variable, main_menu_counter.
 *
 * @param main_menu_state
 * @return None
 */
void Display_Main_Menu(int main_menu_state);

/**
 * @brief Handles main menu selection whenever the PMOD ENC button is pressed.
 *
 * This function calls corresponding functions based on the menu item selected when
 * the button on the PMOD ENC module is pressed.
 *
 * @param None
 * @return None
 */
void Process_Main_Menu_Selection(void);


int main(void)
{
	// Initialize the SysTick timer used to provide blocking delay functions
	SysTick_Delay_Init();

	// Initialize the 16x2 LCD on the EduBase Board
	EduBase_LCD_Init();
	
	// Create custom characters on the LCD
	EduBase_LCD_Create_Custom_Character(HEART_SHAPE_LOCATION, heart_shape);
	EduBase_LCD_Create_Custom_Character(RIGHT_ARROW_LOCATION, right_arrow);
	
	// Initialize the LEDs on the EduBase board (Port B)
	EduBase_LEDs_Init();
	
	// Initialize the PMOD ENC (Rotary Encoder) module
	PMOD_ENC_Init();
	
	// Initialize Timer 0A to generate periodic interrupts every 1 ms
	// and read the state of the PMOD ENC module
	Timer_0A_Interrupt_Init(&PMOD_ENC_Task);

	// Read the state of the PMOD ENC module and assign the value to last_state
	last_state = PMOD_ENC_Get_State();
	
	// Initialize the RGB LED
	RGB_LED_Init();

	while(1)
	{		
		// Redraw menu if selection has changed
		if (prev_main_menu_counter != main_menu_counter)
		{
			EduBase_LCD_Clear_Display();
			Display_Main_Menu(main_menu_counter);
			prev_main_menu_counter = main_menu_counter;
		}
		
		Process_Main_Menu_Selection();
		
		// UPDATE CURRENT DIGIT AS USER ROTATES THE KNOB
		if (main_menu_counter == 0 && position <= 4)
		{
			// update current digit in place
			EduBase_LCD_Set_Cursor(position, 1);
			EduBase_LCD_Send_Data('0' + password_counter);
		}
		
		SysTick_Delay1ms(100);
		
		// IMPORTANT: DON'T DO THIS HERE
		// Rotation Detection
		/*
		direction = PMOD_ENC_Get_Rotation(state, last_state);
		
		if (direction != 0 && position < 4 && main_menu_counter == 0)
		{
			EduBase_LCD_Set_Cursor(position, 1);
			EduBase_LCD_Send_Data('0' + password_counter);
		}
		*/
		/*
		if (direction == 1)
		{
			if (digit < 9)
			{
				digit++;
				EduBase_LCD_Set_Cursor(position, 1);
				EduBase_LCD_Send_Data('0' + digit);
			}
		}
		else if (direction == -1)
		{
			if (digit > 0)
			{
				digit--;
				EduBase_LCD_Set_Cursor(position, 1);
				EduBase_LCD_Send_Data('0' + digit);
			}
		}*/
	}
}

void PMOD_ENC_Task(void)
{
	state = PMOD_ENC_Get_State();
	
	// Main menu counter
	if (PMOD_ENC_Button_Read(state) && !PMOD_ENC_Button_Read(last_state))
	{
			pmod_enc_btn_pressed = 1;
	}

	if (main_menu_counter < 0)
	{
			main_menu_counter = 0;
	}
	else if (main_menu_counter > MENU_MAX_COUNT)
	{
			main_menu_counter = MENU_MAX_COUNT;
	}
	
	// IMPORTANT: DON'T DO THIS HERE
	/*else
	{
			main_menu_counter = main_menu_counter + PMOD_ENC_Get_Rotation(state, last_state);
	}*/
	
	// Password counter
	password_counter = password_counter + PMOD_ENC_Get_Rotation(state, last_state);
	if (password_counter < 0)
	{
			password_counter = 0;
	}
	else if (password_counter > PASSWORD_MAX_COUNT)
	{
			password_counter = PASSWORD_MAX_COUNT;
	}
	
	// IMPORTANT: DON'T DO THIS HERE
	/*else
	{
		password_counter = password_counter + PMOD_ENC_Get_Rotation(state, last_state);
	}*/
	
	last_state = state;
}

void Display_Main_Menu(int main_menu_state)
{
	switch (main_menu_state)
	{
		case 0x00:
		{
			EduBase_LCD_Set_Cursor(1, 0);
			EduBase_LCD_Display_String("ENTER PASSWORD");
			EduBase_LCD_Set_Cursor(0, 1);
			EduBase_LCD_Send_Data(0x03);
			
			// CRUCIAL for starting position & avoiding several complications
			position = 1;
			
			EduBase_LCD_Set_Cursor(1, 1);
			EduBase_LCD_Set_Cursor(position, 1);
			EduBase_LCD_Send_Data('0' + password_counter);
			break;
		}
	}
}

void Process_Main_Menu_Selection(void)
{
	if (pmod_enc_btn_pressed == 1)
	{
		pmod_enc_btn_pressed = 0;

		switch (main_menu_counter)
		{
			case 0x00:
			{
				if (position <= 4)
				{
					// store only if position < 4
					entered_code[position - 1] = password_counter;
					EduBase_LCD_Set_Cursor(position, 1);
					EduBase_LCD_Send_Data('0' + password_counter);
					position++;
				}
				
				if (position == 5)
				{
					// Check password
					uint8_t match = 1;
					for (int i = 0; i < 4; i++)
					{
						if (entered_code[i] != correct_code[i])
						{
							match = 0;
						}
					}
					
					EduBase_LCD_Set_Cursor(1,1);
					
					if(match)
					{
						RGB_LED_Output(RGB_LED_GREEN);
						EduBase_LCD_Display_String("UNLOCKED");
						EduBase_LCD_Set_Cursor(10,1);
						EduBase_LCD_Display_Heart();
						EduBase_LCD_Set_Cursor(11,1);
						EduBase_LCD_Display_Heart();
						EduBase_LCD_Set_Cursor(12,1);
						EduBase_LCD_Display_Heart();
					}
					else
					{
						RGB_LED_Output(RGB_LED_RED);
						EduBase_LCD_Display_String("LOCKED");
					}
					SysTick_Delay1ms(3000);
					RGB_LED_Output(RGB_LED_OFF);
					EduBase_LCD_Clear_Display();
					
					// RESET FOR NEW ATTEMPT
					position = 1;
					password_counter = 0;
					
					// DISPLAY NEW INPUT
					EduBase_LCD_Set_Cursor(1, 0);
					EduBase_LCD_Display_String("ENTER PASSWORD");
					EduBase_LCD_Set_Cursor(0,1);
					EduBase_LCD_Send_Data(0x03);  // arrow
					EduBase_LCD_Set_Cursor(position, 1);
					EduBase_LCD_Send_Data('0' + password_counter);
				}
				break;
			}
		}
	}
}