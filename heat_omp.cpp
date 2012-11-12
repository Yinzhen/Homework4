#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <omp.h>
#include "temperature.h"
using namespace std;



int main(int argc, char *argv[]){
	if (argc != 3) {
    	printf("USAGE: %s <nx> <nthreads>\n", argv[0]);
    	exit(1);
    }

    double start_time = omp_get_wtime();
    const int nx = atoi(argv[1]);
    const int nthreads = atoi(argv[2]);
    const double pi = 3.1415926535897;
    const double delta = 0.25;
    const int n = 2*nx*nx;
    const double dx = pi/nx;
    int chunk;


    string file = argv[0];
    file.append(argv[1]);
    file.append("_output");
    double ** T_c = new_Temperature(nx, dx);
    double ** T_p = new_Temperature(nx, dx);

    chunk = 100;
    #pragma omp parallel for shared(T_p, T_c, nx) private(i, j) schedule(static,chunk)
    for(int t = 0; t < n; t++){
    	for(int i = 1; i < nx/2+2; i++){
    		for(int j = 1; j < nx-1; j++){
				T_c[i][j] = delta*(T_p[i-1][j] +T_p[i+1][j]+T_p[i][j-1]+T_p[i][j+1]); 
                T_c[nx-i+2][j] = T_c[i][j]; 		
    		}
    	}
    	copyT(T_p, T_c, nx);      
    }

	print2file(T_c, nx, file);

    for(int i = 0; i < nx+2; i ++){
        delete [] T_c[i];
        delete [] T_p[i];
    }
    delete [] T_c;
    delete [] T_p;

    double stop_time = omp_get_wtime();
    cout << "Running time is: " << stop_time - start_time << "s" << endl;
    return 0;
}

