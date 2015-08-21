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
#include <termios.h>
#include <signal.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "lexer.h"
#include "y.tab.h"
#include "flex.h"

#include "cli.h"
#include "utils/errprint.h"
#include "utils/watch_list.h"
#include "database.h"
#include "parserops.h"



extern int scparse(void *pparams);

//global values
struct parser_params pparams;
yyscan_t scanner;
struct termios interm, outterm;


/* dummy declaration to silence warning caused by incorrectly
   generated declaration in flex.h */
static int yy_init_globals (yyscan_t yyscanner )
{
    (void) yyscanner;
    (void) yy_init_globals;
    
    return 0;
}


int cleanup(int signo)
{
    wl_destroy(pparams.wl);
    db_close(&pparams.db);
    cs_cl_close(&pparams.cs);
    sclex_destroy(scanner);
    
    setDefaultTerm();    
    
    _exit(0);
}


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
    char cmd_str[2048];
    
    if (argc == 1)
        StartCLI();
    else
    {
        strcpy(cmd_str, argv[1]);
	      strcat(cmd_str, "\n");
	      StartCLIExit(cmd_str);
    }
    
    return 0;
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
void StartCLI()
{
    memset(&pparams, 0, sizeof(pparams));
    db_open(&pparams.db);
    cs_cl_open(&pparams.cs);  //added for cs
	  
    sclex_init(&scanner);
    pparams.scanner = scanner;
	  
    if ((pparams.wl = wl_init(32)) == NULL)
    {
        errprint("StartCLI: Can't allocate watch list.\n");
        return;
    }
    
    scset_extra(&pparams, scanner); 
    cli_db_lock(&pparams);
    if(!db_initialized(&pparams.db))
    {
        db_init_config(&pparams.db);
        cli_init_config(&pparams);
		    
	/* Restore default configuration at startup */
        set_input_buff("lif restore config\n", NULL, scanner);
        db_set_initialized(&pparams.db);
    }
    cli_db_unlock(&pparams);	
    
    //here try to capture signals
    signal(SIGINT, cleanup);
    signal(SIGTERM, cleanup);
    signal(SIGALRM, cleanup);	

#if defined(BUILD_QT10) || defined(BUILD_QT20)
    printf("\nWelcome To QT10/20 Command Line Interface\nCopyright (C) 2010-2012, QiCe Tech Ltd. \n");
#else
    printf("\nWelcome To ATCA3200 Command Line Interface\nCopyright (C) 2014-2016\n");
#endif
    cliFlushInput();
    getDefaultTerm();    
    
    while (scparse(&pparams))
    {
        printf("%s", CLI_PROMPT);
    }
    
    wl_destroy(pparams.wl);
    db_close(&pparams.db);      // close is not shutdown
    cs_cl_close(&pparams.cs);   // close is not shutdown
    sclex_destroy(scanner);
}


void StartCLIExit(char *cmd_str)
{
    memset(&pparams, 0, sizeof(pparams));
    db_open(&pparams.db);
    cs_cl_open(&pparams.cs);  //added for cs
	  
    sclex_init(&scanner);
    pparams.scanner = scanner;
	  
    if ((pparams.wl = wl_init(32)) == NULL)
    {
        errprint("StartCLI: Can't allocate watch list.\n");
        return;
    }
    
    scset_extra(&pparams, scanner);
    
    cli_db_lock(&pparams);
    if(!db_initialized(&pparams.db))
    {
        db_init_config(&pparams.db);
	cli_init_config(&pparams);
	      
	/* Restore default configuration at startup */
	set_input_buff("exit\n", NULL, scanner);
        set_input_buff(cmd_str, NULL, scanner);
    	set_input_buff("lif restore config\n", NULL, scanner);
	db_set_initialized(&pparams.db);
    }
    else
    {
        set_input_buff("exit\n", NULL, scanner);
        set_input_buff(cmd_str, NULL, scanner);
    }
    cli_db_unlock(&pparams);
    
    while (scparse(&pparams))
    {
        printf("%s", CLI_PROMPT);
    }
    
    wl_destroy(pparams.wl);
    db_close(&pparams.db);
    cs_cl_close(&pparams.cs);
    sclex_destroy(scanner);
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
void cliFlushInput()
{
    struct termios term;
    tcgetattr(fileno(stdin), &term);
    tcsetattr(fileno(stdin), TCSAFLUSH, &term);
    fflush(stdin);
}

void getDefaultTerm()
{
    tcgetattr(fileno(stdin), &interm);
    tcgetattr(fileno(stdout), &outterm);
}

void setDefaultTerm()
{
    tcsetattr(fileno(stdin), TCSANOW, &interm);
    tcsetattr(fileno(stdout), TCSANOW, &outterm);
}

