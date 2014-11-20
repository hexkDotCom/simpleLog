/* USRLIB MODULE INFORMATION

	MODULE NAME: simpleLog
	MODULE RETURN TYPE: void 
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

#include <iso646.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

// define SLDEBUG before including simpleLog.h file in order to enable DEBUG output (LOG_DEBUG, LOG_ENTRY, LOG_EXIT, LOG_VAR...)
//#define SLDEBUG
#include "simpleLog.h"

static int displayMessage(time_t msgTime, const int level, const char *origin, const char *message);

void simpleLog(  )
{
/* USRLIB MODULE CODE */
    LOG_ENTRY();

    LOG_NOTICE("This is simpleLog.");
    LOG_NOTICE("Refer to manual or simpleLogTst project for documentation and usage examples.");

    simpleLog_Flush();
    
    LOG_EXIT();
/* USRLIB MODULE END  */
}
//		Message have the following form with [brackets] being optional
//		[TimeStamp] Severity [Origin] Message

// Retain setup variables
static char *SL_PathName		= DEFAULT_SL_PATHNAME;							// path including filename
static char *SL_TimeFormat 		= DEFAULT_SL_TIMEFORMAT;						// not used by default
static char SL_Separator[10]	= DEFAULT_SL_SEPARATOR;							// sapace by default
static unsigned int  SL_FilterLevelConsole	= DEFAULT_SL_FILTER_LEVEL_CONSOLE;	// Write all messages by default
static unsigned int  SL_FilterLevelDataLog	= DEFAULT_SL_FILTER_LEVEL_DATALOG;
static unsigned int  SL_FilterLevelFile		= DEFAULT_SL_FILTER_LEVEL_FILE;		// Write all messages by default

static FILE *file;
static int status = 0;

static char buffer[SL_MAX_MSG_LEN + 128];				// TODO: use define


int simpleLog_Setup(const char *pathName, const char *timeFormat, const char *separator) {
	if (file) {										// if file already opened - try to close it first.
		status = fclose(file);
		if (status != 0) {
			// no need to do anything
		}
	}

	if (pathName!=NULL and pathName[0]!='\0') {
		if (SL_PathName!=NULL) free(SL_PathName);
		SL_PathName=malloc(strlen(pathName)+1);
		strcpy(SL_PathName, pathName);

		file = fopen(SL_PathName, "a");				// open file.
		if (!file) {
			SL_PathName = NULL;
			LOG_ERROR("Can't open file - %s", SL_PathName);
			status = -1;
		}
	}

	if (timeFormat!=NULL and timeFormat[0]!='\0') {	// time format
		if (SL_TimeFormat!=NULL) free(SL_TimeFormat);
		SL_TimeFormat=malloc(strlen(timeFormat)+1);
		strcpy(SL_TimeFormat, timeFormat);
	} else {
		SL_TimeFormat = NULL;
	}
	
	if (separator!=NULL) {							// separator
		strncpy(SL_Separator, separator, 9);
		SL_Separator[9]='\0';
	}
	
	return status;
}

void simpleLog_FilterLevel(const unsigned int filterLevelConsole, const unsigned int filterLevelDatalog, const unsigned int filterLevelFile) {
	SL_FilterLevelConsole	= filterLevelConsole;
	SL_FilterLevelDataLog	= filterLevelDatalog;
	SL_FilterLevelFile		= filterLevelFile;
	return;
}


int simpleLog_Write(const int level, const char *origin, const char *fmt, ... ) {
	int Nb;
	int Nb1;
	va_list str_args;
	time_t now;
	char message[SL_MAX_MSG_LEN]="\0";

	va_start( str_args, fmt );

		Nb1 = vsprintf(message,fmt, str_args);
		if (0<Nb1 and Nb1<1023) {
			va_end( str_args );			// Is this really necessary ?
			va_start( str_args, fmt );
			Nb=vsprintf(message, fmt, str_args);
			if (Nb!=Nb1) // Note: there may be an admissible difference due to a "\n" in the message
				strcpy(message, "/// simpleLog message discrepancy ///");
		} else {
			if (Nb1<=0)
				 strcpy(message, "/// Invalid simpleLog message ///");
			else strcpy(message, "/// simpleLog message is too large ///");
			Nb=strlen(message);	// Just so we skip the next test
		}

	va_end( str_args );

	if (Nb<=0 or message[0]=='\0' or strlen(message)>=1023)	// Some margin
		strcpy(message, "Invalid simpleLog message");

	now = time(NULL);
	status = displayMessage(now, level, origin, message);
	return status;
}

int simpleLog_Flush(void) {
	if (file) {
		status = fclose(file);						// close file
		if (status != 0) {
			printf("Error closing file. Filename - %s", SL_PathName);
			return -1;
		}
		SL_PathName = NULL;
	}
	return 0;
}

static int displayMessage(time_t msgTime, const int level, const char *origin, const char *message) {
	static int j;
	static char *ptrBuffer;

	if (message == NULL) return 0;	// if message is empty - do nothing

	j = 0;
	ptrBuffer = &buffer[0];

	// Optional time string
	if (SL_TimeFormat!=NULL and SL_TimeFormat[0]!='\0') {
		char timeStr[255];
		if (0 < strftime(timeStr, 255, SL_TimeFormat, localtime(&msgTime)))
			j += sprintf(ptrBuffer+j, "%s%s", timeStr, SL_Separator);
	}

	// Severity message. If severity mixes the levels, we print only the most important one
	#define SEVERITY (	\
				level & SL_ERROR   ? SL_MSG_ERROR :	\
				level & SL_WARNING ? SL_MSG_WARNING :	\
				level & SL_NOTICE  ? SL_MSG_NOTICE :	\
				level & SL_DEBUG   ? SL_MSG_DEBUG :	\
				"   ")	// Invalid level

	j += sprintf(ptrBuffer+j, "%s%s", SEVERITY, SL_Separator);
	
	// Optional origin message
	if ( (origin != NULL) and (origin[0]!='\0') )
		j += sprintf(ptrBuffer+j, "%s%s", origin, SL_Separator);

	// Write message itself
	j += sprintf(ptrBuffer+j, "%s", message);

	if (level & SL_FilterLevelConsole) {							// output to console
		printf("%s", ptrBuffer);
	}
	if (level & SL_FilterLevelDataLog) {							// output to datalog stream
		logOutput(MF_Error, 0, ptrBuffer);
	}
	if ( (level & SL_FilterLevelFile) && (SL_PathName != NULL) ) {	// output to file
		j += sprintf(ptrBuffer+j, "%s", "\n");				// new line
		status = fprintf(file, ptrBuffer);					// write to file
		if (status < 0) {			//
			printf("Error writing to file. Status = %d, Filename = %s", status, SL_PathName);
			return -1;
		}
	}
	return 0;
}
/* End simpleLog.c */

