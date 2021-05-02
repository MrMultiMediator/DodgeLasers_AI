#include "iostream"
#include <stdlib.h> //For rand()
#include <cmath> //For exp
#include <SFML/Graphics.hpp>
#include <time.h> //For seeding the random number generator w/ current time
#include "player.h"
//#include <unistd.h>

player::player(int IDD) : NN(IDD){
	stime = 0;
	posy = 600./2.;
	vely = 0.0;
	//play.loadFromFile(fname);
	//sprite.setTexture(play);
	state = "alive";
	stime = 0.0;
	NN = NeuralNet(IDD);
}

//Need to pass in texture by reference to avoid white square problem. This is because passing by reference only passes
//in a copy which is destroyed when the function ends. The texture needs to persist, so I need to pass it be reference.
//According to the docs, "the texture must exist as long as the sprite uses it". And the sprite needs to use it, even
//when it leaves this function, and moves on to the "draw" function
void player::update(sf::Texture& play){
	//Gravitational acceleration
	sprite.setTexture(play);
	vely += 0.3;
	if (NN.output > 0.5) vely -= 0.6;
	posy += vely;
	if (posy > 475.) posy = 475.0;
	if (posy < 40.) posy = 40.0;
	sprite.setPosition(sf::Vector2f(10.0, posy));

	std::cout << posy << "\n";
	std::cout << vely << "\n";
	std::cout << NN.output << "\n";
}

void player::draw(sf::RenderWindow & window){
	window.draw(sprite);
}



NeuralNet::NeuralNet(int IDD){
	std::cout << IDD << "\n";
	srand((unsigned)time( NULL )+IDD); //Use current time as seed for random number generator.
	output = ((double) rand() / (RAND_MAX));
}

double NeuralNet::activate(double val){
	return 1./(1.+exp(-val));
}