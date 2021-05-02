#include "iostream"
#include "gSettings.h"

gSettings::gSettings(int i, const std::string& fname, const std::string& lname){
	play.loadFromFile(fname);
	lasertexture.loadFromFile(lname);
	nplayers = i;
}