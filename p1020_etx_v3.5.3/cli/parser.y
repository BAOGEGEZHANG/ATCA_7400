/*
 *                      Copyright QiCe Tech, 2010-2012
 *
 *                          ALL RIGHTS RESERVED
 *
 * Description:      
 *
 */

%{

#define YYERROR_VERBOSE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

#include "cli.h"
#include "utils/errprint.h"
#include "lexer.h"
#include "database.h"
#include "parserops.h"
#include "utils/watch_list.h"


#define OPT_DEFAULT         0x0002


/*******************************************************************
 *
 * Description:     Macro definitions
 *
 *******************************************************************/
#define YYPARSE_PARAM pparm
#define PARSER_PARAMS           ((struct parser_params *)YYPARSE_PARAM)
#define YYLEX_PARAM             PARSER_PARAMS->scanner
#define PREFIX_LEN_2_MASK(len)  ((0xFFFFFFFFUL << (32 - len)) & 0xFFFFFFFFUL)

#ifndef MIN
#define MIN(a,b)                (((a) <= (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b)                (((a) >= (b)) ? (a) : (b))
#endif

struct cmdbuffers {
  	char cmd[512];
  	struct cmdbuffers *next;
};

void yyerror(char *s);
static char *StrToUpper(char *str);
static int portrange(int a, int b);
%}

%pure_parser

%union {
    int ival;   /* For returning values */
    unsigned int uval;
    struct vlanmap *vptr;
    void **wlptr;   /* watch list entry */
}

/************************************************************************
 ***** Normal tokens used by the parser (Grouped alphabetically, please!)
 ************************************************************************/
%token T_ALL T_ANY T_BOTH T_BYP T_BYPASS
%token T_CLASSIFY T_CLEAR T_CONFIG T_COUNTERS T_CRC_CCITT T_CRC T_CRC_32 T_CRC_NONE
%token T_DISABLE T_DROP T_DEST T_DEFAULT
%token T_ENABLE T_EXIT T_ENCAP T_EGRESS T_ETHERNET T_ETXCONFIG
%token T_FPGA T_FORWARD T_FLOW T_FILE T_FIRMWARE
%token T_GLOBAL T_GET T_GTP
%token T_HDLC T_HASH
%token T_IP T_INFO T_INNER
%token T_KEY
%token T_LIF T_LINE T_L4_PORT T_LENGTH
%token T_MODE T_MAC T_MMS
%token T_NONE T_NORMAL T_NETMASK 
%token T_OFFSET T_OC T_OC192 T_OC48 T_OUTER
%token T_PACKET T_PROTOCOL T_PPP T_PAYLOAD T_PORT T_PORTS T_QUIT
%token T_RANGE T_RATE T_RCLOAD T_READ T_REGISTER T_RESTORE
%token T_RULE T_RULESET1 T_RULESET2
%token T_SAMPLE T_SET T_STATS T_SHOW T_SYSTEM T_SOURCE T_SAVE T_SCRAMBLE 
%token T_SETTINGS T_SONNET T_SDH
%token T_TO T_TRAFFIC T_TRANSFER 
%token T_TCP
%token T_UPDATE T_UDP
%token T_VLAN
%token T_WRITE

/*******************************************************************
 ***** Special tokens
 *******************************************************************/
%token T_INVALID_CHAR T_EOS
%token <ival> T_INT
%token <wlptr> T_WORD_WL T_STRING_WL T_MACADDR_WL
%token <uval> T_IPADDR

%type <ival> bypasscmds byp_or_normal
%type <ival> command crc_mode
%type <ival> default
%type <ival> enable_or_disable encap_mode
%type <ival> fpga_reg fpga_reg_data filtercmds forwardcmds fpgacmds
%type <ival> ipprefixormask gtpdestports gtpcmds
%type <ival> line_mode line_rate mms_size
%type <ival> occmds oc_no port ports portlist pktlen_min pktlen_max range_no hash_source
%type <ival> ruleno1 ruleno2 ruleoffset ruleprotocol rulel4port
%type <ival> switchcmds vlanid sample_rate
%type <ival> transfercmds

%type <wlptr> configfile_wl setrule1option_wl setrule1options_wl setrule2option_wl setrule2options_wl
%type <wlptr> bcmcmd_wl bcmcmds_wl wordcomma_wl worddot_wl

%%


input:  /* NIL */
    | input line
    {
        cliFlushInput();
    };

line: T_EOS
    {
        wl_cleanup(PARSER_PARAMS->wl);
        if (!PARSER_PARAMS->loading_file)
        {
            //printf(CLI_PROMPT);
	    //printf("### ");
            fflush(stdout);
        }
    }
    | command
    {
        wl_cleanup(PARSER_PARAMS->wl);
        if (!PARSER_PARAMS->loading_file)
        {
            //printf(CLI_PROMPT);
	    //printf("### ");
            fflush(stdout);
        }
    }
    | error T_EOS
    {
        wl_cleanup(PARSER_PARAMS->wl);
        yyerrok;
        if (!PARSER_PARAMS->loading_file)
        {
            //printf(CLI_PROMPT);
	    //printf("### ");
            fflush(stdout);
        }
    };


