

EXENAME:=read_mcp3208
EXENAMEII:=hw_$(EXENAME)
EXENAMEIII:=dev_$(EXENAME)
EXENAMEIV:=broad_$(EXENAME)
CFLAGS:=$(CFLAGS) -W -Wall -Wextra -pedantic -std=gnu99
LIBS:=$(LIBS) -lwiringPi -lrt
BCMLIBS:=$(BCMLIBS) -lbcm2835 -lrt

all : $(EXENAME) $(EXENAMEII) $(EXENAMEIII) $(EXENAMEIV)
	
$(EXENAME): readmcp3208.o mcp3208.o delay.o bitwriter.o
	$(CC) -o $(EXENAME) readmcp3208.o\
						mcp3208.o	\
						delay.o		\
						bitwriter.o	\
						$(LIBS)

$(EXENAMEII): hw_readmcp3208.o mcp3208.o delay.o bitwriter.o
	$(CC) -o $(EXENAMEII) hw_readmcp3208.o mcp3208.o\
					      delay.o bitwriter.o		\
						  $(LIBS)

$(EXENAMEIII): dev_readmcp3208.o pispi.o delay.o bitwriter.o
	$(CC) -o $(EXENAMEIII) dev_readmcp3208.o\
					      delay.o\
						  bitwriter.o\
						  pispi.o\
						  $(LIBS)

$(EXENAMEIV): broad_readmcp3208.o broadmcp3208.o broadmcp3208.h bitwriter.o  delay.o utils.o
	$(CC) -o $(EXENAMEIV)\
		     broad_readmcp3208.o\
			 broadmcp3208.o\
			 delay.o\
			 bitwriter.o\
			 utils.o\
			 $(BCMLIBS)

readmcp3208.o : readmcp3208.c
	$(CC) $(CFLAGS) -c readmcp3208.c

hw_readmcp3208.o : hw_readmcp3208.c
	$(CC) $(CFLAGS) -c hw_readmcp3208.c

$(EXENAMEIII).o: pispi.c pispi.h
	$(CC) $(CFLAGS) -c pispi.c

mcp3208.o : mcp3208.c mcp3208.h
	$(CC) $(CFLAGS) -c mcp3208.c

broadmcp3208.o : broadmcp3208.c broadmcp3208.h bitwriter.c utils.h

bitwriter.o : bitwriter.c bitwriter.h
	$(CC) $(CFLAGS) -c bitwriter.c

delay.o:delay.c delay.h
	$(CC) $(CFLAGS) -c delay.c

utils.o : utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

clean:
	rm *.o
