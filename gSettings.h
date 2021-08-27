#pragma once
#include "iostream"
#include <SFML/Graphics.hpp>

class gSettings
{

public:
	//Number of players, and the number of lasers that the current generation of players has seen
	int nplayers, sampleLimit;
	sf::Texture play;
	sf::Texture lasertexture;
	gSettings(int, int, const std::string& fname, const std::string& lname);
};

class generation
{

public:
	int N_lasers, N_death_cycles;
	std::vector<double> laser_x_vels;
	std::string name, best_player, parent_gen;
	generation(std::string, std::string);
	void reload(std::string filename);
};