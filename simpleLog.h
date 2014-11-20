#ifndef __SIMPLE_LOG_H
#define __SIMPLE_LOG_H
// Company: (C) hexk@hexk.com
// Simple things should be simple and complex things should be possible (c) Alan Kay

/** @file simpleLog.h */
/** 
@class simpleLog
@brief This library provides logging facility to log to sdterr, console and files. <br>

In order to use this library please use the following defines: <br>
@ref logDefFunctions use these helper functions<br>
@ref logDefines <br>
@ref logDefaultValues <br>
@ref logLevels <br>

@ref simpleLogFunctions <br>

@ref simpleLog.h full header file<br>

refer to simpleLogTst project for examples of how to use the simpleLog library
*/

/**
@defgroup logLevels log levels
 */ 
/** @{ **/ 
///enables output of ERROR level messages
#define SL_ERROR	0x8
///enables output of WARNING level messages
#define SL_WARNING	0x4
///enables output of NOTICE level messages
#define SL_NOTICE	0x2
///enables output of DEBUG level messages
#define SL_DEBUG	0x1
///enables output of ALL level messages
#define SL_ALL		(SL_ERROR|SL_WARNING|SL_NOTICE|SL_DEBUG)
///disables output of ALL level messages
#define SL_QUIET	0x0
/** @} **/ 


/**
@defgroup logDefFunctions helper functions
 */ 
/** @{ **/ 
///outputs ERROR message
#define LOG_ERROR(...)	 		simpleLog_Write(SL_ERROR, __FUNCTION__, __VA_ARGS__)
///outputs WARNING message
#define LOG_WARNING(...)  		simpleLog_Write(SL_WARNING, __FUNCTION__, __VA_ARGS__)
///outputs NOTICE message
#define LOG_NOTICE(...)  		simpleLog_Write(SL_NOTICE, __FUNCTION__, __VA_ARGS__)
#ifdef SLDEBUG
	///outputs DEBUG message
	#define LOG_DEBUG(...)  		simpleLog_Write(SL_DEBUG, __FUNCTION__, __VA_ARGS__)
	///outputs Function name
	#define LOG_ENTRY()  			simpleLog_Write(SL_DEBUG, __FUNCTION__, "entry")
	///outputs Function name
	#define LOG_EXIT()  			simpleLog_Write(SL_DEBUG, __FUNCTION__, "exit\n")
	///outputs variable (char[]) value
	#define LOG_VAR(a)  			simpleLog_Write(SL_DEBUG, __FUNCTION__, "Variable %s = %s", #a, a)
	///outputs int value
	#define LOG_VAR_INT(a)			simpleLog_Write(SL_DEBUG, __FUNCTION__, "Variable %s = %d", #a, a)
	///outputs double value
	#define LOG_VAR_DBL(a)			simpleLog_Write(SL_DEBUG, __FUNCTION__, "Variable %s = %f", #a, a)
	///outputs string (char[]) value - same as LOG_VAR(a)
	#define LOG_VAR_STR(a)			simpleLog_Write(SL_DEBUG, __FUNCTION__, "Variable %s = %s", #a, a)
#else						// optimized away during production run if SLDEBUG not enabled
	///outputs DEBUG message
	#define LOG_DEBUG(...)
	///outputs Function name
	#define LOG_ENTRY()
	///outputs Function name
	#define LOG_EXIT()
	///outputs variable (char[]) value
	#define LOG_VAR(a)
	///outputs int value
	#define LOG_VAR_INT(a)
	///outputs double value
	#define LOG_VAR_DBL(a)
	///outputs string (char[]) value - same as LOG_VAR(a)
	#define LOG_VAR_STR(a)
#endif
/** @} **/ 



/**
@defgroup logDefines defines
 */
/** @{ **/
///Maximum message length in bytes. Feel free to change it to any value.*/
#define SL_MAX_MSG_LEN 4096

// Tags that are written with the message depending on the severity
#define SL_MSG_ERROR	"ERROR"	// ERR, alternatively
#define SL_MSG_WARNING	"WARNG"	// WRN
#define SL_MSG_NOTICE	"NTICE"	// NTE
#define SL_MSG_DEBUG	"DEBUG"	// DBG
/** @} **/

///<br> default settings - takes effect only when recompiling simpleLog lib

/**
@defgroup logDefaultValues default parameters
 */
