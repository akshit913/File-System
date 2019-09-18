libwadfs.a: wad.o
	ar cr libwadfs.a wad.o
wadfs.o: wad.cpp wad.h
	g++ -c wad.cpp

