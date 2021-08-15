
#ifndef __I2C_H__
#define __I2C_H__

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
#define SYSTEM_CLOCK 				16000000
#define ADXL345_ADDRESS 		0x53
#define DATAX0_ADDRESS			0x32
#define DATAZ0_ADDRESS			0x36
#define DATAZ1_ADDRESS			0x37
#define POWER_CTL_ADDRESS   0x2D
#define MEASURE							(1<<3)
extern void I2C_Init(void);


//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __I2C_H__
