#include "iostream"
#include "fstream"
#include "player.h"
#include "gSettings.h"
#include <sstream>

//void write_temp_data(int N_lasers, std::vector<double> laser_x_vels, std::vector<player> players)
void write_temp_data(generation *gen, std::vector<player> players){
	/// Write readily reloadable information about the current generation and players to allow the user to continue training where they left off
	std::ofstream temp_file;
	temp_file.open("data/temp_data.txt");
	temp_file << "Name: " << gen->name << "\nParent_gen: " << gen->parent_gen << "\nN_lasers: " << gen->N_lasers << "\nN_death_cycles: " << gen->N_death_cycles << "\nlaser_x_vels: ";

	// Write laser x velocities to the file
	for (int i = 0; i < gen->laser_x_vels.size(); i++){
		temp_file << gen->laser_x_vels[i] << " ";
	}
	temp_file << "\n";

	for (int i = 0; i < players.size(); i++){
		temp_file << "Survival_times: ";
		for (int j = 0; j < players[i].st_arr.size(); j++){
			temp_file << players[i].st_arr[j] << " ";
		}
		temp_file << "\n";

		temp_file << "Weights: ";
		for (int j = 0; j < players[i].NN.weights.size(); j++){
			temp_file << players[i].NN.weights[j] << " ";
		}
		temp_file << "\n";

		temp_file << "Bias: " << players[i].NN.bias << "\n";
	}

	temp_file.close();
}

void split2(const std::string& str, std::vector<std::string>& cont, char delim = ' '){
	/// Split a string that is delimited by spaces and append each piece to a vector of strings called cont
	std::stringstream ss(str);
	std::string token;
	while (std::getline(ss, token, delim)){
		cont.push_back(token);
	}
}