#include "Gamer.h"
#include <iostream>

OptimalGamer::OptimalGamer(std::string _name): veryHighPriorityFieldFlag(false), veryHighPriorityField(std::make_pair(0, 0)),
	direction(SeaBattle::MoveDirection::NoDirection), detectedDirection(false),
	changeDirectionToOpposite(false), name(_name) {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			turns[i][j] = SeaBattle::Field::Empty;
		}
	}
} 

SeaBattle::MessageType OptimalGamer::makeTurn(bool* turnTransition) {
	/*if (enemy->allShipsDestroyed()) {
		*turnTransition = true;
		return SeaBattle::MessageType::Miss;
	}
	if (!availableFields()) {
		std::cout << name <<" no availabale fields to turn" << std::endl;
		log << name << " no availabale fields to turn" << std::endl;
		system("pause");
		exit(3);
	}
	log << name <<" ------------" << std::endl <<"computer's turn" << std::endl;
	if (veryHighPriorityFieldFlag) {
		log << name << " vhp turn generating..." << std::endl;
		std::pair <int, int> turn = generateVeryHighPriorityTurn();
		log << name << " vhp turn generated to: (" << turn.first << "," << turn.second << ")" << std::endl;
		if (enemy->getTurn(turn.first, turn.second)) {
			turns[turn.second][turn.first] = SeaBattle::Field::FiredShip;
			detectedDirection  = true;
			if (enemy->shipIsFullyDestroyed(turn.first, turn.second)) {
				updateTurns(turn.first, turn.second);
				veryHighPriorityFieldFlag = false;
				//reset vht parameters
				direction = SeaBattle::MoveDirection::NoDirection; 
				detectedDirection  = false;
				prevVHPturn = std::make_pair(0, 0);
				changeDirectionToOpposite = false;
			}
			else {
				prevVHPturn = veryHighPriorityField;
				veryHighPriorityField = turn;
			}
			return makeTurn(turnTransition);
		}
		else {
			turns[turn.second][turn.first] = SeaBattle::Field::FiredEmpty;
			if (detectedDirection ) {
				if (!enemy->shipIsFullyDestroyed(prevVHPturn.first, prevVHPturn.second)) {
					changeDirectionToOpposite = true;
				}
			}
		}
	}
	else {
		if (highPriorityFields()) {
			log << name << " hp turn generating..." << std::endl;
			std::pair <int, int> turn = generateHighPriorityTurn();
			log << name << " hp turn generated to: (" << turn.first << "," << turn.second << ")" << std::endl;
			if (enemy->getTurn(turn.first, turn.second)) {
				turns[turn.second][turn.first] = SeaBattle::Field::FiredShip;
				if (enemy->shipIsFullyDestroyed(turn.first, turn.second)) {
					updateTurns(turn.first, turn.second);
				}
				else {
					veryHighPriorityFieldFlag = true;
					veryHighPriorityField = std::make_pair(turn.first, turn.second);
				}
				return makeTurn(turnTransition);
			}
			else {
				turns[turn.second][turn.first] = SeaBattle::Field::FiredEmpty;
			}
		}
		else {
			log << name << " turn generating..." << std::endl;
			std::pair <int, int> turn = generateTurn();
			log << name << " turn generated to: (" << turn.first << "," << turn.second << ")" << std::endl;
			if (enemy->getTurn(turn.first, turn.second)) {
				turns[turn.second][turn.first] = SeaBattle::Field::FiredShip;
				if (enemy->shipIsFullyDestroyed(turn.first, turn.second)) {
					updateTurns(turn.first, turn.second);
				}
				else {
					veryHighPriorityFieldFlag = true;
					veryHighPriorityField = turn;
				}
				return makeTurn(turnTransition);
			}
			else {
				turns[turn.second][turn.first] = SeaBattle::Field::FiredEmpty;
			}
		}
	}
	*turnTransition = true;
	return SeaBattle::MessageType::Miss;*/


	if (enemy->allShipsDestroyed()) {
		log << name << " ------------" << "all enemy ships are destroyed!" << std::endl;
		*turnTransition = true;
		return SeaBattle::MessageType::Miss;
	}
	
	if (veryHighPriorityFieldFlag) {
		log << name << " generating vhp turn..." << std::endl;
		std::pair<int, int> t = generateVeryHighPriorityTurn();
		log << name << " vhp turn generated to (" << t.first << "," << t.second << ")" << std::endl;

		if (enemy->getTurn(t.first, t.second)) {
			turns[t.second][t.first] = SeaBattle::Field::FiredShip;

			veryHighPriorityField = t;

			if (!detectedDirection ) {
				detectedDirection  = true;
			}

			if (enemy->shipIsFullyDestroyed(t.first, t.second)) {
				log << name << " vhp ship is fully destroyed" << std::endl;
				updateTurns(t.first, t.second);

				//reset vhp parameters
				veryHighPriorityFieldFlag = false;
				direction = SeaBattle::MoveDirection::NoDirection;
				detectedDirection  = false;
				changeDirectionToOpposite = false;
			}

			return makeTurn(turnTransition);
		}
		else {
			turns[t.second][t.first] = SeaBattle::Field::FiredEmpty;
			if (detectedDirection ) {
				changeDirectionToOpposite = true;
			}
		}
	
	}
	else {
		log << name << " generating turn..." << std::endl;
		std::pair<int, int> t = generateTurn();
		log << name << " turn generated to (" << t.first << "," << t.second << ")" << std::endl;

		if (enemy->getTurn(t.first, t.second)) {
			turns[t.second][t.first] = SeaBattle::Field::FiredShip;

			if (enemy->shipIsFullyDestroyed(t.first, t.second)) {
				log << name << " ship is fully destroyed" << std::endl;
				updateTurns(t.first, t.second);
			}
			else {
				veryHighPriorityFieldFlag = true;
				veryHighPriorityField = t;
			}

			return makeTurn(turnTransition);
		}

		turns[t.second][t.first] = SeaBattle::Field::FiredEmpty;
	}
	*turnTransition = true;
	return SeaBattle::MessageType::Miss;
}

