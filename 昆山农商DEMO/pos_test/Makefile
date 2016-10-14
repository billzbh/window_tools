CC = arm-linux-gcc 
CFLAGS = -Wall -Os
LDFLAGS = -lKdmAPI

all: test
test: test.c
	$(CC) $<  $(CFLAGS)  -L ./ $(LDFLAGS)  -o $@
clean:
	rm test test.o
	
	
#gcc test.c  -L ./ -lunionKdmAPI -o test
#cp libunionKdmAPI.so /usr/lib
#./test

#arm-linux-gcc4.3.2
