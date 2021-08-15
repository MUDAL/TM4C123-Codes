#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_sysctl.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/timer.h"
#include "usblib/usblib.h"
#include "usblib/usbhid.h"
#include "usblib/device/usbdevice.h"
#include "usblib/device/usbdhid.h"
#include "usblib/device/usbdhidkeyb.h"
#include "usb_keyb_structs.h"
#include "buttons.h"
#include "Timer.h"

volatile bool device_Connected = false; //Indicates connection to USB host

//*****************************************************************************
//
// Handles asynchronous events from the HID keyboard driver.
//
// \param pvCBData is the event callback pointer provided during
// USBDHIDKeyboardInit().  This is a pointer to our keyboard device structure
// (&g_sKeyboardDevice).
// \param ui32Event identifies the event we are being called back for.
// \param ui32MsgData is an event-specific value.
// \param pvMsgData is an event-specific pointer.
//
// This function is called by the HID keyboard driver to inform the application
// of particular asynchronous events related to operation of the keyboard HID
// device.
//
// \return Returns 0 in all cases.
//
//*****************************************************************************
uint32_t
KeyboardHandler(void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgData,
                void *pvMsgData)
{
    switch (ui32Event)
    {
        // The host has connected to us and configured the device.
        case USB_EVENT_CONNECTED:
					device_Connected = true;
          break;
       
        // The host has disconnected from us.
        case USB_EVENT_DISCONNECTED:
            device_Connected = false;
            break;
        
        default:
						break;
        
    }
    return(0);
}

int
main(void)
{
    // Set the clocking to run from the PLL at 50MHz.
    MAP_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    // Configure the required pins for USB operation.
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    MAP_GPIOPinTypeUSBAnalog(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5);
		
    Buttons_Init();
		Timer0_Init();
		Timer1_Init();
	
		Buttons_t button1 = {0,0,0};
		Buttons_t button2 = {0,0,1};
		Buttons_t button[] = {button1, button2};
		const uint8_t BUTTON[] = {BUTTON1, BUTTON2};
		uint8_t buttonRegister = 0;
	
    device_Connected = false;
		
    USBStackModeSet(0, eUSBModeForceDevice, 0);// Initialize the USB stack for device mode.
    //
    // Pass our device information to the USB HID device class driver,
    // initialize the USB controller, and connect the device to the bus.
    //
    USBDHIDKeyboardInit(0, &g_sKeyboardDevice);
		
    while(1) {
			
        if(device_Connected) {
					
						for (int i = 0; i < 2; i++){
							button[i].pressed = !(GPIO_PORTF_DATA_R & BUTTON[i]);
							Buttons_Poll(&button[i], &buttonRegister);
						}
						
						if (Buttons_pressTimeElapsed){
						
							switch(buttonRegister){
								case 0x01:
									USBDHIDKeyboardKeyStateChange((void *)&g_sKeyboardDevice,HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_O ,true);
									USBDHIDKeyboardKeyStateChange((void *)&g_sKeyboardDevice,HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_O ,false);
									break;
								
								case 0x02:
									USBDHIDKeyboardKeyStateChange((void *)&g_sKeyboardDevice,HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_L ,true);
									USBDHIDKeyboardKeyStateChange((void *)&g_sKeyboardDevice,HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_L ,false);
									break;
								
								case 0x03:
									USBDHIDKeyboardKeyStateChange((void *)&g_sKeyboardDevice,HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_M ,true);
									USBDHIDKeyboardKeyStateChange((void *)&g_sKeyboardDevice,HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_M ,false);
									break;
								
							}
							buttonRegister = 0;
							Buttons_pressTimeElapsed = 0;
							
					}
						
					
			}
		}
}

