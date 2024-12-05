string.o: src/string.cpp src/string.h src/types.h
	gcc -g -c src/string.cpp -lc

vector.o: src/vector.cpp src/vector.h src/types.h
	gcc -g -c src/vector.cpp -lc

day1: src/day1.cpp string.o vector.o src/types.h 
	gcc -g -o day1 src/day1.cpp string.o vector.o -lc

day2: src/day2.cpp string.o src/types.h 
	gcc -g -o day2 src/day2.cpp string.o -lc

day2_2: src/day2_2.cpp string.o vector.o src/types.h 
	gcc -g -o day2_2 src/day2_2.cpp string.o vector.o -lc

day3: src/day3.cpp src/types.h 
	gcc -g -o day3 src/day3.cpp -lc

day3_2: src/day3_2.cpp src/types.h 
	gcc -g -o day3_2 src/day3_2.cpp -lc

day4: src/day4.cpp src/types.h 
	gcc -g -o day4 src/day4.cpp -lc

day4_2: src/day4_2.cpp src/types.h 
	gcc -g -o day4_2 src/day4_2.cpp -lc
