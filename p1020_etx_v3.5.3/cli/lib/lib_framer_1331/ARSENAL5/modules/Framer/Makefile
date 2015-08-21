#/***********************************************************************/
#/* This file contains unpublished documentation and software           */
#/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
#/* in whole or in part, of the information in this file without a      */
#/* written consent of an officer of Cortina Systems Incorporated is    */
#/* strictly prohibited.                                                */
#/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
#/***********************************************************************/
#
# Makefile for building framer.o
#
# This file is released on Wed Nov 23 22:30:10 2005.
#

##############################################################
# The following 3 lines must be defined for each m2r Makefile
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
HDRS            =    arn5_framer.h        \
                     arn5_framer_stats.h  \
                     arn5_public.h        \
                     arn5_common.h        \
                     arn5_irq_priv.h      \
                     arn5_framer_irq.h    \
                     arn5_framer_priv.h   \
                     arn5_framer_atm.h    \
                     arn5_framer_gfp.h    \
                     arn5_framer_pkt.h    \
                     arn5_framer_soh.h    \
                     arn5_framer_alm.h    \
                     cs_types.h          \
                     arn5_error.h

INC            +=    -I.

# put all the source files here (without dir part)
SRCS            =    arn5_framer.c        \
                     arn5_framer_stats.c  \
                     arn5_framer_irq.c    \
                     arn5_framer_atm.c    \
                     arn5_framer_pkt.c    \
                     arn5_framer_soh.c    \
                     arn5_framer_alm.c    \
                     arn5_framer_gfp.c

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

