#include "iostream"
#include <SFML/Graphics.hpp>
#include "background.h"
#include "player.h"
#include "laser.h"
#include "gSettings.h"
#include "fcns.h"
//#include "collision_detect.h"

//if (!background.loadFromFile("assets/background.png")) std::cout << "Could not load assets/background.png";

int main(int argc, char *argv[])
{
	bool pressed = false;
	bool cr = false;
	gSettings *gs = new gSettings(3,"assets/player.png","assets/spear.png");

	double posx = 0.0, posy = 0.0;

	std::vector<background> bg;
	background *bg1 = new background(0.0, 0.0, 7.0, "assets/background.png");
	background *bg2 = new background(-2418.0, 0.0, 7.0, "assets/background.png");
	//bg.push_back(background(0.0, 0.0, 7.0, "assets/background.png"));
	//bg.push_back(background(-2418.0, 0.0, 7.0, "assets/background.png"));
	bg.push_back(*bg1);
	bg.push_back(*bg2);

	//Generate players
	std::vector<player> players;
	for (int i = 0; i < gs->nplayers; i++){
		players.push_back(player(i));
	}

	//Generate lasers. The numer of lasers is a command line argument.
	//std::vector<laser*> lasers; //heap allocation
	std::vector<laser> lasers; //stack allocation
	for (int i = 0; i < std::stoi(argv[1]); i++){
		//lasers.push_back(new laser(-norm_dist(4.5, 2.))); //heap allocation
		lasers.push_back(laser(-norm_dist(4.5, 2.))); //stack allocation
	}

	sf::RenderWindow window(sf::VideoMode(800, 600), "Dodge Lasers");
	window.setFramerateLimit(60);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			cr = checkreturn(event, cr);
		}

		window.clear(sf::Color(25,29,33));

		//Screen is active
		if (cr == false){
			//Update background using iterator
			for (std::vector<background>::iterator iter = bg.begin(); iter != bg.end(); ++iter){
				(*iter).draw(window);
				(*iter).update();
			}
			//Update lasers using iterator
			for (std::vector<laser>::iterator iter = lasers.begin(); iter != lasers.end(); ++iter){
				(*iter).draw(window);
				(*iter).update(gs->lasertexture);
			}
			//Update players using iterator
			for (std::vector<player>::iterator iter = players.begin(); iter != players.end(); ++iter){
				(*iter).draw(window);
				(*iter).update(gs->play);
			}
		//Screen is frozen
		} else {
			//Update background using iterator
			for (std::vector<background>::iterator iter = bg.begin(); iter != bg.end(); ++iter) (*iter).draw(window);
			//Update lasers using iterator
			for (std::vector<laser>::iterator iter = lasers.begin(); iter != lasers.end(); ++iter) (*iter).draw(window);
			//Update players using iterator
			for (std::vector<player>::iterator iter = players.begin(); iter != players.end(); ++iter) (*iter).draw(window);
		}

		pressed = show_mouse_coords(pressed, sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

		check_reached_end(lasers); //If any lasers have reached the end of the screen, delete them
		collision_detect(lasers, players); //Detect collisions between lasers and players
		
		window.display();
	}

	return 0;
}

//Old update background code
//bg[0].draw(window);
//bg[1].draw(window);
//bg[0].update();
//bg[1].update();
