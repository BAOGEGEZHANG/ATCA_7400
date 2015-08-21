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
#define REG_BASE(X)            (((X == 0x0003)||(X == 0x1003)||(X == 0x2003)) ? 0x100000 : 0x000000)
#define FRAMER_BASE(X)         (((X == 0x0003)||(X == 0x1003)||(X == 0x2003)) ? 0x180000 : 0x200000)
#define MAC_BASE(X)            (((X == 0x0003)||(X == 0x1003)||(X == 0x2003)) ? 0x140000 : 0x100000)

// MAC internal registers
#define MAC_CFGREG_OFFSET(X)   (MAC_BASE(X) + 0x000)
#define MAC_STATSREG_OFFSET(X) (MAC_BASE(X) + 0x800)

// common
#define CAM0_DATA(X)           (REG_BASE(X) + 0x00)
#define CAM1_DATA(X)           (REG_BASE(X) + 0x04)
#define CAM2_DATA(X)           (REG_BASE(X) + 0x08)
#define CAM3_DATA(X)           (REG_BASE(X) + 0x0C)
#define CAM4_DATA(X)           (REG_BASE(X) + 0x10)
#define CAM0_MASK(X)           (REG_BASE(X) + 0x14)
#define CAM1_MASK(X)           (REG_BASE(X) + 0x18)
#define CAM2_MASK(X)           (REG_BASE(X) + 0x1C)
#define CAM3_MASK(X)           (REG_BASE(X) + 0x20)
#define CAM4_MASK(X)           (REG_BASE(X) + 0x24)
#define CAM_ADDR(X)            (REG_BASE(X) + 0x28)

#define CAPTURE_CTL_REG(X)	   (((X == 0x2003)) ? (REG_BASE(X) + 0x30) : (REG_BASE(X) + 0x28))	      
#define FLASH_DATA(X)          (REG_BASE(X) + 0xE0)
#define FLASH_ADDR(X)          (REG_BASE(X) + 0xE4)
// 240 << 2  -> 900
#define MAC_RX_CFG(X)				   (MAC_BASE(X) + 0x900)	
#define MAC_TX_CFG(X)				   (MAC_BASE(X) + 0xa00)

// MC20E
#define PKTLEN_ADDR_REG(X)     (REG_BASE(X) + 0x44)
#define PKTLEN_DATA_REG(X)     (REG_BASE(X) + 0x48)
#define TAG_MAP_REG(X)         (REG_BASE(X) + 0x50)
#define PKT_IN_LO_0(X)         (REG_BASE(X) + 0x80)
#define PKT_IN_HI_0(X)         (REG_BASE(X) + 0x84)
#define PKT_OUT_LO_0(X)        (REG_BASE(X) + 0x88)
#define PKT_OUT_HI_0(X)        (REG_BASE(X) + 0x8C)
#define PKT_IN_LO_1(X)         (REG_BASE(X) + 0x90)
#define PKT_IN_HI_1(X)         (REG_BASE(X) + 0x94)
#define PKT_OUT_LO_1(X)        (REG_BASE(X) + 0x98)
#define PKT_OUT_HI_1(X)        (REG_BASE(X) + 0x9C)
#define I2C_BASEADDR0(X)       (REG_BASE(X) + 0xE8)
#define I2C_BASEADDR1(X)       (REG_BASE(X) + 0xEC)

// MC20P_GTP
#define GTP_DEST_PORTS(X)      (REG_BASE(X) + 0x20)
#define GTP_HASH_MODE(X)       (REG_BASE(X) + 0x24)


#endif  /* __QNFREG_H__ */

