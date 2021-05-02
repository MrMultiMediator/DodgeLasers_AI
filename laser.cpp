#include "laser.h"
#include <stdlib.h>

laser::laser(){
	posy = rand() % 475 + 40;
	posx = 800;
	velx = -4;
}

void laser::update(sf::Texture& play){
	sprite.setTexture(play);
	posx += velx;
	sprite.setPosition(sf::Vector2f(posx, posy));
}

void laser::draw(sf::RenderWindow & window){
	window.draw(sprite);
}