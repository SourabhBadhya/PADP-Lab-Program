#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
#define SEED 35791246

int main(int argc, char *argv[]) {
	int n = atoi(argv[1]), i, count = 0;
	int numThreads = atoi(argv[2]);
	double z, pi, x,y;

	srand(SEED);

	double time1 = omp_get_wtime();
	#pragma omp parallel private(x, y, z) shared(count) num_threads(numThreads)
	for ( i=0; i<n; i++) {
		x = (double)rand()/RAND_MAX;
		y = (double)rand()/RAND_MAX;
		z = x*x+y*y;
		if (z<=1) count++;
	}
	pi=(double)count/n*4;
	double time2 = omp_get_wtime();
	printf("Number of trials= %d , estimate of pi is %g \n",n,pi);
	double totalTime = time2 - time1;
	printf("Number of iterations=%d\nTime taken=%f\nNumber of threads=%d\n",n,totalTime,numThreads);
	return 0;
}