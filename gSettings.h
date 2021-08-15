#include "iostream"
#include <SFML/Graphics.hpp>
class gSettings
{

public:
	//Number of players, and the number of lasers that the current generation of players has seen
	int nplayers, lasers_ever;
	sf::Texture play;
	sf::Texture lasertexture;
	gSettings(int, const std::string& fname, const std::string& lname);
};