std::pair<int, int> OptimalGamer::generateVeryHighPriorityTurn() {
	/*if (changeDirectionToOpposite) {
		// opposite direction turn
		changeDirectionToOpposite = false;
		switch (direction) {
		case SeaBattle::MoveDirection::Up: {
			direction = SeaBattle::MoveDirection::Down;
			int i = 0;
			while (turns[veryHighPriorityField.second - i][veryHighPriorityField.first] == SeaBattle::Field::FiredShip) {
				i++;
			}
			return std::make_pair(veryHighPriorityField.first, veryHighPriorityField.second - i);
		}
		case SeaBattle::MoveDirection::Right: {
			direction = SeaBattle::MoveDirection::Left;
			int i = 0;
			while (turns[veryHighPriorityField.second][veryHighPriorityField.first - i] == SeaBattle::Field::FiredShip) {
				i++;
			}
			return std::make_pair(veryHighPriorityField.first - i, veryHighPriorityField.second);
		}
		case SeaBattle::MoveDirection::Down: {
			direction = SeaBattle::MoveDirection::Up;
			int i = 0;
			while (turns[veryHighPriorityField.second + i][veryHighPriorityField.first] == SeaBattle::Field::FiredShip) {
				i++;
			}
			return std::make_pair(veryHighPriorityField.first, veryHighPriorityField.second + i);
		}
		case  SeaBattle::MoveDirection::Left: {
			direction = SeaBattle::MoveDirection::Right;
			int i = 0;
			while (turns[veryHighPriorityField.second][veryHighPriorityField.first + i] == SeaBattle::Field::FiredShip) {
				i++;
			}
			return std::make_pair(veryHighPriorityField.first + i, veryHighPriorityField.second);
		}
		}
	}
	else {
		if (!detectedDirection ) {
			switch (direction) {
			case SeaBattle::MoveDirection::NoDirection: {
				direction = SeaBattle::MoveDirection::Up;
				break;
			}
			case SeaBattle::MoveDirection::Up: {
				direction = SeaBattle::MoveDirection::Right;
				break;
			}
			case SeaBattle::MoveDirection::Right: {
				direction = SeaBattle::MoveDirection::Down;
				break;
			}
			case SeaBattle::MoveDirection::Down: {
				direction = SeaBattle::MoveDirection::Left;
				break;
			}
			default: break;
			}
			switch (direction) {
			case SeaBattle::MoveDirection::Up: {
				if (veryHighPriorityField.second < 9 && turns[veryHighPriorityField.second + 1][veryHighPriorityField.first] != SeaBattle::Field::FiredEmpty) {
					return std::make_pair(veryHighPriorityField.first, veryHighPriorityField.second + 1);
				}
				else {
					return generateVeryHighPriorityTurn();
				}
			}
			case SeaBattle::MoveDirection::Right: {
				if (veryHighPriorityField.first < 9 && turns[veryHighPriorityField.second][veryHighPriorityField.first + 1] != SeaBattle::Field::FiredEmpty) {
					return std::make_pair(veryHighPriorityField.first + 1, veryHighPriorityField.second);
				}
				else {
					return generateVeryHighPriorityTurn();
				}
			}
			case SeaBattle::MoveDirection::Down: {
				if (veryHighPriorityField.second > 0 && turns[veryHighPriorityField.second - 1][veryHighPriorityField.first] != SeaBattle::Field::FiredEmpty) {
					return std::make_pair(veryHighPriorityField.first, veryHighPriorityField.second - 1);
				}
				else {
					return generateVeryHighPriorityTurn();
				}
			}
			case  SeaBattle::MoveDirection::Left: {
				if (veryHighPriorityField.first > 0 && turns[veryHighPriorityField.second][veryHighPriorityField.first - 1] != SeaBattle::Field::FiredEmpty) {
					return std::make_pair(veryHighPriorityField.first - 1, veryHighPriorityField.second);
				}
				else {
					std::cout << "programme failed generating vhp turn" << std::endl;
					printTurns();
					exit(1);
				}
			}
			}
		}
		else {
			switch (direction) {
			case SeaBattle::MoveDirection::Up: {
				if (veryHighPriorityField.second < 9) {
					return std::make_pair(veryHighPriorityField.first, veryHighPriorityField.second + 1);
				}
				break;
			}
			case SeaBattle::MoveDirection::Right: {
				if (veryHighPriorityField.first < 9) {
					return std::make_pair(veryHighPriorityField.first + 1, veryHighPriorityField.second);
				}
				break;
			}
			case SeaBattle::MoveDirection::Down: {
				if (veryHighPriorityField.second > 0) {
					return std::make_pair(veryHighPriorityField.first, veryHighPriorityField.second - 1);
				}
				break;
			}
			case  SeaBattle::MoveDirection::Left: {
				if (veryHighPriorityField.first > 0) {
					return std::make_pair(veryHighPriorityField.first - 1, veryHighPriorityField.second);
				}
				break;
			}
			}
			changeDirectionToOpposite = true;
			return generateHighPriorityTurn();
		}
	}*/
	
	if (changeDirectionToOpposite) {
		changeDirectionToOpposite = false;
		switch (direction) {
		case SeaBattle::MoveDirection::Up: {
			direction = SeaBattle::MoveDirection::Down;
			int i = 0;
			while (turns[veryHighPriorityField.second - i][veryHighPriorityField.first] == SeaBattle::Field::FiredShip) {
				if (veryHighPriorityField.second - i - 1 < 0) {
					std::cout << "prgramm failed while changing direction to opposyte" << std::endl;
					exit(1);
				}
				i++;
			}
			return std::make_pair(veryHighPriorityField.first, veryHighPriorityField.second - i);
		}
		case SeaBattle::MoveDirection::Right: {
			direction = SeaBattle::MoveDirection::Left;
			int i = 0;
			while (turns[veryHighPriorityField.second][veryHighPriorityField.first - i] == SeaBattle::Field::FiredShip) {
				if (veryHighPriorityField.first - i - 1 < 0) {
					std::cout << "prgramm failed while changing direction to opposyte" << std::endl;
					exit(1);
				}
				i++;
			}
			return std::make_pair(veryHighPriorityField.first - i, veryHighPriorityField.second);
		}
		case SeaBattle::MoveDirection::Down: {
			direction = SeaBattle::MoveDirection::Up;
			int i = 0;
			while (turns[veryHighPriorityField.second + i][veryHighPriorityField.first] == SeaBattle::Field::FiredShip) {
				if (veryHighPriorityField.second + i + 1 > 9) {
					std::cout << "prgramm failed while changing direction to opposyte" << std::endl;
					exit(1);
				}
				i++;
			}
			return std::make_pair(veryHighPriorityField.first, veryHighPriorityField.second + i);
		}
		case SeaBattle::MoveDirection::Left: {
			direction = SeaBattle::MoveDirection::Right;
			int i = 0;
			while (turns[veryHighPriorityField.second][veryHighPriorityField.first + i] == SeaBattle::Field::FiredShip) {
				if (veryHighPriorityField.first + i + 1 > 9) {
					std::cout << "prgramm failed while changing direction to opposyte" << std::endl;
					exit(1);
				}
				i++;
			}
			return std::make_pair(veryHighPriorityField.first + i, veryHighPriorityField.second);
		}
		}
	}


	if (!detectedDirection ) {
		switch (direction) {
			case SeaBattle::MoveDirection::NoDirection: {
				direction = SeaBattle::MoveDirection::Up;
				break;
			}
			case SeaBattle::MoveDirection::Up: {
				direction = SeaBattle::MoveDirection::Right;
				break;
			}
			case SeaBattle::MoveDirection::Right: {
				direction = SeaBattle::MoveDirection::Down;
				break;
			}
			case SeaBattle::MoveDirection::Down: {
				direction = SeaBattle::MoveDirection::Left;
				break;
			}
			case SeaBattle::MoveDirection::Left: { // unreachable ?
				std::cout << "programme failed to generate vhp turn at detecting direction";
				exit(0);
			}
		}
	}

	switch (direction) {
	case SeaBattle::MoveDirection::Up: {
		if (veryHighPriorityField.second < 9 && turns[veryHighPriorityField.second + 1][veryHighPriorityField.first] == SeaBattle::Field::Empty) {
			return std::make_pair(veryHighPriorityField.first, veryHighPriorityField.second + 1);
		}
		else {
			if (detectedDirection  &&  (veryHighPriorityField.second >= 9 || turns[veryHighPriorityField.second + 1][veryHighPriorityField.first] != SeaBattle::Field::Empty)) {
				changeDirectionToOpposite = true;
			}
			return generateVeryHighPriorityTurn();
		}
	}
	case SeaBattle::MoveDirection::Right: {
		if (veryHighPriorityField.first < 9 && turns[veryHighPriorityField.second][veryHighPriorityField.first + 1] == SeaBattle::Field::Empty) {
			return std::make_pair(veryHighPriorityField.first + 1, veryHighPriorityField.second);
		}
		else {
			if (detectedDirection  && (veryHighPriorityField.first >= 9|| turns[veryHighPriorityField.second][veryHighPriorityField.first + 1] != SeaBattle::Field::Empty)) {
				changeDirectionToOpposite = true;
			}
			return generateVeryHighPriorityTurn();
		}
	}
	case SeaBattle::MoveDirection::Down: {
		if (veryHighPriorityField.second > 0 && turns[veryHighPriorityField.second - 1][veryHighPriorityField.first] == SeaBattle::Field::Empty) {
			return std::make_pair(veryHighPriorityField.first, veryHighPriorityField.second - 1);
		}
		else {
			if (detectedDirection  && (veryHighPriorityField.second <= 0 || turns[veryHighPriorityField.second - 1][veryHighPriorityField.first] != SeaBattle::Field::Empty)) {
				changeDirectionToOpposite = true;
			}
			return generateVeryHighPriorityTurn();
		}
	}
	case SeaBattle::MoveDirection::Left: {
		if (veryHighPriorityField.first > 0 && turns[veryHighPriorityField.second][veryHighPriorityField.first - 1] == SeaBattle::Field::Empty) {
			return std::make_pair(veryHighPriorityField.first - 1, veryHighPriorityField.second);
		}
		else { // ????
			std::cout << "programme failed to generate vhp turn at making turn";
			exit(0);
		}
	}
	}
}

