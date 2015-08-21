/*
 *                          Copyright QiCe Tech, 2010-2012
 *
 *                          ALL RIGHTS RESERVED
 *
 * Description:      
 *
 */

#ifndef __DATABASE_H__
#define __DATABASE_H__

#include "db_client/db_client.h"
#include "lexer.h"
#include "include/qnftypes.h"


#ifdef BUILD_MC20E_PPOE
#define MAX_RULE_NO     		32
#else
#define MAX_RULE_NO     		64
#endif
#define MAX_KEY_NO      		20
#define MAX_KEY_LEN     		40
#define MAX_OC_NUM 			4
#define MAX_PKT_RANGE                16
#define MAX_CMD_BUF                    1024
#ifdef  BUILD_QT10
#define MAX_BYPASS_PORTS          4
#else
#define MAX_BYPASS_PORTS          12
#endif

#define DRV_CONF_FILE_TMP         "/etc/sysconfig/qnf.conf.tmp"
#define DRV_CONF_FILE               	"/etc/sysconfig/qnf.conf"
#define DEFAULT_SHM_DEV       	"/dev/shm/stats"
#define ETXCONFIGFILE			"/usr/local/bin/config.soc"

#if 0
typedef struct{
    unsigned int line_rate;
    unsigned int frame;
    unsigned int encap;
    unsigned int framer_scramble;
    unsigned int mapper_scramble;
    unsigned int fcs;
    unsigned int    pass_crc;
    unsigned int    max_pkt_size;
    unsigned int min_pkt_size;
}conf_t;


typedef struct{
    conf_t configs[MAX_OC_NUM];
}confs_t;
#endif

typedef struct {
    unsigned short  flag;
    unsigned char   protocol;
    unsigned long   sourceip;
    unsigned long   sourceip_mask;
    unsigned long   destip;
    unsigned long   destip_mask;
    unsigned short  sourcel4port;
    unsigned short  destl4port; 
    unsigned short  pktlen_range;   
    unsigned short  id;
}rule1options_t;    


typedef struct {
    unsigned short  flag;
    unsigned char   offset;     // no larger than 20 Bytes
    unsigned char   key[MAX_KEY_NO];
    unsigned char   mask[MAX_KEY_NO];
    unsigned short  id;
}rule2options_t;    

typedef struct {
    char cmds[MAX_CMD_BUF];
} bcm_cmds_t;

typedef struct {
    unsigned short  fwd_mode[MAX_RULE_NO][MAX_RULE_NO];
}forward_mode_t;


/*******************************************************************
 *
 * Description:     Function prototypes
 *
 *******************************************************************/

extern struct parser_params pparams; 

extern int db_open(DATABASE *pdb);
extern int db_close(DATABASE *pdb);
extern int db_dump(DATABASE *pdb, char *pattern);
extern int db_save(DATABASE *pdb, char *file);
extern int db_init_config(DATABASE *pdb);
extern int db_initialized(DATABASE *pdb);
extern int db_set_initialized(DATABASE *pdb);
extern int db_reset_default_config(DATABASE *pdb);

/* filter functions */
extern int db_filter1_rule_exist(DATABASE *pdb, unsigned long oc_no, unsigned long ruleno);
extern int db_filter2_rule_exist(DATABASE *pdb, unsigned long oc_no, unsigned long ruleno);
extern int db_get_filter1_rule_num(DATABASE *pdb, unsigned long oc_no, unsigned long *rule_num);
extern int db_get_filter2_rule_num(DATABASE *pdb, unsigned long oc_no, unsigned long *rule_num);
extern int db_set_filter1_rule_num(DATABASE *pdb, unsigned long oc_no, unsigned long rule_num);
extern int db_set_filter2_rule_num(DATABASE *pdb, unsigned long oc_no, unsigned long rule_num);
extern int db_set_filter1_rule(DATABASE *pdb, unsigned long oc_no, rule1options_t *opts);
extern int db_set_filter2_rule(DATABASE *pdb, unsigned long oc_no, rule2options_t *opts);
extern int db_get_filter1_rule(DATABASE *pdb, unsigned long oc_no, rule1options_t *opts);
extern int db_get_filter2_rule(DATABASE *pdb, unsigned long oc_no, rule2options_t *opts);
extern int db_delete_filter1_rule(DATABASE *pdb, unsigned long oc_no, unsigned long ruleno);
extern int db_delete_filter2_rule(DATABASE *pdb, unsigned long oc_no, unsigned long ruleno);
extern int db_set_pktlen_range(DATABASE *pdb, unsigned long oc_no, unsigned long range_no, unsigned long pktlen_min, unsigned long pktlen_max);
extern int db_get_pktlen_range(DATABASE *pdb, unsigned long oc_no, unsigned long range_no, unsigned long *pktlen_min, unsigned long *pktlen_max);
extern int db_set_sample_rate(DATABASE *pdb, unsigned long oc_no, unsigned long sample_rate);
extern int db_get_sample_rate(DATABASE *pdb, unsigned long oc_no, unsigned long *sample_rate);

