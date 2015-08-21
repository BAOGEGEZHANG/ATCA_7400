#/***********************************************************************/
#/* This file contains unpublished documentation and software           */
#/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
#/* in whole or in part, of the information in this file without a      */
#/* written consent of an officer of Cortina Systems Incorporated is    */
#/* strictly prohibited.                                                */
#/* Copyright (c) 2004 by Cortina Systems Incorporated.                 */
#/***********************************************************************/
#
# Makefile for building pprbs.o
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
HDRS            =    cs_types.h          \
                     cs_rtos.h           \
		     arn5_pprbs.h         \
		     arn5_pprbs_priv.h    \
		     arn5_pprbs_irq.h     \
		     arn5_pprbs_stats.h

INC            +=    -I.

# put all the source files here (without dir part)
SRCS            =    arn5_pprbs.c         \
		     arn5_pprbs_irq.c     \
		     arn5_pprbs_stats.c

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