command:
      bypasscmds
    | filtercmds
    | forwardcmds
    | transfercmds
    | gtpcmds
    | occmds
    | fpgacmds
    | switchcmds
    | T_LIF T_RESTORE T_CONFIG default configfile_wl T_EOS
    {
        char filename[256];
        
        FILE *fp;
        struct cmdbuffers *pheader, *pnew;
        
        if((pheader = malloc(sizeof(struct cmdbuffers))) == NULL)
        {
            errprint("Out of memory in malloc cmdbuffers.\n");
            return -1;
        }
        
        memset(pheader, 0, sizeof(struct cmdbuffers));
        pheader->next = NULL;
        
        if($4 & OPT_DEFAULT)
            cli_reset_default_config(PARSER_PARAMS);
      	else
      	{
        	cli_reset_default_config(PARSER_PARAMS);
        	
        	if ($5 == NULL)
          		strcpy(filename, DEFAULT_CONFIG_FILE);
        	else
        	{
          		strcpy(filename, WL_PTR($5));
          		wl_free_entry($5);
        	}
			
        	if((fp = fopen(filename,"r")) == NULL)
        	{
          		errprint("Can't open %s to read.\n", filename);
          		break;
        	}
        	else
        	{
          		while(!feof(fp))
          		{
            		if((pnew = malloc(sizeof(struct cmdbuffers))) == NULL)
            		{ 
              			errprint("Out of memory in malloc cmdbuffers.\n");
              			return -1;
            		}
			
            		memset(pnew, 0, sizeof(struct cmdbuffers));
            		if (pheader->next == NULL)
            		{
              			pnew->next = NULL;
              			pheader->next = pnew;
            		}
            		else
            		{
              			pnew->next = pheader->next;
              			pheader->next = pnew;
            		} 
          			
            		fgets(pnew->cmd, 512, fp);
          		}//while
				
          		fclose(fp);
				
          		pnew = pheader->next;
							while(pnew != NULL)
          		{
            		pheader = pnew;
            		set_input_buff(pnew->cmd, NULL, PARSER_PARAMS->scanner);
            		pnew = pnew->next;
            		free(pheader);
          		}//while
        	}//else
      	}
		}
  	| T_LIF T_SAVE T_CONFIG configfile_wl T_EOS
  	{
    	if($4 == NULL)
    	{
      		if(cli_save_config_file(PARSER_PARAMS, DEFAULT_CONFIG_FILE) < 0)
      		{
        		errprint("Error saving configurations.\n");
        		break;
      		}
    	}
    	else
    	{
      		if(cli_save_config_file(PARSER_PARAMS, WL_PTR($4)) < 0)
      		{
        		errprint("Error saving configurations to file %s.\n", WL_PTR($4));
        		wl_free_entry($4);
        		break;
      		}
      		
      		wl_free_entry($4);
    	}
  	}
  	| T_LIF T_SHOW T_CONFIG T_EOS
  	{
    	if(cli_show_config(PARSER_PARAMS) < 0)
    	{
      		errprint("Could not show configurations.\n");
      		break;
    	}
  	} 
    | T_LIF T_SHOW T_CONFIG T_FILE T_EOS
    {
        if(cli_show_config_file() < 0)
        {
      		errprint("Could not show configurations.\n");
      		break;
    	}
    }
    | T_LIF T_SHOW T_SYSTEM T_INFO T_EOS
    {
        if (cli_show_system() < 0)
      	{
        	errprint("Could not show system information.\n");
        	break;
      	}
    }
    | T_LIF T_UPDATE T_FIRMWARE T_OC oc_no T_EOS
    {
      	if (cli_update_firmware($5) < 0)
      	{
        	errprint("Could not update firmware.\n");
        	break;
      	}
    } 
    | T_ETXCONFIG T_EOS
    {
        if (cli_etxconfig() < 0)
        {
            errprint("Could not edit config file.\n");
            break;
        }
    }
    | T_EXIT T_EOS
    {
	YYACCEPT;
    }
    | T_QUIT T_EOS
    {
	YYACCEPT;
    };

    
default:
    {
        $$ = 0;
    }
    | T_DEFAULT
    {
        $$ = OPT_DEFAULT;
    };

configfile_wl: 
    {
        $$ = NULL;
    }
    | T_STRING_WL
    {
        $$ = $1;
    };

enable_or_disable: T_ENABLE
    {
      	$$ = ENABLED;
    }
    |   T_DISABLE
    {
        $$ = DISABLED;
    };


/*******************************************************************
 ***** bypass Commands
 *******************************************************************/
 bypasscmds:
    T_BYPASS T_SHOW T_EOS
    {
        cli_bypass_get(PARSER_PARAMS);
    }
    | T_BYPASS T_SET T_LINE portlist T_MODE byp_or_normal T_EOS
    {
        cli_bypass_set(PARSER_PARAMS, $4, $6);
    };

port:   T_INT        
    {		   	
        if ( $1 < 0 || $1 > MAX_BYPASS_PORTS-1 )			
        {				
            errprint("Valid port range is: 0 - %d\n", MAX_BYPASS_PORTS-1);				
	    YYERROR;			
	}			
	$$ = $1;		
    };

ports:  port	    
	  {		    
	      $$ = 1 << $1;	    
	  }
    |   T_INT '-' T_INT	    
	  {		    
	      if( $1 < 0 || $1 > MAX_BYPASS_PORTS-1)		    
		    {			    
		        errprint("Invalid port %d.\n", $1);			    
		        YYERROR;		    
		    }             
	      if($3 < 0 || $3 > MAX_BYPASS_PORTS-1)		    
	      {			    
			errprint("Invalid port %d.\n", $1);			    
		        YYERROR;		    
	      }		    
	      $$ = portrange($1, $3);	    
	  };

portlist: ports	|   ports ',' portlist	    
	  {		    
	      $$ = $1 | $3;	    
	  };

byp_or_normal: T_NORMAL
    {
      	$$ = ENABLED;
    }
    |   T_BYP
    {
        $$ = DISABLED;
    };

