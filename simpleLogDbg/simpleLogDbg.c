/*
 * simpleLogDbg.c
 *
 *  Created on: Nov 13, 2014
 *      Author: kiuser
 */

#include "keithley.h"
#include "simpleLog.h"

void local_message_output( int flag, int msg_id, char * message )
{
	printf( "DTLOG %s\n", message );
	//printf(message);
}

void main()
{
	int status;

	logOutputRegister( &local_message_output );

	printf("\n"); // just an empy line

	status = tstsel( 1);
	if ( status != 0 )
	{
		printf(" tstsel error = %i, Exiting\n", status );
		PTexit( status );
	}

	/*
		Place user library module call below.
	*/

	simpleLogTst();

	/*
		Place user library module call above.
	*/

	printf("\n"); // just an empty line
	tstdsl( );

}

