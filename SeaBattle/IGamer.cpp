#include "Gamer.h"
#include <iostream>

SeaBattle::MoveDirection getDirection(sf::Keyboard::Key k) {
	switch (k) {
	case sf::Keyboard::Left: return SeaBattle::MoveDirection::Left;
	case sf::Keyboard::Right: return SeaBattle::MoveDirection::Right;
	case sf::Keyboard::Down: return SeaBattle::MoveDirection::Down;
	case sf::Keyboard::Up: return SeaBattle::MoveDirection::Up;
	default: {
		std::cout << "programm can't get direction" << std::endl;
		exit(1);
	}
	}
}

IGamer::IGamer(): shipsPlaced(0), currentPos(std::make_pair(0, 0)), isVertical(false), enemy(nullptr), destroyedShipFields(0) {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			playfield[i][j] = SeaBattle::Field::Empty;
		}
	}
}

IGamer::~IGamer() { };

bool IGamer::isSuitablePoint(int x, int y, bool isVertical) const {
	if (isVertical) {
		for (int i = 0; i < SeaBattle::shipSize[shipsPlaced]; i++) {
			if (!this->isSuitableVicinity(x, y - i)) {
				return false;
			}
		}
	}
	else {
		for (int i = 0; i < SeaBattle::shipSize[shipsPlaced]; i++) {
			if (!this->isSuitableVicinity(x + i, y)) {
				return false;
			}
		}
	}
	return true;
}

bool IGamer::isSuitableVicinity(int x, int y) const {
	if (x < 0 || x > 9 || y < 0 || y > 9) {
		return false;
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (y - 1 + i >= 0 && y - 1 + i < 10 && x - 1 + j >= 0 && x - 1 + j < 10) {
				if (playfield[y - 1 + i][x - 1 + j] != SeaBattle::Field::Empty) {
					return false;
				}
			}
		}
	}
	return true;
}

bool IGamer::destroyedShipField(int x, int y) const {
	return false;
}

SeaBattle::Field IGamer::getField(int x, int y) const {
	if (x >= 0 && x < 10 && y >= 0 && y < 10) {
		return playfield[y][x];
	}
	else {
		return SeaBattle::Field::Empty;
	}
}

bool IGamer::isShipField(int x, int y) const {
	if (isVertical) {
		if (x == currentPos.first) {
			if (currentPos.second - SeaBattle::shipSize[shipsPlaced] < y && y <= currentPos.second) {
				return true;
			}
		}
	}
	else {
		if (y == currentPos.second) {
			if (currentPos.first <= x && x < currentPos.first + SeaBattle::shipSize[shipsPlaced]) {
				return true;
			}
		}
	}
	return false;
}

void IGamer::setEnemy(IGamer* player) {
	if (enemy == nullptr) {
		enemy = player;
	}
}

bool IGamer::firedPoint(int x, int y) const {
	if (playfield[y][x] == SeaBattle::Field::FiredEmpty || playfield[y][x] == SeaBattle::Field::FiredShip) {
		return true;
	}
	return false;
}

std::pair<int, int> IGamer::getCurrentPos() const {
	return std::make_pair(currentPos.first, currentPos.second);
}

SeaBattle::Field IGamer::getFieldForDraw(int x, int y) const {
	return playfield[y][x];
}

int IGamer::getPlacedShips() const {
	return shipsPlaced;
}

