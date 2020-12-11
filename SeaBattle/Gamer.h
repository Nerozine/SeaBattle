#pragma once
#include <utility>
#include <vector>
#include <map>
#include <istream>
#include <iostream>
#include <fstream>

#include <string>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#undef min
#undef max

namespace SeaBattle {
	const int shipSize[10] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
	const int TotalShips = 10;
	const int TotalShipFields = 4 + 3 * 2 + 2 * 3 + 1 * 4;
	enum class GameStage { PlacingShips, Game, ResultScreen };
	enum class MoveDirection { NoDirection, Down, Up, Left, Right };
	enum class Field { Empty, Ship, FiredShip, FiredEmpty };
	enum class MessageType { EmptyMessage, ShipHit, ShipDestroyed, HitField, Miss, CantRotate, ShipIsPlaced,
						CantPlaceShip };
}

SeaBattle::MoveDirection getDirection(sf::Keyboard::Key k);

class IGamer {
protected:
	int destroyedShipFields;
	SeaBattle::Field playfield[10][10];
	int shipsPlaced;
	std::pair<int, int> currentPos;
	bool isVertical;
	IGamer* enemy;
	IGamer();
	bool isSuitablePoint(int x, int y, bool isVertical) const;
	bool isSuitableVicinity(int x, int y) const;
	bool destroyedShipField(int x, int y) const;
	SeaBattle::Field getField(int x, int y) const;
public:
	bool isShipField(int x, int y) const;
	bool allShipsDestroyed() const;
	bool shipIsFullyDestroyed(int x, int y) const;
	bool getTurn(int x, int y);
	void setEnemy(IGamer* player);
	virtual SeaBattle::MessageType makeTurn(bool* turnTransition) = 0;
	bool firedPoint(int x, int y) const;
	virtual SeaBattle::MessageType setShip() = 0;
	int getPlacedShips() const;
	bool canMove(SeaBattle::MoveDirection direction, SeaBattle::GameStage stage) const;
	void makeMove(SeaBattle::MoveDirection direction);
	bool canRotate() const;
	void rotate();
	virtual ~IGamer() = 0;
	std::pair<int, int> getCurrentPos() const;
	SeaBattle::Field getFieldForDraw(int x, int rev_y) const;
};

class RandomGamer : public IGamer {
private:
	bool INCUDE_DATE_IN_NAME_LOG = true;

	std::string name;
	std::ofstream log;

	std::pair<int, int> turns[100];
	int turnsCounter;
public:
	explicit RandomGamer(std::string _name);
	virtual SeaBattle::MessageType makeTurn(bool* turnTransition) override;
	virtual SeaBattle::MessageType setShip() override;
	~RandomGamer();

	void printWin();
};

class ConsoleGamer : public IGamer {
private:
	std::string name;
public:
	explicit ConsoleGamer(std::string _name);
	virtual SeaBattle::MessageType makeTurn(bool* turnTransition) override;
	virtual SeaBattle::MessageType setShip() override;
	~ConsoleGamer();
};

class OptimalGamer : public IGamer {
private:
	bool INCUDE_DATE_IN_NAME_LOG = true;
	bool availableFields() const;
	void printTurns() const;

	const std::string name;
	std::ofstream log;

	SeaBattle::Field turns[10][10];
	bool veryHighPriorityFieldFlag;
	std::pair <int, int> veryHighPriorityField;

	std::pair <int, int> generateVeryHighPriorityTurn();
	std::pair <int, int> generateTurn();

	void updateTurns(int x, int y);
	SeaBattle::Field getFieldFromTurns(int x, int y) const;
	void setFieldsInTurns(int x, int y);

	SeaBattle::MoveDirection direction;
	bool detectedDirection;
	bool changeDirectionToOpposite;
public:
	explicit OptimalGamer(std::string _name);
	virtual SeaBattle::MessageType makeTurn(bool* turnTransition) override;
	virtual SeaBattle::MessageType setShip() override;
	~OptimalGamer();

	void printWin();
};