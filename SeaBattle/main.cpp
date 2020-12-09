//#include "optionparser.h"
#include "Gamer.h"
#include "Game.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <istream>
#include <string>
#include <fstream>

enum  optionIndex { UNKNOWN, HELP, COUNT, FIRST, SECOND  };


int main(int argc, char* argv[]) {
	/*SYSTEMTIME st;
	GetSystemTime(&st);
	log.open(std::to_string(st.wDay) + "." + std::to_string(st.wMonth) + "." + std::to_string(st.wYear) + "_" +
		std::to_string(st.wHour) + "_" + std::to_string(st.wMinute) + "_" + std::to_string(st.wSecond) + "_game_log.txt");*/

	std::srand(std::time(nullptr));
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Sea Battle");
	Game SeaBattle(window);
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