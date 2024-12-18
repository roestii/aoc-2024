string.o: src/string.cpp src/string.h src/types.h
	gcc -g -c src/string.cpp -lc

vector.o: src/vector.cpp src/vector.h src/types.h
	gcc -g -c src/vector.cpp -lc

queue.o: src/queue.cpp src/queue.h src/types.h
	gcc -g -c src/queue.cpp -lc

stack.o: src/stack.cpp src/stack.h src/types.h
	gcc -g -c src/stack.cpp -lc

position_set.o: src/position_set.cpp src/position_set.h src/types.h
	gcc -g -c src/position_set.cpp -lc

character_map.o: src/character_map.cpp src/character_map.h src/types.h
	gcc -g -c src/character_map.cpp -lc

rule_vector.o: src/rule_vector.cpp src/rule_vector.h src/types.h
	gcc -g -c src/rule_vector.cpp -lc

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

day5: src/day5.cpp string.o vector.o rule_vector.o src/types.h 
	gcc -g -o day5 src/day5.cpp vector.o rule_vector.o string.o -lc

day6: src/day6.cpp position_set.o string.o src/types.h 
	gcc -g -o day6 src/day6.cpp position_set.o string.o -lc

day6_2: src/day6_2.cpp position_set.o string.o src/types.h 
	gcc -g -o day6_2 src/day6_2.cpp position_set.o string.o -lc

day7: src/day7.cpp queue.o string.o vector.o src/types.h 
	gcc -g -o day7 src/day7.cpp queue.o vector.o string.o -lc -lm

day8: src/day8.cpp character_map.o position_set.o string.o vector.o src/types.h 
	gcc -g -o day8 src/day8.cpp character_map.o position_set.o vector.o string.o -lc -lm
