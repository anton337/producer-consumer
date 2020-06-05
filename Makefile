all:
	g++ -std=c++11 -O4 -g0 test.cpp -o producer_consumer -lboost_thread -lboost_system
