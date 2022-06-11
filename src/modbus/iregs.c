#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "appinfo.h"
#include "modbus.h"
#define PRODUCT_THRH 0
#define PRODUCT_WATER 1
static uint16_t mb_iregs[MB_IREG_ADDR_MAX - MB_IREG_ADDR_BASE];
#if defined(PRODUCT_THRH)
static	uint16_t mb_pr_iregs[MB_IREG_ADDR_THRH_MAX - MB_IREG_ADDR_THRH_BASE];
#elif defined(PRODUCT_WATER)
static	uint16_t mb_pr_iregs[MB_IREG_ADDR_WATER_MAX - MB_IREG_ADDR_WATER_BASE];
#endif


void modbus_ireg_update(mb_ireg_addr_t addr, uint16_t value)
{
//	if (addr >= MB_IREG_ADDR_BASE && addr < MB_IREG_ADDR_MAX){
//		mb_iregs[addr - MB_IREG_ADDR_BASE] = value;
//	}
#if defined(PRODUCT_THRH)
	if (addr >= MB_IREG_ADDR_THRH_BASE && addr < MB_IREG_ADDR_THRH_MAX){
		mb_pr_iregs[addr - MB_IREG_ADDR_THRH_BASE] = value;
	}
#elif defined(PRODUCT_WATER)
	if (addr >= MB_IREG_ADDR_WATER_BASE && addr < MB_IREG_ADDR_WATER_MAX){
		mb_pr_iregs[addr - MB_IREG_ADDR_WATER_BASE] = value;
	}
#endif	
}
/**
 *@return 1-allow read;0-read operation not allowed
 */
static uint16_t modbus_ireg_r_check(uint16_t index){
//	if (index >= MB_IREG_ADDR_BASE && index < MB_IREG_ADDR_MAX)
//	{
//		return 1;
//	}
#if defined(PRODUCT_THRH)
	if (index >= MB_IREG_ADDR_THRH_BASE && index < MB_IREG_ADDR_THRH_MAX)
	{
		return 1;
	}
#elif defined(PRODUCT_WATER)
	if (index >= MB_IREG_ADDR_WATER_BASE && index < MB_IREG_ADDR_WATER_MAX)
	{
		return 1;
	}
#endif
	return 0;
}

static uint16_t modbus_ireg_read(uint16_t index){
//	if (index >= MB_IREG_ADDR_BASE && index < MB_IREG_ADDR_MAX)
//	{
//		return mb_iregs[index - MB_IREG_ADDR_BASE];
//	}
#if defined(PRODUCT_THRH)
	if (index >= MB_IREG_ADDR_THRH_BASE && index < MB_IREG_ADDR_THRH_MAX)
	{
		return mb_pr_iregs[index - MB_IREG_ADDR_THRH_BASE];
	}

#elif defined(PRODUCT_WATER)
	if (index >= MB_IREG_ADDR_WATER_BASE && index < MB_IREG_ADDR_WATER_MAX)
	{
		return mb_pr_iregs[index - MB_IREG_ADDR_WATER_BASE];
	}
#endif
	return 0;
}

ModbusError modbus_ireg_callback(void *ctx, 
	const ModbusRegisterCallbackArgs *args,
	ModbusRegisterCallbackResult *out)
{
	mb_slave_ctx_t *sctx = (mb_slave_ctx_t *)ctx;
	out->exceptionCode = MODBUS_EXCEP_NONE;
	switch(args->query){
		case MODBUS_REGQ_R_CHECK:
			if(!modbus_ireg_r_check(args->index)){
				return MODBUS_ERROR_INDEX;
			}
			break;
		case MODBUS_REGQ_R:
			out->value = modbus_ireg_read(args->index);
			break;
		default:
			return MODBUS_ERROR_INDEX;
	}
	return MODBUS_OK;
}

void modbus_iregs_init(){
	memset(mb_pr_iregs, 0, sizeof(mb_pr_iregs));
}



