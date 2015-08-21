/*
 *                          Copyright QiCe Tech, 2010-2012
 *
 *                          ALL RIGHTS RESERVED
 *
 * Description:      
 *
 */

%{
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "lexer.h"
#include "y.tab.h"
#include "utils/errprint.h"
#include "utils/watch_list.h"


#define YY_NO_UNPUT
#define YY_DECL int yylex(YYSTYPE *lvalp, yyscan_t yyscanner)
#define YY_EXTRA_TYPE struct parser_params *

extern int readline_input(char *buf, int maxlen);
#undef YY_INPUT
#define YY_INPUT(buf, retval, maxlen) (retval = readline_input(buf, maxlen))

#define yylval_param lvalp

struct token_entry {
    char 		*pcTokenName;
    int 		token;
    unsigned 	uMatchLength;
};


/* The token table must be sorted alphabetically !! */
static struct token_entry token_table[] =
{
    {"all",                 T_ALL,              3},
    {"any",                 T_ANY,              3},
    {"both",		    T_BOTH,		3},
    {"byp",                 T_BYP,              3},
    {"bypass",              T_BYPASS,           4},
    {"classify",            T_CLASSIFY,         3},
    {"clear",               T_CLEAR,            3},
    {"config",              T_CONFIG,           3},
    {"counters",            T_COUNTERS,         3},
    {"crc",                 T_CRC,              3},
    {"crc_ccitt",           T_CRC_CCITT,        5},
    {"crc_32",              T_CRC_32,           5},
    {"crc_none",            T_CRC_NONE,         5},
    {"default",             T_DEFAULT,          3}, 
    {"destination",         T_DEST,             3},
    {"disable",             T_DISABLE,          4},
    {"drop",                T_DROP,             4},
    {"enable",              T_ENABLE,           3},
    {"encap",               T_ENCAP,            3},
    {"egress",              T_EGRESS,           4},
    {"ethernet",            T_ETHERNET,         3},
    {"etxconfig",           T_ETXCONFIG,   	3},
    {"exit",                T_EXIT,             4},
    {"flow",                T_FLOW,             4},
    {"fpga",                T_FPGA,             4},
    {"file",                T_FILE,             4},
    {"firmware",            T_FIRMWARE,         3},
    {"forward",             T_FORWARD,          3},
    {"get",                 T_GET,              3},
    {"global",              T_GLOBAL,           3},
    {"gtp",		    T_GTP,		3},
    {"hash",                T_HASH,             4},
    {"hdlc",                T_HDLC,             4},
    {"info",                T_INFO,             4},
    {"inner",		    T_INNER,            5},
    {"ip",                  T_IP,               2},
    {"key",                 T_KEY,              3},
    {"l4_port",             T_L4_PORT,          4},
    {"length",              T_LENGTH,           3},
    {"lif",                 T_LIF,              3},
    {"line",                T_LINE,             4},
    {"mac",                 T_MAC,              3},
    {"mms",                 T_MMS,              3},
    {"mode",                T_MODE,             4},
    {"netmask",             T_NETMASK,          3},
    {"none",                T_NONE,             3},
    {"normal",              T_NORMAL,           3},
    {"offset",              T_OFFSET,           4},
    {"oc",                  T_OC,               2},
    {"oc48",                T_OC48,             4},
    {"oc192",               T_OC192,            4}, 
    {"outer",		    T_OUTER,		4},
    {"packet",              T_PACKET,           3},
    {"payload",             T_PAYLOAD,          3},
    {"ppp",                 T_PPP,              3},
    {"protocol",            T_PROTOCOL,         4},
    {"port",		    T_PORT,		4},
    {"ports",		    T_PORTS,		5},
    {"quit",                T_QUIT,             4},
    {"range",               T_RANGE,            3},
    {"rate",                T_RATE,             4},
    {"rcload",              T_RCLOAD,           4},
    {"read",                T_READ,             3},
    {"register",            T_REGISTER,         3},
    {"restore",             T_RESTORE,          4},
    {"rule",                T_RULE,             4},
    {"ruleset1",            T_RULESET1,         8},
    {"ruleset2",            T_RULESET2,         8},
    {"sample",              T_SAMPLE,           3},
    {"save",                T_SAVE,             3},
    {"scramble",            T_SCRAMBLE,         3},
    {"sdh",                 T_SDH,              3},
    {"settings",            T_SETTINGS,         4},
    {"set",                 T_SET,              3},
    {"show",                T_SHOW,             4},
    {"sonet",               T_SONNET,           3},
    {"source",              T_SOURCE,           3},
    {"system",              T_SYSTEM,           3},
    {"stats",               T_STATS,            5},
    {"tcp",                 T_TCP,              3},
    {"to",                  T_TO,               2},
    {"traffic",		    T_TRAFFIC,		4},
    {"transfer",            T_TRANSFER,         4},
    {"udp",                 T_UDP,              3},
    {"update",              T_UPDATE,           3},
    {"vlan",                T_VLAN,             3},
    {"write",               T_WRITE,            3},
    {"",                    0,                  0}
};

/* Function prototypes */
static int iStrMatch(char *str1, char *str2, int iMinMatch);
static int FindToken(char *input);

%}

