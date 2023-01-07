all: system

system: displayUI.cpp displayUI.h
	g++ -std=c++14 -o system displayUI.cpp -lwthttp -lwt
	./system --docroot . --http-address 0.0.0.0 --http-port 9000
