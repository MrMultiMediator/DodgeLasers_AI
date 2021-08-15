#ifndef LASER_H_
#define LASER_H_
#include "iostream"
#include <SFML/Graphics.hpp>

class laser
{
public:
	double posx, velx, left, right, top, bottom;
	float posy;
	bool todelete; //If True, the laser has reached the end of the screen and should be destroyed

	sf::Sprite sprite;

	laser(double);
	void update(sf::Texture&); //Need to pass in texture by reference to avoid white square problem
	void draw(sf::RenderWindow & window);
};

void check_reached_end(std::vector<laser> &lasers, int &lasers_ever);

#endif