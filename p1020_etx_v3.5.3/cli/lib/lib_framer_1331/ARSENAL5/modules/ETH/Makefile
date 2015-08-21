#/***********************************************************************/
#/* This file contains unpublished documentation and software           */
#/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
#/* in whole or in part, of the information in this file without a      */
#/* written consent of an officer of Cortina Systems Incorporated is    */
#/* strictly prohibited.                                                */
#/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
#/***********************************************************************/
#
# Makefile for building eth.o
#
# This file is released on Wed Nov 23 22:30:10 2005.
#

##############################################################
# The following 3 lines must be defined for each mdd Makefile
##############################################################
CHIP            =    ARSENAL5
CHIPROOT        =    $(shell cd ../..; pwd)
include $(CHIPROOT)/Makefile.common

##############################################################
# Local makefile options
##############################################################

# put the SRCS searching path here like PATH1:PATH1
VPATH           =   $(ARN5GENERALDIR):$(CSPLATFORMDIR) \
                   :$(CHIPROOT)

# put the dependant header files here
HDRS            =    cs_types.h        \
                     cs_rtos.h         \
		     arn5_eth_api.h     \
		     arn5_eth_data.h    \
		     arn5_eth_irq.h     \
		     arn5_eth_priv.h    \
		     arn5_eth_stats.h

INC            +=    -I.

# put all the source files here (without dir part)
SRCS            =    arn5_eth.c         \
		     arn5_eth_irq.c     \
		     arn5_eth_stats.c

OBJS            =    $(SRCS:%.c=$(BUILDDIR)/%.o)

###########################################
# building target
###########################################
target: platform $(OBJS)

platform:
	$(CSPLATFORMDIR)/makebuilddir $(BUILDDIR)

$(BUILDDIR)/%.o : %.c $(HDRS)
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	$(RM) $(OBJS)


