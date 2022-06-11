#ifndef __MODBUS_IREGS_H__
#define __MODBUS_IREGS_H__
#include "stdint.h"
#include "lightmodbus/lightmodbus.h"

#include "appinfo.h"

#define PRODUCT_THRH 0
#define PRODUCT_WATER  1
typedef enum mb_ireg_addr{
	MB_IREG_ADDR_BASE = 0,
	MB_IREG_ADDR_MAX,
#if defined(PRODUCT_THRH)
	MB_IREG_ADDR_THRH_BASE = 1,
	MB_IREG_ADDR_TEMPERATURE = MB_IREG_ADDR_THRH_BASE,
	MB_IREG_ADDR_HUMIDITY,
	MB_IREG_ADDR_THRH_MAX,

#elif defined(PRODUCT_WATER)
	MB_IREG_ADDR_WATER_BASE = 1,
	MB_IREG_ADDR_WATER_LEVEL = MB_IREG_ADDR_WATER_BASE,
	MB_IREG_ADDR_WATER_MAX,

#endif
} mb_ireg_addr_t;

void modbus_ireg_update(mb_ireg_addr_t addr, uint16_t value);
void modbus_iregs_init();
ModbusError modbus_ireg_callback(void *ctx, 
	const ModbusRegisterCallbackArgs *args,
	ModbusRegisterCallbackResult *out);

#endif
