#include "iostream"
#include "laser.h"
#include <stdlib.h>

laser::laser(double vel){
	posy = rand() % 475 + 40;
	posx = 800.1;
	velx = vel;
	todestroy = false;
}

void laser::update(sf::Texture& play){
	sprite.setTexture(play);
	posx += velx;
	sprite.setPosition(sf::Vector2f(posx, posy));

	if (posx < 200){
		todestroy = true;
	}

	std::cout << posx << " " << velx << "\n";
}

void laser::draw(sf::RenderWindow & window){
	window.draw(sprite);
}

void check_reached_end(std::vector<laser> &lasers){
	//See if any laser has reached the end of the screen. Not a class function
	std::vector<int> todelete; //Specifies which elements to delete
	int counter = 0;
	for (std::vector<laser>::iterator iter = lasers.begin(); iter != lasers.end(); ++iter){
		if ((*iter).todestroy == true) todelete.push_back(counter);
		counter++;
	}

	//Delete the last elements first and the first elements last. Reverse the order of the vector to make this happen
	std::reverse(todelete.begin(),todelete.end());  

	//for (std::vector<laser>::iterator iter = todelete.begin(); iter != todelete.end(); ++iter){
	for (int i=0; i < todelete.size(); i++){
		std::cout << todelete[i] << "\n";
		lasers[todelete[i]] = lasers.back();
		lasers.pop_back();
	}
}