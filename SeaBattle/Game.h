#pragma once
#include <SFML/Graphics.hpp>
#include "Gamer.h"

class Game  {
private:
	bool drawTurnTransition;
	bool nextGameInSeries;
	bool newSeries;

	bool choosingFirstPlayer;

	std::pair<int, int> seriesScore;
	IGamer* winner;
	IGamer* playerA;
	IGamer* playerB;
	IGamer* playersTurn;
	SeaBattle::GameStage stage;
	SeaBattle::MessageType message;
	bool turnTransition;
	sf::Font font;
	sf::RenderWindow& window;

	void setWinner();

	void processMessage(std::string &toWrite) const;
	void processKeyTurnTransition();
	void processResultScreen(sf::Keyboard::Key k);

	void drawEnemyPlayField(sf::RenderWindow& window, double x_start, double y_start) const;
	void drawPlayerPlayField(sf::RenderWindow& window, double x_start, double y_start) const;
	void drawPlayFields(sf::RenderWindow& window) const;

	void drawMenu(sf::RenderWindow& window) const;
	void drawMainFieldBorder(sf::RenderWindow& window, double start_x, double start_y) const;
	void drawSeriesScore(sf::RenderWindow& window) const;
	void drawPlacingShipsField(sf::RenderWindow& window) const;
	void drawPlacingShipsControl(sf::RenderWindow& window) const;
	void drawGameControl(sf::RenderWindow& window) const;
	void drawTurnTransitionMenu() const;

	const double deltaFields = 30.f;
	const double fieldSize = 35.f;
	const double fieldThicness = 2.f;
	const double delta = 3.f;
	const double MainFieldSize = 10 * (fieldSize + fieldThicness * 2 + delta) + delta;
	const std::pair<double, double> gameControlTextPos = std::make_pair(950.f, 90.f);
	const std::pair<double, double> mainFieldStartPos = std::make_pair(20.f, 60.f);
	const std::pair<double, double> gameDescriptionTextPos = std::make_pair(100.f, 540.f);
	const int placingShipsControlTextCharacterSize = 20;
	const int gameDescriptionTextCharacteSize = 20;
	const int gameControlTextCharacterSize = 20;
	const std::pair<double, double> placingShipsControlTextPos = std::make_pair(500.f, 90.f);
	const int turnTransitionCharacterSize = 30;

	sf::Color FiredEmptyColor = sf::Color(255, 127, 80);
	sf::Color CurrentFieldColor = sf::Color::Yellow;
	sf::Color FiredShipColor = sf::Color::Red;
	sf::Color EnemyShipColor = sf::Color::White;
	sf::Color PlayersShipColor = sf::Color(82, 255, 184);

public:
	bool nextGameInSeriesFlag() const;
	bool newSeriesFlag() const;

	void menu(sf::RenderWindow& window);
	void init();

	void processStage();
	void processKey(sf::Keyboard::Key k);
	void processTurnTransition();
	void processTurn();

	void drawGame(sf::RenderWindow &window);

	IGamer* getTurnPlayer() const;
	IGamer* getWinner() const;

	explicit Game(sf::RenderWindow& window);
	~Game();
};