bool IGamer::canMove(SeaBattle::MoveDirection direction, SeaBattle::GameStage stage) const {
	if (stage == SeaBattle::GameStage::PlacingShips) {
		if (this->isVertical) {
			if (direction == SeaBattle::MoveDirection::Down) {
				if (currentPos.second >= SeaBattle::shipSize[shipsPlaced]) {
					return true;
				}
				else {
					return false;
				}
			}
		}
		else {
			if (direction == SeaBattle::MoveDirection::Right) {
				if (currentPos.first <= 9 - SeaBattle::shipSize[shipsPlaced]) {
					return true;
				}
				else {
					return false;
				}
			}
		}
	}
	switch (direction) {
	case SeaBattle::MoveDirection::Down: {
		if (currentPos.second > 0) {
			return true;
		}
		return false;
	}
	case SeaBattle::MoveDirection::Up: {
		if (currentPos.second < 9) {
			return true;
		}
		return false;
	}
	case SeaBattle::MoveDirection::Left: {
		if (currentPos.first > 0) {
			return true;
		}
		return false;
	}
	case SeaBattle::MoveDirection::Right: {
		if (currentPos.first < 9) {
			return true;
		}
		return false;
	}
	default: {
		std::cout << "programme failed to detect direction" << std::endl;
		exit(1);
	}
	}
}

void IGamer::makeMove(SeaBattle::MoveDirection direction) {
	switch (direction) {
	case SeaBattle::MoveDirection::Down: {
		currentPos.second--;
		break;
	}
	case SeaBattle::MoveDirection::Left: {
		currentPos.first--;
		break;
	}
	case SeaBattle::MoveDirection::Up: {
		currentPos.second++;
		break;
	}
	case SeaBattle::MoveDirection::Right: {
		currentPos.first++;
		break;
	}
	default: {
		std::cout << "programme failed to detect direction" << std::endl;
		exit(1);
	}
	}
}

bool IGamer::canRotate() const {
	if (isVertical) {
		if (currentPos.first <= 9 - SeaBattle::shipSize[shipsPlaced] + 1) {
			return true;
		}
	}
	else {
		if (currentPos.second >= SeaBattle::shipSize[shipsPlaced] - 1) {
			return true;
		}
	}
	return false;
}

void IGamer::rotate() {
	isVertical = !isVertical;
}

bool IGamer::allShipsDestroyed() const {
	return (destroyedShipFields == SeaBattle::TotalShipFields);
}

bool IGamer::shipIsFullyDestroyed(int x, int y) const {
	// Left check
	for (int i = 1; i < 4; i++) {
		SeaBattle::Field f = getField(x + i, y);
		if (f == SeaBattle::Field::Ship) {
			//std::cout << "(" << x + i << "," << y << ")" << "is a ship field so ship is not fully destroyed" <<  std::endl;
			return false;
		}
		if (f != SeaBattle::Field::FiredShip) {
			break;
		}
	}
	// Right check
	for (int i = 1; i < 4; i++) {
		SeaBattle::Field f = getField(x - i, y);
		if (f == SeaBattle::Field::Ship) {
			//std::cout << "(" << x - i << "," << y << ")" << "is a ship field so ship is not fully destroyed" << std::endl;
			return false;
		}
		if (f != SeaBattle::Field::FiredShip) {
			break;
		}
	}
	// Up check
	for (int i = 1; i < 4; i++) {
		SeaBattle::Field f = getField(x, y + i);

		if (f == SeaBattle::Field::Ship) {
			//std::cout << "(" << x << "," << y + i << ")" << "is a ship field so ship is not fully destroyed" << std::endl;
			return false;
		}
		if (f != SeaBattle::Field::FiredShip) {
			break;
		}
	}
	// Down check
	for (int i = 1; i < 4; i++) {
		SeaBattle::Field f = getField(x, y - i);
		if (f == SeaBattle::Field::Ship) {
			//std::cout << "(" << x << "," << y - i << ")" << "is a ship field so ship is not fully destroyed" << std::endl;
			return false;
		}
		if (f != SeaBattle::Field::FiredShip) {
			break;
		}
	}
	return true;
}

bool IGamer::getTurn(int x, int y) {
	if (playfield[y][x] == SeaBattle::Field::Ship) {
		playfield[y][x] = SeaBattle::Field::FiredShip;
		destroyedShipFields++;
		return true;
	}
	else {
		playfield[y][x] = SeaBattle::Field::FiredEmpty;
	}
	return false;
}