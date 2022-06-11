#ifndef HARD_MODBUS_UART_MODBUS_H_
#define HARD_MODBUS_UART_MODBUS_H_


#include "CH58x_common.h"


typedef enum STATUS{
	IDLE=1,
	REC_FLAG,
	REC_FINISH_FLAG,
	CAW,
}status;
	
uint8_t i;

#define MB_BAUDRATE  9600

extern uint8_t req_buffer[256];
extern uint8_t req_len;
void timer_init();
//void uart_fuction(void);
void judge_status_function(void);
int modbus_uart_recv();
void modbus_init();
int modbus_rtu(uint8_t *data,int len);



#endif /* HARD_MODBUS_UART_MODBUS_H_ */
