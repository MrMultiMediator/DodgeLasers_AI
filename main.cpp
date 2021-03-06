#include <stdlib.h>
#include <time.h>
#include "iostream"
#include "fstream"
#include <SFML/Graphics.hpp>
#include "background.h"
#include "player.h"
#include "laser.h"
#include "gSettings.h"
#include "fcns.h"

//if (!background.loadFromFile("assets/background.png")) std::cout << "Could not load assets/background.png";

int main(int argc, char *argv[])
{
	srand(time(0));
	bool pressed = false;
	bool cr = false;
	std::string directive(argv[2]); // File to read in current generation from. If it says "None", start new generation
	std::string line;
	std::vector<std::string> vline;
	std::ifstream inp(directive); // Not sure why this doesn't throw an error. Sometimes the file "directive" doesn't even exist. Maybe it's because we don't try reading it in that case

	gSettings *gs = new gSettings(50,111,"assets/player.png","assets/spear.png",0.5); // Game settings object
	generation *gen = new generation(gen_random(12), "None", std::stoi(argv[1])); // New generation object. Parent generation name is the second parameter.
	gen->reload(directive); // Reload a previous generation from file if the user requests it.

	double posx = 0.0, posy = 0.0;

	std::vector<background> bg;
	background *bg1 = new background(0.0, 0.0, 7.0, "assets/background.png");
	background *bg2 = new background(-2418.0, 0.0, 7.0, "assets/background.png");
	//bg.push_back(background(0.0, 0.0, 7.0, "assets/background.png"));
	//bg.push_back(background(-2418.0, 0.0, 7.0, "assets/background.png"));
	bg.push_back(*bg1);
	bg.push_back(*bg2);

	//Generate lasers. The numer of lasers is a command line argument.
	std::vector<laser> lasers; //stack allocation
	for (int i = 0; i < gen->n_LOS; i++){
		//lasers.push_back(new laser(-norm_dist(4.5, 2.))); //heap allocation
		lasers.push_back(laser(-norm_dist(4.6, 2.25), gen->laser_x_vels)); //stack allocation
		gen->N_lasers++;
	}

	// Generate players
	std::vector<player> players;

	// Reload players from file. Open file and skip first five lines which are relevant for the generation on the whole but not the players.
	if (directive != "None"){
		std::getline(inp, line); std::getline(inp, line); std::getline(inp, line); std::getline(inp, line); std::getline(inp, line);
	}
	for (int i = 0; i < gs->nplayers; i++){
		players.push_back(player(i, gen->n_LOS));

		// Reload Neural Network from file
		if (directive != "None"){
			std::getline(inp, line); vline.push_back(line);
			std::getline(inp, line); vline.push_back(line);
			std::getline(inp, line); vline.push_back(line);
			// Reload Neural Net here. Pass in the three lines of the file relevant for this player, and the number of lasers
			players[i].reload_NN(vline[0], vline[1], vline[2], gen->n_LOS);
			vline.clear();
		}
	}

	if (directive != "None") inp.close();

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

		//Screen is active (ctrl key)
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
				(*iter).update(gs->play);
				(*iter).reload_inputs(lasers);
				(*iter).propagate();
				(*iter).draw(window);
			}
		//Screen is frozen (ctrl key)
		} else {
			//Update background using iterator
			for (std::vector<background>::iterator iter = bg.begin(); iter != bg.end(); ++iter) (*iter).draw(window);
			//Update lasers using iterator
			for (std::vector<laser>::iterator iter = lasers.begin(); iter != lasers.end(); ++iter) (*iter).draw(window);
			//Update players using iterator
			for (std::vector<player>::iterator iter = players.begin(); iter != players.end(); ++iter) (*iter).draw(window);
		}

		pressed = show_mouse_coords(pressed, sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, players, gen);

		//std::cout << "N_lasers = " << gen->N_lasers << "\n";
		check_reached_end(lasers, gen->N_lasers, gen->laser_x_vels); //If any lasers have reached the end of the screen, delete them
		collision_detect(lasers, players); //Detect collisions between lasers and players
		check_restart(players, lasers, gs, gen);
		
		window.display();
	}

	delete gen; delete gs;
	return 0;
}
