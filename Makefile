unblockMe:
	g++ -Isrc/include -c unblockMe.cpp
	g++ unblockMe.o -o unblockMe -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system
play: unblockMe
	./unblockMe