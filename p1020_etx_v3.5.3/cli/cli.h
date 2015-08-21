/*
 *                  		Copyright QiCe Tech, 2007-2010
 *
 *                          ALL RIGHTS RESERVED
 *
 * Description:      
 *
 */


#ifndef __CLI_H__
#define __CLI_H__

/*******************************************************************
 *
 * Description:     Constant definitions
 *
 *******************************************************************/

#define CLI_PROMPT                  		"CLI> "
#define DEFAULT_CONFIG_FILE         	"/usr/local/etc/save.conf"

extern void StartCLI(void);
extern void StartCLIExit(char *cmd_str);
extern void cliFlushInput(void);
extern void getDefaultTerm(void);
extern void setDefaultTerm(void);

extern struct parser_params pparams;

#endif




