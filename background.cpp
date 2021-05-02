#include <SFML/Graphics.hpp>
#include "background.h"

background::background(double x, double y, double vx, const std::string& fname){
	posx = x;
	posy = y;
	velx = vx;
	bg.loadFromFile(fname);
	sprite.setTexture(bg);
}

void background::update(){
	posx += velx;
	if (posx >= 2418.0) posx = -2418.0; //Scroll
	sprite.setPosition(sf::Vector2f(posx, posy));
}

void background::draw(sf::RenderWindow & window){
	window.draw(sprite);
}