std::pair<int, int> OptimalGamer::generateTurn() {
	std::pair <int, int> turn;
	while (true) {
		turn.first = std::rand() % 10;
		turn.second = std::rand() % 10;
		if (turns[turn.second][turn.first] == SeaBattle::Field::Empty) {
			return turn;
		}
	}
	//std::pair<int, int>t = turns_test[turnCounter];
	//turnCounter++;
	//return turn;
}

bool OptimalGamer::availableFields() const {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (turns[i][j] == SeaBattle::Field::Empty) {
				return true;
			}
		}
	}
	return false;
}

void OptimalGamer::printTurns() const {
	std::cout << std::endl;
	for (int j = 9; j >= 0; j--) {
		for (int i = 0; i < 10; i++) {
			if (turns[j][i] == SeaBattle::Field::Empty) {
				std::cout << " e";
			}
			if (turns[j][i] == SeaBattle::Field::FiredEmpty) {
				std::cout << " o";
			}
			if (turns[j][i] == SeaBattle::Field::FiredShip) {
				std::cout << " x";
			}
			if (turns[j][i] == SeaBattle::Field::Ship) {
				std::cout << " s";
			}
		}
		std::cout << std::endl;
	}
}

/*bool OptimalGamer::highPriorityFields() const {
	if (nonSingleFieldShipsDestroyed >= SeaBattle::TotalShips - 4) {
		return false;
	}
	for (int j = 0; j < 10; j++) {
		for (int i = 0; i < 5; i++) {
			if (j % 2 == 0) {
				if (turns[j][i * 2] == SeaBattle::Field::Empty) {
					return true;
				}
			}
			else {
				if (turns[j][i * 2 + 1] == SeaBattle::Field::Empty) {
					return true;
				}
			}
		}
	}
	return false;
}*/

