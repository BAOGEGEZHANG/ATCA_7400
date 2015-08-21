#
#    General Makefile
#

OBJECTS := cs_server.o

.PHONY: all cs_server clean

all: $(TOPDIR)/utils/objs.o cs_server

cs_server: $(OBJECTS) $(TOPDIR)/utils/objs.o
	$(CC) $^ -lutil -pthread -o $@
	
%.o : %.c
	$(CC) $(TARGET_APP_FLAGS) -I. -c $< -o $@

clean:
	$(RM) $(OBJECTS)
	$(RM) cs_server

