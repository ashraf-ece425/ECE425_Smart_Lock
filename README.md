# ECE 425 Final Project: Smart Lock
December 14th, 2025 \
ECE 425 - MicroProcessor Systems \
Professor Aaron Nanas \
Author: Ashraf Abdelgawad

## Introduction
The Smart Lock project is inspired by technology that promotes safety and security. The goal for this project was to design a program that stores a 4-digit password and compares it to a user-entered password. If the passwords are a match, the Servo Motor will rotate from a 0૦ (locked) to a 90૦ (unlocked) position. Originally, the idea was to connect a 4x3 keypad (4 rows, 3 columns) to the Tiva-C Series TM4C123G Microcontroller, which would be used to directly enter the user password. Due to difficulties accessing a reliable keypad, this plan was modified to incorporate the PMOD ENC Rotary Encoder to perform this task using a slightly different approach. Some issues arose when trying to implement the Servo Motor as the Lock Arm for this device. So instead, this hardware component was set aside, and the project mainly focused on program implementation using the LCD and PMOD ENC units. The program takes the user-input password via PMOD ENC, compares it to the password stored in the program, and determines if the “lock” would unlock and displays this to the user through the LCD. More features can be added in the future to incorporate the Servo Motor and Keypad, once the program has been implemented successfully.

## Functional Block Diagram
![Block Diagram](../ECE425_Smart_Lock/Smart_Lock_Diagram.png)\
*Figure 1: Functional Block Diagram of Smart Lock System*

## Background and Methodology
The Smart Lock project incorporates a few embedded system concepts to implement the program, including General-Purpose Input/Output (GPIO), Interrupts, and the Liquid Crystal Display (LCD) driver. General-Purpose Timer Modules (GPTM) and Pulse-Width Modulation (PWM) concepts were also intended to be used, and will be used in the future, to properly implement the Servo Motor. The hardware used in this project include the Tiva-C Series TM4C123G microcontroller along with its Development Kit (EduBase Board), the EduBase RGB LED, the PMOD ENC Rotary Encoder, and the LCD unit. Software components like the SysTick Delay driver were also necessary in this project, to implement time delays which are essential to proper LCD operation as well as to turn the RGB LED off after a specified amount of time. The EduBase LCD and PMOD ENC drivers interface with each other and the microcontroller, and play vital roles in the program implementation of this project. 

The LCD can typically be used to display a menu to the user, among other uses, and allow them to choose between different menu options. In this project, the LCD displays a single menu option, for the user to “Enter Password”, and actively displays the user-entered password as each digit is selected. The program then determines if the user-entered password matches the stored password (which can easily be changed), then displays a “LOCKED” message if the password does not match or an “UNLOCKED” message if the passwords match. A supplementary RGB LED signal will be activated during this process, yielding a RED light for “LOCKED” and a GREEN light for “UNLOCKED”, to give further clarity to the user.

Counters typically coincide with the number of menu options displayed on the LCD, but a secondary password_counter variable is used to determine the digit value range (0 to 9). Instead of using a keypad to directly select each digit value, the dial on the PMOD ENC is used to increment/decrement the digit value and its button is used to select the digit value. The switch on the PMOD ENC is not needed for this project, and therefore is left unused. A position variable is also utilized to determine the current digit position and to move to the next digit position every time the user selects a digit.

## Results and Video Demonstration Link
When the program is successfully implemented and downloaded onto the microcontroller, we are able to see the predicted output on the LCD. The digit value is incremented/decremented in real time as the user turns the PMOD ENC dial, and the digit position increments every time the button is pressed. Keeping each selected digit displaying on the LCD, while the user chooses the next digit, posed a challenge initially and the currently entered password (or partial password) wasn’t displaying properly. With some program modification, this feature was implemented successfully. The program also resets when a password is entered, allowing the user to enter another password should they enter the wrong one.

In a short video demonstration, we see that when the user enters the wrong password, a “LOCKED” message appears and the red light flashes. When the user enters the correct password, an “UNLOCKED” message is displayed and the green light flashes. A custom “right arrow” character is displayed at the beginning of the second row of the LCD to direct the user to exactly where their password will be displayed as they are entering it in. A few custom “heart” characters also display with the “UNLOCKED” message for aesthetic purposes. Below is a screenshot of the display message when the user enters the correct password, followed by a video demonstration of the testing and final results for this project:\
\
!["UNLOCKED" message with Green Light](../ECE425_Smart_Lock/IMG_20251210_230151.jpg)\
*Figure 2: “UNLOCKED” Message with Green Light Signal*

[Video Demonstration Link](../ECE425_Smart_Lock/VID_20251210_225802.mp4)

## Software Used
Keil μVision (Developed by ARM Keil) - Version 2025

## Table of Components Used

| Type | Model |
|:-----:|:------:|
|Microcontroller|Tiva C Series TM4C123GH6PM LaunchPad|
|Development Kit|EduBase-V2 Solderless Breadboard|
|Rotary Encoder|PMOD ENC|
|Servo Motor|HS-485HB Deluxe|
|RGB LED|Connected to EduBase Board|
|Cable|USB-A to Micro-USB|
## Table of Pins Used

|LaunchPad Pin|Corresponding Component and Pin|
|:-----:|:------:|
|PA2|Data Pin 4 (D4)|
|PA3|Data Pin 5 (D5)|
|PA4|Data Pin 6 (D6)|
|PA5|Data Pin 7 (D7)|
|PC6|Enable Pin (E)|
|PE0|Register Select Pin (RS)|
|PF1|RGB LED RED|
|PF2|RGB LED BLUE|
|PF3|RGB LED GREEN|
|PD0|PMOD ENC Pin 1 (A)|
|PD1|PMOD ENC Pin 2 (B)|
|PD2|PMOD ENC Pin 3 (BTN)|
|PD3|PMOD ENC Pin 4 (SWT)|
|GND|PMOD ENC Pin 5 (GND)|
|3.3 V|PMOD ENC Pin 6 (VCC)|