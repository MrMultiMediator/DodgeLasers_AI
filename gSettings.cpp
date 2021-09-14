#include "iostream"
#include "file_io.h"
#include "fstream"
#include "gSettings.h"

gSettings::gSettings(int i, int sL, const std::string& fname, const std::string& lname, double std_s){
	sampleLimit = sL;
	play.loadFromFile(fname);
	lasertexture.loadFromFile(lname);
	nplayers = i;
	std_scale = std_s; // How many standard deviations to look ahead for player survival time cutoff (i.e. which are selected for cloning and mutation)
}

generation::generation(std::string nombre, std::string parent, int nl){
	n_LOS = nl;
	mmag = 0.05;
	mutate_style = "abs";
	N_lasers = 0;
	N_death_cycles = 0;
	name = nombre;
	parent_gen = parent;
	std::cout << "New generation " << name << "\n";
}

void generation::birth(std::string nombre, std::string parent){
	N_lasers = 0;
	N_death_cycles = 0;
	name = nombre;
	parent_gen = parent;
	std::cout << "New generation " << name << "\n";
}

void generation::reload(std::string filename){
	/// Reload a generation from file.
	std::string	line;
	std::vector<std::string> vline;

	if (filename == "None"){
		std::cout << "Proceeding with randomly generated player set.\n";
		return;
	}

	std::cout << "Loading player neural nets from file " << filename << ".\n";

	//std::cout << "Name = " << name << "\n";

	std::ifstream inp(filename);

	std::getline(inp, line);
	split2(line,vline);
	name = vline[1];

	vline.clear();
	std::getline(inp, line);
	split2(line, vline);
	parent_gen = vline[1];

	vline.clear();
	std::getline(inp, line);
	split2(line, vline);
	N_lasers = stoi(vline[1]);

	vline.clear();
	std::getline(inp, line);
	split2(line, vline);
	N_death_cycles = stoi(vline[1]);

	vline.clear();
	std::getline(inp, line);
	split2(line, vline);
	
	for (std::vector<std::string>::iterator itvline = vline.begin()+1; itvline != vline.end(); ++itvline){
		laser_x_vels.push_back(std::stod(*itvline));
	}

	//std::cout << "Name = " << name << "\n";

	inp.close();
}