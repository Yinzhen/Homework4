#include <mpi.h>
#include "temperature.h"

int main(int argc, char *argv[]){
    if (argc != 2) {
        printf("USAGE: %s <nx>\n", argv[0]);
        exit(1);
    }

    const int nx = atoi(argv[1]);
    const double pi = acos(0)*2;
    const double delta = 0.25;
    const int n = 2*nx*nx;
    const double dx = pi/nx;
    double ** T_c;
    double ** T_p;
    


    string file = argv[0];
    file.append(argv[1]);
    file.append("_output");


    int tag = 123;
    int n_tsk, rank;
    double time_start =  MPI_Wtime(); 
    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &n_tsk);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int interval = nx/n_tsk;
    /**** Create send and receive ***/

    double send_bound1[nx];
    double send_bound2[nx];
    double receive_bound1[nx];
    double receive_bound2[nx];

    /****** Create new segment ******/
    T_c = new double* [interval + 2];
    T_p = new double* [interval + 2];

    for(int i = 0; i < interval + 2; i++){
        T_c[i] = new  double[nx];
        T_p[i] = new  double[nx];
    }

    /****** Initialize array ********/
    for(int i = 0; i < interval + 2; i++){
        for(int j = 1; j < nx-1; j ++){
            T_c[i][j] = 0;
            T_p[i][j] = 0;
        }
    }
    for(int i = 0; i < interval + 2; i++){
        T_p[i][0] = pow(cos(dx*(rank*interval-1+i)), 2);        
        T_p[i][nx-1] = pow(sin(dx*(rank*interval-1+i)), 2);
        T_c[i][0] = T_p[i][0];
        T_c[i][nx-1] = T_p[i][nx-1];
    }

    for(int t = 0; t < n; t ++){
        for(int i = 1; i < interval+1; i++){
            for(int j = 1; j < nx-1; j++){
                T_c[i][j] = delta*(T_p[i-1][j] +T_p[i+1][j]+T_p[i][j-1]+T_p[i][j+1]);                
            }
        }
	for(int i = 1; i < interval+1; i++){
	    for(int j = 1; j < nx-1; j++){
               T_p[i][j] =  T_c[i][j];
            }
	}
        /*** Initial message ******/
        for(int i = 0; i < nx; i++){
            send_bound1[i] = T_c[1][i];
            send_bound2[i] = T_c[interval][i];
        
	}
        if (rank != 0){
	     MPI_Recv(&receive_bound2, nx, MPI_DOUBLE, rank-1, tag+1, MPI_COMM_WORLD, &status);
	}
	MPI_Send(&send_bound2, nx, MPI_DOUBLE, (rank+1)%n_tsk, tag+1, MPI_COMM_WORLD);
	if (rank == 0){
	      MPI_Recv(&receive_bound2, nx, MPI_DOUBLE, n_tsk-1, tag+1, MPI_COMM_WORLD, &status);
	}	
	if (rank != n_tsk -1){
             MPI_Recv(&receive_bound1, nx, MPI_DOUBLE, rank+1, tag, MPI_COMM_WORLD, &status);
        }		
	MPI_Send(&send_bound1, nx, MPI_DOUBLE, ((rank-1)%n_tsk+n_tsk)%n_tsk, tag, MPI_COMM_WORLD);	
	if (rank == n_tsk-1){
	      MPI_Recv(&receive_bound1, nx, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
	}
        for(int i = 0; i < nx; i++){
            T_p[0][i] = receive_bound2[i];
            T_p[interval+1][i] = receive_bound1[i];
        }
    }
    
    if(rank != 0){
        double send_T[nx*interval];    
        for(int i = 0; i < interval; i++){      
            for(int j = 0; j < nx; j ++){
                send_T[i*nx+j] = T_c[i+1][j];
            }
        }
        MPI_Send(&send_T, nx*interval, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);

    }
    else{
        double receive_T[nx*interval];
        double ** T = new double*[nx];
        double v_sum = 0;
        for(int i = 0; i < nx; i++){
            T[i] = new double [nx];
        }
        for(int i = 1; i < interval + 1; i++){
            for(int j = 0; j < nx; j ++){
                T[i-1][j] = T_c[i][j];
                v_sum += T_c[i][j];
            }
        }
        for(int i = 1; i < n_tsk; i++){
            MPI_Recv(&receive_T, nx*interval, MPI_DOUBLE, i, tag, MPI_COMM_WORLD, &status);
            for(int j = 0; j < interval; j++){      
                for(int k = 0; k < nx; k ++){
                    T[i*interval+j][k] = receive_T[j*nx+k];
                    v_sum += receive_T[j*nx+k];
                }
            }
        }
        double time_end =  MPI_Wtime();
        cout << "Volume average for "<<nx<<" x "<<nx<<" "<<v_sum/nx/nx<<endl;
        cout <<"Running time is "<<time_end-time_start<< "s" <<endl;
        print2file(T, nx, file);
        for(int i = 0; i < nx; i++){
	    delete [] T[i];
	}
	delete [] T;
    }
    for(int i = 0; i < interval + 2; i++){
        delete [] T_c[i];
        delete [] T_p[i];
    }
    delete [] T_c;
    delete [] T_p;

    MPI_Finalize();  
}






