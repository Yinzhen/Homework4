#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <omp.h>
#include "temperature.h"
using namespace std;



int main(int argc, char *argv[]){
	if (argc != 2) {
    	printf("USAGE: %s <nx> <nthreads>\n", argv[0]);
    	exit(1);
    }

    double start_time = omp_get_wtime();
    const int nx = atoi(argv[1]);
    //const int nthreads = atoi(argv[2]);
    const double pi = 3.1415926535897;
    const double delta = 0.25;
    const int n = 2*nx*nx;
    const double dx = pi/nx;
    int t, i;
    int chunk;
    chunk = 128/4;


    string file = argv[0];
    file.append(argv[1]);
    file.append("_output");
    double ** T_c = new_Temperature(nx, dx);
    double ** T_p = new_Temperature(nx, dx);






    for(t = 0; t < n; t++){ 
        omp_set_num_threads(1); 
        #pragma omp parallel
        {
        int thread = omp_get_thread_num();
    	for(i = 1; i < nx+1; i++){
            int j;
    		for(j = 1; j < nx-1; j++){
				T_c[i][j] = delta*(T_p[i-1][j] +T_p[i+1][j]+T_p[i][j-1]+T_p[i][j+1]); 		
    		}
    	}
        }
    	for(int i = 1; i < nx+1; i ++){
            for(int j = 0; j < nx; j++){
                T_p[i][j] = T_c[i][j];
            }
        }
        for(int j = 0; j < nx; j++){
            T_p[0][j] = T_c[nx][j];
            T_p[nx+1][j] = T_c[1][j];
        } 
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

