#include <tclap/CmdLine.h>
#include "Gamer.h"
#include "Game.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <istream>
#include <string>
#include <fstream>

void printPlayerParamInfo() {
	std::cout << "\tcorrect values are: " << std::endl;
	std::cout << "\t\tconsole\tfor setting console player" << std::endl;
	std::cout << "\t\trandom\tfor setting random player" << std::endl;
	std::cout << "\t\toptimal\tfor setting optimal player" << std::endl;
}

void parseCommandLineArgs(int argc, char** argv, bool& setFPT, char& FPT, bool& setSPT, char& SPT,bool& setNR, int& NR) {

	std::string s = "no";

	// Wrap everything in a try block.  Do this every time, 
	// because exceptions will be thrown for problems.
	try {

		// Define the command line object, and insert a message
		// that describes the program. The "Command description message" 
		// is printed last in the help text. The second argument is the 
		// delimiter (usually space) and the last one is the version number. 
		// The CmdLine object parses the argv array based on the Arg objects
		// that it contains. 
		TCLAP::CmdLine cmd("Command description message", ' ', "0.9", false);

		// Define a value argument and add it to the command line.
		// A value arg defines a flag and a type of value that it expects,.
		TCLAP::ValueArg<int> countArg("c", "count", "Set number of rounds to play", false, -1, "int");
		TCLAP::ValueArg<std::string> firstPlayerTypeArg("f", "first", "Set first player type", false, s, "string");
		TCLAP::ValueArg<std::string> secondPlayerTypeArg("s", "second", "Set second player type", false, s, "string");

		// Add the argument nameArg to the CmdLine object. The CmdLine object
		// uses this Arg to parse the command line.
		cmd.add(countArg);
		cmd.add(firstPlayerTypeArg);
		cmd.add(secondPlayerTypeArg);

		// Define a switch and add it to the command line.
		// A switch arg is a boolean argument and only defines a flag that
		// indicates true or false.  In this example the SwitchArg adds itself
		// to the CmdLine object as part of the constructor.  This eliminates
		// the need to call the cmd.add() method.  All args have support in
		// their constructors to add themselves directly to the CmdLine object.
		// It doesn't matter which idiom you choose, they accomplish the same thing.
		TCLAP::SwitchArg helpSwitch("h", "help", "Print commands description", cmd, false);

		// Parse the argv array.
		cmd.parse(argc, argv);

		// Get the value parsed by each arg. 
		int count = countArg.getValue();
		std::string firstPlayerType = firstPlayerTypeArg.getValue();
		std::string secondPlayerType = secondPlayerTypeArg.getValue();
		bool help = helpSwitch.getValue();

		// Do what you intend. 

		if (help) {
			std::cout << "-h\t-help" << helpSwitch.getDescription() << std::endl;
			std::cout << "-c\t-count" << countArg.getDescription() << std::endl;
			std::cout << "-f\t-first" << firstPlayerTypeArg.getDescription() << std::endl;
			printPlayerParamInfo();
			std::cout << "-s\t-second" << secondPlayerTypeArg.getDescription() << std::endl;
			printPlayerParamInfo();
			std::cout << "all launch parameters are optional" << std::endl;
			exit(0);
		}

		if (firstPlayerType != s) {
			setFPT = true;
			if (firstPlayerType == std::string("random")) {
				FPT = 'r';
			}
			else if (firstPlayerType == std::string("optimal")) {
				FPT = 'o';
			}
			else if (firstPlayerType == std::string("console")) {
				FPT = 'c';
			}
			else {
				std::cout << "Error: incorrect format for first player type!" << std::endl;
				exit(0);
			}
			std::cout << "first player type successfully set!" << std::endl;
		}

		if (secondPlayerType != s) {
			setSPT = true;
			if (secondPlayerType == std::string("random")) {
				SPT = 'r';
			}
			else if (secondPlayerType == std::string("optimal")) {
				SPT = 'o';
			}
			else if (secondPlayerType == std::string("console")) {
				SPT = 'c';
			}
			else {
				std::cout << "Error: incorrect format for second player type!" << std::endl;
				exit(0);
			}
			std::cout << "second player type successfully set!" << std::endl;
		}

		if (count > 0) {
			setNR = true;
			NR = count;
			std::cout << "count successfully set!" << std::endl;
		}
		else {
			if (count != -1) {
				std::cout << "Error: incorrect format for count!" << std::endl;
				exit(0);
			}
		}

	}
	catch (TCLAP::ArgException& e)  // catch any exceptions
	{
		std::cerr << "error: " << e.error() << std:: endl << " for arg " << e.argId() << std::endl;
	}
}

int main(int argc, char* argv[]) {

	bool setFPT = false; //set first playertype from console
	bool setSPT = false; //set second playertype from console
	bool setNR = false; // set number of rounds to play from console

	char FPT = 0;
	char SPT = 0;
	int NR = -1;

	parseCommandLineArgs(argc, argv, setFPT, FPT, setSPT, SPT, setNR, NR);

	std::srand(std::time(nullptr));
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Sea Battle");
	Game SeaBattle(window, setFPT, FPT, setSPT, SPT, setNR, NR);
	window.setFramerateLimit(60);

	bool series = true;
	bool game = true;
	
	while (series) {
		SeaBattle.menu(window);
		while (game) {
			SeaBattle.init();
			sf::Event event;
			while (window.isOpen()) {
				// process game stage 
				SeaBattle.processStage();
				// process player's turn
				if (dynamic_cast<ConsoleGamer*>(SeaBattle.getTurnPlayer()) == nullptr && SeaBattle.getWinner() == nullptr) {
					SeaBattle.processTurn();
				}
				else {
					while (window.pollEvent(event)) {
						if (event.type == sf::Event::Closed) {
							window.close();
						}
						if (event.type == sf::Event::KeyPressed) {
							SeaBattle.processKey(event.key.code);
						}
						if (event.type == sf::Event::Resized) {
							double w = static_cast<double>(event.size.width);
							double h = static_cast<double>(event.size.height);
							window.setView(sf::View(sf::Vector2f(w / 2.0, h / 2.0), sf::Vector2f(w, h)));
						}
					}
				}
				// update display
				SeaBattle.drawGame(window);
				// process turn transition 
				SeaBattle.processTurnTransition();
				if (SeaBattle.nextGameInSeriesFlag() || SeaBattle.newSeriesFlag()) {
					break;
				}
			}
			if (!SeaBattle.newSeriesFlag() && !SeaBattle.nextGameInSeriesFlag()) {
				std::cout << "normal exit" << std::endl;
				exit(0);
			}
			if (SeaBattle.newSeriesFlag()) {
				break;
			}
		}
	}
	return 0;
}