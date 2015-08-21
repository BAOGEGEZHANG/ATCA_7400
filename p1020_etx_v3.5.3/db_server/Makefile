#
#    General Makefile
#

OBJECTS := db_server.o

.PHONY: all db_server clean

all: $(TOPDIR)/utils/objs.o $(TOPDIR)/db_client/objs.o db_server

db_server: $(OBJECTS) $(TOPDIR)/utils/objs.o $(TOPDIR)/db_client/objs.o
	$(CC) $^ -pthread -o $@
	
%.o : %.c
	$(CC) $(TARGET_APP_FLAGS) -I. -c $< -o $@	

clean:
	$(RM) $(OBJECTS)
	$(RM) db_server

