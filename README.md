This C++ code trains a neural network to play the game Dodge Lasers at human skill level or better. The original, human playable game, developed in JavaScript is available here. https://mrmultimediator.github.io/Dodge_Lasers/ (hold spacebar to accelerate upwards)

General:

This code trains a neural net to dodge lasers using a genetic algorithm. After the current generation has been exposed to the requested number of lasers, a new generation is created. Every generation, generational data are saved (appended) to "data/g_data.txt", and player data are saved (appended) to "data/p_data.txt". Additionally, every time all players die, the current generation is saved in "data/temp_data.txt", which is loadable from the command line so that users can pick up where they left off.

To start a new lineage and preserve the data from the previous lineages (g_data and p_data) without contaminating lineages with one another, run "cd data && ./mv.sh && cd .." This will preserve up to 26 lineages.

Hardcoded Parameters:

1.) The number of players and the number lasers required to move to the next generation can be changed by modifying the creation of the game settings object in "main.cpp" (1st and 2nd parameter in the code below, respectively) gSettings *gs = new gSettings(50,15,"assets/player.png","assets/spear.png",0.5);

2.) The selection criteria which decides which players are selected for cloning and mutation for the following generation are hardcoded in the "select" function in player.cpp

3.) The y positions of the lasers are set to sample the height of the screen uniformly. If you want the lasers to only sample the center of the screen for initial training purposes, comment out the 2nd line in the constructor "laser::laser" and uncomment the third line. This speeds up initial training because the first generation is immobile by default.

Compilation/Running Instructions:

1.) Run "./compile.sh" to compile in Linux. This has only been tested in Ubuntu Linux using Windows Subsystem for Linux in Windows 10.

2.) Run "./DL N None" to begin training with a new lineage. N = Number of lasers.

3.) Run "./DL N data/temp_data.txt" to continue training with the most recently saved or autosaved generation of players.

4.) Left click the screen to manually save the current generation to "data/temp_data"
