#include <iostream>
#include <fstream>
#include <math.h>
#include "temperature.h"

using std::ofstream;
using std::cout;
using std::endl;


double ** new_Temperature(int nx, double dx){
	double ** T;
	T = new double *[nx];
	for(int i = 0; i < nx; i ++){
		T[i] = new double [nx];
	}
	for(int i = 0; i < nx; i ++){
		for(int j = 0; j < nx; j ++){
			if(j == 0){
				T[i][j] = pow(cos(i*dx), 2);
			}
			else if(j == nx-1){
				T[i][j] = pow(sin(i*dx), 2);
			}
			else{
				T[i][j] = 0;
			}			
		}
	}
	return T;
}

void print2file(double **T, int nx, string file){
	ofstream output;
	output.open(file.c_str());
  	if (output.is_open()){
  		for (int j = nx-1; j >= 0; j--){
  			for (int i = 0; i < nx; i++){
  				output <<T[i][j]<<" ";
  			}
  			output<<"\n";
  		}
    	output.close();
  	}
  	else cout << "Unable to open file";
}






