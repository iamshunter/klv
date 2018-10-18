all : tt libklv.a klv.x

tt : tt.go 
	go build tt.go 

libklv.a : l3Sum.o
	ar r libklv.a l3Sum.o
	ranlib libklv.a
	ar t libklv.a

l3Sum.o : l3Sum.cpp l3Sum.h
	g++ -std=gnu++11 -I. -O3 -c l3Sum.cpp

klv.x : klv.o libklv.a
	g++ -std=gnu++11 -I. -o klv.x klv.o -lz -L. -lklv

klv.o : klv.cpp
	g++ -std=gnu++11 -I. -c klv.cpp

clean :
	rm -f tt klv.o klv.x l3Sum.o libklv.a t1.pb.go
