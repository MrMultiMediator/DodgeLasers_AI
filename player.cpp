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

void player::birth(){
	/// Similar to revive, but it is for a newly born player in a new generation. st_arr doesn't need a push_back.
	state = "a";
	posy = 250.;
	vely = 0.5*dvely;
	stime = 0.;
	st_arr.clear();
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

void player::clone(player &seed_player){
	/// Copy the parameters from seed_player's neural net to the current player's neural net.
	NN.clone(seed_player.NN);
}

void player::mutate(player &seed_player, double mmag, std::string mutate_style){
	/// Copy a mutated version of the seed_player's neural net to the current player's neural net.
	NN.mutate(seed_player.NN, mmag, mutate_style);
}

NeuralNet::NeuralNet(int IDD, int nlasers){
	srand((unsigned)time( NULL )+IDD); //Use current time as seed for random number generator.
	output = ((double) rand() / (RAND_MAX));

	inputs.push_back(0.0); //Player velcocity
	weights.push_back(0.01); //Weight for player velocity
	bias = 0.0; //Bias for player velocity

	for (int i = 0; i < nlasers; i++){
		inputs.push_back(0.0); //Delta_y: Difference between player y and laser y
		//weights.push_back(2.*rand_gen()-1.);
		weights.push_back(0.);
		inputs.push_back(0.0); //Delta_x: Laser x position relative to player surface (where collisions start)
		//weights.push_back(2.*rand_gen()-1.);
		weights.push_back(0.);
		inputs.push_back(0.0); //Laser x velocities
		//weights.push_back(2.*rand_gen()-1.);
		weights.push_back(0.);
	}
}

double NeuralNet::activate(double val){
	return 1./(1.+exp(-val));
}

void NeuralNet::reload_inputs(double vely, double posy, std::vector<laser> &lasers){
	/// Reload the inputs to the neural network from the current game state (positions and velocities of lasers and current player)
	int j = 1;
	double mean_las_vel = -4.6;
	double std_las_vel = 2.25;
	inputs[0] = vely/30.;
	//std::cout << output << "\n";
	//x = 798 -> 25, y = 46 -> 556

	for (int i = 0; i < lasers.size(); i++){
		//inputs[j] = (posy - lasers[i].posy + 39)/510.; // Delta_y: Difference between player y and laser y shifted by 39 so that the zero point is when the laser is right in the middle of the player. Normalized by playable game width
		//inputs[j+1] = (lasers[i].posx - 25)/773.; // Delta_x: Laser x position relative to player surface (where collisions start)
		//inputs[j+2] = (lasers[i].velx - mean_las_vel)/std_las_vel; // Laser x velocities. Shifted by mean velocity and normalized by standard deviation.

		//inputs[j] = (510.-(posy - lasers[i].posy + 39))/510.; // Delta_y: Difference between player y and laser y shifted by 39 so that the zero point is when the laser is right in the middle of the player. Normalized by playable game height
		//inputs[j+1] = (800.1-lasers[i].posx)/800.; // Laser x position set so that the value of the neuron grows as the laser gets closer, rather than shrinks (the game works such that the lasers are travelling in the negative x-direction). Value will be between 0 and 1.
		//inputs[j+2] = (lasers[i].velx)/(mean_las_vel-std_las_vel); // Laser x velocities normalized by mean laser velocity plus the standard deviation (i.e. so that most players have a value less than 1).

		if (abs(posy - lasers[i].posy + 39) < 100.){
			inputs[j] = (100.-abs(posy - lasers[i].posy + 39))/100.;
		} else inputs[j] = 0.;
		inputs[j] *= (posy - lasers[i].posy + 39)/abs(posy - lasers[i].posy + 39);
		inputs[j] *= (800.1-lasers[i].posx)/800.;
		inputs[j] *= ((lasers[i].velx)/(mean_las_vel-std_las_vel)); // Laser x velocities normalized by mean laser velocity plus the standard deviation (i.e. so that most players have a value less than 1).
		inputs[j+1] = 0.;
		inputs[j+2] = 0.;

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

void NeuralNet::clone(NeuralNet ref_net){
	/// Copy the parameters from the reference neural net to the current neural net.
	bias = ref_net.bias;
	for (int i = 0; i < ref_net.weights.size(); i++){
		weights[i] = ref_net.weights[i];
	}
}

void NeuralNet::mutate(NeuralNet ref_net, double mmag, std::string mutate_style){
	/// Copy a mutated version of the refernce neural net's parameters to the current player's neural net.
	double mutation = 0.;
	bias = ref_net.bias;
	weights[0] = ref_net.weights[0];

	if (mutate_style == "rel"){
		for (int i = 1; i < ref_net.weights.size(); i++){
			mutation = mmag*(rand_gen()-0.5);
			weights[i] = ref_net.weights[i] + mutation;
		}
	} else if (mutate_style == "abs"){
		for (int i = 1; i < ref_net.weights.size(); i++){
			// One out of every four players will have a single weight with a wildcard
			if (rand_gen() > (1./(4.*weights.size()))){
				mutation = ref_net.weights[i]*mmag*(rand_gen()-0.5);
				weights[i] = ref_net.weights[i] + mutation;
			} else { // Wildcard
				std::cout << "Wildcard activated\n";
				weights[i] = rand_gen()*0.002-0.001;
			}
		}
	} else {
		std::cout << "ERROR: mutate style could not be found\n";
	}
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
	/// Check if all players have died. If not, exit the function. If so, check if N_lasers > sampleLimit and act accordingly (revive if not, new generation if so)
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
		// X Step 4: Write player and generation information to archive.
		// X Step 5: Create a temporary new vector of "seed" players.
		//	Step 6: Sample from seed players. Replace the Neural Network parameters in the original set of
		//			players with the new set of parameters mutated or cloned from the seed players.
		std::vector<player> seed_players;
		double average, std_dev, max_stime; // Average of average player survival times, average of std. dev of player survival times, maximum average player survival time (i.e. best player)
		double sum = 0., sq_sum = 0.;
		int counter = 0;
		int spnum; // Seed player number
		std::vector<double> stime,std_dev_data; // Vectors that hold the average survival times of each player, and standard deviations for survival time of each player.

		write_temp_data(gen, players);
		write_gen(gen);
		write_players(gen, players);

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
		select(players, seed_players, average, std_dev, max_stime, gs->std_scale, gen->n_LOS);
		counter = 0;

		// Clone the seed players (i.e. give some players the same neural net as the seed players)
		for (std::vector<player>::iterator itplay = seed_players.begin(); itplay != seed_players.end(); ++itplay){
			players[counter].clone((*itplay));
			counter++;
		}

		// Mutate the seed players until we reach the total number of players (i.e. give the remaining players mutated versions of the seed players' neural nets)
		// Use all seed players uniformly, showing no preference for better players among them.
		while (counter < players.size()){
			spnum = counter % seed_players.size();
			players[counter].mutate(seed_players[spnum], gen->mmag, gen->mutate_style);
			counter++;
		}

		// Rebirth the generation (i.e. change the settings appropriately: Rename, set parent name, etc)
		std::string name = gen->name;
		gen->birth(gen_random(12), name);

		// Rebirth all players (ie. change the settings appropriately: Set state to 'alive', survival time to zero, clear survival time array, etc)
		for (std::vector<player>::iterator itplay = players.begin(); itplay != players.end(); ++itplay){
			(*itplay).birth();
		}

		for (std::vector<laser>::iterator itlas = lasers.begin(); itlas != lasers.end(); ++itlas){
			(*itlas).todelete = true;
		}

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
		std::cout << "N_death_cycles = " << gen->N_death_cycles << ", N_lasers = " << gen->N_lasers << "\n";
	}
}

void select(std::vector<player> &players, std::vector<player> &seed_players, double average, double std_dev, double max_stime, double std_scale, int N_lasers){
	/// N_lasers in this case equals the number of lasers on the screen (not N_lasers ever which is the standard meaning)
	double a = 0.5;
	double shift = average + std_scale*std_dev; //This will make x = 0 for the analytic function that determines the probability be set to the std cutoff distance (minimum lookout point)
	double factor1 = log(2.)/(max_stime-shift);
	double stat, x;
	int select_counter = 0, fit_counter = 0, i = 0;

	while (select_counter <= 0){
		for (std::vector<player>::iterator itplay = players.begin(); itplay != players.end(); ++itplay){
			x = ((*itplay).st_ave-shift)*factor1*(std_dev/(*itplay).st_std);

			//Avoid division by zero for players with extremely small average standard deviation.
			if ((*itplay).st_std < 0.1) x = ((*itplay).st_ave-shift)*factor1*(std_dev/0.1);

			stat = 1.5*rand_gen();
			//stat = 0.;
			//Add player i to seed_players if the selection function value is greater than the random number stat
			if (exp(a*x)-0.9 > stat){
				seed_players.push_back(player(select_counter, N_lasers));
				seed_players.back().clone((*itplay));
				std::cout << i << "  " << exp(a*x) << "  survival time = " << (*itplay).st_ave << " +/- " << (*itplay).st_std << " " << stat << "\n";
				select_counter++;
			}
			//Keep track of the number of players that were qualified, so we can determine how many of the qualified players ultimately were selected
			if (exp(a*x)-0.9 > 0){
				fit_counter++;
			}
			i++;
		}
	}
	std::cout << fit_counter << "   fit players\n";
	std::cout << select_counter << "   fit players selected for next generation.\n";
	std::cout << stat << "\n";
}