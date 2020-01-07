CFLAGS= -Wall -Werror -std=gnu++03 -pedantic -ggdb3

ffosh: ffosh.cpp shellworker.cpp vectorHelper.cpp
	g++ $(CFLAGS) -o ffosh ffosh.cpp shellworker.cpp vectorHelper.cpp

clean:
	rm -f *~