void OptimalGamer::updateTurns(int x, int y) {
	if (getFieldFromTurns(x - 1, y) == SeaBattle::Field::FiredShip || getFieldFromTurns(x + 1, y) == SeaBattle::Field::FiredShip) {
		//nonSingleFieldShipsDestroyed++;
		// ship is horizontal
		for (int i = 0; i < 4; i++) {
			if (getFieldFromTurns(x - i, y) == SeaBattle::Field::FiredShip) {
				setFieldsInTurns(x - i, y);
			}
			else {
				break;
			}
		}
		for (int i = 0; i < 4; i++) {
			if (getFieldFromTurns(x + i, y) == SeaBattle::Field::FiredShip) {
				setFieldsInTurns(x + i, y);
			}
			else {
				break;
			}
		}
	}
	else {
	/*if (getFieldFromTurns(x - 1, y) != SeaBattle::Field::FiredShip && getFieldFromTurns(x + 1, y) != SeaBattle::Field::FiredShip
		&& getFieldFromTurns(x, y) == SeaBattle::Field::FiredShip) {
		if (getFieldFromTurns(x, y - 1) == SeaBattle::Field::FiredShip || getFieldFromTurns(x, y + 1) == SeaBattle::Field::FiredShip) {
			nonSingleFieldShipsDestroyed++;
		}*/

		// ship is vertical or 1-field ship
		for (int i = 0; i < 4; i++) {
			if (getFieldFromTurns(x, y - i) == SeaBattle::Field::FiredShip) {
				setFieldsInTurns(x, y - i);
			}
			else {
				break;
			}
		}
		for (int i = 0; i < 4; i++) {
			if (getFieldFromTurns(x, y + i) == SeaBattle::Field::FiredShip) {
				setFieldsInTurns(x, y + i);
			}
			else {
				break;
			}
		}
	}

}