/*******************************************************************
 ***** filter Commands
 *******************************************************************/
 filtercmds:
    T_LIF T_SET T_CLASSIFY T_OC oc_no T_RULESET1 ruleno1 setrule1options_wl T_EOS
    {
      	rule1options_t opts;
      	
      	memset(&opts, 0, sizeof(rule1options_t));
      	opts.flag = WL_PTR_OF($8, rule1options_t)->flag;
	
      	if(opts.flag & RULE_OPT_PROTOCOL)
        	opts.protocol = WL_PTR_OF($8, rule1options_t)->protocol;
      	
      	if(opts.flag & RULE_OPT_SRCIP)
      	{
        	opts.sourceip = WL_PTR_OF($8, rule1options_t)->sourceip;
        	opts.sourceip_mask = WL_PTR_OF($8, rule1options_t)->sourceip_mask;
      	}
      	
      	if(opts.flag & RULE_OPT_DESTIP)
      	{
        	opts.destip = WL_PTR_OF($8, rule1options_t)->destip;
        	opts.destip_mask = WL_PTR_OF($8, rule1options_t)->destip_mask;
      	}
      	
      	if(opts.flag & RULE_OPT_SRCL4PORT)
        	opts.sourcel4port = WL_PTR_OF($8, rule1options_t)->sourcel4port;
      	
      	if(opts.flag & RULE_OPT_DESTL4PORT)
        	opts.destl4port = WL_PTR_OF($8, rule1options_t)->destl4port;
      	
      	if(opts.flag&RULE_OPT_PACKETLEN)
        	opts.pktlen_range = WL_PTR_OF($8, rule1options_t)->pktlen_range;
      	
      	opts.id = $7;
      	
      	if (opts.id == MAX_RULE_NO)
      	{    
        	errprint("Specify the correct rule index please (0 - %d)\n", MAX_RULE_NO-1);
        	break;
      	}
      	else
        	cli_fpga_set_ruleset1(PARSER_PARAMS, $5, &opts);
    }
  	| T_LIF T_SET T_CLASSIFY T_OC oc_no T_RULESET2 ruleno2 setrule2options_wl T_EOS
    {
      	int i;
      	rule2options_t opts;
      	
      	memset(&opts, 0, sizeof(rule2options_t));
      	opts.flag = WL_PTR_OF($8, rule2options_t)->flag;
      	
      	if(opts.flag & RULE_OPT_OFFSET)
        	opts.offset = WL_PTR_OF($8, rule2options_t)->offset;
      	
      	if(opts.flag & RULE_OPT_KEY)
      	{
        	for(i=0; i< MAX_KEY_NO; i++)
        	{
          		opts.key[i] = WL_PTR_OF($8, rule2options_t)->key[i];
          		opts.mask[i] = WL_PTR_OF($8, rule2options_t)->mask[i];
        	}
      	}
        
      	opts.id = $7;
      	if (opts.id == MAX_RULE_NO)
      	{    
        	errprint("Specify the correct rule index please (0 - %d)\n", MAX_RULE_NO-1);
        	break;
      	}
      	else
        	cli_fpga_set_ruleset2(PARSER_PARAMS, $5, &opts);
    }
  	| T_LIF T_CLEAR T_CLASSIFY T_OC oc_no T_RULESET1 ruleno1 T_EOS
    {
      	int i;
        
      	if ($7 == MAX_RULE_NO)
      	{
        	for(i=0; i < MAX_RULE_NO; i++)
          		if(cli_fpga_clear_ruleset1(PARSER_PARAMS, $5, i) < 0)
          		{   
            		errprint("Error clear filter rules.\n");
            		break;
          		}
      	}
      	else
      	{    
        	if(cli_fpga_clear_ruleset1(PARSER_PARAMS, $5, $7) < 0)
        	{   
          		errprint("Error clear filter rules.\n");
          		break;
        	}
      	}
    }
  	| T_LIF T_CLEAR T_CLASSIFY T_OC oc_no T_RULESET2 ruleno2 T_EOS
    {
      	int i;
        
      	if ($7 == MAX_RULE_NO)
      	{
        	for(i=0; i < MAX_RULE_NO; i++)
          		if(cli_fpga_clear_ruleset2(PARSER_PARAMS, $5, i) < 0)
          		{   
            		errprint("Error clear filter rules.\n");
            		break;
          		}
      	}
      	else
      	{    
        	if(cli_fpga_clear_ruleset2(PARSER_PARAMS, $5, $7) < 0)
        	{   
          		errprint("Error clear filter rules.\n");
          		break;
        	}
      	}
    }
  	| T_LIF T_SHOW T_CLASSIFY T_OC oc_no T_RULESET1 ruleno1 T_EOS
    {
      	int i;
            
      	if ($5 == MAX_RULE_NO)
      	{
        	for(i=0; i < MAX_RULE_NO; i++)
        		if(cli_get_filter_ruleset1(PARSER_PARAMS, $5, i) < 0)
        		{   
          			errprint("Error clear filter rules.\n");
          			break;
       	 		}
      	}
      	else
      	{    
        	if(cli_get_filter_ruleset1(PARSER_PARAMS, $5, $7) < 0)
        	{   
          		errprint("Error clear filter rules.\n");
          		break;
        	}    
      	}
    }
  	| T_LIF T_SHOW T_CLASSIFY T_OC oc_no T_RULESET2 ruleno2 T_EOS
    {
      	int i;
            
      	if ($5 == MAX_RULE_NO)
      	{
        	for(i=0; i < MAX_RULE_NO; i++)
        		if(cli_get_filter_ruleset2(PARSER_PARAMS, $5, i) < 0)
        		{   
          			errprint("Error clear filter rules.\n");
          			break;
        		}
      	}
      	else
      	{    
          	if(cli_get_filter_ruleset2(PARSER_PARAMS, $5, $7) < 0)
          	{   
            	errprint("Error clear filter rules.\n");
            	break;
          	}    
      	}
    }
  	| T_LIF T_SET T_OC oc_no T_PACKET T_LENGTH T_RANGE range_no pktlen_min '-' pktlen_max T_EOS
    {
      	if ($9 > $11)
      	{
        	errprint("Wrong order of packet length range.\n");
        	break;
      	}
      
      	if (cli_set_pktlen_range(PARSER_PARAMS, $4, $8, $9, $11) < 0)
      	{
        	errprint("Error set packet length range.\n");
        	break;
      	}
    }
  	| T_LIF T_GET T_OC oc_no T_PACKET T_LENGTH T_RANGE range_no T_EOS
    {
      	if (cli_get_pktlen_range(PARSER_PARAMS, $4, $8) < 0)
      	{
        	errprint("Error get packet length range.\n");
        	break;
      	}
    };


oc_no: T_INT
    {
        if ($1 < 0 || $1 > MAX_OC_NUM)
        {
          	errprint("Invalid oc number, must between 0 - %d.\n", MAX_OC_NUM);
          	YYERROR;
        }
        $$ = $1;
    };

range_no: T_INT
    {
      	if ($1 < 0 || $1 > (MAX_PKT_RANGE-1))
      	{
        	errprint("valid range number is 0-%d.\n", (MAX_PKT_RANGE-1));
        	YYERROR;
      	}
      	$$ = $1;
    };
    
pktlen_min: T_INT
    {
      	if ($1 < 0 || $1 > 1518)
      	{
        	errprint("valid range number is 0-1518.\n");
        	YYERROR;
      	}
      	$$ = $1;
    };

pktlen_max: T_INT
    {
      	if ($1 < 0 || $1 > 1518)
      	{
        	errprint("valid range number is 0-1518.\n");
        	YYERROR;
      	}
      	$$ = $1;
    };
    
ruleno1: T_INT
    {
      	if( $1 < 0 || $1 > MAX_RULE_NO - 1)
      	{
       	errprint("valid number is: 0-%d\n", MAX_RULE_NO - 1);
        	YYERROR;      
      	}
      	$$ = $1;
    }
    | T_ALL
    {
        $$ = MAX_RULE_NO ;
    };