%option reentrant bison-bridge header-file="flex.h"
   
%x STRING
   
integer  [0-9]+
hexdigit [0-9a-fA-F]
hexint   {hexdigit}+
macdigit {hexdigit}{2}
macnum   {macdigit}:{macdigit}:{macdigit}:{macdigit}:{macdigit}:{macdigit}
ipaddr   {integer}\.{integer}\.{integer}\.{integer}
words    [a-zA-Z][a-zA-Z0-9_-]*

%%

{integer}   { lvalp->ival = atoi(yytext); return T_INT; }

0x{hexint}  { sscanf(yytext, "0x%x", &lvalp->ival); return T_INT; }

{macnum}    {
                unsigned char *mac;
                unsigned int imac[6];
                
                if ((mac = malloc(6)) == NULL)
                {
                    errprint("Out of memory in lexer - macaddr.\n");
                    return yytext[0];
                }
                
                sscanf(yytext, "%x:%x:%x:%x:%x:%x",
                    &imac[0], &imac[1], &imac[2], 
                    &imac[3], &imac[4], &imac[5]);

                mac[0] = (unsigned char)imac[0]; mac[1] = (unsigned char)imac[1];
                mac[2] = (unsigned char)imac[2]; mac[3] = (unsigned char)imac[3];
                mac[4] = (unsigned char)imac[4]; mac[5] = (unsigned char)imac[5];

                if ((lvalp->wlptr = wl_add_entry(yyextra->wl, mac)) == NULL)
                {
                    errprint("Watch list full in lexer - macaddr.\n");
                    free(mac);
                    return yytext[0];
                }
                    
                return T_MACADDR_WL;
            }

{ipaddr}    {
                unsigned int ui0;
                unsigned int ui1;
                unsigned int ui2;
                unsigned int ui3;

                sscanf(yytext, "%d.%d.%d.%d", &ui3, &ui2, &ui1, &ui0);
                if (ui3 < 0 || ui3 > 255 || ui2 < 0 || ui2 > 255 ||
                    ui1 < 0 || ui1 > 255 || ui0 < 0 || ui0 > 255)
                {
                    errprint("Invalid IP address: %s\n", yytext);
                    return yytext[0];
                }

                lvalp->uval = (ui3 << 24) | (ui2 << 16) | (ui1 << 8) | ui0;
                
                return T_IPADDR;
            }

{words}     {
                int token;
                void *vp;

                token = FindToken(yytext);
                if (token > 0)
                {
                    return token; /* A valid token found, returning token to Bison */
                }

                if ((vp = strdup(yytext)) == NULL)
                {
                    errprint("Out of memory in lexer - words.\n");
                    return yytext[0];
                }
                if ((lvalp->wlptr = wl_add_entry(yyextra->wl, vp)) == NULL)
                {
                    errprint("Watch list full in lexer - words.\n");
                    free(vp);
                    return yytext[0];
                }
                
                return T_WORD_WL;
            }
            
\"          BEGIN(STRING);
<STRING>[^\"\n]* {
                    void *vp;

                    if ((vp = strdup(yytext)) == NULL)
                    {
                        errprint("Out of memory in lexer - string.\n");
                        return yytext[0];
                    }
                    if ((lvalp->wlptr = wl_add_entry(yyextra->wl, vp)) == NULL)
                    {
                        errprint("Watch list full in lexer - string.\n");
                        free(vp);
                        return yytext[0];
                    }
                    
                    return T_STRING_WL;
                 }

<STRING>[\"]    {
                    BEGIN(INITIAL);
                }

<STRING>\r?\n   {
                    BEGIN(INITIAL);
                    return T_EOS;
                }

\r?\n       return T_EOS;

#.*\r?\n    return T_EOS;

[ \t]+
[\x80-\xFF] {
                if (isalpha(yytext[0]))
                {
                    return yytext[0];
                }
                else
                {
                    return T_INVALID_CHAR;
                }
            }

.           return yytext[0];

%%

/*********************************************************
 *** set_input_buff
 ***
 *** Set up the lexer to get input from a buffer.
 *** After end is reached it  switches back to stdin as input.
 ***
 *********************************************************/
int
set_input_buff(char *buff, void *eob_callback, void *yyscanner)
{
    struct parser_params *pparm;
    struct input_buffer *buffrec;
    
    (void)yyunput;
    pparm = yyget_extra(yyscanner);

    if ((buffrec = malloc(sizeof(struct input_buffer))) == NULL)
    {
        errprint("out of memory in set_input_buffer.\n");
        return -1;
    }
    memset(buffrec, 0, sizeof(struct input_buffer));
    buffrec->callback = eob_callback;
    buffrec->next = pparm->input;
    buffrec->input = sc_scan_string(buff, yyscanner);
    pparm->input = buffrec;
    pparm->loading_file++;
    
    return 0;
}

