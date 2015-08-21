/*
 *                          Copyright QiCe Tech, 2007-2010
 *
 *                          ALL RIGHTS RESERVED
 *
 * Description:      
 *
 */

#ifndef __PARSEROPS_H__
#define __PARSEROPS_H__
#include "lexer.h"
#include "database.h"



// rule1 options
#define RULE_OPT_PROTOCOL               0x0001
#define RULE_OPT_SRCIP                  0x0002
#define RULE_OPT_DESTIP                 0x0004
#define RULE_OPT_SRCL4PORT              0x0008
#define RULE_OPT_DESTL4PORT             0x0010
#define RULE_OPT_PACKETLEN          	0x0020

// rule2 options
#define RULE_OPT_OFFSET             	0x0001
#define RULE_OPT_KEY                	0x0002


// defines used by database and drvops
#define FILTER_NO_DEF                   0x0

#define ENABLED             				0x1
#define DISABLED            				0x0

// forware related
#define FORWARD_OPT_VLAN            	0x1
#define FORWARD_OPT_SAMP                0x2
#define FORWARD_OPT_DROP                0x4
#define FORWARD_DEF                     0x0     //no drop, no vlan, no sample

// packet length range
#define PKTLEN_MIN				0
//#define PKTLEN_MAX				1500
#define PKTLEN_MAX				0

// sample rate
#define SAMPLE_RATE				100

// tcp flow mms
#define	TCP_MMS				1460


#define OC_LINERATE_192C            	0x0
#define OC_LINERATE_48C             	0x1
#define OC_LINERATE_DEF             	OC_LINERATE_192C
#define OC_LINE_SONNET                  0x1
#define OC_LINE_SDH                     0x0
#define OC_ENCAP_PPP                    0x1
#define OC_ENCAP_HDLC                   0x0
#define OC_CRC_CCITT                    0x0
#define OC_CRC_32                       0x1
#define OC_CRC_NONE                     0x2

#define PROMPT1(str)                	"     " str
#define PROMPT2(prompt,format)      	"     %-40s" format, prompt


/***************************************************************************************/
/* system functions */
/***************************************************************************************/
extern void cli_db_lock(struct parser_params *ppar);
extern void cli_db_unlock(struct parser_params *ppar);
extern int cli_init_config(struct parser_params *ppar);
extern int cli_reset_default_config(struct parser_params *ppar);
extern int cli_show_config(struct parser_params *ppar);
extern int cli_show_config_file();
extern int cli_save_config_file(struct parser_params *ppar, char *name);
extern int cli_save_config(struct parser_params *ppar, FILE *fp);
extern int cli_update_firmware(int oc_no);
extern int cli_etxconfig();
extern int cli_show_system();


/***************************************************************************************/
/* filter/forward/transfer functions */
/***************************************************************************************/
extern int cli_fpga_set_ruleset1(struct parser_params *ppar, int oc_no, rule1options_t *opts);
extern int cli_fpga_set_ruleset2(struct parser_params *ppar,  int oc_no, rule2options_t *opts);
extern int cli_set_pktlen_range(struct parser_params *ppar, int oc_no, int range_no, int pktlen_min, int pktlen_max);
extern int cli_get_pktlen_range(struct parser_params *ppar, int oc_no, int range_no);
extern int cli_fpga_clear_ruleset1(struct parser_params *ppar,  int oc_no, int ruleno);
extern int cli_fpga_clear_ruleset2(struct parser_params *ppar,  int oc_no, int ruleno);
extern int cli_get_filter_ruleset1(struct parser_params *ppar,  int oc_no, unsigned long rule_id);
extern int cli_get_filter_ruleset2(struct parser_params *ppar,  int oc_no, unsigned long rule_id);
extern int cli_set_forward_mode(struct parser_params *ppar,  int oc_no, unsigned long ruleno1, unsigned long ruleno2, unsigned long forward_mode);
extern int cli_get_forward_mode(struct parser_params *ppar,  int oc_no, unsigned long ruleno1, unsigned long ruleno2);
extern int cli_set_sample_rate(struct parser_params *ppar, int oc_no, int sample_rate);
extern int cli_get_sample_rate(struct parser_params *ppar, int oc_no);
extern int cli_set_eth_mac(struct parser_params *ppar, int oc_no, unsigned int src_dst, unsigned char mac[6]);
extern int cli_get_eth_mac(struct parser_params *ppar, int oc_no);

extern int cli_set_gtp_dest_ports(struct parser_params *ppar, int oc_no, int gtpdestports);
extern int cli_set_gtp_hash_mode(struct parser_params *ppar, int oc_no, int out_both);
extern int cli_get_gtp_settings(struct parser_params *ppar, int oc_no);

/***************************************************************************************/
/* switch functions */
/***************************************************************************************/
extern int cli_bcm_cmds(struct parser_params *ppar, unsigned char *cmds);
extern int cli_bcm_cmds_v2(struct parser_params *ppar, bcm_cmds_t *opts);


/***************************************************************************************/
/* framer functions */
/***************************************************************************************/
extern int cli_set_oc_line_rate(struct parser_params *ppar, int oc_no, unsigned long line_rate);
extern int cli_set_oc_line_mode(struct parser_params *ppar, int oc_no, unsigned long line_mode);
extern int cli_set_oc_encap_mode(struct parser_params *ppar, int oc_no, unsigned long encap_mode);
extern int cli_set_oc_sonnet_scramble_enable(struct parser_params *ppar, int oc_no, unsigned long enable_or_disable);
extern int cli_set_oc_payload_scramble_enable(struct parser_params *ppar, int oc_no, unsigned long enable_or_disable);
extern int cli_set_oc_crc_mode(struct parser_params *ppar, int oc_no, unsigned long crc_mode);
extern int cli_get_oc_settings(struct parser_params *ppar);
extern int cli_palermo_get_stats(int oc_no);
extern int cli_get_framer_stats(int oc_no);
extern int cli_get_framer_running_cfg(int oc_no);
extern int cli_save_oc_settings(struct parser_params *ppar);


/***************************************************************************************/
/* FPGA functions */
/***************************************************************************************/
extern int cli_fpga_reg_read(int oc_no, unsigned long reg_no);
extern int cli_fpga_reg_write(int oc_no, unsigned long reg_no, unsigned long reg_data);
extern int cli_get_fpga_stats(int oc_no);
extern int cli_get_packet_stats();
extern int cli_fpga_set_flow_enable(struct parser_params *ppar, int oc_no, unsigned long enable_or_disable);
extern int cli_fpga_get_flow_enable(struct parser_params *ppar, int oc_no);
extern int cli_fpga_set_flow_mms(struct parser_params *ppar, int oc_no, unsigned long mms_size);
extern int cli_fpga_get_flow_mms(struct parser_params *ppar, int oc_no);
extern int cli_fpga_set_traffic_enable(struct parser_params *ppar, int oc_no, unsigned long enable_or_disable);
extern int cli_fpga_get_traffic_enable(struct parser_params *ppar, int oc_no);

/***************************************************************************************/
/* BYPASS functions */
/***************************************************************************************/
extern int cli_bypass_get(struct parser_params *ppar);
extern int cli_bypass_set(struct parser_params *ppar, int pbmp, unsigned long enable_or_disable);

#endif

