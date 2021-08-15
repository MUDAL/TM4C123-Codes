
#ifndef __DRIVERS_BUTTONS_H__
#define __DRIVERS_BUTTONS_H__

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
// Declarations.
//
//*****************************************************************************
#define BUTTON1								0x10
#define BUTTON2								0x01

typedef struct{
	
	uint8_t pressed;
	uint8_t prevPressed;
	uint8_t bitLocation;

} Buttons_t;

extern volatile uint8_t Buttons_pressTimeElapsed;

extern void Buttons_Init(void);
extern bool Buttons_Debounce(uint8_t buttonStatus);
extern void Buttons_Poll(Buttons_t *button, uint8_t *p_buttonRegister);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __DRIVERS_BUTTONS_H__
