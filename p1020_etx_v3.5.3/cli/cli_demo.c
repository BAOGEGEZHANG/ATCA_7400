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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>

#include "lexer.h"
#include "y.tab.h"
#include "flex.h"
#include "cli_demo.h"

#include "utils/errprint.h"
#include "utils/watch_list.h"
#include "database.h"
#include "parserops.h"


extern int scparse(void *pparams);

/* dummy declaration to silence warning caused by incorrectly
   generated declaration in flex.h */
static int yy_init_globals (yyscan_t yyscanner )
{
    (void) yyscanner;
    (void) yy_init_globals;
    return 0;
}

void cliFlushInput()
{
    struct termios term;
    tcgetattr(fileno(stdin), &term);
    tcsetattr(fileno(stdin), TCSAFLUSH, &term);
    fflush(stdin);
}

/*******************************************************************
 *
 * Description:     Start the CLI.
 *
 * Modified args:   NONE
 *
 * Return value:    N/A.
 *
 *******************************************************************/
int
start_cli_demo()
{
    int qnffd = -1;
    struct parser_params pparams;
    yyscan_t scanner;

#ifdef BUILD_MC10P_FLOW
    confs_t cfg;
#endif

    memset(&pparams, 0, sizeof(pparams));
    db_open(&pparams.db);
    
    sclex_init(&scanner);
    pparams.scanner = scanner;
    
    if ((pparams.wl = wl_init(32)) == NULL)
    {
        errprint("StartCLI: Can't allocate watch list.\n");
        return -1;
    }        
    scset_extra(&pparams, scanner);

    cli_db_lock(&pparams);
    if(!db_initialized(&pparams.db))
    {
#ifdef BUILD_MC10P_FLOW
	memset(&cfg, 0, sizeof(confs_t));
	qnf_framer_mc10p_init_1(&cfg);
	
	qnffd = qnf_open("/dev/qnf0");
	qnf_framer_mc10p_init_2(qnffd, 0, &cfg);
	close(qnffd);
	
	sleep(5);
	
	qnffd = qnf_open("/dev/qnf1");
	if (qnffd != -1)
	{
	    qnf_framer_mc10p_init_2(qnffd, 1, &cfg);
	    close(qnffd);
	}
#endif

#ifdef BUILD_MC20P_GTP
        framer_open("/dev/framer0");		// first init meta 0
        sleep(5);
        framer_open("/dev/framer1");		// second init meta 1
#endif

#ifdef BUILD_MC20P_MC4P25
	framer_open("/dev/framer0");		// first init meta 0
	sleep(5);
	qnffd = qnf_open("/dev/qnf1");
	if (qnffd != -1)
	{	
	    qnf_framer_init(qnffd);
	    close(qnffd);
	}
#endif

	// the origional initializations
    	db_init_config(&pparams.db);
	//cli_init_config(&pparams);
	
	/* Restore default configuration at startup */
        set_input_buff("exit\n", NULL, scanner);
    	set_input_buff("lif restore config\n", NULL, scanner);
	 
	db_set_initialized(&pparams.db);
        cliFlushInput();
    }
    cli_db_unlock(&pparams);	
    
    scparse(&pparams);
    sleep(2);
    
    wl_destroy(pparams.wl);    
    db_close(&pparams.db);
    sclex_destroy(scanner);

    return 0;
}

int
stop_cli_demo()
{
    return 0;
}


/*******************************************************************
 *
 * Description:     Empty input buffer.
 *
 * Modified args:   NONE
 *
 * Return value:    N/A.
 *
 *******************************************************************/



/*******************************************************************
 *
 * Description:     Temporary main function.
 *
 * Modified args:   NONE
 *
 * Return value:    N/A.
 *
 *******************************************************************/
int main(int argc, char *argv[])
{
    start_cli_demo();
    return 0;
}

