all:
	cd src && make all
clean:
	cd src && make clean
	rm -f test
	cd lib && rm -rf *
install:
	cp src/common.h /opt/include
	mv lib/liboptions.so.1.0 /opt/lib
	ln -sf /opt/lib/liboptions.so.1.0 /opt/lib/liboptions.so
	ln -sf /opt/lib/liboptions.so.1.0 /opt/lib/liboptions.so.1
uninstall:
	rm /opt/lib/liboptions.so*
	rm /opt/include/common.h
test:
	cd src && make test
