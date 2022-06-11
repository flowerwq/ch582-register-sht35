#include "uart-modbus.h"
#include "CH58x_common.h"
#include <stdio.h>
#include <assert.h>
/*#include "timer.h"*/
#include "litmodbus.h"
#include <lightmodbus/lightmodbus.h>

#define REG_COUNT 16
static uint16_t registers[REG_COUNT];
static uint16_t inputRegisters[REG_COUNT];
static uint8_t coils[REG_COUNT / 8];          // Each coil corresponds to one bit
static uint8_t discreteInputs[REG_COUNT / 8]; // Each input corresponds to one bit

ModbusErrorInfo err;
ModbusSlave slave;

/*
    A register callback that prints out everything what's happening
*/
ModbusError registerCallback(
    const ModbusSlave *slave,
    const ModbusRegisterCallbackArgs *args,
    ModbusRegisterCallbackResult *result)
{
    // Use functions from debug utilities to nicely
    // convert enum values to strings
    switch (args->query)
    {
        // Pretend we allow all access
        // Tip: Change MODBUS_EXCEP_NONE to something else
        //      and see what happens
        case MODBUS_REGQ_R_CHECK:
        case MODBUS_REGQ_W_CHECK:
            result->exceptionCode =args->index < REG_COUNT ? MODBUS_EXCEP_NONE :
            MODBUS_EXCEP_ILLEGAL_ADDRESS;
            break;

        // Return 7 when reading
        case MODBUS_REGQ_R:
//            result->value = 7;
			 switch (args->type)
            {
                case MODBUS_HOLDING_REGISTER:
					result->value = registers[args->index];
					break;
                case MODBUS_INPUT_REGISTER:   
					result->value = inputRegisters[args->index];
					break;
                case MODBUS_COIL:             
					result->value = modbusMaskRead(coils, args->index);
					break;
                case MODBUS_DISCRETE_INPUT:   
					result->value = modbusMaskRead(discreteInputs, args->index);
					break;
            }
						
            break;

        // Ignore write requests
        case MODBUS_REGQ_W:
			  switch (args->type)
            {
                case MODBUS_HOLDING_REGISTER:
					registers[args->index] = args->value;
					break;
                case MODBUS_COIL:          
					modbusMaskWrite(coils, args->index, args->value);
					break;
                default:                   
					break;
            }
            break;
    }

    // Always return MODBUS_OK
    return MODBUS_OK;
}

/*
    Exception callback for printing out exceptions
*/
ModbusError slaveExceptionCallback(const ModbusSlave *slave, uint8_t function, ModbusExceptionCode code)
{
    printf("Slave exception %s (function %d)\n", modbusExceptionCodeStr(code), function);

    // Always return MODBUS_OK
    return MODBUS_OK;
}

/*
    Helper function for printing out frames
*/
void printBytes(const uint8_t *data, int length)
{
    for (int i = 0; i < length; i++)
        PRINT("0x%02x ", data[i]);
}

/*
    Helper function for printing out ModbusErrorInfo
*/
void printErrorInfo(ModbusErrorInfo err)
{
    if (modbusIsOk(err))
    {
        PRINT("OK\r\n");
    }
    else
    {
        PRINT("%s: %s\r\n",
            modbusErrorSourceStr(modbusGetErrorSource(err)),
            modbusErrorStr(modbusGetErrorCode(err)));
    }
}



//receive datas,start timer  and update status
int modbus_uart_recv(){
	//status:IDLE
	//save data
	//T1.5 START
	//IDLE -> recv
	//status:RECVING
	//save data
	//reset T1.5
	//status: CAW
	//drop data
	//CAW -> IDLE
	
	req_buffer[req_len]=UART3_RecvByte();
	req_len++;	
	switch (status)
	{	case IDLE:
		{
			if(req_len == 1)
			{
				TMR0_TimerInit((FREQ_SYS / 1000)*((1.5*10/MB_BAUDRATE)*1000));
				TMR0_Enable();
				status=REC_FLAG;
				TMR0_ITCfg(ENABLE, TMR0_3_IT_CYC_END); // 开启中断
				PFIC_EnableIRQ(TMR0_IRQn);
					
			}
			break;
		}
		case REC_FLAG:
		{			
			TMR0_TimerInit((FREQ_SYS / 1000)*((1.5*10/MB_BAUDRATE)*1000));
			TMR0_Enable();
			break;
			
		}
	
		case CAW:
		{
	      //drop data
	      //update status
	      //timer close
			req_len=0;
			status=IDLE;
			TMR0_Disable();
			break;
		}
	}
	return 0;
	
}

//slave init and Assign status to the initial value  
void modbus_init(){
    PRINT("%s:start ...\r\n", __FUNCTION__);

    // Init slave
    err = modbusSlaveInit(
        &slave,
        registerCallback,
        slaveExceptionCallback,
        modbusStaticAllocator,
        modbusSlaveDefaultFunctions,
        modbusSlaveDefaultFunctionCount);
    PRINT("Master init: "); 
	printErrorInfo(err); 
	PRINT("\r\n");
	status=IDLE;
	PRINT("%s:finish \r\n", __FUNCTION__);

}


//parse Request in RTU modle
int modbus_rtu(uint8_t *data,int len){
	//assert(modbusIsOk(err) && "modbusSlaveInit() failed!");
    err = modbusParseRequestRTU(
            &slave,
            1,
            data,
            len);
    PRINT("Parse request: ");
    printErrorInfo(err);
    PRINT("\r\n");

    // Check for any serious errors
    if(modbusGetGeneralError(err) == MODBUS_OK){

    // Print out the response
	    PRINT("Response: ");
	    printBytes(modbusSlaveGetResponse(&slave), modbusSlaveGetResponseLength(&slave));
	    PRINT("\r\n");
	}
    return 0;
}

__INTERRUPT
__HIGH_CODE
void TMR0_IRQHandler(void) // TMR0 定时中断
{
	if(TMR0_GetITFlag(TMR0_3_IT_CYC_END))
    {
        TMR0_ClearITFlag(TMR0_3_IT_CYC_END); // 清除中断标志
		switch (status)
		{	
			case REC_FLAG://update status  //reset timer(T2)
			{
				status=CAW;
				TMR0_TimerInit((FREQ_SYS / 1000)*((2.0*10/MB_BAUDRATE)*1000));
				TMR0_ITCfg(ENABLE, TMR0_3_IT_CYC_END); // 开启中断
				PFIC_EnableIRQ(TMR0_IRQn);
				break;
			}
			case CAW:
			{
				status=REC_FINISH_FLAG;
				break;
			}	
		}
	}
}



//////	status: recv_finish
//////	parse packet
//////	send response
//////	recv_finish -> IDLE

void judge_status_function(){
	if(status == REC_FINISH_FLAG)
	{		
			modbus_rtu(req_buffer,req_len);
			GPIOB_ResetBits(GPIO_Pin_18);
			UART3_SendString(modbusSlaveGetResponse(&slave), modbusSlaveGetResponseLength(&slave));
			while(R8_UART3_TFC || !(UART3_GetLinSTA() & RB_LSR_TX_ALL_EMP));//use fifo ,but must wait it empty
			GPIOB_SetBits(GPIO_Pin_18);
			status=IDLE;
			req_len = 0;
	}
}


