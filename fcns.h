#include <SFML/Graphics.hpp>
#include "player.h"
#include "gSettings.h"

double rand_gen();
double normalRandom();
double norm_dist(double mean, double std_dev);
bool show_mouse_coords(bool, int, int, std::vector<player> &players, generation *gen);
bool checkreturn(sf::Event, bool);
std::string gen_random(const int len);