/* forward functions */
extern int db_set_forward_mode(DATABASE *pdb, unsigned long oc_no, unsigned long ruleno1, unsigned long ruleno2, unsigned long forward_mode);
extern int db_get_forward_mode(DATABASE *pdb, unsigned long oc_no, unsigned long ruleno1, unsigned long ruleno2, unsigned long *forward_mode);
extern int db_forward_mode_exist(DATABASE *pdb, unsigned long oc_no, unsigned long ruleno1, unsigned long ruleno2);
extern int db_delete_forward_mode(DATABASE *pdb, unsigned long oc_no, unsigned long ruleno1, unsigned long ruleno2);

/* gtp functions */
extern int db_set_gtp_dest_ports(DATABASE *pdb, unsigned long oc_no, unsigned long gtpdestports);
extern int db_get_gtp_dest_ports(DATABASE *pdb, unsigned long oc_no, unsigned long *gtpdestports);
extern int db_set_gtp_hash_mode(DATABASE *pdb, unsigned long oc_no, unsigned long out_both);
extern int db_get_gtp_hash_mode(DATABASE *pdb, unsigned long oc_no, unsigned long *out_both);

/* transfer functions */
extern int db_set_eth_mac(DATABASE *pdb, unsigned long oc_no, unsigned int src_dst, unsigned char mac[6]);
extern int db_get_eth_mac(DATABASE *pdb, unsigned long oc_no, unsigned int src_dst, unsigned char mac[6]);
extern int db_set_flow_enable(DATABASE *pdb, unsigned long oc_no, unsigned long enable_or_disable);
extern int db_get_flow_enable(DATABASE *pdb, unsigned long oc_no, unsigned long *enable_or_disable);
extern int db_set_flow_mms(DATABASE *pdb, unsigned long oc_no, unsigned long mms_size);
extern int db_get_flow_mms(DATABASE *pdb, unsigned long oc_no, unsigned long *mms_size);
extern int db_set_traffic_enable(DATABASE *pdb, unsigned long oc_no, unsigned long enable_or_disable);
extern int db_get_traffic_enable(DATABASE *pdb, unsigned long oc_no, unsigned long *enable_or_disable);

/* oc functions */
extern int db_set_oc_line_rate(DATABASE *pdb, unsigned long oc_no, unsigned long line_rate);
extern int db_set_oc_line_mode(DATABASE *pdb, unsigned long oc_no, unsigned long line_mode);
extern int db_set_oc_encap_mode(DATABASE *pdb, unsigned long oc_no, unsigned long encap_mode);
extern int db_set_oc_sonnet_scramble_enable(DATABASE *pdb, unsigned long oc_no, unsigned long enable_or_disable);
extern int db_set_oc_payload_scramble_enable(DATABASE *pdb, unsigned long oc_no, unsigned long enable_or_disable);
extern int db_set_oc_crc_mode(DATABASE *pdb, unsigned long oc_no, unsigned long crc_mode);
extern int db_set_oc_pass_crc_mode(DATABASE *pdb, unsigned long oc_no, unsigned long pass_crc);
extern int db_set_oc_max_pkt_size(DATABASE *pdb, unsigned long oc_no, unsigned long max_pkt_size);
extern int db_set_oc_min_pkt_size(DATABASE *pdb, unsigned long oc_no, unsigned long min_pkt_size);
extern int db_get_oc_line_rate(DATABASE *pdb, unsigned long oc_no, unsigned long *line_rate);
extern int db_get_oc_line_mode(DATABASE *pdb, unsigned long oc_no, unsigned long *line_mode);
extern int db_get_oc_encap_mode(DATABASE *pdb, unsigned long oc_no, unsigned long *encap_mode);
extern int db_get_oc_sonnet_scramble_enable(DATABASE *pdb, unsigned long oc_no, unsigned long *enable_or_disable);
extern int db_get_oc_payload_scramble_enable(DATABASE *pdb, unsigned long oc_no, unsigned long *enable_or_disable);
extern int db_get_oc_crc_mode(DATABASE *pdb, unsigned long oc_no, unsigned long *crc_mode);
extern int db_get_oc_pass_crc_mode(DATABASE *pdb, unsigned long oc_no, unsigned long *pass_crc);
extern int db_get_oc_max_pkt_size(DATABASE *pdb, unsigned long oc_no, unsigned long *max_pkt_size);
extern int db_get_oc_min_pkt_size(DATABASE *pdb, unsigned long oc_no, unsigned long *min_pkt_size);


#endif

