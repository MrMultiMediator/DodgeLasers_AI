#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
using namespace std;

double rand_gen() {
	//return a uniformly distributed random value
	return ( (double)(rand()) + 1. )/( (double)(RAND_MAX) + 1. );
}
double normalRandom() {
	//return a normally distributed random value
	double v1=rand_gen();
	double v2=rand_gen();
	return cos(2*3.14*v2)*sqrt(-2.*log(v1));
}
double norm_dist(double mean, double std_dev) {
	double x = normalRandom()*std_dev+mean;

	if (x < 0.6) x = mean;

	return x;
}