SeaBattle::Field OptimalGamer::getFieldFromTurns(int x, int y) const {
	if (x >= 0 && x <= 9 && y >= 0 && y <= 9) {
		return turns[y][x];
	}
	return SeaBattle::Field::Empty;
}

void OptimalGamer::setFieldsInTurns(int x, int y) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (x - 1 + i >= 0 && x - 1 + i <= 9 && y - 1 + j >= 0 && y - 1 + j <= 9) {
				if (turns[y - 1 + j][x - 1 + i] != SeaBattle::Field::FiredShip && turns[y - 1 + j][x - 1 + i] != SeaBattle::Field::FiredEmpty) {
					turns[y - 1 + j][x - 1 + i] = SeaBattle::Field::FiredEmpty;
					log << name << " field at (" << x - 1 + i << "," << y - 1 + j << ")" << " marked as FierdEmpty" << std::endl;
				}
			}
		}
	}
}

/*std::pair <int, int> OptimalGamer::generateHighPriorityTurn() const {
	while (true) {
		int x;
		int y = std::rand() % 10;
		if (y % 2 == 0) {
			x = (std::rand() % 5) * 2;
		}
		else {
			x = (std::rand() % 5) * 2 + 1;
		}
		if (turns[y][x] != SeaBattle::Field::FiredEmpty && turns[y][x] != SeaBattle::Field::FiredShip) {
			return std::make_pair(x, y);
		}
	}
}*/

