#include "Gamer.h"

ConsoleGamer::ConsoleGamer(std::string _name) {
	name = _name;
	currentPos.first = 0;
	currentPos.second = 0;
}

SeaBattle::MessageType ConsoleGamer::makeTurn(bool* turnTransition) {
	if (!enemy->firedPoint(currentPos.first, currentPos.second)) {
		if (enemy->getTurn(currentPos.first, currentPos.second)) {
			*turnTransition = false;
			if (enemy->shipIsFullyDestroyed(currentPos.first, currentPos.second)) {
				return SeaBattle::MessageType::ShipDestroyed;
			}
			return SeaBattle::MessageType::ShipHit;
		}
		else {
			*turnTransition = true;
			return SeaBattle::MessageType::Miss;
		}
	}
	else {
		*turnTransition = false;
		return SeaBattle::MessageType::HitField;
	}
}

SeaBattle::MessageType ConsoleGamer::setShip() {
	if (isSuitablePoint(currentPos.first, currentPos.second, isVertical)) {
		if (isVertical) {
			for (int i = 0; i < SeaBattle::shipSize[shipsPlaced]; i++) {
				playfield[currentPos.second - i][currentPos.first] = SeaBattle::Field::Ship;
			}
		}
		else {
			for (int i = 0; i < SeaBattle::shipSize[shipsPlaced]; i++) {
				playfield[currentPos.second][currentPos.first + i] = SeaBattle::Field::Ship;
			}
		}
		shipsPlaced++;
		return SeaBattle::MessageType::ShipIsPlaced;
	}
	return SeaBattle::MessageType::CantPlaceShip;
}

ConsoleGamer::~ConsoleGamer() { }