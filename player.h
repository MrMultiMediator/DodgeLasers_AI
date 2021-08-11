#ifndef PLAYER_H_
#define PLAYER_H_
#include "iostream"
#include "laser.h"
#include <SFML/Graphics.hpp>

class NeuralNet
{
public:
	std::vector<double> inputs;
	double output;

	NeuralNet(int, int);
	double activate(double val);
	void reload_inputs(double vely, double posy, std::vector<laser> &lasers);
};

class player
{

public:
	double posy, vely, stime, left, right, top, bottom;
	std::string state;
	NeuralNet NN; //player has a neural net

	//sf::Texture play;
	sf::Sprite sprite;

	player(int IDD, int nlasers); 
	void update(sf::Texture&); //Need to pass in texture by reference to avoid white square problem
	void draw(sf::RenderWindow & window);
	void reload_inputs(std::vector<laser> &lasers); //Reload the input neurons with data
};

void collision_detect(std::vector<laser> &lasers, std::vector<player> &players);

#endif