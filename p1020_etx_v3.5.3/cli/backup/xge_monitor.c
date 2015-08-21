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
#include <sys/ipc.h>
#include <sys/shm.h>

#include "utils/errprint.h"
#include "cs_client/cs_client.h"
#include "cs_server/cs_server.h"
#include "xge_monitor.h"


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
#define SNMPGEPORTNO        24
#define SNMPXGEPORTNO	      4
#define DEFAULT_SHM_DEV     "/dev/shm/stats"

typedef struct cnt_ge_stat
{
    unsigned long    portLink;
    unsigned long    portPpsRx;
    unsigned long    portPpsTx;
    unsigned long    portBwdRx;
    unsigned long    portBwdTx;
} cnt_ge_stat_t;

typedef struct cnt_xge_stat
{
    unsigned long    portLink;
    unsigned long    portPpsRx;
    unsigned long    portPpsTx;
    unsigned long long portBwdRx;
    unsigned long long portBwdTx;
} cnt_xge_stat_t;

typedef struct cnt_stats
{
    cnt_ge_stat_t  gestat[SNMPGEPORTNO];
    cnt_xge_stat_t xgestat[SNMPXGEPORTNO];
} cnt_stats_t;

char *shmbuf = "\0";


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
start_xge_monitor(int val)
{
    res_result_t result;
    char cmd_str[4][128];
    cnt_stats_t *stats_map;
    unsigned int linkup;
    unsigned int timeout=150; //4*150=600s
    
    
    memset(&cs, 0, sizeof(CONSRV));
    strcpy(cmd_str[0], "port xe1 enable=false\n");
		strcpy(cmd_str[1], "port xe1 enable=true\n");    
    strcpy(cmd_str[2], "port xe3 enable=false\n");
		strcpy(cmd_str[3], "port xe3 enable=true\n");    
    
    
    // first open the connection
    cs_cl_open(&cs);
    
    // quite easy
    while ((g_run) && (timeout))
    {
    	stats_map = (cnt_stats_t *)shmbuf;
			linkup = stats_map->xgestat[1].portLink;
			if (linkup == 0)
			{	
				// clear previous result
				memset(&result, 0, sizeof(res_result_t));
    		// issue command to cs_server
    		cs_cl_insert_str(&cs, cmd_str[0], &result);
    		
    		sleep(1);
    		
    		// clear previous result
				memset(&result, 0, sizeof(res_result_t));
    		// issue command to cs_server
    		cs_cl_insert_str(&cs, cmd_str[1], &result);
    	}
    	
    	if (val == 4)
    	{	
    		linkup = stats_map->xgestat[3].portLink;
				if (linkup == 0)
				{	
					// clear previous result
					memset(&result, 0, sizeof(res_result_t));
    			// issue command to cs_server
    			cs_cl_insert_str(&cs, cmd_str[2], &result);
    			
    			sleep(1);
    			
    			// clear previous result
					memset(&result, 0, sizeof(res_result_t));
    			// issue command to cs_server
    			cs_cl_insert_str(&cs, cmd_str[3], &result);
    		}
    	}
    	
    	// sleep for a while
    	sleep(4);
    	timeout--;
    }
    
    // close gracefully
    cs_cl_close(&cs);
    
    
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
stop_xge_monitor()
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
    int shmid;
		char *devname = DEFAULT_SHM_DEV;
		key_t key;
		
		
    if (argc == 2)
    {
    	  val = atoi(argv[1]);
    }
    else
    {
    		val = 4;
    }	
    
    key = ftok(devname, 0);
		if (key == -1)
		{
			printf("[snmp_init]: ftok failed!\n");
			return -1;
		}
		
		if ((shmid = shmget(key, sizeof(cnt_stats_t), IPC_CREAT)) < 0)
		{
			printf("[snmp_init]: shmget failed!\n");
			return -1;
		}
		
		if ((shmbuf = shmat(shmid, 0, 0)) < (char *)0)
		{
			printf("[snmp_init]: shmat failed!\n");
			return -1;
		}
		
    
    sleep(5);	// wait for snmpd is running
    
    start_xge_monitor(val);
		
    return 0;
}

