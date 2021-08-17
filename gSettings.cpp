#include "iostream"
#include "gSettings.h"

gSettings::gSettings(int i, int sL, const std::string& fname, const std::string& lname){
	sampleLimit = sL;
	play.loadFromFile(fname);
	lasertexture.loadFromFile(lname);
	nplayers = i;
	lasers_ever = 0;
}