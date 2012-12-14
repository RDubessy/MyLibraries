all:
	cd src && make all
clean:
	cd src && make clean
	rm -f test
	cd lib && rm -rf *
install:
	cp src/*.h /opt/include
	mv lib/libmathexpr.so.1.0 /opt/lib
	ln -sf /opt/lib/libmathexpr.so.1.0 /opt/lib/libmathexpr.so
	ln -sf /opt/lib/libmathexpr.so.1.0 /opt/lib/libmathexpr.so.1
uninstall:
	rm /opt/lib/libmathexpr.so*
test:
	cd src && make test
