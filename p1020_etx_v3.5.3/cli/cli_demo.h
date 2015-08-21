/*
 *                  		Copyright QiCe Tech, 2007-2010
 *
 *                          ALL RIGHTS RESERVED
 *
 * Description:      
 *
 */


#ifndef __CLI_DEMO_H__
#define __CLI_DEMO_H__

/*******************************************************************
 *
 * Description:     Constant definitions
 *
 *******************************************************************/

//#define CLI_PROMPT                  "CLI> "
#define DEFAULT_CONFIG_FILE         "/usr/local/etc/save.conf"

//extern void StartCLI(void);
//extern void cliFlushInput(void);

extern int start_cli_demo();
extern int stop_cli_demo();

//extern struct parser_params pparams;


#endif
