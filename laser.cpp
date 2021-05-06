#include "iostream"
#include "laser.h"
#include <stdlib.h>

laser::laser(double vel){
	left=0.0, right=56.0, top=1.0, bottom=5.0;
	posy = rand() % 475 + 40;
	posx = 800.1;
	velx = vel;
	todelete = false;
}

void laser::update(sf::Texture& play){
	sprite.setTexture(play);
	posx += velx;
	sprite.setPosition(sf::Vector2f(posx, posy));

	//If we reach some threshold location (the "end of the screen") set todelete to true
	if (posx < -46){
		todelete = true;
	}
}

void laser::draw(sf::RenderWindow & window){
	window.draw(sprite);
}

void check_reached_end(std::vector<laser> &lasers){
	//See if any laser has reached the end of the screen. If yes, delete the laser.
	//Passing vector by reference to allow manipulation of the laser vector outside
	//of its original scope (the main function). Not a class function

	std::vector<int> todelete; //Specifies which elements to delete of the lasers vector.
	int counter = 0; //Enumerates the current element when checking if it's reached the end.
	for (std::vector<laser>::iterator iter = lasers.begin(); iter != lasers.end(); ++iter){
		if ((*iter).todelete == true) todelete.push_back(counter);
		counter++;
	}

	//Delete the last elements first and the first elements last. Reverse the order of the vector to make this happen
	std::reverse(todelete.begin(),todelete.end());  

	//for (std::vector<laser>::iterator iter = todelete.begin(); iter != todelete.end(); ++iter){
	for (int i=0; i < todelete.size(); i++){
		lasers[todelete[i]] = lasers.back();
		lasers.pop_back();
	}
}