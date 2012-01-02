CC = g++
CFLAGS += -Wall -fPIC
LFLAGS += -shared -Wl,-soname,liboptions.so.1
all : liboptions.so.1.0

liboptions.so.1.0 : common.o
	$(CC) $(LFLAGS) $^ -o $@ && mv $@ ../lib/

common.o : %.o : %.cpp
	$(CC) $(CFLAGS) -c $<

clean :
	rm -rf *.o

test :
	$(CC) -Wall -I. -L/opt/lib main.cpp -loptions -o test && mv test ../
