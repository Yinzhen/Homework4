#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_
#include <string>
using std::string;

double ** new_Temperature(int nx, double dx);

void print2file(double **T, int nx, string file);

#endif // Temperature
