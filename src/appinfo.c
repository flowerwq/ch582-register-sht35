#include "appinfo.h"
#include "version.h"
static appinfo_t appinfo __attribute__((section(".appinfo"))) = {
	APP_MAGIC,
	VID_CG,
	APP_PID,
	CURRENT_VERSION(),
	__DATE__ __TIME__,
};

const appinfo_t *appinfo_get(){
	return &appinfo;
}
