							ELECTRONIC BRAILLE MACHINE.

=========================================================
The embedded software for this project was developed by:
1.) Raji Olaoluwa
2.) Akinlade Joseph
3.) Ayanleye Oluwatoyin

The documentation for this software was created on 10/11/2019
Check documentation.txt for background information on the electronic braille machine
=========================================================


============= Contact Information =========================
E-mail 	 (Raji Olaoluwa): 	olaoluwaraji1999@gmail.com
E-mail 	 (Akinlade Joseph): 
LinkedIn (Raji Olaoluwa): 	https://www.linkedin.com/in/olaoluwa-raji-043a18155/
=========================================================


============= Reason for developing this project ========================
 The motivation for this project was to create a more convenient way of communication for the visually impaired. 
 ========================================================
 
 ============= 	Frameworks used =========================
 1.) Embedded C, 
 2.) Keil uVision IDE, 
 3.) CMSIS library for Texas Instrument Microcontrollers,
 4.) PuTTy (Serial window)
 5.) NotePad++ (with Speech plugin installed)
 ========================================================
 
 ============= Features =================================
 This prototype includes:
 1.) A tm4c123gh6pm tiva launchpad (Texas Instruments microcontroller)
 2.) tactile push button (x 8)
 3.) 330 ohms resistor (x 8)
 4.) 10k resistor (x 8)
 5.) 10 microfarad capacitor (x 8)
 
 The resistors and capacitors are used for debouncing the push buttons.
 ========================================================
 
 ============= Code Description =========================
 1.) The INIT.h Library contains all the initializations. It configures the
 device registers, timers and UART of the microcontroller.
 2.) The UART0.h Library contains the code responsible for the transmission of
 data from the microcontroller to the serial port of the PC.
 3.) startup_TM4C123.s is the startup assembly code for the microcontroller developed
 by ARM.
 4.) "TM4C123.h" is the device header(part of the CMSIS library). It contains all device
 registers with their respective addresses.
 5.)  				FUNCTIONS AND DECLARATIONS:
 
 --> char buttonPressed[9] = "00000000"; (see main.c)
 This is a character array in which each element spells the button(s) pressed e.g
 "10000000" means button 1 was pressed
 "02300000" means buttons 2 and 3 were pressed
 "000x0000" means space key was pressed
 "00000007" means backspace key was pressed
 "00000060" means button 6 was pressed
 "10004500" means buttons 1, 4 and 5 were pressed
 "02004560" means buttons 2, 4, 5 and 6 were pressed
 
 --> volatile uint16_t timer_notCounting = 1; (see main.c)
This variable is set when Timer0A stops counting and raises an interrupt(see void TIMER0A_Handler(){...} in main.c).
It is cleared when a button triggers an interrupt and starts Timer0A. While 
Timer0A is counting, timer_notCounting = 0.

--> volatile uint16_t check_buttonPressed = 0; (see main.c)
This variable is set when Timer0A stops counting and raises an interrupt(see void TIMER0A_Handler(){...} in main.c).
If check_buttonPressed = 1, the main loop checks the combinations of buttons pressed while the timer was still counting
(i.e. when check_buttonPressed = 0). The main loop clears 'check_buttonPressed' after checking the buttons pressed through
'buttonPressed[9]'.

--> void clearArray(char* buttonArray); (see main.c)
This function makes buttonPressed[9] = "00000000" after the main loop is done checking the buttons pressed before 
Timer0A timed out.
 
 
=========================================================


============= Installations and Setups (for Windows) =========================
1.) Refer to [https://putty.org/] to download PuTTY.
After downloading and installing PuTTY,
--> Open PuTTY
--> Click on the Session tab
--> Type 115200 in the Speed dialog box (this is the baud rate the used by the UART as configured in the software)
--> Connect your Tiva Launchpad to the computer
--> Open Device manager and check the port number for the device under Ports(COM & LPT)
--> Go back to PuTTY and enter this port number into Serial line dialog box (e.g COM1 , COM4)
--> Type a name for the session under Saved Sessions dialog box (e.g type Braille)
--> Click on Save to save the session
--> Click Open

PuTTY can be used to check (debug) that the keyboard sends the right characters to
the PC's serial port.

2.) Refer to [https://www.keil.com/demo/eval/arm.htm#/DOWNLOAD] and fill all the necessary details 
to download MDK-ARM Version 5.28a (i.e Keil uVision 5).
--> Under the 'Which device are you using?' dialog box, enter tm4c123gh6pm
--> After downloading and installing Keil uVision 5 IDE, the software packs have to be installed
--> Open the Keil uVision 5 IDE and navigate to 'Pack Installer'
--> Click on 'Devices' tab and type tm4c123gh6pm
--> On the other pane of the window, click update/install for the packs associated with the aforementioned microcontroller

3.) Visit 
#add info on how to download datasheet and device drivers
#add info notepad++ and speech plugin
=========================================================
#add info on the challenges faced and possible updates/features to be added
#---------------------------
							|
							 ----1.)Caps lock button
							 ----2.)Read whole document button
							 ----3.)Slide switch to change to numbers
