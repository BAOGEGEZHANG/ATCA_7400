/*
 *                  		Copyright QiCe Tech, 2007-2010
 *
 *                          ALL RIGHTS RESERVED
 *
 * Description:      
 *
 */
 
/*******************************************************************
 *
 * Description:     Includes
 *
 *******************************************************************/ 

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "utils/errprint.h"
#include "cs_client/cs_client.h"
#include "cs_server/cs_server.h"
#include "snmp_monitor.h"


/*******************************************************************
 *
 * Description:     Constant definitions
 *
 *******************************************************************/


/*******************************************************************
 *
 * Description:     Macro definitions
 *
 *******************************************************************/
#define DEBUG(lvl, msg...)  if (dbdebug >= (lvl)) { printf(msg); }
#define DO_DEBUG(lvl, blk)  if (dbdebug >= (lvl)) blk

/*******************************************************************
 *
 * Description:     Data type definitions
 *
 *******************************************************************/

/*******************************************************************
 *
 * Description:     Function prototypes
 *
 *******************************************************************/

/*******************************************************************
 *
 * Description:     Static (global) variables
 *
 *******************************************************************/
static int dbdebug = 0;
static int g_run = 1;

CONSRV cs;

/*******************************************************************
 *
 * Description:     Description....
 *
 * Modified args:   None
 *
 * Return value:    0 if sucessful negative if error.
 *
 *******************************************************************/
void
cleanup(int signo)
{
    cs_cl_close(&cs);		
    exit(0);
}


/*******************************************************************
 *
 * Description:     Description....
 *
 * Modified args:   None
 *
 * Return value:    0 if sucessful negative if error.
 *
 *******************************************************************/ 
int
start_snmp_monitor()
{
    res_result_t result;
    char cmd_str[32] = "snmp";
    		
    memset(&cs, 0, sizeof(CONSRV));
    strcat(cmd_str, "\n");
    		
    // first open the connection
    cs_cl_open(&cs);
    
    //signal(SIGINT, cleanup);
    //signal(SIGTERM, cleanup);
    //signal(SIGALRM, cleanup);
    
    
    // quite easy
    while (g_run)
    {
	// clear previous result
	memset(&result, 0, sizeof(res_result_t));
			
    	// issue command to cs_server
    	cs_cl_insert_str(&cs, cmd_str, &result);
    	
    	// sleep for a while
    	sleep(4);
    }
    //never stop!!!
    		
    return 0;
}

/*******************************************************************
 *
 * Description:     Description....
 *
 * Modified args:   None
 *
 * Return value:    0 if sucessful negative if error.
 *
 *******************************************************************/
int
stop_traffic_monitor()
{
    g_run = 0;

    return 0;
}




/*******************************************************************
 *
 * Description:     Main function used in Linux.
 *
 * Modified args:   None
 *
 * Return value:    0 if sucessful negative if error.
 *
 *******************************************************************/
int main(int argc, char *argv[])
{
    int val;
		
    if (argc == 2 && argv[1][0] == '-')
    {
        switch (argv[1][1])
        {
            case 'd':
            case 'D':
                val = atoi(&argv[1][2]);
                dbdebug = val;
                break;
            default:
                errprint("Invalid option\n");
                exit(1);
        }
    }
    
    start_snmp_monitor();

    return 0;
}

