#include "iostream"
#include "fstream"
#include "player.h"
#include "gSettings.h"

//void write_temp_data(int N_lasers, std::vector<double> laser_x_vels, std::vector<player> players)
void write_temp_data(generation *gen, std::vector<player> players){
	std::ofstream temp_file;
	temp_file.open("temp_data.txt");
	temp_file << "N_lasers: " << gen->N_lasers << "\n" << "laser_x_vels: ";

	// Write laser x velocities to the file
	for (int i = 0; i < gen->laser_x_vels.size(); i++){
		temp_file << gen->laser_x_vels[i] << " ";
	}
	temp_file << "\n";

	//for (int i = 0; i < players.size(); i++){

	//}

	temp_file.close();
}