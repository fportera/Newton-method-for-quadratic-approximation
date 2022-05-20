all: pwq newt

pwq: pwq.cpp 
	g++ -o pwq pwq.cpp -O3
newt: newt.cpp 
	g++ -o newt newt.cpp -O3
