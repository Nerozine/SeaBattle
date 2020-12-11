#include "Gamer.h"
#include <iostream>

RandomGamer::RandomGamer(std::string _name) : turnsCounter(0), name(_name) {
	int tmp[10][10] = {};
	for (int i = 0; i < 100;) {
		int testX = std::rand() % 10;
		int testY = std::rand() % 10;
		if (tmp[testY][testX] == 0) {
			tmp[testY][testX]++;
			turns[i] = std::make_pair(testX, testY);
			i++;
		}
	}
}

SeaBattle::MessageType RandomGamer::makeTurn(bool* turnTransition) {
	bool f = true;
	while (f) {
		f = enemy->getTurn(turns[turnsCounter].first, turns[turnsCounter].second);
		log << "random turn generated to: (" << turns[turnsCounter].first << "," << turns[turnsCounter].second << ")" << std::endl;
		turnsCounter++;
		if (turnsCounter == 100) {
			break;
		}
	}
	*turnTransition = true;
	return SeaBattle::MessageType::HitField;
}

SeaBattle::MessageType RandomGamer::setShip() {
	if (!log.is_open()) {
		if (INCUDE_DATE_IN_NAME_LOG) {
			SYSTEMTIME st;
			GetLocalTime(&st);
			log.open(std::to_string(st.wDay) + "." + std::to_string(st.wMonth) + "." + std::to_string(st.wYear) + "_" +
				std::to_string(st.wHour) + "_" + std::to_string(st.wMinute) + "_" + std::to_string(st.wSecond) + "_" + name + "_rg_game_log.txt");
		}
		else {
			log.open(name + "_rg_game_log.txt");
		}
	}
	while (shipsPlaced != SeaBattle::TotalShips) {
		isVertical = std::rand() % 2;
		currentPos.first = std::rand() % 10;
		currentPos.second = std::rand() % 10;
		if (isSuitablePoint(currentPos.first, currentPos.second, isVertical)) {
			if (isVertical) {
				for (int i = 0; i < SeaBattle::shipSize[shipsPlaced]; i++) {
					log << "ship is at: (" << currentPos.first << "," << currentPos.second - i << ")" << std::endl;
					playfield[currentPos.second - i][currentPos.first] = SeaBattle::Field::Ship;
				}
			}
			else {
				for (int i = 0; i < SeaBattle::shipSize[shipsPlaced]; i++) {
					log << "ship is at: (" << currentPos.first + i << "," << currentPos.second << ")" << std::endl;
					playfield[currentPos.second][currentPos.first + i] = SeaBattle::Field::Ship;
				}
			}
			shipsPlaced++;
		}
	}
	log << "all ships are successfully placed" << std::endl;
	return SeaBattle::MessageType::ShipIsPlaced;
}

void RandomGamer::printWin() {
	log << name << " won!" << std::endl;
}


RandomGamer::~RandomGamer() { log.close(); }