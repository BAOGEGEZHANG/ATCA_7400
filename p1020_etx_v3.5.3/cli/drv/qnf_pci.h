/*
 * Copyright (c) 2010-2015 Qice Technology Ltd, Beijing, China.
 * All rights reserved.
 *
 * This source code is proprietary to Qice Technology Limited and no part
 * of it may be redistributed, published or disclosed except as outlined in
 * the written contract supplied with this product.
 *
 *******************************************************************************
 * File Name  		: 	qnfpci.h
 * Description		:	  header file for pci id vendor
 *******************************************************************************/

#ifndef __QNFPCI_H__
#define __QNFPCI_H__


#define PCI_VENDOR_ID_QICE			0xeaea
#define PCI_DEVICE_ID_MC10P       	0x0003
#define PCI_DEVICE_ID_MC10P_FLOW  	0x0004
#define PCI_DEVICE_ID_MC20P  		0x1003    //fully support
#define PCI_DEVICE_ID_MC20P_GTP	0x1004
#define PCI_DEVICE_ID_MC20P_LAN	0x1005
#define PCI_DEVICE_ID_MC20E		0x2003    // fully support
#define PCI_DEVICE_ID_MC20E_PPOE	0x2004    // fully support
#define PCI_DEVICE_ID_MC4P25		0x3003	// fully support
#define PCI_DEVICE_ID_MC4P25_VLAN	0x3004	// fully support


#ifdef __linux__
# ifdef __KERNEL__
struct pci_device_id qnf_pci_tbl[] __devinitdata = {
    { PCI_DEVICE(PCI_VENDOR_ID_QICE,    PCI_DEVICE_ID_MC10P) },
    { PCI_DEVICE(PCI_VENDOR_ID_QICE,    PCI_DEVICE_ID_MC10P_FLOW) },
    { PCI_DEVICE(PCI_VENDOR_ID_QICE,    PCI_DEVICE_ID_MC20P) },
    { PCI_DEVICE(PCI_VENDOR_ID_QICE,    PCI_DEVICE_ID_MC20P_GTP) },
    { PCI_DEVICE(PCI_VENDOR_ID_QICE,    PCI_DEVICE_ID_MC20P_LAN) },
    { PCI_DEVICE(PCI_VENDOR_ID_QICE,    PCI_DEVICE_ID_MC20E) },
    { PCI_DEVICE(PCI_VENDOR_ID_QICE,    PCI_DEVICE_ID_MC20E_PPOE) },
    { PCI_DEVICE(PCI_VENDOR_ID_QICE,    PCI_DEVICE_ID_MC4P25) },
    { PCI_DEVICE(PCI_VENDOR_ID_QICE,    PCI_DEVICE_ID_MC4P25_VLAN) },
    {0,}
};
# endif /* __KERNEL__ */
#endif /* __linux__ */


#endif /* __QNFPCI_H__ */
