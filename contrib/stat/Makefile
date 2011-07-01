OBJS = stat_client_cron.o  ../../stat.o ../../mapping.o ../../tunif.o
OBJS2 = stat_client.o

CFLAGS = -Wall -g -DDEBUG
LIBS = -ljson
INC = -I../../include

stat_client: $(OBJS2)
	g++ $(CFLAGS) -o $@ $(OBJS2) $(LIBS) $(INC)

.cpp.o:
	g++ -c $(CFLAGS) $< $(INC)

clean:
	rm -f *.o stat_client_cron stat_client *~
