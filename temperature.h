#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
using namespace std;

double ** new_Temperature(int nx, double dx);

void print2file(double **T, int nx, string file);

#endif // Temperature
