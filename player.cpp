#include "iostream"
#include <stdlib.h> //For rand()
#include <cmath> //For exp
#include <SFML/Graphics.hpp>
#include <time.h> //For seeding the random number generator w/ current time
#include "player.h"
#include "laser.h"
//#include <unistd.h>

player::player(int IDD, int nlasers) : NN(IDD, nlasers){
	left=2.0, right=15.0, top=2.0, bottom=83.0;
	stime = 0;
	posy = 600./2.;
	vely = 0.0;
	//play.loadFromFile(fname);
	//sprite.setTexture(play);
	state = "a"; //a is for alive
	stime = 0.0;
	NN = NeuralNet(IDD, nlasers);
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

	//std::cout << NN.output << "\n";
}

void player::draw(sf::RenderWindow & window){
	window.draw(sprite);
}

void player::reload_inputs(std::vector<laser> &lasers){
	NN.reload_inputs(vely, posy, lasers);
}



NeuralNet::NeuralNet(int IDD, int nlasers){
	srand((unsigned)time( NULL )+IDD); //Use current time as seed for random number generator.
	output = ((double) rand() / (RAND_MAX));

	inputs.push_back(0.0); //Player velcocity

	for (int i = 0; i < nlasers; i++){
		inputs.push_back(0.0); //Delta_y: Difference between player y and laser y
		inputs.push_back(0.0); //Delta_x: Laser x position relative to player surface (where collisions start)
		inputs.push_back(0.0); //Laser x velocities
	}
}

double NeuralNet::activate(double val){
	return 1./(1.+exp(-val));
}

void NeuralNet::reload_inputs(double vely, double posy, std::vector<laser> &lasers){
	int j = 1;
	inputs[0] = vely;

	for (int i = 0; i < lasers.size(); i++){
		inputs[j] = posy - lasers[i].posy; // Delta_y: Difference between player y and laser y
		inputs[j+1] = lasers[i].posx - 25; // Delta_x: Laser x position relative to player surface (where collisions start)
		inputs[j+2] = lasers[i].velx; // Laser x velocities
		j += 3;
	}

	for (int i = 0; i < inputs.size(); i++){
		std::cout << inputs[i] <<"\n";
	}
	std::cout << "\n";
}

void collision_detect(std::vector<laser> &lasers, std::vector<player> &players){
	int count1, count2;
	count1=0;
	for (std::vector<laser>::iterator itlas = lasers.begin(); itlas != lasers.end(); ++itlas){
		count1++;
		count2 = 0;
		for (std::vector<player>::iterator itplay = players.begin(); itplay != players.end(); ++itplay){
			count2++;
			if ((*itlas).posx <= 22 && 10 <= (*itlas).posx + 55 && (*itplay).posy <= (*itlas).posy+4 && (*itlas).posy <= (*itplay).posy + 82){
				//std::cout << "collision\n";
			}

			//std::cout << (*itplay).posy << " " << (*itlas).posx << " " << (*itlas).posy << "\n";
			//std::cout << count1 << " " << count2 << "\n";
		}
	}
}
