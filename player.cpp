#include "iostream"
#include <stdlib.h> //For rand()
#include <cmath> //For exp
#include <SFML/Graphics.hpp>
#include <time.h> //For seeding the random number generator w/ current time
#include "player.h"
#include "laser.h"
#include "fcns.h"
//#include <unistd.h>

player::player(int IDD, int nlasers) : NN(IDD, nlasers){
	left=2.0, right=15.0, top=2.0, bottom=83.0;
	stime = 0;
	posy = 250.;
	dvely = 0.2; //Change in velocity due to acceleration
	vely = 0.5*dvely;
	//play.loadFromFile(fname);
	//sprite.setTexture(play);
	state = "a"; //a is for alive
	NN = NeuralNet(IDD, nlasers);
}

void player::revive(){
	state = "a";
	posy = 250.;
	vely = 0.5*dvely;
	st_arr.push_back(stime);
	stime = 0.;

	for (int i = 0; i < st_arr.size(); i++){
		std::cout << st_arr[i] << " ";
	}
	std::cout << "\n";
}

/* Need to pass in texture by reference to avoid white square problem. This is because passing by value only passes
in a copy which is destroyed when the function ends. The texture needs to persist, so I need to pass it by reference.
According to the docs, "the texture must exist as long as the sprite uses it". And the sprite needs to use it, even
when it leaves this function, and moves on to the "draw" function */
void player::update(sf::Texture& play){
	//Gravitational acceleration
	sprite.setTexture(play);
	vely += dvely;
	if (NN.output > 0.5) vely -= 2.0*dvely;

	posy += vely;

	// Boundaries
	if (state == "a"){
		stime++; //Increment the survival time
		if (posy > 475.){
			posy = 475.0;
			state = "d"; // Dead
		}
		if (posy < 40.){
			posy = 40.0;
			state = "d"; // Dead
		}
	}
	sprite.setPosition(sf::Vector2f(10.0, posy));
}

void player::draw(sf::RenderWindow & window){
	if (state == "a") window.draw(sprite);
	//window.draw(sprite);
}

void player::reload_inputs(std::vector<laser> &lasers){
	NN.reload_inputs(vely, posy, lasers);
}

void player::propagate(){
	NN.propagate();
}


NeuralNet::NeuralNet(int IDD, int nlasers){
	srand((unsigned)time( NULL )+IDD); //Use current time as seed for random number generator.
	output = ((double) rand() / (RAND_MAX));

	inputs.push_back(0.0); //Player velcocity
	weights.push_back(0.01); //Weight for player velocity
	biases.push_back(0.0); //Bias for player velocity

	for (int i = 0; i < nlasers; i++){
		inputs.push_back(0.0); //Delta_y: Difference between player y and laser y
		weights.push_back(2.*rand_gen()-1.);
		biases.push_back(2.*rand_gen()-1.);
		inputs.push_back(0.0); //Delta_x: Laser x position relative to player surface (where collisions start)
		weights.push_back(2.*rand_gen()-1.);
		biases.push_back(2.*rand_gen()-1.);
		inputs.push_back(0.0); //Laser x velocities
		weights.push_back(2.*rand_gen()-1.);
		biases.push_back(2.*rand_gen()-1.);
	}
}

double NeuralNet::activate(double val){
	return 1./(1.+exp(-val));
}

void NeuralNet::reload_inputs(double vely, double posy, std::vector<laser> &lasers){
	int j = 1;
	double mean_las_vel = -4.5;
	double std_las_vel = 2.0;
	inputs[0] = vely/30.;
	//std::cout << output << "\n";
	//x = 798 -> 25, y = 46 -> 556

	for (int i = 0; i < lasers.size(); i++){
		inputs[j] = (posy - lasers[i].posy + 39)/510.; // Delta_y: Difference between player y and laser y shifted by 39 so that the zero point is when the laser is right in the middle of the player. Normalized by playable game width
		inputs[j+1] = (lasers[i].posx - 25)/773.; // Delta_x: Laser x position relative to player surface (where collisions start)
		inputs[j+2] = (lasers[i].velx - mean_las_vel)/std_las_vel; // Laser x velocities. Shifted by mean velocity and normalized by standard deviation.
		j += 3;
	}
}

void NeuralNet::propagate(){
	double temp = 0.0;
	for (int i = 0; i < inputs.size(); i++){
		temp += weights[i]*inputs[i];
		temp += biases[i];
	}
	output = activate(temp);
}

void collision_detect(std::vector<laser> &lasers, std::vector<player> &players){
	int count1, count2;
	count1=0;
	for (std::vector<laser>::iterator itlas = lasers.begin(); itlas != lasers.end(); ++itlas){
		count1++;
		count2 = 0;
		for (std::vector<player>::iterator itplay = players.begin(); itplay != players.end(); ++itplay){
			count2++;
			if ((*itplay).state == "a"){
				if ((*itlas).posx <= 22 && 10 <= (*itlas).posx + 55 && (*itplay).posy <= (*itlas).posy+4 && (*itlas).posy <= (*itplay).posy + 82){
					(*itplay).state = "d";
				}
			}
			//std::cout << (*itplay).posy << " " << (*itlas).posx << " " << (*itlas).posy << "\n";
			//std::cout << count1 << " " << count2 << "\n";
		}
	}
}

void check_restart(std::vector<player> &players, std::vector<laser> &lasers, int sample_limit, int lasers_ever, int &N_death_cycles){

	// If any players are still alive, exit the function immediately
	for (std::vector<player>::iterator itplay = players.begin(); itplay != players.end(); ++itplay){
		if ((*itplay).state == "a") return;
	}

	// All players are dead
	if (lasers_ever >= sample_limit){

	// If the sample limit hasn't been reached, revive all players
	} else {
		for (std::vector<player>::iterator itplay = players.begin(); itplay != players.end(); ++itplay){
			(*itplay).revive();
		}
		for (std::vector<laser>::iterator itlas = lasers.begin(); itlas != lasers.end(); ++itlas){
			(*itlas).todelete = true;
		}
		N_death_cycles++;
		std::cout << "N_death_cycles = " << N_death_cycles << "\n";
	}
}