ruleno2: T_INT
    {
      	if( $1 < 0 || $1 > MAX_RULE_NO - 1)
      	{
        	errprint("valid number is: 0-%d\n", MAX_RULE_NO - 1);
        	YYERROR;      
      	}
      	$$ = $1;
    }
    | T_ALL
    {
        $$ = MAX_RULE_NO ;
    };

ruleprotocol: T_TCP
    {
      	$$ = 0x06;
    }
  	| T_UDP
    {
      	$$ = 0x11;
    }
  	| T_INT
    {
      	if ( $1 <0 || $1 > 255 )
      	{
        	errprint("Valid protocol is: 0 - 255\n");
        	YYERROR;
      	}
      	$$ = $1;
    };

ipprefixormask:   '/' T_INT
    {
      	if ($2 < 0 || $2 > 32)
      	{
          	errprint("Invalid prefix length %d\n", $2);
            YYERROR;                          
        }
        $$ = PREFIX_LEN_2_MASK($2);
    }
    | T_NETMASK T_IPADDR  
    {   
        $$ = $2;
    };

rulel4port: T_INT
    {
      	if ( $1 <0 || $1 > 65535 )
      	{
        	errprint("Valid protocol is: 0x0000 - 0xFFFF\n");
        	YYERROR;
      	}
      	$$ = $1;
    };

