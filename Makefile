all : tt libklv.a klvTest.x

tt : tt.go 
	go build tt.go 

libklv.a : l3Sum.o klvL1.o util.o
	ranlib libklv.a
	ar t libklv.a

l3Sum.o : l3Sum.cpp l3Sum.h
	g++ -std=gnu++11 -I. -O3 -c l3Sum.cpp
	ar r libklv.a l3Sum.o

klvL1.o : klvL1.cpp klvL1.h
	g++ -std=gnu++11 -I. -c klvL1.cpp
	ar r libklv.a klvL1.o

util.o : util.cpp util.h
	g++ -std=gnu++11 -I. -c util.cpp
	ar r libklv.a util.o

klvTest.x : klvTest.o libklv.a
	g++ -std=gnu++11 -I. -o klvTest.x klvTest.o -lz -L. -lklv

klvTest.o : klvTest.cpp
	g++ -std=gnu++11 -I. -c klvTest.cpp

clean :
	rm -f tt klvTest.o klvTest.x klvL1.o l3Sum.o util.o libklv.a t1.pb.go
