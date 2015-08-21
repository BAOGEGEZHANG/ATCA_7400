/*
 *                  		Copyright QiCe Tech, 2007-2010
 *
 *                          ALL RIGHTS RESERVED
 *
 * Description:      
 *
 */


#ifndef __LEXER_H__
#define __LEXER_H__

#include "utils/watch_list.h"
#include "db_client/db_client.h"
#include "cs_client/cs_client.h"

struct input_buffer {
    void *input;
    FILE *fp;
    void (*callback)();
    struct input_buffer *next;
};

struct parser_params {
    int loading_file;
    struct watch_list *wl;
    struct input_buffer *input;
    void *scanner;
    DATABASE db;
    CONSRV cs;
    unsigned int db_lock;	
    char *shmbuf;
};


extern int sclex();
extern int set_input_buff(char *buff, void *eob_callback, void *yyscanner);
extern int set_input_file(char *name, void *eof_callback, void *yyscanner);

#endif
