#include "iostream"
#include <SFML/Graphics.hpp>
class gSettings
{

public:
	int nplayers;
	sf::Texture play;
	sf::Texture lasertexture;
	gSettings(int, const std::string& fname, const std::string& lname);
};