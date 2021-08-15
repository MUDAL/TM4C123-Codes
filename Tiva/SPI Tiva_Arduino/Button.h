
#ifndef __BUTTON_H__
#define __BUTTON_H__

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// Prototypes.
//
//*****************************************************************************
#define BUTTON1		0x10
#define BUTTON2		0x01

typedef struct{
	
	uint8_t pressed;
	uint8_t prevPressed;

} Button_t;

extern Button_t button1;
extern Button_t button2;
extern uint8_t Button_debounced;
extern void Button_Init(void);
bool Button_Debounce(uint8_t buttonStatus);
//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __BUTTON_H__
