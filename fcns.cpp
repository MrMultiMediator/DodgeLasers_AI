//#include <cstdlib>
//#include <ctime>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "fcns.h"
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

bool show_mouse_coords(bool pressed, int mousex, int mousey){

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) pressed = true;

	if (!(sf::Mouse::isButtonPressed(sf::Mouse::Left)) && (pressed)) {
		std::cout << mousex << ", " << mousey << "\n";
		pressed = false;
	}

	return pressed;
}

bool checkreturn(sf::Event event, bool cr){
	if (event.type == sf::Event::KeyPressed){
		if ((event.key.code == sf::Keyboard::RControl) && cr == false){
			std::cout << "Screen frozen\n";
			return true;
		}
	}
	if (event.type == sf::Event::KeyPressed){
		if ((event.key.code == sf::Keyboard::RControl) && cr == true){
			std::cout << "Screen restored\n";
			return false;
		}
	}
	return cr;
}