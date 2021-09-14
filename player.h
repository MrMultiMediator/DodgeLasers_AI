#ifndef PLAYER_H_
#define PLAYER_H_
#include "iostream"
#include "laser.h"
#include "gSettings.h"
#include <SFML/Graphics.hpp>

class NeuralNet
{
public:
	std::vector<double> inputs, weights;
	double bias, output;

	NeuralNet(int, int);
	double activate(double val);
	void reload_inputs(double vely, double posy, std::vector<laser> &lasers);
	void reload_NN(std::string w, std::string b, int nlasers); // Reload Neural Net parameters from file
	void propagate(); // Forward propagation to get output value
	void clone(NeuralNet ref_net);
	void mutate(NeuralNet ref_net, double mmag, std::string mutate_style);
};

class player
{

public:
	double posy, vely, dvely, stime, left, right, top, bottom, st_ave, st_std;
	std::vector<double> st_arr; //Survival time array
	std::string state;
	NeuralNet NN; //player has a neural net

	//sf::Texture play;
	sf::Sprite sprite;

	player(int IDD, int nlasers);
	void revive();
	void birth();
	void update(sf::Texture&); //Need to pass in texture by reference to avoid white square problem
	void draw(sf::RenderWindow & window);
	void reload_inputs(std::vector<laser> &lasers); //Reload the input neurons with data
	void reload_NN(std::string st, std::string w, std::string b, int nlasers); // Reload Neural Net parameetrs from file
	void propagate(); // Forward propagation to get output value
	void clone(player &seed_player);
	void mutate(player &seed_player, double mmag, std::string mutate_style);
};

void collision_detect(std::vector<laser> &lasers, std::vector<player> &players);
void check_restart(std::vector<player> &players, std::vector<laser> &lasers, gSettings *gs, generation *gen);
void select(std::vector<player> &players, std::vector<player> &seed_players, double average, double std_dev, double max_stime, double std_scale, int N_lasers);

#endif