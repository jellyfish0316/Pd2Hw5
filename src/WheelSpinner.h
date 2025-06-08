#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <sstream>
using namespace std;

void show_wheel(const vector<string>& problems, int current);
int spin_wheel(const vector<string>& problems, mt19937& rng);


