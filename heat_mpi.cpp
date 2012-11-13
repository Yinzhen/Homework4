#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <mpi.h>
#include "temperature.h"
using namespace std;



int main(int argc, char *argv[]){
    clock_t start_time = clock();
	if (argc != 2) {
    	printf("USAGE: %s <nx>\n", argv[0]);
    	exit(1);
    }
    

}