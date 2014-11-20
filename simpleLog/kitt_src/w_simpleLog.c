#include <COM_usrlib.h>
_declspec(dllimport)
extern int HardwareOffline;
extern void outputf(char *fmt, ...);

void  simpleLog();

int w_simpleLog(callinfo_t *callinfo)
{
	if(!HardwareOffline)
		(void)simpleLog();

	return(0);
}
