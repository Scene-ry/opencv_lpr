mkdir crops
g++ main.cpp -std=c++11 `pkg-config --cflags --libs opencv` -o main.out
./main.out
