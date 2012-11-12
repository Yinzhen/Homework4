#include <iostream>
#include <fstream>
#include <math.h>
#include "temperature.h"

using std::ofstream;
using std::cout;
using std::endl;


double ** new_Temperature(int nx, double dx){
	double ** T;
	T = new double *[nx+1];
	for(int i = 0; i < nx+1; i ++){
		T[i] = new double [nx];
	}
	for(int i = 1; i < nx+1; i ++){
		for(int j = 0; j < nx; j ++){
			if(j == 0){
				T[i][j] = pow(cos((i-1)*dx), 2);
			}
			else if(j == nx-1){
				T[i][j] = pow(sin((i-1)*dx), 2);
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
  			for (int i = 1; i < nx+1; i++){
  				output <<T[i][j]<<" ";
  			}
  			output<<"\n";
  		}
    	output.close();
  	}
  	else cout << "Unable to open file";
}


void copyT(double **T_p, double **T_c, int nx){
	for(int i = 1; i < nx+1; i ++){
		for(int j = 0; j < nx; j++){
			T_p[i][j] = T_c[i][j];
		}
	}
	for(int j = 0; j < nx; j++){
		T_p[0][j] = T_c[nx][j];
	}
}




