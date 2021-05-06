#include "iostream"
#include <SFML/Graphics.hpp>

class NeuralNet
{
public:
	std::vector<double> inputs;
	double output;

	NeuralNet(int);
	double activate(double val);
};

class player
{

public:
	double posy, vely, stime, left, right, top, bottom;
	std::string state;
	NeuralNet NN;

	//sf::Texture play;
	sf::Sprite sprite;

	player(int IDD); 
	void update(sf::Texture&); //Need to pass in texture by reference to avoid white square problem
	void draw(sf::RenderWindow & window);
};