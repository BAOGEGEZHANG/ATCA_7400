/*
 * Copyright (c) 2010-2015 Qice Technology Ltd, Beijing, China.
 * All rights reserved.
 *
 * This source code is proprietary to Qice Technology Limited and no part
 * of it may be redistributed, published or disclosed except as outlined in
 * the written contract supplied with this product.
 *
 *******************************************************************************
 *  qnfreg.h: this file defines the internal registers
 *******************************************************************************/

#ifndef __QNFREG_H__
#define __QNFREG_H__


// base address
#define FRAMER_BASE              	(0x180000)

// MAC internal registers
#define MAC_BASE                    	(0x140000)
#define MAC_CFGREG_OFFSET    	(MAC_BASE + 0x000)
#define MAC_STATSREG_OFFSET 	(MAC_BASE + 0x800)

// FPGA internal registers
#define REG_BASE                   		(0x100000)
#define CAM0_DATA               		(REG_BASE + 0x00)
#define CAM1_DATA               		(REG_BASE + 0x04)
#define CAM2_DATA               		(REG_BASE + 0x08)
#define CAM3_DATA               		(REG_BASE + 0x0C)
#define CAM4_DATA            		(REG_BASE + 0x10)
#define CAM0_MASK               		(REG_BASE + 0x14)
#define CAM1_MASK               		(REG_BASE + 0x18)
#define CAM2_MASK               		(REG_BASE + 0x1C)
#define CAM3_MASK               		(REG_BASE + 0x20)
#define CAM4_MASK            		(REG_BASE + 0x24)
#define CAM_ADDR                 		(REG_BASE + 0x28)

#ifdef BUILD_MC10P_FLOW
#define TAG_MAP_REG          		(REG_BASE + 0x2C)
#define LEN_THRES0           		(REG_BASE + 0x30)
#define LEN_THRES1           		(REG_BASE + 0x34)
#define LEN_THRES2           		(REG_BASE + 0x38)
#define LEN_THRES3           		(REG_BASE + 0x3C)
#define LEN_THRES4           		(REG_BASE + 0x40)
#define LEN_THRES5           		(REG_BASE + 0x44)
#define LEN_THRES6           		(REG_BASE + 0x48)
#define LEN_THRES7           		(REG_BASE + 0x4C)
#define FLOW_ENABLE_REG      	(REG_BASE + 0x50)
#define CAPTURE_START_REG        	(REG_BASE + 0x54)
#define TCP_MMS_REG             		(REG_BASE + 0x58)
#define SAMPLE_RATE_REG         	(REG_BASE + 0x5C)
#define MAC_ADDR0                 		(REG_BASE + 0x60)
#define MAC_ADDR1                 		(REG_BASE + 0x64)
#define MAC_ADDR2                 		(REG_BASE + 0x68)
#define MAC_ADDR_UPDATE     	(REG_BASE + 0x6C)
#define I2C_BASEADDR            		(REG_BASE + 0x70)
#define FLASH_DATA                	(REG_BASE + 0x74)
#define FLASH_ADDR                	(REG_BASE + 0x78)
// YOU SHOULD READ LOW STATS FIRST, THEN HIGH
#define FPGA_STATS0_IN_LO    	(REG_BASE + 0x80)
#define FPGA_STATS0_IN_HI    	(REG_BASE + 0x84)
#define FPGA_STATS1_CLS_LO    	(REG_BASE + 0x88)
#define FPGA_STATS1_CLS_HI    	(REG_BASE + 0x8C)
#define FPGA_STATS2_SCH_LO    	(REG_BASE + 0x90)
#define FPGA_STATS2_SCH_HI    	(REG_BASE + 0x94)
#define FPGA_STATS3_FT_LO    	(REG_BASE + 0x98)
#define FPGA_STATS3_FT_HI    	(REG_BASE + 0x9C)
#define FPGA_STATS4_FC_LO    	(REG_BASE + 0xa0)
#define FPGA_STATS4_FC_HI    	(REG_BASE + 0xa4)
#define FPGA_STATS5_DROP    	(REG_BASE + 0xa8)
#define BUILD_DATE          		(REG_BASE + 0xf8)
#endif

