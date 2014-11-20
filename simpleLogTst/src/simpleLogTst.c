/* USRLIB MODULE INFORMATION

	MODULE NAME: simpleLogTst
	MODULE RETURN TYPE: int 
	NUMBER OF PARMS: 0
	ARGUMENTS:
	INCLUDES:
#include "keithley.h"
	END USRLIB MODULE INFORMATION
*/
/* USRLIB MODULE HELP DESCRIPTION

	END USRLIB MODULE HELP DESCRIPTION */
/* USRLIB MODULE PARAMETER LIST */
#include "keithley.h"

// define SLDEBUG before including simpleLog.h file in order to enable DEBUG output (LOG_DEBUG, LOG_ENTRY, LOG_EXIT, LOG_VAR...)
// undefine SLDEBUG to disable DEBUG output and to compile out DEBUG functions so they don't take ANY runtime.
#define SLDEBUG
#include "simpleLog.h"


int simpleLogTst(  )
{
/* USRLIB MODULE CODE */

	int i = 123;
	double d = 3.14;
	char a[42] = "test string";
	
    LOG_ENTRY();


    LOG_DEBUG("This is LOG_DEBUG message.");
	LOG_ERROR("This is LOG_ERROR message.");
	LOG_WARNING("This is LOG_WARNING message.");
	LOG_NOTICE("This is LOG_NOTICE message.");

	LOG_VAR(a);
	LOG_VAR_INT(i);
	LOG_VAR_DBL(d);
	LOG_VAR_STR(a);

	simpleLog_Setup(NULL, "%Y%m%d-%H%M%S ", " ");
	LOG_DEBUG("This is LOG_DEBUG message with time stamp.");
	simpleLog_Setup(NULL, NULL, " "); // no outpit to file, no timeformat, use space as separator
	
	LOG_DEBUG("Changing filter to ignore all but error messages.");
	simpleLog_FilterLevel(SL_ERROR, SL_ERROR, SL_ERROR);
	LOG_WARNING("This is LOG_WARNING message. Should not be printed.");
	LOG_NOTICE("This is LOG_NOTICE message. Should not be printed.");
	LOG_DEBUG("This is LOG_DEBUG message. Should not be printed.");
	LOG_ERROR("This is LOG_ERROR message. Should be only message printed.");
	
	simpleLog_FilterLevel(SL_ALL, SL_NOTICE | SL_WARNING | SL_ERROR, SL_ALL);

	// Optional before quitting the program
    simpleLog_Flush();

    LOG_EXIT();

    return 0;
/* USRLIB MODULE END  */
}

/* End simpleLogTst.c */

