CFLAGS = -g -Ofast -std=c++11
CC = g++

resize: resize.cpp setupfinal.cpp
	$(CC) $(CFLAGS) -o resize resize.cpp setupfinal.cpp

clean:
	$(RM) resize