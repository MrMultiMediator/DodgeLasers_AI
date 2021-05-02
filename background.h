#include "iostream"
#include <SFML/Graphics.hpp>

class background
{

public:
	double posx, posy, velx;
	sf::Texture bg;
	sf::Sprite sprite;
	background(double x, double y, double vx, const std::string& fname);
	void update();
	void draw(sf::RenderWindow & window);
};