#include "CH58x_common.h"
#include "uid.h"
#include "stdio.h"
#include "worktime.h"
#include "configtool.h"
#include "version.h"
#include "modbus.h"
#include "appinfo.h"
#include "uid.h"
#include "sensor.h"
#include "display.h"
#include "uart-modbus.h"
#include "oled.h"
#include "bmp.h"

#include "sht35.h"
/*********************************************************************
 * @fn      main
 *
 * @brief   主函数
 *
 * @return  none
 */
int uuid_dump(){
	uint8_t uuid[10] = {0};
	int i = 0;
	GET_UNIQUE_ID(uuid);
	PRINT("deviceid:");
	for (i = 0; i < 8; i++){
		PRINT("%02x ", uuid[i]);
	}
	PRINT("\r\n");
	return 0;
}
int reset_dump(){
	SYS_ResetStaTypeDef rst = SYS_GetLastResetSta();
	PRINT("rst(");
	switch(rst){
		case RST_STATUS_SW:
			PRINT("sw reset");
			break;
		case RST_STATUS_RPOR:
			PRINT("poweron");
			break;
		case RST_STATUS_WTR:
			PRINT("wdt");
			break;
		case RST_STATUS_MR:
			PRINT("manual reset");
			break;
		case RST_STATUS_LRM0:
			PRINT("software wakeup");
			break;
		case RST_STATUS_GPWSM:
			PRINT("shutdown wakeup");
			break;
		case RST_STATUS_LRM1:
			PRINT("wdt wakeup");
			break;
		case RST_STATUS_LRM2:
			PRINT("manual wakeup");
			break;
	}
	PRINT(")\r\n");
	return 0;
}

int main()
{
    uint32_t appversion;
	worktime_t worktime = 0;
	char buf[DISPLAY_LINE_LEN + 1];
    SetSysClock(CLK_SOURCE_PLL_60MHz);
	worktime_init();
	
    /* 配置串口1：先配置IO口模式，再配置串口 */
    GPIOA_SetBits(GPIO_Pin_9);
    GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);      // RXD-配置上拉输入
    GPIOA_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA); // TXD-配置推挽输出，注意先让IO口输出高电平
    UART1_DefInit();
	//rs485en3 485方向控制
	GPIOB_ModeCfg(GPIO_Pin_18,GPIO_ModeOut_PP_5mA);
	GPIOB_SetBits(GPIO_Pin_18);
	
	reset_dump();
	PRINT("app start ...\r\n");
	uuid_dump();
	
	OLED_Init();
	OLED_DisPlay_On();
//	OLED_ShowPicture(32, 0, 64, 64, (uint8_t *)smail_64x64_1, 1);
//	OLED_Refresh();

	OLED_Clear();
	oled_seneor_flag();
	OLED_Refresh();
	
	sht35_init();
	display_init();
	cfg_init();
	sensor_init();
	
	while(worktime_since(worktime) < 1000){
		__nop();
	}
//	OLED_Clear();
	//DISPLAY_PRINT("Boot:%s", CURRENT_VERSION_STR());
//	if (upgrade_app_available()){
//		appversion = upgrade_app_version();
//		version_str(appversion, buf, DISPLAY_LINE_LEN);
//		DISPLAY_PRINT("APP:%s", buf);
//	}else{
//		DISPLAY_PRINT("APP:none");
//	}
	PRINT("main loop start ...\r\n");
    while(1){
		OLED_Refresh();
		
		//更新显示时间
		if (worktime_since(worktime) >= 1000){
			worktime = worktime_get();
				if ((worktime / 1000) % 2){ 
				read_display_data();
//				OLED_ShowPicture(32, 0, 64, 64, (uint8_t *)smail_64x64_2, 1);
				}
////			else{
//			}
		}
		sensor_run();
	}
}



