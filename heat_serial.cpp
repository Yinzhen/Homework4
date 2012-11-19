#include "temperature.h"
using namespace std;



int main(int argc, char *argv[]){
    clock_t start_time = clock();
	if (argc != 2) {
    	printf("USAGE: %s <nx>\n", argv[0]);
    	exit(1);
    }

    const int nx = atoi(argv[1]);
    const double pi = acos(0)*2;
    const double delta = 0.25;
    const int n = 2*nx*nx;
    const double dx = pi/nx;
    double v_sum = 0;
    string file = argv[0];
    file.append(argv[1]);
    file.append("_output");

    double ** T_c = new_Temperature(nx, dx);
    double ** T_p = new_Temperature(nx, dx);
    

    for(int t = 0; t < n; t++){
    	for(int i = 0; i < nx; i++){
    		for(int j = 1; j < nx-1; j++){
                if(i == 0){
                    T_c[i][j] = delta*(T_p[nx-1][j] +T_p[i+1][j]+T_p[i][j-1]+T_p[i][j+1]);
                }
                else if(i == nx -1){
                    T_c[i][j] = delta*(T_p[i-1][j] +T_p[0][j]+T_p[i][j-1]+T_p[i][j+1]);
                }
                else{
                    T_c[i][j] = delta*(T_p[i-1][j] +T_p[i+1][j]+T_p[i][j-1]+T_p[i][j+1]);
                }
    		}
    	}
    	for(int i = 0; i < nx; i ++){
            for(int j = 0; j < nx; j++){
                T_p[i][j] = T_c[i][j];
            }
        }    
    }

    clock_t end_time = clock();

	print2file(T_c, nx, file);
    for(int i = 0; i < nx; i ++){
        for(int j = 0; j < nx; j++){
            v_sum += T_c[i][j];
        }
    }

    for(int i = 0; i < nx; i ++){
        delete [] T_c[i];
        delete [] T_p[i];
    }
    delete [] T_c;
    delete [] T_p;

    cout << "Volume average for "<<nx<<" x "<<nx<<" "<<v_sum/nx/nx<<endl;
    cout << "Running time is: " << static_cast<double>(end_time-start_time)/CLOCKS_PER_SEC << "s" << endl;
    return 0;
}

