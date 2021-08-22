#include "iostream"
#include "gSettings.h"

gSettings::gSettings(int i, int sL, const std::string& fname, const std::string& lname){
	sampleLimit = sL;
	play.loadFromFile(fname);
	lasertexture.loadFromFile(lname);
	nplayers = i;
}

generation::generation(std::string nombre){
	N_lasers = 0;
	N_death_cycles = 0;
	name = nombre;
	std::cout << "New generation " << name << "\n";
}