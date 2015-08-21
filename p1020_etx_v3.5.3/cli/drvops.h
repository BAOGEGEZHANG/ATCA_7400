/*
 *                          Copyright QiCe Tech, 2010-2012
 *
 *                          ALL RIGHTS RESERVED
 *
 * Description:      
 *
 */

#ifndef __DRVOPS_H__
#define __DRVOPS_H__

#include "database.h"

// functions
int drv_update_firmware(int oc_no);

int drv_reset_forward_default_config(int oc_no);
int drv_reset_global_default_config(int oc_no);

int drv_set_ruleset1(int oc_no, rule1options_t *opts);
int drv_set_ruleset2(int oc_no, rule2options_t *opts);

int drv_set_forward_mode(int oc_no, unsigned int fwd_mode);

int drv_set_gtp_dest_ports(int oc_no, int gtpdestports);

int drv_set_gtp_hash_mode(int oc_no, int out_both);

int drv_set_pktlen_range(int oc_no, int range_no, int pktlen_min, int pktlen_max);

int drv_set_sample_rate(int oc_no, int sample_rate);

int drv_set_eth_mac(int oc_no, int src_dst, unsigned char mac[6]);

int drv_fpga_reg_read(int oc_no, unsigned int reg_no, unsigned int *reg_data);

int drv_fpga_reg_write(int oc_no, unsigned int reg_no, unsigned int reg_data);

int drv_get_fpga_stats(int oc_no);
int drv_get_packet_stats(); // for MC20E

int drv_get_framer_running_cfg(int oc_no);
	
int drv_get_framer_stats(int oc_no);

int drv_get_palermo_stats(int oc_no);

int drv_set_flow_enable(int oc_no, int enable_or_disable);

int drv_set_flow_mms(int oc_no, int mms_size);

int drv_set_traffic_enable(int oc_no, int enable_or_disable);


int drv_set_bcm_cmds(CONSRV *pcs, unsigned char *cmds);


int drv_bypass_get();
int drv_bypass_set(CONSRV *pcs, int pbmp, int enable_or_disable);

#endif /* _DRVOPS_H__ */

