#include <COM_usrlib.h>
_declspec(dllimport)
extern int HardwareOffline;
extern void outputf(char *fmt, ...);

int  simpleLogTst();

int w_simpleLogTst(callinfo_t *callinfo)
{
	int *ret = (int *)malloc(sizeof(int ));
	if(!HardwareOffline)
		*ret = (int )simpleLogTst();

	add_result("simpleLogTst", RETVAL_NAME, INT_P, ret, 1);
	return(0);
}
