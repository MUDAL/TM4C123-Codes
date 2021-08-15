#ifndef UART_H
#define UART_H

enum constants{MAX = 30};
void UART_Init(void);
unsigned char UART_InChar(void);
void UART_OutChar(unsigned char data);
void UART_InString(char* readStr);
void UART_OutString(char* string);

#endif //UART_H
