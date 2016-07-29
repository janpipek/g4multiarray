# test: G4MultiArray.hh G4MultiArrayTools.hh test.cc
test: multi_array.hh test.cc
	g++ -std=c++11 -g -o test test.cc