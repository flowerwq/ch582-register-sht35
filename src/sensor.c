#include "stdio.h"
#include "CH58x_common.h"
#include "worktime.h"
#include "modbus.h"
#include "configtool.h"
#include "appinfo.h"
#include "utils.h"
#include "uid.h"

#define TAG "SENSOR"

#define MB_OPT_RESET 0
#define MB_OPT_RELOAD	1

typedef  struct sensor_context {
	uint8_t flag_init;
	uint32_t worktime;
}sensor_ctx_t;
static sensor_ctx_t sensor_ctx;
/*
 * @brief Save configuration 
 */
static int sensor_save_config(){
	cfg_uart_t cfg_uart = {0};
	cfg_get_mb_uart(&cfg_uart);
	cfg_uart.baudrate = modbus_reg_get(MB_REG_ADDR_BAUDRATE_H);
	cfg_uart.baudrate <<= 16;
	cfg_uart.baudrate += modbus_reg_get(MB_REG_ADDR_BAUDRATE_L);
	cfg_update_mb_uart(&cfg_uart);
	
	return 0;
}

static int mb_reg_before_write(mb_reg_addr_t addr, uint16_t value){
	switch(addr){
		case MB_REG_ADDR_SAFE_ACCESS_CTRL:
			if (value && value != 0x3736 ){
				return -1;
			}
			break;
	}
	return 0;
}

static int mb_opt_handle(uint16_t action){
	switch(action){
		case MB_OPT_RESET:
			SYS_ResetExecute();
			break;
		case MB_OPT_RELOAD:
			sensor_save_config();
			break;
		default:
			LOG_ERROR(TAG, "unknown operation(%d)", action);
			break;
	}
	return 0;
}

static int mb_reg_after_write(mb_reg_addr_t addr, uint16_t value){
	switch(addr){
		case MB_REG_ADDR_TEST_1:
			if (value != 0x5AA5){
				modbus_reg_update(MB_REG_ADDR_TEST_1, 0x5AA5);
			}
			break;
		case MB_REG_ADDR_TEST_2:
			modbus_reg_update(MB_REG_ADDR_TEST_2, ~value);
			break;
		case MB_REG_ADDR_OPT_CTRL:
			mb_opt_handle(value);
			break;
		case MB_REG_ADDR_SAFE_ACCESS_CTRL:
			if (0x3736 == value){
				modbus_sa_ctrl(1);
			}else if (0 == value){
				modbus_sa_ctrl(0);
			}
			break;
	}
	return 0;
}

int mb_init(){
	mb_callback_t mb_cbs  = {0};
	const appinfo_t *appinfo = appinfo_get();
	mb_cbs.after_reg_write = mb_reg_after_write;
	modbus_init(&mb_cbs);
	modbus_reg_update(MB_REG_ADDR_APP_STATE, MB_REG_APP_STATE_APP);
	modbus_reg_update(MB_REG_ADDR_APP_VID, appinfo->vid);
	modbus_reg_update(MB_REG_ADDR_APP_PID, appinfo->pid);
	modbus_reg_update(MB_REG_ADDR_VERSION_H, appinfo->version >> 16);
	modbus_reg_update(MB_REG_ADDR_VERSION_L, appinfo->version);
	modbus_reg_update_uid(get_uid(), UID_LENGTH);
	modbus_reg_update(MB_REG_ADDR_TEST_1, 0x5AA5);
	return 0;
}


int sensor_init(){
	if (sensor_ctx.flag_init){
		return 0;
	}
	memset(&sensor_ctx, 0, sizeof(sensor_ctx_t));
	mb_init();
	sensor_ctx.flag_init = 1;
	return 0;
}


int sensor_run(){
	uint32_t worktime = 0;
	modbus_run();
	worktime = worktime_get()/1000;
	if (worktime != sensor_ctx.worktime){
		sensor_ctx.worktime = worktime;
		modbus_reg_update(MB_REG_ADDR_WORKTIME_H, (worktime / 1000) >> 16);
		modbus_reg_update(MB_REG_ADDR_WORKTIME_L, (worktime / 1000));
	}
	return 0;
}