/*********************************************************
 *** set_input_file
 ***
 *** Set up the lexer to get input from a specific file.
 *** After end of file is reached for this file, it 
 *** switches back to stdin as input.
 *** Calls to this function can not be nested, ie loading
 *** a file that has a command that loads a file ...
 ***
 *********************************************************/
int
set_input_file(char *name, void *eof_callback, void *yyscanner)
{
    FILE *fp;
    struct parser_params *pparm;
    struct input_buffer *buffrec;

    if ((fp = fopen(name, "r")) == NULL)
    {
        errprint("Can't open file %s for input.\n", name);
        return -1;
    }

    pparm = scget_extra(yyscanner);
    
    if ((buffrec = malloc(sizeof(struct input_buffer))) == NULL)
    {
        errprint("out of memory in set_input_buffer.\n");
        return -1;
    }
    memset(buffrec, 0, sizeof(struct input_buffer));
    
    buffrec->callback = eof_callback;
    buffrec->next = pparm->input;
    buffrec->input = sc_create_buffer(fp, YY_BUF_SIZE, yyscanner);
    buffrec->fp = fp;
    
    pparm->loading_file++;
    pparm->input = buffrec;
    sc_switch_to_buffer(buffrec->input, yyscanner);

    return 0;
}

/*********************************************************
 *** yywrap
 ***
 *** Used by scanner at end of file to check if
 *** scanning should continue.
 ***
 *********************************************************/
int
yywrap(yyscan_t yyscanner)
{
    struct parser_params *pparm;
    struct input_buffer *ibuf;
    
    pparm = scget_extra(yyscanner);
    
    ibuf = pparm->input;
    if (ibuf == NULL)
    {
        /* No active buffer, scanning should stop */
        return 1;
    }

    if (ibuf->fp != NULL)
    {
        fclose(ibuf->fp);
    }
    sc_delete_buffer(ibuf->input, yyscanner);
    
    pparm->loading_file--;
    if (ibuf->callback)
    {
        (ibuf->callback)(pparm);
    }
    
    pparm->input = ibuf->next;
    free(ibuf);

    ibuf = pparm->input;
    if (ibuf == NULL)
    {
        screstart(stdin, yyscanner);
    }
    else
    {
        sc_switch_to_buffer(ibuf->input, yyscanner);
    }

    return 0;
}

/********************************************************
 ***added for readline
 ***
 ***
 ********************************************************/
static char *line_read = (char *)NULL;

int
readline_input(char *buf, int maxlen)
{   
    char *pchar;
    int number=0;   

    if(line_read)
    {
        free(line_read); 
        line_read = (char *)NULL;
    }

    //start the timer  
    alarm(60*20);
    //alarm(60*2);
    
    line_read = readline("CLI> ");
    if (line_read != NULL)
    {
        if(buf)
        {
            memset(buf,0,sizeof(buf));
            for( number = 0, pchar = line_read; 
            	number < maxlen && *pchar != '\0'; number++,pchar++)
                buf[number] = *pchar;
            if (*pchar == '\0')
                buf[number++] = '\n';
            if (number >= maxlen)
                printf("Too many chars in comand line.\n");

            if(*line_read)
                add_history(line_read);
        }
        else
        {
            number = 0;
            printf("BUF is NULL!!!\n");
        }   
    }   
    return number;
}

/*********************************************************
 *** ulStrMatch
 ***
 *** Match two strings and return 0 if equal.
 *** The two string can have different length, they
 *** are considered equal if the shortest string is at least
 *** iMinMatch characters long, and they match the other string.
 *** Str1 is considered the 'correct' string, so if str2
 *** has extra characters it will not match
 ***
 *********************************************************/
static int iStrMatch(char *str1, char *str2, int iMinMatch)
{
    int iLen = 0;
    unsigned long ulFlag;

    ulFlag = 0;
    while ((*str1 != '\0') && (*str2 != '\0')) {
        if (toupper(*str1) != toupper(*str2)) {
            /* Mismatch, return */
            return 1;
        }
        str1++;
        str2++;
        iLen++;
    }

    if (*str1 == '\0' && *str2 != '\0') {
        /* Trailing characters in str2, mismatch */
        return 1;
    }

    /* Strings match so far, make sure shortest string is
     * at least iMinMatch chars long */
    return !(iLen >= iMinMatch);
}

/*********************************************************
 *** FindToken
 ***
 *** Search for token in token table, and return token value.
 *** Return 0 if token is not found.
 ***
 *********************************************************/
static int FindToken(char *input)
{
    struct token_entry *token;

    token = token_table;
    while (token->uMatchLength != 0) {
        if (tolower(input[0]) == token->pcTokenName[0]) {
            if (!iStrMatch(token->pcTokenName, input, token->uMatchLength)) {
                return token->token;
            }
        }
        if (tolower(input[0]) < token->pcTokenName[0]) {
            break;
        }
        token++;
    }

    return 0;
}
