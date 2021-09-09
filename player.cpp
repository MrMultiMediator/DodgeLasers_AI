#include "iostream"
#include <stdlib.h> //For rand()
#include <cmath> //For exp
#include <SFML/Graphics.hpp>
#include <numeric>
#include <time.h> //For seeding the random number generator w/ current time
#include "player.h"
#include "laser.h"
#include "fcns.h"
#include "file_io.h"
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

	//for (int i = 0; i < st_arr.size(); i++){
	//	std::cout << st_arr[i] << " ";
	//}
	//std::cout << "\n";
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
}

void player::reload_inputs(std::vector<laser> &lasers){
	NN.reload_inputs(vely, posy, lasers);
}

void player::reload_NN(std::string st, std::string w, std::string b, int nlasers){
	/// Reload Neural Network from file for this player
	std::vector<std::string> vline;
	split2(st, vline);

	for (std::vector<std::string>::iterator itst = vline.begin()+1; itst != vline.end(); ++itst){
		st_arr.push_back(stoi(*itst));
	}
	vline.clear();
	NN.reload_NN(w, b, nlasers);
}

void player::propagate(){
	NN.propagate();
}


NeuralNet::NeuralNet(int IDD, int nlasers){
	srand((unsigned)time( NULL )+IDD); //Use current time as seed for random number generator.
	output = ((double) rand() / (RAND_MAX));

	inputs.push_back(0.0); //Player velcocity
	weights.push_back(0.01); //Weight for player velocity
	bias = 0.0; //Bias for player velocity

	for (int i = 0; i < nlasers; i++){
		inputs.push_back(0.0); //Delta_y: Difference between player y and laser y
		weights.push_back(2.*rand_gen()-1.);
		inputs.push_back(0.0); //Delta_x: Laser x position relative to player surface (where collisions start)
		weights.push_back(2.*rand_gen()-1.);
		inputs.push_back(0.0); //Laser x velocities
		weights.push_back(2.*rand_gen()-1.);
	}
}

double NeuralNet::activate(double val){
	return 1./(1.+exp(-val));
}