#if defined(BUILD_MC20E) || defined(BUILD_MC20E_PPOE)
#define CAPTURE_CTL_REG		(REG_BASE + 0x30)
#define PKTLEN_ADDR_REG		(REG_BASE + 0x44)
#define PKTLEN_DATA_REG		(REG_BASE + 0x48)
#define TAG_MAP_REG 			(REG_BASE + 0x50)
#define FPGA_STATS0_LO    		(REG_BASE + 0x80)
#define FPGA_STATS0_HI    		(REG_BASE + 0x84)
#define FPGA_STATS1_LO    		(REG_BASE + 0x88)
#define FPGA_STATS1_HI    		(REG_BASE + 0x8C)
#define FPGA_STATS2_LO    		(REG_BASE + 0x90)
#define FPGA_STATS2_HI    		(REG_BASE + 0x94)
#define FPGA_STATS3_LO    		(REG_BASE + 0x98)
#define FPGA_STATS3_HI    		(REG_BASE + 0x9C)
#define FLASH_DATA              		(REG_BASE + 0xE0)
#define FLASH_ADDR              		(REG_BASE + 0xE4)
#define I2C_BASEADDR0           	(REG_BASE + 0xE8)
#define I2C_BASEADDR1           	(REG_BASE + 0xEC)

#define MAC_STATS_RX_FRAMES_0	       (MAC_BASE + 0x2000)
#define MAC_STATS_RX_BYTES64_0	(MAC_BASE + 0x2004)
#define MAC_STATS_RX_BYTES65_0	(MAC_BASE + 0x2005)
#define MAC_STATS_RX_BYTES128_0	(MAC_BASE + 0x2006)
#define MAC_STATS_RX_BYTES256_0	(MAC_BASE + 0x2007)
#define MAC_STATS_RX_BYTES512_0	(MAC_BASE + 0x2008)
#define MAC_STATS_RX_BYTES1024_0	(MAC_BASE + 0x2009)
#define MAC_STATS_RX_FRAMES_1		(MAC_BASE + 0x3000)
#define MAC_STATS_RX_BYTES64_1	(MAC_BASE + 0x3004)
#define MAC_STATS_RX_BYTES65_1	(MAC_BASE + 0x3005)
#define MAC_STATS_RX_BYTES128_1	(MAC_BASE + 0x3006)
#define MAC_STATS_RX_BYTES256_1	(MAC_BASE + 0x3007)
#define MAC_STATS_RX_BYTES512_1	(MAC_BASE + 0x3008)
#define MAC_STATS_RX_BYTES1024_1	(MAC_BASE + 0x3009)
#endif

#ifdef BUILD_MC20P_GTP
#define GTP_DEST_PORTS			(REG_BASE + 0x20)
#define GTP_HASH_MODE			(REG_BASE + 0x24)
#define CAPTURE_CTL_REG		(REG_BASE + 0x28)
#define FLASH_DATA              		(REG_BASE + 0xE0)
#define FLASH_ADDR              		(REG_BASE + 0xE4)
// 240 << 2  -> 900
#define MAC_RX_CFG				(MAC_BASE + 0x900)	
#define MAC_TX_CFG				(MAC_BASE + 0xa00)
#endif

#ifdef BUILD_MC20P
#define CAPTURE_CTL_REG		(REG_BASE + 0x28)
#define FLASH_DATA              		(REG_BASE + 0xE0)
#define FLASH_ADDR              		(REG_BASE + 0xE4)
#endif

#ifdef BUILD_MC20P_MC4P25
#define CAPTURE_CTL_REG			(REG_BASE + 0x28)
#define FLASH_DATA              			(REG_BASE + 0xE0)
#define FLASH_ADDR              			(REG_BASE + 0xE4)
#define REG_BASE_MC4P25			(0x00000)
#define FRAMER_BASE_MC4P25        	(0x200000)
#define CAPTURE_CTL_REG_MC4P25	(REG_BASE_MC4P25 + 0x28)
#define FLASH_DATA_MC4P25         	(REG_BASE_MC4P25 + 0xE0)
#define FLASH_ADDR_MC4P25         	(REG_BASE_MC4P25 + 0xE4)
#define MAC_ADDR0_MC4P25          	(REG_BASE_MC4P25 + 0xF0)
#define MAC_ADDR1_MC4P25          	(REG_BASE_MC4P25 + 0xF4)
#define MAC_ADDR2_MC4P25          	(REG_BASE_MC4P25 + 0xF8)
#define MAC_ADDR_UPDATE_MC4P25    (REG_BASE_MC4P25 + 0xFC)
#endif


#endif  /* __QNFREG_H__ */

