CC = g++
CFLAGS += -Wall -fPIC
LFLAGS += -shared -Wl,-soname,libmathexpr.so.1
all : libmathexpr.so.1.0

libmathexpr.so.1.0 : expression.o myexceptions.o
	$(CC) $(LFLAGS) $^ -o $@ && mv $@ ../lib/

myexceptions.o expression.o : %.o : %.cpp
	$(CC) $(CFLAGS) -c $<

clean :
	rm -rf *.o

test :
	$(CC) -Wall -I. -L/opt/lib main.cpp -lmathexpr -o test && mv test ../
