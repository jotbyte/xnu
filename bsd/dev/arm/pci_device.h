/*
 * Copyright (c) 2000-2007 Apple Inc. All rights reserved.
 */
/*
 * @OSF_FREE_COPYRIGHT@
 *
 */
/*
 * HISTORY
 *
 * Revision 1.2  1998/09/30 21:20:44  wsanchez
 * Merged in IntelMerge1 (mburg: Intel support)
 *
 * Revision 1.1.2.1  1998/09/30 18:18:50  mburg
 * Changes for Intel port
 *
 * Revision 1.1.1.1  1998/03/07 02:25:45  wsanchez
 * Import of OSF Mach kernel (~mburg)
 *
 * Revision 1.1.6.2  1995/12/15  10:52:14  bernadat
 *      Split dev and vendor ids.
 *      [95/11/15            bernadat]
 *
 * Revision 1.1.6.1  1995/02/23  17:22:27  alanl
 *      Taken from DIPC2_SHARED
 *      [1995/01/03  19:09:31  alanl]
 *
 * Revision 1.1.2.1  1994/10/11  18:24:42  rwd
 *      Created.
 *      [1994/10/11  18:15:31  rwd]
 *
 * $EndLog$
 */
/*
 * Taken from
 *
 *  Copyright (c) 1994	Wolfgang Stanglmeier, Koeln, Germany
 *			<wolf@dentaro.GUN.de>
 */

#ifndef __PCI_DEVICE_H__
#define __PCI_DEVICE_H__

/*------------------------------------------------------------
 *
 *  Per driver structure.
 *
 *------------------------------------------------------------
 */

typedef unsigned short pci_vendor_id_t;
typedef unsigned short pci_dev_id_t;

typedef union {
	unsigned long cfg1;
	struct {
		unsigned char   enable;
		unsigned char   forward;
		unsigned short  port;
	} cfg2;
} pcici_t;

struct pci_driver {
	int                 (*probe )(pcici_t pci_ident);/* test whether device
	                                                  *  is present */
	int                 (*attach)(pcici_t pci_ident);/* setup driver for a
	                                                  *  device */
	pci_vendor_id_t     vendor_id;                  /* vendor pci id */
	pci_dev_id_t        device_id;                  /* device pci id */
	char                *name;                      /* device name */
	char                *vendor;                    /* device long name */
	void                (*intr)(int);               /* interupt handler */
};

/*-----------------------------------------------------------
 *
 *  Per device structure.
 *
 *  It is initialized by the config utility and should live in
 *  "ioconf.c". At the moment there is only one field.
 *
 *  This is a first attempt to include the pci bus to 386bsd.
 *  So this structure may grow ..
 *
 *-----------------------------------------------------------
 */

struct pci_device {
	struct pci_driver * pd_driver;
};

/*-----------------------------------------------------------
 *
 *  This functions may be used by drivers to map devices
 *  to virtual and physical addresses. The va and pa
 *  addresses are "in/out" parameters. If they are 0
 *  on entry, the mapping function assigns an address.
 *
 *-----------------------------------------------------------
 */

int pci_map_mem(pcici_t tag,
    unsigned long entry,
    vm_offset_t *va,
    vm_offset_t *pa);
#endif /*__PCI_DEVICE_H__*/