#include "iostream"
#include "player.h"
#include "gSettings.h"

//void write_temp_data(int N_lasers, std::vector<double> laser_x_vels, std::vector<player> players);
void write_temp_data(generation *gen, std::vector<player> players);
void write_gen(generation *gen);
void write_players(generation *gen, std::vector<player> players);

void split2(const std::string& str, std::vector<std::string>& cont, char delim = ' ');