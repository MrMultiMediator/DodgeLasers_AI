#pragma once
#include "iostream"
#include <SFML/Graphics.hpp>

class gSettings
{

public:
	//Number of players, and the number of lasers that the current generation of players has seen
	int nplayers, sampleLimit;
	double std_scale; // How many standard deviations to look ahead for player survival time cutoff (i.e. which are selected for cloning and mutation)
	sf::Texture play;
	sf::Texture lasertexture;
	gSettings(int, int, const std::string& fname, const std::string& lname, double);
};

class generation
{

public:
	int N_lasers, N_death_cycles, n_LOS; // n_LOS = Number of lasers on screen
	double mmag; // Mutation magnitude
	std::vector<double> laser_x_vels;
	std::string name, best_player, parent_gen, mutate_style; // Mutate style: 'abs' or 'rel' = absolute or relative
	generation(std::string, std::string, int);
	void reload(std::string filename);
	void birth(std::string, std::string);
};