setrule1option_wl: T_ANY
    {
      	rule1options_t *opts;   
        if((opts = malloc(sizeof(rule1options_t))) == NULL)
        {     
            errprint("Out of memory in setruleoption_wl.\n");     
            YYERROR;    
        } 
      	
        memset(opts, 0, sizeof(rule1options_t));
        
        if(($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in setruleoption_wl.\n");
            free(opts);
          	YYERROR;
        } 
    }
  	| T_PROTOCOL ruleprotocol 
    {   
        rule1options_t *opts;   
        if((opts = malloc(sizeof(rule1options_t))) == NULL)
        {     
            errprint("Out of memory in setruleoption_wl.\n");     
            YYERROR;    
        }
        
        opts->protocol = $2;    
        opts->flag = RULE_OPT_PROTOCOL;   
            
        if(($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in setruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        } 
    }
  	| T_SOURCE T_IP T_IPADDR ipprefixormask
    {
        rule1options_t *opts;
		
        if((opts = malloc(sizeof(rule1options_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
        
        opts->sourceip = $3;
        opts->sourceip_mask = $4;
        opts->flag = RULE_OPT_SRCIP;
		
        if(($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in setruleoption_wl.\n");
          	free(opts); 
            YYERROR;  
        }
    }
  	| T_DEST T_IP T_IPADDR ipprefixormask
    {
        rule1options_t *opts;
            
        if((opts = malloc(sizeof(rule1options_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
        
        opts->destip = $3;
        opts->destip_mask = $4;
        opts->flag = RULE_OPT_DESTIP;
		
        if(($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in setruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
    }
  	| T_SOURCE T_L4_PORT rulel4port
    {
        rule1options_t *opts;
            
        if((opts = malloc(sizeof(rule1options_t))) == NULL)
        {     
            errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;  
        } 
		
        opts->sourcel4port = $3;
        opts->flag = RULE_OPT_SRCL4PORT;
		
        if(($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {     
            errprint("Watch list full in setruleoption_wl.\n");
          	free(opts);   
            YYERROR;  
        }
    }
    | T_DEST T_L4_PORT rulel4port 
    {   
        rule1options_t *opts; 
  
        if((opts = malloc(sizeof(rule1options_t))) == NULL)   
        {     
            errprint("Out of memory in setruleoption_wl.\n");     
            YYERROR;    
        }
        opts->destl4port = $3;    
        opts->flag = RULE_OPT_DESTL4PORT;   

        if(($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)    
        {     
            errprint("Watch list full in setruleoption_wl.\n");     
            free(opts);     
            YYERROR;    
        }       
    }
    | T_PACKET T_LENGTH T_RANGE range_no
    {
    	rule1options_t *opts;
    	
        if((opts = malloc(sizeof(rule1options_t))) == NULL)
        {
            errprint("Out of memory in setruleoption_wl.\n");
            YYERROR;
        }
        
        opts->pktlen_range = $4;
        opts->flag = RULE_OPT_PACKETLEN;
        
        if(($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
            errprint("Watch list full in setruleoption_wl.\n");
            free(opts);
            YYERROR; 
        }
    };

setrule1options_wl: setrule1option_wl
    {
        $$ = $1;  
    }
   	| setrule1option_wl setrule1options_wl
    {
        if (WL_PTR_OF($1, rule1options_t)->flag & RULE_OPT_PROTOCOL)
        {
          	WL_PTR_OF($2, rule1options_t)->protocol = WL_PTR_OF($1, rule1options_t)->protocol;
        }
		
        if (WL_PTR_OF($1, rule1options_t)->flag & RULE_OPT_SRCIP)
        {
          	WL_PTR_OF($2, rule1options_t)->sourceip = WL_PTR_OF($1, rule1options_t)->sourceip;
          	WL_PTR_OF($2, rule1options_t)->sourceip_mask = WL_PTR_OF($1, rule1options_t)->sourceip_mask;
        }
		
        if (WL_PTR_OF($1, rule1options_t)->flag & RULE_OPT_DESTIP)
        {
          	WL_PTR_OF($2, rule1options_t)->destip = WL_PTR_OF($1, rule1options_t)->destip;
          	WL_PTR_OF($2, rule1options_t)->destip_mask = WL_PTR_OF($1, rule1options_t)->destip_mask;
        }
		
        if (WL_PTR_OF($1, rule1options_t)->flag & RULE_OPT_SRCL4PORT)
        {
          	WL_PTR_OF($2, rule1options_t)->sourcel4port = WL_PTR_OF($1, rule1options_t)->sourcel4port;
        }
		
        if (WL_PTR_OF($1, rule1options_t)->flag & RULE_OPT_DESTL4PORT)
        {
          	WL_PTR_OF($2, rule1options_t)->destl4port = WL_PTR_OF($1, rule1options_t)->destl4port;
        } 
		
        if (WL_PTR_OF($1, rule1options_t)->flag & RULE_OPT_PACKETLEN)
        {
          	WL_PTR_OF($2, rule1options_t)->pktlen_range = WL_PTR_OF($1, rule1options_t)->pktlen_range;
        }
        
        WL_PTR_OF($2, rule1options_t)->flag |= WL_PTR_OF($1, rule1options_t)->flag;
        wl_free_entry($1);
        
        $$ = $2;
    };

ruleoffset: T_INT
    {
        if ($1 < 0 || $1 >19)
        {
          	errprint("Invalid offset, must between 0 - 19.\n");
          	YYERROR;
        }
        $$ = $1;
    };

setrule2option_wl:  T_ANY
    {
        rule2options_t *opts;
		
        if ((opts = malloc(sizeof(rule2options_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        memset(opts, 0, sizeof(rule2options_t));
      	
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
    }
  	| T_OFFSET ruleoffset
    {
        rule2options_t *opts;
		
        if ((opts = malloc(sizeof(rule2options_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        opts->offset = $2;
        opts->flag = RULE_OPT_OFFSET;
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
    }
  	| T_KEY T_STRING_WL
    {
        int i, j, len;
        char key_word_fs, key_word_ls, key[256];
        rule2options_t *opts;
        
        if((opts = malloc(sizeof(rule2options_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
        memset(opts->key, 0, MAX_KEY_NO);
        memset(opts->mask, 0xff, MAX_KEY_NO);
        
        /* do the process here */
        strcpy(key, WL_PTR($2));
        StrToUpper(key);
        //errprint("key is: %s\n", key);
        
        len = strlen(key);
        //errprint("key length is: %d\n", len);
		
        if ((len > MAX_KEY_LEN) || (len == 0) || (len % 2 != 0))
        {
          	errprint("Wrong key word length, must between 1 - 20 Bytes.\n");
          	YYERROR;
        }
        
        for (i=0; i < len /2; i++)
          	opts->mask[i] = 0x0;
    	
        for (i=0,j=0; i<len-1; )
        {
          	if ((key[i] >= 0x30) && (key[i] <= 0x39))
            	key_word_fs = key[i] - 0x30;
          	else if ((key[i] >= 0x41) && (key[i] <= 0x46))
            	key_word_fs = key[i] - 0x37;
          	else
          	{
            	errprint("Isn't a valid heximal digital!\n");
            	YYERROR;
          	}
			
          	if ((key[i+1] >= 0x30) && (key[i+1] <= 0x39))
            	key_word_ls = key[i+1] - 0x30;
          	else if ((key[i+1] >= 0x41) && (key[i+1] <= 0x46))
            	key_word_ls = key[i+1] - 0x37;
          	else
          	{
            	errprint("Isn't a valid heximal digital!\n");
            	YYERROR;
          	}
      		
          	opts->key[j] = ((key_word_fs << 4) & 0xf0)|(key_word_ls & 0xf);
          	j++;
          	i+=2;     
        }     
        opts->flag = RULE_OPT_KEY;
    
        if(($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in setruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
    };

setrule2options_wl: setrule2option_wl
    {
        $$ = $1;  
    }
    | setrule2option_wl setrule2options_wl
    {
        int i;
		
        if (WL_PTR_OF($1, rule2options_t)->flag & RULE_OPT_OFFSET)
        {
          	WL_PTR_OF($2, rule2options_t)->offset = WL_PTR_OF($1, rule2options_t)->offset;
        }
		
        if (WL_PTR_OF($1, rule2options_t)->flag & RULE_OPT_KEY)
        {
          	for (i=0; i<MAX_KEY_NO; i++)
          	{
            	WL_PTR_OF($2, rule2options_t)->key[i] = WL_PTR_OF($1, rule2options_t)->key[i];
            	WL_PTR_OF($2, rule2options_t)->mask[i] = WL_PTR_OF($1, rule2options_t)->mask[i];
          	}
        }
    	
        WL_PTR_OF($2, rule2options_t)->flag |= WL_PTR_OF($1, rule2options_t)->flag;
        wl_free_entry($1);
    	
        $$ = $2;
    };


/*******************************************************************
 ***** forward Commands
 *******************************************************************/
 forwardcmds:
    T_LIF T_SET T_CLASSIFY T_OC oc_no T_RULESET1 ruleno1 T_RULESET2 ruleno2 T_FORWARD T_MODE T_DEFAULT T_EOS
    {
      	unsigned int fwd_mode;
      	
      	//T:OP => "000"
      	fwd_mode = 0x0000;
      	
      	{
          	if (cli_set_forward_mode(PARSER_PARAMS, $5, $7, $9, fwd_mode) < 0)
          	{
            	errprint("Error set forward mode for classify rule.\n");
            	break;
          	}
       }
    }
  	| T_LIF T_SET T_CLASSIFY T_OC oc_no T_RULESET1 ruleno1 T_RULESET2 ruleno2 T_FORWARD T_MODE T_DROP T_EOS
    {
      	unsigned int fwd_mode;
      	
      	//T:OP => "1xx"
      	fwd_mode = 0x4000;
      	
      	{
          	if (cli_set_forward_mode(PARSER_PARAMS, $5, $7, $9, fwd_mode) < 0)
          	{
            	errprint("Error set forward mode for classify rule.\n");
            	break;
          	}
      	}
    } 
  	| T_LIF T_SET T_CLASSIFY T_OC oc_no T_RULESET1 ruleno1 T_RULESET2 ruleno2 T_FORWARD T_MODE T_TO T_VLAN vlanid T_EOS
    {
      	unsigned int fwd_mode;
      	
      	//T:OP => "001"
      	fwd_mode = 0x1000 | $14;
      	
      	{
          	if (cli_set_forward_mode(PARSER_PARAMS, $5, $7, $9, fwd_mode) < 0)
          	{
            	errprint("Error set forward mode for classify rule.\n");
            	break;
          	}
      	}
    } 
  	| T_LIF T_SET T_CLASSIFY T_OC oc_no T_RULESET1 ruleno1 T_RULESET2 ruleno2 T_FORWARD T_MODE T_TO T_VLAN vlanid T_SAMPLE T_EOS
    {
      	unsigned int fwd_mode;
      	
      	//T:OP => "011"
      	fwd_mode = 0x3000 | $14;
      	
      	{
          	if (cli_set_forward_mode(PARSER_PARAMS, $5, $7, $9, fwd_mode) < 0)
          	{
            	errprint("Error set forward mode for classify rule.\n");
            	break;
          	}
      	}
    }
  	| T_LIF T_SET T_CLASSIFY T_OC oc_no T_RULESET1 ruleno1 T_RULESET2 ruleno2 T_FORWARD T_MODE T_SAMPLE T_EOS
    {
      	unsigned int fwd_mode;
      	
      	//T:OP => "010"
      	fwd_mode = 0x2000;
      	
      	{
          	if (cli_set_forward_mode(PARSER_PARAMS, $5, $7, $9, fwd_mode) < 0)
          	{
            	errprint("Error set forward mode for classify rule.\n");
            	break;
          	}
      	}
    }
  	| T_LIF T_SHOW T_FORWARD T_MODE T_OC oc_no T_RULESET1 ruleno1 T_RULESET2 ruleno2 T_FORWARD T_MODE T_EOS
    {
      	if (cli_get_forward_mode(PARSER_PARAMS, $6, $8, $10) < 0)
      	{
        	errprint("Error get forward mode.\n");
        	break;
      	}
    }
  	| T_LIF T_SET T_OC oc_no T_SAMPLE sample_rate T_EOS
    {
      	if (cli_set_sample_rate(PARSER_PARAMS, $4, $6) < 0)
      	{
        	errprint("Error set sample rate.\n");
        	break;
      	}
    }
  	| T_LIF T_SHOW T_OC oc_no T_SAMPLE T_EOS
    {
      	if (cli_get_sample_rate(PARSER_PARAMS, $4) < 0)
      	{
        	errprint("Error get sample rate.\n");
        	break;
      	}
    };

vlanid: T_INT
  	{
    	if ($1 < 1 || $1 > 4095)
    	{
      		errprint("Invalid vlan id, should be 1-4095!\n");
      		YYERROR;
    	}
    	$$ = $1;
  	};
  	
sample_rate: T_INT
  	{
    	if ($1 < 1 || $1 > 99)
    	{
      		errprint("Invalid sample rate, should be 1-99!\n");
      		YYERROR;
    	}
    	$$ = $1;
  	};
  	
  	
/*******************************************************************
 ***** transfer Commands
 *******************************************************************/
transfercmds:
  	T_LIF T_SET T_OC oc_no T_ETHERNET T_SOURCE T_MAC T_MACADDR_WL T_EOS
  	{
    	if (cli_set_eth_mac(PARSER_PARAMS, $4, 0, WL_PTR($8)) < 0)
    	{
      		errprint("Error set transfer mode.\n");
      		break;
    	}
  	}
  	| T_LIF T_SET T_OC oc_no T_ETHERNET T_DEST T_MAC T_MACADDR_WL T_EOS
  	{
    	if (cli_set_eth_mac(PARSER_PARAMS, $4, 1, WL_PTR($8)) < 0)
    	{
      		errprint("Error set transfer mode.\n");
      		break;
    	}
  	}
  	| T_LIF T_SHOW T_OC oc_no T_TRANSFER T_MODE T_EOS
  	{
    	if (cli_get_eth_mac(PARSER_PARAMS, $4) < 0)
    	{
      		errprint("Error get transfer mode.\n");
      		break;
    	}
  	};
  	

/*******************************************************************
 ***** transfer Commands
 *******************************************************************/
gtpcmds:
	  T_LIF T_SET T_GTP T_OC oc_no T_HASH T_DEST gtpdestports T_EOS
  	{
    	if (cli_set_gtp_dest_ports(PARSER_PARAMS, $5, $8) < 0)
    	{
      		errprint("Error set transfer mode.\n");
      		break;
    	}
  	}
  	| T_LIF T_SET T_GTP T_OC oc_no T_HASH T_MODE hash_source T_EOS
  	{
    	if (cli_set_gtp_hash_mode(PARSER_PARAMS, $5, $8) < 0)
    	{
      		errprint("Error set gtp hash mode.\n");
      		break;
    	}
  	}
	  | T_LIF T_SHOW T_GTP T_OC oc_no T_EOS
  	{
    	if (cli_get_gtp_settings(PARSER_PARAMS, $5) < 0)
    	{
      		errprint("Error get transfer mode.\n");
      		break;
    	}
  	};

gtpdestports: T_INT
		{
			if ($1 < 1 || $1 > 24)
    		{
      			errprint("Invalid vlan id, should be 1-24!\n");
      			YYERROR;
    		}
    		$$ = $1;
		};

hash_source: T_INNER
	{
		$$ = 0x1;
	}
	| T_OUTER
	{
		$$ = 0x2;
	}
	| T_BOTH
	{
		$$ = 0x3;
	};

	
/*******************************************************************
 ***** Switch Commands
 *******************************************************************/
 switchcmds:
  	T_STRING_WL T_EOS
  	{
  		if (cli_bcm_cmds(PARSER_PARAMS, WL_PTR($1)) < 0)
        {
                errprint("Error send switch cmds.\n");
                break;
        }
  	}
  	|  bcmcmds_wl T_EOS
  	{
  		bcm_cmds_t opts;
      	
      	memset(&opts, 0, sizeof(bcm_cmds_t));
      	strcpy(opts.cmds, WL_PTR_OF($1, bcm_cmds_t)->cmds);
      	
      	
  		if (cli_bcm_cmds_v2(PARSER_PARAMS, &opts) < 0)
    	{
      		errprint("Error send switch cmds.\n");
      		break;
    	}
  	};


wordcomma_wl: T_WORD_WL
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, WL_PTR($1));
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_WORD_WL ',' wordcomma_wl
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, WL_PTR_OF($3, bcm_cmds_t)->cmds);
        strcat(opts->cmds, ",");
        strcat(opts->cmds, WL_PTR($1)); /* add it to the end */
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	};

worddot_wl: T_WORD_WL '.' T_WORD_WL
	{
		bcm_cmds_t *opts;
		
        	if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        	{
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        	}
		
        strcpy(opts->cmds, WL_PTR($1));
        strcat(opts->cmds, ".");
        strcat(opts->cmds, WL_PTR($3)); /* add it to the end */
	
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	};
	
/* treat token as words temporary */
bcmcmd_wl: T_INT
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        sprintf(opts->cmds, "%d", $1);
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_IPADDR
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        sprintf(opts->cmds, "%d.%d.%d.%d", (($1 >> 24) & 0xff),(($1 >> 16) & 0xff),(($1 >> 8) & 0xff),(($1 >> 0) & 0xff) );
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_STRING_WL
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "\"");
        strcat(opts->cmds, WL_PTR($1)); /* add it to the end */
	strcat(opts->cmds, "\"");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| wordcomma_wl
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, WL_PTR_OF($1, bcm_cmds_t)->cmds);
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_WORD_WL '=' wordcomma_wl
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, WL_PTR($1));
        strcat(opts->cmds, "=");
        strcat(opts->cmds, WL_PTR_OF($3, bcm_cmds_t)->cmds); /* add it to the end */
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| worddot_wl
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, WL_PTR_OF($1, bcm_cmds_t)->cmds);
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_WORD_WL '=' T_INT
	{
		char tmpint[128];
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
        
		memset(tmpint, 0, sizeof(char)*128);
        strcpy(opts->cmds, WL_PTR($1));
        strcat(opts->cmds, "=");
        sprintf(tmpint, "%d", $3);
        strcat(opts->cmds, tmpint); /* add it to the end */
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_WORD_WL '=' T_STRING_WL
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, WL_PTR($1));
        strcat(opts->cmds, "=\"");
        strcat(opts->cmds, WL_PTR($3)); /* add it to the end */
		strcat(opts->cmds, "\"");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_ALL
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "all");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_CLEAR
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "clear");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_CONFIG
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "config");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_COUNTERS
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "counters");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_DISABLE
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "disable");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_DROP
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "drop");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_DEFAULT
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "default");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_ENABLE
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "enable");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_ENABLE '=' T_WORD_WL
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
	  
        strcpy(opts->cmds, "enable");
        strcat(opts->cmds, "=");
        strcat(opts->cmds, WL_PTR($3)); /* add it to the end */
	  
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_ENCAP
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "encap");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_EGRESS
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "egress");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}	
	| T_FORWARD
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "forward");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_FILE
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "file");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_GLOBAL
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "global");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_GET
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "get");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_IP
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "ip");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_INFO
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "info");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_LENGTH
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "length");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_LENGTH '=' T_INT
	{
		char tmpint[128];
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		memset(tmpint, 0, sizeof(char)*128);
		sprintf(tmpint, "%d", $3);
		
        strcpy(opts->cmds, "length=");
		strcat(opts->cmds, tmpint);
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}	
	| T_MODE
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "mode");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_MAC
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "mac");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_NONE
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "none");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_NETMASK
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "netmask");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_OFFSET
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "offset");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_OFFSET '=' T_INT
	{
		char tmpint[128];
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		memset(tmpint, 0, sizeof(char)*128);
		sprintf(tmpint, "%d", $3);
		
        strcpy(opts->cmds, "offset=");
		strcat(opts->cmds, tmpint);
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_PORT
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "port");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_PORTS
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
	 
        strcpy(opts->cmds, "ports");
	 
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_PROTOCOL
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "protocol");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_RANGE
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "range");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_RATE
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "rate");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_RCLOAD
	{
				bcm_cmds_t *opts;
				
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
				
        strcpy(opts->cmds, "rcload");
				
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	
	| T_READ
	{
				bcm_cmds_t *opts;
				
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "read");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_SET
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "set");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_STATS
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "stats");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_SHOW
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "show");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_SOURCE
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "source");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_SAVE
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "save");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_UPDATE
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "update");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_VLAN
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "vlan");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	}
	| T_WRITE
	{
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "write");
		
        if (($$ = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	};

bcmcmds_wl: bcmcmd_wl
    {
        $$ = $1;
    }
    | bcmcmd_wl bcmcmds_wl
    {
        char str_tmp[128];
        
        memset(str_tmp, 0, 128*(sizeof(char)));
        str_tmp[0] = 32;  //blank space
        strcat(str_tmp, WL_PTR_OF($2, bcm_cmds_t)->cmds);
        
    	strcat(WL_PTR_OF($1, bcm_cmds_t)->cmds, str_tmp);
        wl_free_entry($2);
    	
        $$ = $1;
    };  

/*******************************************************************
 ***** OC Commands
 *******************************************************************/
 occmds:
  	T_LIF T_SET T_OC oc_no T_LINE T_RATE line_rate T_EOS
  	{
    	if(cli_set_oc_line_rate(PARSER_PARAMS, $4, $7) < 0)
    	{
      		errprint("Error set oc line mode.\n");
      		break;
    	}
  	}
    | T_LIF T_SET T_OC oc_no T_LINE T_MODE line_mode T_EOS
    {
      	if(cli_set_oc_line_mode(PARSER_PARAMS, $4, $7) < 0)
      	{
        	errprint("Error set oc line mode.\n");
        	break;
      	}
    }
    | T_LIF T_SET T_OC oc_no T_ENCAP T_MODE encap_mode T_EOS
    {
      	if (cli_set_oc_encap_mode(PARSER_PARAMS, $4, $7) < 0)
      	{
        	errprint("Error set OC encapsulate mode.\n");
        	break;
      	}
    }
    | T_LIF T_SET T_OC oc_no T_SONNET T_SCRAMBLE enable_or_disable T_EOS
    {
      	if (cli_set_oc_sonnet_scramble_enable(PARSER_PARAMS, $4, $7) < 0)
      	{
        	errprint("Error setting sonnet scramble enable.\n");
        	break;
      	}
    }
    | T_LIF T_SET T_OC oc_no T_PAYLOAD T_SCRAMBLE enable_or_disable T_EOS
    {
      	if (cli_set_oc_payload_scramble_enable(PARSER_PARAMS, $4, $7) < 0)
      	{
        	errprint("Error setting payload scramble enable.\n");
        	break;
      	}
    }
    | T_LIF T_SET T_OC oc_no T_CRC T_MODE crc_mode T_EOS
    {
      	if (cli_set_oc_crc_mode(PARSER_PARAMS, $4, $7) < 0)
      	{
        	errprint("Error setting oc crc mode.\n");
        	break;
      	}
    }
    | T_LIF T_SHOW T_OC T_SETTINGS T_EOS
    {
      	if (cli_get_oc_settings(PARSER_PARAMS) < 0)
      	{
        	errprint("Error getting oc settings.\n");
        	break;
      	} 
    }
    | T_LIF T_SHOW T_OC oc_no T_COUNTERS T_EOS
    {
      	if (cli_palermo_get_stats($4) < 0)
      	{
        	errprint("Error getting oc counters.\n");
        	break;
      	}
    }
    | T_LIF T_SHOW T_OC oc_no T_FLOW T_EOS
    {
      	if (cli_get_framer_stats($4) < 0)
      	{
        	errprint("Error getting oc flow info.\n");
        	break;
      	}
    }
    | T_LIF T_SHOW T_OC oc_no T_CONFIG T_EOS
    {
      	if (cli_get_framer_running_cfg($4) < 0)
      	{
        	errprint("Error getting oc flow info.\n");
        	break;
      	}
    }    
    | T_LIF T_SAVE T_OC T_SETTINGS T_EOS
    {
      	if (cli_save_oc_settings(PARSER_PARAMS) < 0)
      	{
        	errprint("Error save oc settings to profile.\n");
        	break;
      	}
    };


line_rate:  T_OC192
  	{
    	$$ = OC_LINERATE_192C;
  	}
  	| T_OC48
  	{
    	$$ = OC_LINERATE_48C;
  	};

line_mode:  T_SONNET
  	{
    	$$ = OC_LINE_SONNET;
  	}
  	| T_SDH
  	{
    	$$ = OC_LINE_SDH;
  	};

encap_mode: T_PPP
  	{ 
    	$$ = OC_ENCAP_PPP;
  	}
  	| T_HDLC
  	{
    	$$ = OC_ENCAP_HDLC;
  	};

crc_mode: T_CRC_CCITT
  	{
    	$$ = OC_CRC_CCITT;
  	}
  	| T_CRC_32
  	{
    	$$ = OC_CRC_32;
  	}
  	| T_CRC_NONE
  	{
    	$$ = OC_CRC_NONE;
  	};


/*******************************************************************
 **** Fpga Commands
 *******************************************************************/
fpgacmds:
    T_LIF T_OC oc_no T_FPGA T_REGISTER fpga_reg T_READ T_EOS
    {
      	if (cli_fpga_reg_read($3, $6) < 0)
      	{
        	errprint("Error reading fpga register.\n");
        	break;
      	}
    }
  	| T_LIF T_OC oc_no T_FPGA T_REGISTER fpga_reg T_WRITE fpga_reg_data T_EOS
    {
      	if (cli_fpga_reg_write($3, $6, $8) < 0)
      	{
        	errprint("Error writting fpga register.\n");
        	break;
      	}
    }
  	| T_LIF T_OC oc_no T_SHOW T_FPGA T_FLOW T_EOS
    {
      	if (cli_get_fpga_stats($3) < 0)
      	{
        	errprint("Error getting fpga stats.\n");
        	break;
      	}
    }
    | T_LIF T_PACKET T_FLOW T_EOS
    {
      	if (cli_get_packet_stats() < 0)
      	{
        	errprint("Error getting packet stats.\n");
        	break;
      	}
    }
    | T_LIF T_SET T_OC oc_no T_FLOW enable_or_disable T_EOS
    {
    	if (cli_fpga_set_flow_enable(PARSER_PARAMS, $4, $6) < 0)
    	{
    		errprint("Error setting flow function.\n");
    		break;
    	}
    }
    | T_LIF T_GET T_OC oc_no T_FLOW T_EOS
    {
    	if (cli_fpga_get_flow_enable(PARSER_PARAMS, $4) < 0)
    	{
    		errprint("Error setting flow function.\n");
    		break;
    	}
    }
    | T_LIF T_SET T_OC oc_no T_FLOW T_TCP T_MMS mms_size T_EOS
    {
    	if (cli_fpga_set_flow_mms(PARSER_PARAMS, $4, $8) < 0)
    	{
    		errprint("Error setting flow function.\n");
    		break;
    	}
    }
    | T_LIF T_GET T_OC oc_no T_FLOW T_TCP T_MMS T_EOS
    {
    	if (cli_fpga_get_flow_mms(PARSER_PARAMS, $4) < 0)
    	{
    		errprint("Error setting flow function.\n");
    		break;
    	}
    }
    | T_LIF T_SET T_OC oc_no T_TRAFFIC enable_or_disable T_EOS
    {
    	if (cli_fpga_set_traffic_enable(PARSER_PARAMS, $4, $6) < 0)
    	{
    		errprint("Error setting flow function.\n");
    		break;
    	}
    }
    | T_LIF T_GET T_OC oc_no T_TRAFFIC T_EOS
    {
    	if (cli_fpga_get_traffic_enable(PARSER_PARAMS, $4) < 0)
    	{
    		errprint("Error setting flow function.\n");
    		break;
    	}
    };

fpga_reg: T_INT
    {
      	if ($1 < 0 || $1 > 0xffffffff)
      	{
        	errprint("Valid register range is: 0 - 0xffffffff\n");
        	YYERROR;
      	}
      	$$ = $1;
    };

fpga_reg_data: T_INT
    {
      	if ( $1 < 0 || $1 > 0xffffffff )
      	{
        	errprint("Valid fpga reg data is: 0x0000 - 0xffffffff\n");
        	YYERROR;
      	}
      	$$ = $1;
    }; 

mms_size: T_INT
	{
		if ( $1 < 500 || $1 > 1500 )
      	{
        	errprint("Valid MMS is: 500 - 1500\n");
        	YYERROR;
      	}
      	$$ = $1;
	};

%%

#include "flex.h"
   
/* dummy declaration to silence warning caused by incorrectly
   generated declaration in flex.h */
static int yy_init_globals (yyscan_t yyscanner )
{
    (void) yyscanner;
    (void) yy_init_globals;
    return 0;
}

/*******************************************************************
 *
 * Description:     Parser error function.
 *
 * Modified args:   NONE
 *
 * Return value:    N/A.
 *
 *******************************************************************/
void scerror(char *s)
{
    printf("%s\n", s);
}

static char *StrToUpper(char *str)
{
    char *p = str;

    while (*p != '\0')
    {
        *p = toupper(*p);
        p++;
    }

    return str;
}

/*******************************************************************
 *
 * Description:     Make a port mask with all ports from
 *                  MIN(a,b) to MAX(a,b) in it.
 *
 * Modified args:   None
 *
 * Return value:    port mask.
 *
 *******************************************************************/
static int portrange(int a, int b)
{
    int start;
    int stop;
    int i;
    int mask = 0;

    start = (a < b) ? a : b;
    stop  = (a < b) ? b : a;

    for (i = start; i <= stop; i++)
    {
        mask |= 1 << i;
    }

    return mask;
}
