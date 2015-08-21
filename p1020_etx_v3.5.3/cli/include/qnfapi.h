/*
 * Copyright (c) 2010-2015 Qice Technology Ltd, Beijing, China.
 * All rights reserved.
 *
 * This source code is proprietary to Qice Technology Limited and no part
 * of it may be redistributed, published or disclosed except as outlined in
 * the written contract supplied with this product.
 *
 *******************************************************************************
 *  qnfapi.c: this file defines the api functions
 *******************************************************************************/

#ifndef __QNFAPI_H__
#define __QNFAPI_H__

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <ctype.h>
#include <errno.h>
#include <regex.h>

#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <inttypes.h>

#include "qnftypes.h"
#include "qnfreg.h"

// defines for flash update
#define BANK_SIZE_W          (512 * 1024)
#define BLOCK_SIZE_W         (64 * 1024)
#define BLOCK_SIZE_B         (128 * 1024)


// reg read/write
__inline__ unsigned int reg_read(int qnffd, unsigned int offset );
__inline__ unsigned int reg_write(int qnffd, unsigned int offset, unsigned int value );

// open/close function
int qnf_open(char * qnfname);
int qnf_close(int qnffd);


// framer mc10p related
#ifdef BUILD_MC10P_FLOW
__inline__ unsigned int framer_read(int qnffd, unsigned int offset, unsigned short *value );
__inline__ unsigned int framer_write(int qnffd, unsigned int offset, unsigned short value);
int qnf_framer_mc10p_init_1(confs_t *cfg);
int qnf_framer_mc10p_init_2(int qnffd, int device, confs_t *cfg);
int qnf_running_config(int device);
int qnf_stats(int device);
unsigned long long qnf_framer_stats(int device);
#endif

#ifdef BUILD_MC20P
__inline__ unsigned int framer_reg_read(int framerfd, unsigned int offset);
__inline__ unsigned int framer_reg_write(int framerfd, unsigned int offset, unsigned int value);
int framer_open(char * framername);
int framer_close(int framerfd);
int framer_reg_dump(int framerfd);
int capture_start(int qnffd, int mode);
int capture_stop(int qnffd, int mode);
#endif

#ifdef BUILD_MC20P_GTP
__inline__ unsigned int framer_reg_read(int framerfd, unsigned int offset);
__inline__ unsigned int framer_reg_write(int framerfd, unsigned int offset, unsigned int value);
int framer_open(char * framername);
int framer_close(int framerfd);
int framer_reg_dump(int framerfd);
int capture_start(int qnffd, int mode);
int capture_stop(int qnffd, int mode);
#endif

#ifdef BUILD_MC20P_MC4P25
__inline__ unsigned int framer_reg_read(int framerfd, unsigned int offset);
__inline__ unsigned int framer_reg_write(int framerfd, unsigned int offset, unsigned int value);
int framer_reg_dump(int framerfd);
int framer_open(char * framername);
int framer_close(int framerfd);
int capture_start(int qnffd, int mode);
int capture_stop(int qnffd, int mode);
__inline__ unsigned int framer_read(int qnffd, unsigned int offset, unsigned short *value );
__inline__ unsigned int framer_write(int qnffd, unsigned int offset, unsigned short value);
int qnf_framer_init(int qnffd);
int capture_start_mc4p25(int qnffd, int mode);
int capture_stop_mc4p25(int qnffd, int mode);
#endif

#if defined(BUILD_MC20E) || defined(BUILD_MC20E_PPOE)
int capture_start(int qnffd, int mode);
int capture_stop(int qnffd, int mode);
int qnf_cam_tuple_init(int qnffd);
int qnf_cam_tuple_set(int qnffd,unsigned int entry,qnf_rule_tuple_t *rule);
int qnf_cam_tuple_clear(int qnffd, unsigned entry);
int qnf_cam_payload_init(int qnffd);
int qnf_cam_payload_set(int qnffd,unsigned int entry,qnf_rule_payload_t *rule);
int qnf_cam_payload_clear(int qnffd, unsigned int entry);
int qnf_cam_lut_init(int qnffd);
int qnf_cam_lut_set(int qnffd, int rule1_index, int rule2_index, qnf_action_t *action);
int qnf_cam_lut_clear(int qnffd, int rule1_index, int rule2_index);
int qnf_set_pktlen_range(int qnffd, int index, int range_lo, int range_hi);
#endif

// flash update apis
void set_logic_num(int qnffd,uint64_t sn_logic);
void set_hardware_num(int qnffd,uint64_t no);
uint64_t read_logic_num(int qnffd);
uint64_t read_hardware_num(int qnffd);
uint16_t bit_swap(uint16_t value);
void flash_write(int qnffd, uint32_t addr, uint16_t data);
uint16_t flash_read(int qnffd, uint32_t addr);
void reconfig(int qnffd);
void set_config_register(int qnffd, uint16_t data);
void clear_stats_regs(int qnffd);
void block_unlock(int qnffd, uint8_t bank, uint8_t block);
void bank_unlock(int qnffd, uint8_t bank);
void block_erase(int qnffd,uint8_t bank, uint8_t block);
void bank_erase(int qnffd, uint8_t bank);
void word_program(int qnffd,uint32_t addr,uint16_t data);
void buffer_program(int qnffd,uint32_t start_addr,int n,uint16_t* pdata);
void block_program(int qnffd, uint8_t bank, uint8_t block, uint32_t w_size, uint16_t* pdata);
void bank_program(int qnffd, uint8_t bank, uint32_t w_size, uint16_t* pdata);
void blank_check_command(int qnffd,uint32_t blockToCheck);
void locking_operation_command(int qnffd,uint32_t addr,lock_stat_t lock_operation);
void protection_register_program_command(int qnffd,uint32_t addr,uint16_t data);
void buffer_enhanced_factory_program_command(int qnffd,uint32_t start_addr , uint32_t count,uint16_t data_flow[]);
int  compare_file_order(char * path1,char *path2);


#endif /* __QNFAPI_H__ */
