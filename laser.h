#include "iostream"
#include <SFML/Graphics.hpp>

class laser
{
public:
	int posx, posy, velx;

	sf::Sprite sprite;

	laser();
	void update(sf::Texture&); //Need to pass in texture by reference to avoid white square problem
	void draw(sf::RenderWindow & window);
};