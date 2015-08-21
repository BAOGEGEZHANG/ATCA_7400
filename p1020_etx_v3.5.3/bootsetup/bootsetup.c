/*
 *                  		Copyright QiCe Tech, 2007-2010
 *
 *                          ALL RIGHTS RESERVED
 *
 * Description:      
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef _LINUX
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#endif

#include "bootsetup.h"
#include "db_client/db_client.h"
#include "db_server/db_server.h"

/*******************************************************************
 *
 * Description:     Start the system
 *
 * Modified args:   None.
 *
 * Return value:    Returns 0 on success, or an error code on failure.
 *
 *******************************************************************/
static int 
bootsetup_start( void )
{   
    /* start database server */
    if (start_db_srv() < 0)
    {
        errprint("Failed to start db server.\n");
        return -1;
    }

#if defined (BUILD_QT20) || defined (BUILD_QT10)    
    /* start the snmp monitor process */
    if (start_bypass_monitor() < 0)
    {
        errprint("Failed to start snmp monitor process.\n");
        return -1;
    }
#endif
    
    return 0;
}

/*******************************************************************
 *
 * Description:     Stop the system
 *
 * Modified args:   None.
 *
 * Return value:    Returns 0 on success, or an error code on failure.
 *
 *******************************************************************/
static int 
bootsetup_stop( )
{
    DATABASE db;
	  
    if (db_cl_open(&db) >= 0)
    {
        if (db_cl_shutdown(&db) < 0)
        {
            errprint("bootsetup: db_shutdown returned error.\n");
            return -1;
        }
    }
    
    /* need to cleanup bcm_srv ? */
    
    
    /* need to cleanup snmp monitor ? */
    
    
    return 0;
}

/*******************************************************************
 *
 * Description:     Bootsetup main function.
 *
 * Modified args:   None.
 *
 * Return value:    Returns 0 on success, or an error code on failure.
 *
 *******************************************************************/
int 
bootsetup(char *operation)
{
    if (strcmp(operation, "start") == 0)
    {
        return bootsetup_start();
    }

    if (strcmp(operation, "stop") == 0)
    {
        return bootsetup_stop();
    }

    return -1;
}

/*******************************************************************
 *
 * Description:     Fork of a child
 *
 * Modified args:   None.
 *
 * Return value:    Returns 0 on success, or an error code on failure.
 *
 *******************************************************************/
static int
bootsetup_fork(char *binary, char *arg0)
{
    pid_t pid;
        
    /* perform x*nix voodoo to daemonize db_server properly
     * see ch 12 Stevens, Unix network programming */
    {
        int fd;
        
        if ((pid = fork()) == 0)
        {
            
            setsid();
            signal(SIGHUP, SIG_IGN);
            
            if ((pid = fork()) != 0)
            {
                exit(0);
            }
            
            chdir("/");
            umask(0);

            /* Close all inherited file descriptors */
            for (fd = getdtablesize(); fd >= 0; --fd) 
            {
                (void) close(fd);
            }
            
            /* Use console as stdout and stderr */
            fd = open("/dev/console", O_RDWR);
            dup2(1, fd);
            dup2(2, fd);
            
            if (execl(binary, arg0, NULL) != 0)
            {
                errprint("Failed to start database server.\n");
                return -1;
            }
        }
    }
    sleep(1);

    return 0;
}

/*******************************************************************
 *
 * Description:     Linux-specific implementation of start_db_srv()
 *
 * Modified args:   None.
 *
 * Return value:    Returns 0 on success, or an error code on failure.
 *
 *******************************************************************/
int
start_db_srv(void)
{
    return bootsetup_fork("/usr/local/sbin/db_server", "db_server");
}

/*******************************************************************
 *
 * Description:     Linux-specific implementation of stop_db_srv()
 *
 * Modified args:   None.
 *
 * Return value:    0
 *
 *******************************************************************/
int
stop_db_srv(void)
{
    return 0;
}

#if defined (BUILD_QT20) || defined (BUILD_QT10)
/*******************************************************************
 *
 * Description:     Linux-specific implementation of start_db_srv()
 *
 * Modified args:   None.
 *
 * Return value:    Returns 0 on success, or an error code on failure.
 *
 *******************************************************************/
int
start_bypass_monitor(void)
{
    return bootsetup_fork("/usr/local/sbin/bypass_monitor", "bypass_monitor");
}

/*******************************************************************
 *
 * Description:     Linux-specific implementation of stop_db_srv()
 *
 * Modified args:   None.
 *
 * Return value:    0
 *
 *******************************************************************/
int
stop_bypass_monitor(void)
{
    return 0;
}
#endif

/*******************************************************************
 *
 * Description:     Linux-specific main function
 *
 * Modified args:   None.
 *
 * Return value:    Returns 0 on success, or an error code on failure.
 *
 *******************************************************************/
int
main(int argc, char *argv[])
{
    if (argc > 1)
    {
        if (strcmp(argv[1], "start") == 0)
        {
            if (bootsetup("start") != 0)
            {
                return 1;
            }
        }
        else if (strcmp(argv[1], "stop") == 0)
        {
            if (bootsetup("stop") != 0)
            {
                return 1;
            }
        }
    }
    else
    {
        if (bootsetup("start") != 0)
        {
            return 1;
        }
    }
    return 0;
}