/** @{ **/
/// path including filename - default - no output to file
#define DEFAULT_SL_PATHNAME 		NULL
/// timeformat - not used by default
#define DEFAULT_SL_TIMEFORMAT 		NULL
/// separator - sapace is used by default
#define DEFAULT_SL_SEPARATOR 		" "
/// output to console - enables all messages by default
#define DEFAULT_SL_FILTER_LEVEL_CONSOLE		SL_ALL
/// output to datalog - enables Notice, Warning and Error messages ignoring all DEBUG output
#define DEFAULT_SL_FILTER_LEVEL_DATALOG		(SL_NOTICE | SL_WARNING | SL_ERROR)
/// output to file - enables all messages by default
#define DEFAULT_SL_FILTER_LEVEL_FILE 		SL_ALL
/** @} **/ 

/**
@defgroup simpleLogFunctions functions
 */ 
/** @{ **/ 
/**
// @brief	Sets the name of the log file and other parameters.
//
// Should be called only once.  <br>
// The file is opened in append mode. @ref simpleLog_Flush must be called to close the file at the end of the program run. <br>
// Call to this function is optional same as (NULL, NULL, " ") <br>
// @param	fileName - Pathname of the file to write to. Pass NULL to disable output to file.
// @param	timeFormat - Optional strftime format string. Pass NULL to disable. Suggested "%Y%m%d-%H%M%S" or "%Y/%m/%d-%H:%M:%S"
// @param	separator - Character(s) used as separators between fields <br>
// 
// @return 0 if file was open successfully. If logging to file was not enabled function always returns 0.
//
// Examples: (NULL, NULL, " ") print all messages no time stamp <br>
//			 ("Msg.log", "%H:%M ", ", ") <br>
//			 ("Msg.log", "%Y%m%d-%H%M%S ", "\t") <br>
//			 (NULL, "%Y%m%d-%H%M%S ",  " ") <br>
**/
__declspec(dllexport) int simpleLog_Setup(const char *fileName, const char *timeFormat, const char *separator);

/**
// @brief	Sets the current filter level <br>
// @param	filterLevelConsole - Output level to CV console. Combination of the various @ref logLevels. Messages that don't match will be ignored <br>
// @param	filterLevelDatalog - Output level to Datalog stream. See @ref logLevels <br>
// @param	filterLevelFile - Output level to File. See @ref logLevels. In order to output to file @ref simpleLog_Setup should be called with proper fileName <br>
//
// Example and default levels:	simpleLog_FilterLevel(SL_ALL, SL_NOTICE | SL_WARNING | SL_ERROR, SL_ALL)
**/
__declspec(dllexport) void simpleLog_FilterLevel(const unsigned int filterLevelConsole, const unsigned int filterLevelDatalog, const unsigned int filterLevelFile);

/**
// @brief	Logs a message at a given level
// @param	level - Log level to use. If it doesn't match the filter, it's discarded
// @param	origin - Optional part of the message that doesn't need to be different. Origin Suggested value is __func__ to pass the origin function name
// @param	fmt  - Usual printf syntax and extra parameters
//
// @return 0 if message was written to file successfully. If logging to file was not enabled function always returns 0.
//
// Example:	(SL_DEBUG, __func__, "Current number %d", Nb)
**/
__declspec(dllexport) int simpleLog_Write(const int level, const char *origin, const char *fmt, ... );

/**
// @brief	Flushes log messages. Closes file (if open)
//
// Should be called at the very end of the running program if @ref simpleLog_Setup was called prior and logging to file was enabled.
// Also, simpleLog_Flush can be called if output to file is not needed anymore.
//
// @return 0 if file was closed successfully.
**/
__declspec(dllexport) int simpleLog_Flush(void);
/** @} **/ 


#endif

// /**
// @mainpage Main Page
// @section simpleLogSec Simple Logging library	
//
// @subsection levelLog Logging defines and functions
//
// @ref simpleLog - @copybrief simpleLog<br>
//<br>
//<br>
// Simple logging facility with the following features:<br>
//-   Compatible with Keithley's 4200S development environment (KULT, Eclipse or Visual Studio)<br>
//-   Uses simple and flexible DEFINE functions<br>
//-   Optional time tagging<br>
//-   Works in append mode:<br>
//&nbsp;-       files can be manipulated externally<br>
//&nbsp;-       reliable, if the program crashes, everything is already written (except possible repeated messages)<br>
//-   Use log level filters: Error/Warning/Notice/Debug<br>
//-   Can write to stderr, console or files<br>
//-   Message have the following form with [brackets] being optional: [TimeStamp] Severity [Origin] [repeat count] Message<br>
//<br>
//<br>
//	see simpleLogTst project for usage examples.
// @version 1.2014.05.01
//*//
