/*
 *                  		Copyright QiCe Tech, 2007-2010
 *
 *                          ALL RIGHTS RESERVED
 *
 * Description:      
 *
 */

#ifndef __DBIPC_H__
#define __DBIPC_H__

#define DB_SRV_PORT      7667
#define DB_SRV_BUF_SIZE  2048

#include "utils/hash.h"

typedef enum {
    DBIPC_CLOSE,
    DBIPC_GETKEY,
    DBIPC_ADDKEY,
    DBIPC_DELKEY,
    DBIPC_LOCK,
    DBIPC_UNLOCK,
    DBIPC_SHUTDOWN,
    DBIPC_KEYEXISTS,
    DBIPC_GETFIRST,
    DBIPC_GETNEXT
} dbipc_cmd_t;

typedef struct {
    hash_search_state_t h_state;
    struct hash_datum key;
    int data_length;
    unsigned char data[0];
} db_search_state_t;



struct dbipcrec {
    dbipc_cmd_t cmd;
    union {
        struct {
            int key_size;
        } c_getkey;

        struct {
            int key_size;
            int data_size;
        } c_addkey;

        struct {
            int key_size;
        } c_delkey;
        
        struct {
            int key_size;
        } c_keyexists;
        
        struct {
            int pattern_size;
        } c_getfirst;
        
        struct {
            int pattern_size;
            hash_search_state_t h_state;
        } c_getnext;
    } req;
    int result;
    union {
        struct {
            int data_size;
        } c_getkey;
        
        struct {
            hash_search_state_t h_state;
            int key_size;
            int data_size;
        } c_getfirst;
        
        struct {
            hash_search_state_t h_state;
            int key_size;
            int data_size;
        } c_getnext;
    } res;
    unsigned char data[0];
};

extern int start_db_srv(void);
extern int stop_db_srv(void);
extern int g_db_server_port;


#endif