void NeuralNet::reload_inputs(double vely, double posy, std::vector<laser> &lasers){
	/// Reload the inputs to the neural network from the current game state (positions and velocities of lasers and current player)
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

void NeuralNet::reload_NN(std::string w, std::string b, int nlasers){
	/// Reload Neural Network from file for this player
	std::vector<std::string> vline;
	int counter = 0;
	split2(w, vline);

	for (std::vector<std::string>::iterator itw = vline.begin()+1; itw != vline.end(); ++itw){
		weights[counter] = stod(*itw);
		counter++;
	}
	vline.clear();
	split2(b, vline);
	bias = stod(vline[1]);
	vline.clear();
}

void NeuralNet::propagate(){
	/// Feedforward (i.e propagate) Neural Network.
	double temp = 0.0;
	for (int i = 0; i < inputs.size(); i++){
		temp += weights[i]*inputs[i];
	}
	temp += bias;
	output = activate(temp);
}

void collision_detect(std::vector<laser> &lasers, std::vector<player> &players){
	/// Detect collisions between players and lasers. If a collision is detected, kill that player.
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

void check_restart(std::vector<player> &players, std::vector<laser> &lasers, gSettings *gs, generation *gen){

	// If any players are still alive, exit the function immediately
	for (std::vector<player>::iterator itplay = players.begin(); itplay != players.end(); ++itplay){
		if ((*itplay).state == "a") return;
	}

	// All players are dead
	// Sample limit has been reached. Create a new generation
	if (gen->N_lasers >= gs->sampleLimit){
		// X Step 1: Calculate average of the average player survival times
		// X Step 2: Calculate average standard deviations for player survival times
		// X Step 3: Calculate maximum average survival time ('best player')
		//  Step 4: Clone and mutate the most fit players. Create a temporary new vector of "seed" players,
		//		   and sample from those. After the seed players are created, replace the Neural Network
		//		   parameters in the original set of players with the new set of parameters.  Then delete
		//		   the temporary new vector of players.
		std::vector<player> seed_players;
		double average, std_dev, max_stime; // Average of average player survival times, average of std. dev of player survival times, maximum average player survival time (i.e. best player)
		double sum = 0., sq_sum = 0.;
		int counter = 0;
		std::vector<double> stime,std_dev_data; // Vectors that hold the average survival times of each player, and standard deviations for survival time of each player.

		write_temp_data(gen, players);

		for (std::vector<player>::iterator itplay = players.begin(); itplay != players.end(); ++itplay){
			sum = std::accumulate((*itplay).st_arr.begin(), (*itplay).st_arr.end(), 0.0);
			(*itplay).st_ave = sum/(*itplay).st_arr.size();
			sq_sum = std::inner_product((*itplay).st_arr.begin(), (*itplay).st_arr.end(), (*itplay).st_arr.begin(), 0.0);
			(*itplay).st_std = std::sqrt(sq_sum/((*itplay).st_arr.size()) - pow((*itplay).st_ave, 2));

			stime.push_back((*itplay).st_ave);
			std_dev_data.push_back((*itplay).st_std);

			//std::cout << "Player " << counter << " average survival time = " << (*itplay).st_ave << " +/- " <<(*itplay).st_std << "\n";
			counter++;

			sum = 0.; sq_sum = 0.;
		}

		sum = std::accumulate(stime.begin(), stime.end(), 0.0);
		average = sum/stime.size(); sum = 0.;
		sum = std::accumulate(std_dev_data.begin(), std_dev_data.end(), 0.0);
		std_dev = sum/std_dev_data.size();

		max_stime = *std::max_element(stime.begin(), stime.end());
		std::cout << "Data collection complete\nMax average survival time = " << max_stime << "\n";
		std::cout << "Extracting most fit players as seed players for next generation...\n";

		// Populate the seed_players vector with the best players
		select(players, seed_players, average, std_dev, max_stime, gs->std_scale, gen->N_lasers);

		std::cout << "Done\n\n";

		stime.clear(); std_dev_data.clear();

	// If the sample limit hasn't been reached, revive all players. Write a temporary data file as well.
	} else {
		for (std::vector<player>::iterator itplay = players.begin(); itplay != players.end(); ++itplay){
			(*itplay).revive();
		}
		for (std::vector<laser>::iterator itlas = lasers.begin(); itlas != lasers.end(); ++itlas){
			(*itlas).todelete = true;
		}
		gen->N_death_cycles++;
		write_temp_data(gen, players);
		std::cout << "N_death_cycles = " << gen->N_death_cycles << "\n";
		std::cout << "N_lasers = " << gen->N_lasers << "\n";
	}
}

void select(std::vector<player> &players, std::vector<player> &seed_players, double average, double std_dev, double max_stime, double std_scale, int N_lasers){
	double a = 1.0;
	double shift = average + std_scale*std_dev; //This will make x = 0 for the analytic function that determines the probability be set to the std cutoff distance (minimum lookout point)
	double factor1 = log(2.)/(max_stime-shift);
	double stat, x;
	int select_counter = 0, fit_counter = 0, i = 0;

	for (std::vector<player>::iterator itplay = players.begin(); itplay != players.end(); ++itplay){
		x = ((*itplay).st_ave-shift)*factor1*(std_dev/(*itplay).st_std);

		//Avoid division by zero for players with extremely small average standard deviation.
		if ((*itplay).st_std < 0.1) x = ((*itplay).st_ave-shift)*factor1*(std_dev/0.1);

		stat = rand_gen();
		stat = 0.;
		//Add player i to seed_players if the selection function value is greater than the random number stat
		if (exp(a*x)-0.9 > stat){
			seed_players.push_back(player(select_counter, N_lasers));
			std::cout << i << "  " << exp(a*x) << "  survival time = " << (*itplay).st_ave << " +/- " << (*itplay).st_std << " " << stat << "\n";
			select_counter++;
		}
		//Keep track of the number of players that were qualified, so we can determine how many of the qualified players ultimately were selected
		if (exp(a*x)-0.9 > 0){
			fit_counter++;
		}
		i++;
	}
	std::cout << fit_counter << "   fit players\n";
	std::cout << select_counter << "   fit players selected for next generation.\n";
	std::cout << stat << "\n";
}