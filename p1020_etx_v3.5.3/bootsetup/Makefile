#
#    General Makefile
#

OBJECTS := bootsetup.o

.PHONY: all bootsetup clean

all: bootsetup

bootsetup: $(OBJECTS) $(TOPDIR)/cli/libutils.a
	$(CC) $(OBJECTS) -L$(TOPDIR)/cli -lutils -o $@
	
%.o : %.c
	$(CC) $(TARGET_APP_FLAGS) -I. -c $< -o $@

clean:
	$(RM) $(OBJECTS)
	$(RM) bootsetup

