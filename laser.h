#include "iostream"
#include <SFML/Graphics.hpp>

class laser
{
public:
	double posx, velx;
	int posy;
	bool todestroy; //If True, the laser has reached the end of the screen and should be destroyed

	sf::Sprite sprite;

	laser(double);
	void update(sf::Texture&); //Need to pass in texture by reference to avoid white square problem
	void draw(sf::RenderWindow & window);
};

void check_reached_end(std::vector<laser> &lasers);