SeaBattle::MessageType OptimalGamer::setShip() {
	if (!log.is_open()) {
		if (INCUDE_DATE_IN_NAME_LOG) {
			SYSTEMTIME st;
			GetLocalTime(&st);
			log.open(std::to_string(st.wDay) + "." + std::to_string(st.wMonth) + "." + std::to_string(st.wYear) + "_" +
				std::to_string(st.wHour) + "_" + std::to_string(st.wMinute) + "_" + std::to_string(st.wSecond) + "_" + name + "_og_game_log.txt");
		}
		else {
			log.open(name + "_og_game_log.txt");
		}
	}

	while (shipsPlaced != SeaBattle::TotalShips) {
		currentPos.first = std::rand() % 10;
		currentPos.second = std::rand() % 10;
		if (currentPos.first == 9 || currentPos.first == 0 || currentPos.second == 9 || currentPos.second == 0 || SeaBattle::shipSize[shipsPlaced] == 1) {
			if (currentPos.first == 9 || currentPos.first == 0) {
				isVertical = true;
			}
			else {
				isVertical = false;
			}
			if (isSuitablePoint(currentPos.first, currentPos.second, isVertical)) {
				if (isVertical) {
					for (int i = 0; i < SeaBattle::shipSize[shipsPlaced]; i++) {
						log << name << " ship is at: (" << currentPos.first << "," << currentPos.second - i << ")" << std::endl;
						playfield[currentPos.second - i][currentPos.first] = SeaBattle::Field::Ship;
					}
				}
				else {
					for (int i = 0; i < SeaBattle::shipSize[shipsPlaced]; i++) {
						log << name << " ship is at: (" << currentPos.first + i << "," << currentPos.second << ")" << std::endl;
						playfield[currentPos.second][currentPos.first + i] = SeaBattle::Field::Ship;
					}
				}
				shipsPlaced++;
			}
		}
	}

	log << name << " all ships are successfully placed" << std::endl;
	return SeaBattle::MessageType::ShipIsPlaced;
}


void OptimalGamer::printWin() {
	log << name << " won!" << std::endl;
}

OptimalGamer::~OptimalGamer() { log.close(); }