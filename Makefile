default:
	mkdir -p bin
	cd src && make
	cd player && make

clean:
	rm -Rf bin

