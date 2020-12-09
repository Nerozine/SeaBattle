#include "Game.h"

Game::Game(sf::RenderWindow& window) :
	playerA(nullptr), playerB(nullptr), winner(nullptr),
	stage(SeaBattle::GameStage::PlacingShips), turnTransition(false),
	playersTurn(nullptr), message(SeaBattle::MessageType::EmptyMessage),
	seriesScore(std::make_pair(0,0)), nextGameInSeries(false), newSeries(false),
	drawTurnTransition(false), window(window) , choosingFirstPlayer(true) {
	if (!font.loadFromFile("9303.ttf")) {
		std::cout << "programme failed loading font" << std::endl;
		exit(1);
	}
}

bool Game::nextGameInSeriesFlag() const {
	return nextGameInSeries;
}

bool Game::newSeriesFlag() const {
	return newSeries;
}

Game::~Game() {}

void Game::setWinner() {
	if (playerA->allShipsDestroyed()) {
		winner = playerB;
		seriesScore.second++;
	}
	else {
		winner = playerA;
		seriesScore.first++;
	}
	if (dynamic_cast<ConsoleGamer*>(winner) == nullptr) {
		if (dynamic_cast<OptimalGamer*>(winner) != nullptr) {
			dynamic_cast<OptimalGamer*>(winner)->printWin();
		}
		else {
			dynamic_cast<RandomGamer*>(winner)->printWin();
		}
	}
}

void Game::drawEnemyPlayField	(sf::RenderWindow& window, double x_start, double y_start) const {
	IGamer* player = playerA;
	IGamer* enemy = playerB;
	if (playersTurn != playerA) {
		player = playerB;
		enemy = playerA;
	}

	sf::RectangleShape shape;
	for (int i = 9; i >= 0; i--) {
		for (int j = 0; j < 10; j++) {
			shape.setSize(sf::Vector2f(fieldSize, fieldSize));
			shape.setPosition(sf::Vector2f(x_start + j * (fieldSize + delta + fieldThicness * 2), y_start + i * (fieldSize + delta + fieldThicness * 2)));
			shape.setOutlineThickness(2.f);
			shape.setFillColor(sf::Color::Transparent);

			shape.setOutlineColor(sf::Color(0, 25, 200));
			if (enemy->getFieldForDraw(j, 9 - i) == SeaBattle::Field::FiredEmpty) {
				shape.setOutlineColor(FiredEmptyColor);
			}
			if (enemy->getFieldForDraw(j, 9 - i) == SeaBattle::Field::FiredShip) {
				shape.setOutlineColor(FiredShipColor);
			}
			if (player->getCurrentPos() == std::make_pair(j, 9 - i)) {
				shape.setOutlineColor(CurrentFieldColor);
			}

			// show enemy ships
			if (enemy->getFieldForDraw(j, 9 - i) == SeaBattle::Field::Ship) {
				shape.setOutlineColor(EnemyShipColor);
			}

			window.draw(shape);
		}
	}
}

void Game::drawPlayerPlayField(sf::RenderWindow& window, double x_start, double y_start) const {
	IGamer* player = playerA;
	if (playersTurn != playerA) {
		player = playerB;
	}

	sf::RectangleShape shape;
	for (int i = 9; i >= 0; i--) {
		for (int j = 0; j < 10; j++) {
			shape.setSize(sf::Vector2f(fieldSize, fieldSize));
			shape.setPosition(sf::Vector2f(x_start + j * (fieldSize + delta + fieldThicness * 2), y_start + i * (fieldSize + delta + fieldThicness * 2)));
			shape.setOutlineThickness(2.f);
			shape.setFillColor(sf::Color::Transparent);

			shape.setOutlineColor(sf::Color(0, 25, 200));
			if (player->getFieldForDraw(j, 9 - i) == SeaBattle::Field::Ship) {
				shape.setOutlineColor(PlayersShipColor);
			}
			if (player->getFieldForDraw(j, 9 - i) == SeaBattle::Field::FiredEmpty) {
				shape.setOutlineColor(FiredEmptyColor);
			}
			if (player->getFieldForDraw(j, 9 - i) == SeaBattle::Field::FiredShip) {
				shape.setOutlineColor(FiredShipColor);
			}

			window.draw(shape);
		}
	}
}

void Game::drawPlayFields(sf::RenderWindow& window) const {
	sf::RectangleShape shape(sf::Vector2f(MainFieldSize, MainFieldSize));
	shape.setOutlineThickness(2.f);
	shape.setOutlineColor(sf::Color(148, 119, 139));
	shape.setFillColor(sf::Color::Transparent);
	shape.move(mainFieldStartPos.first, mainFieldStartPos.second);
	window.draw(shape);

	shape.move(MainFieldSize + deltaFields, 0);
	window.draw(shape);
}

void Game::processMessage(std::string& toWrite) const {

	toWrite += "Current position(x,y): (" + std::to_string(playersTurn->getCurrentPos().first) +
		"," + std::to_string(playersTurn->getCurrentPos().second) + ")\n";

	switch (stage) {
	case SeaBattle::GameStage::Game: {
		toWrite += "\nGame stage: game\n";
		switch (message) {
		case SeaBattle::MessageType::HitField: {
			toWrite += "\nYou can't shoot this field again!";
			break;
		}
		case SeaBattle::MessageType::Miss: {
			toWrite += "\nYou missed!";
			break;
		}
		case SeaBattle::MessageType::ShipDestroyed: {
			toWrite += "\nYou destroyed enemies ship!";
			break;
		}
		case SeaBattle::MessageType::ShipHit: {
			toWrite += "\nYou hit enemies ship";
			break;
		}
		}
		break;
	}
	case SeaBattle::GameStage::PlacingShips: {
		toWrite += "\nGame stage: placing ships";
		switch (message) {
		case SeaBattle::MessageType::CantRotate: {
			toWrite += "\nCan't rotate ship here!";
			break;
		}
		case SeaBattle::MessageType::ShipIsPlaced: {
			toWrite += "\nShip succesfuly placed!";
			break;
		}
		case SeaBattle::MessageType::CantPlaceShip: {
			toWrite += "\nCan't place ship here!";
			break;
		}
		}
		toWrite += "\n\n" + std::to_string(playersTurn->getPlacedShips()) + " ship(s) is(are) already placed";
		toWrite += "\nYou need to place " + std::to_string(SeaBattle::TotalShips - playersTurn->getPlacedShips()) + " more ship(s)";
		break;
		}
	}
}

void Game::processKeyTurnTransition() {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			std::cout << "normal exit" << std::endl;
			exit(0);
		}
		if (event.type == sf::Event::KeyPressed) {
			switch (event.key.code) {
			case sf::Keyboard::Enter: {
				drawTurnTransition = false;
				if (playersTurn == playerA) {
					playersTurn = playerB;
				}
				else {
					playersTurn = playerA;
				}
				turnTransition = false;
				break;
			}
			default: break;
			}
		}
	}
}

void Game::drawMenu(sf::RenderWindow& window) const {
	window.clear(sf::Color::Black);
	sf::Text menuText;
	int menuTextCharacterSize = 30;

	menuText.setFont(font);
	menuText.setCharacterSize(menuTextCharacterSize);
	if (choosingFirstPlayer) {
		menuText.setString("Choose first player type:\n");
	}
	else {
		menuText.setString("Choose second player type:\n");
	}
	sf::Vector2f centerPos = sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2 - menuTextCharacterSize * 3);
	menuText.setPosition(centerPos.x - menuText.getGlobalBounds().width / 2, centerPos.y - menuText.getGlobalBounds().height / 2);
	window.draw(menuText);

	menuText.setString("\n\tf1 - Random Gamer\n\n\tf2 - Console Gamer\n\n\tf3 - Optimal Gamer\n\n\tesc - exit");
	menuText.setPosition(centerPos.x - menuText.getGlobalBounds().width / 2, 
		centerPos.y - menuText.getGlobalBounds().height / 2 + 4 * menuTextCharacterSize);
	window.draw(menuText);

	window.display();
}

void Game::drawMainFieldBorder(sf::RenderWindow& window, double start_x, double start_y) const {
	sf::RectangleShape shape(sf::Vector2f(MainFieldSize, MainFieldSize));
	shape.setOutlineThickness(2.f);
	shape.setOutlineColor(sf::Color(148, 119, 139));
	shape.setFillColor(sf::Color::Transparent);
	shape.move(start_x, start_y);
	window.draw(shape);
}

void Game::drawSeriesScore(sf::RenderWindow& window) const {
	int seriesScoreCharacterSize = 20;
	
	sf::Text seriesScoreText;
	seriesScoreText.setFont(font);
	seriesScoreText.setCharacterSize(seriesScoreCharacterSize);
	seriesScoreText.setString("Series score: " + std::to_string(seriesScore.first) + " - " + std::to_string(seriesScore.second));
	sf::Vector2f position = sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2);
	seriesScoreText.setPosition(position.x - seriesScoreText.getGlobalBounds().width / 2, 20);

	window.draw(seriesScoreText);
}

void Game::drawPlacingShipsField(sf::RenderWindow& window) const {
	sf::Text text;
	sf::RectangleShape shape;
	std::pair<double, double> placingShipsTextPos = std::make_pair(100.f, 540.f);
	text.setFont(font);

	double startPosX = mainFieldStartPos.first + 5.f;
	double startPosY = mainFieldStartPos.second + 5.f;

	for (int i = 9; i >= 0; i--) {
		for (int j = 0; j < 10; j++) {
			shape.setSize(sf::Vector2f(fieldSize, fieldSize));
			shape.setPosition(sf::Vector2f(startPosX + j * (fieldSize + delta + fieldThicness * 2),
				startPosY + i * (fieldSize + delta + fieldThicness * 2)));
			shape.setOutlineThickness(2.f);
			shape.setFillColor(sf::Color::Transparent);

			shape.setOutlineColor(sf::Color(0, 25, 200));
			if (playersTurn->getFieldForDraw(j, 9 - i) == SeaBattle::Field::Ship) {
				shape.setOutlineColor(sf::Color(82, 255, 184));
			}
			if (playersTurn->isShipField(j, 9 - i)) {
				shape.setOutlineColor(sf::Color::Yellow);
			}
			window.draw(shape);
		}
	}
	std::string toWrite;
	processMessage(toWrite);

	text.setString(toWrite);
	text.setCharacterSize(20);

	placingShipsTextPos = std::make_pair(mainFieldStartPos.first + MainFieldSize / 2 - text.getGlobalBounds().width / 2, placingShipsTextPos.second);

	text.setPosition(placingShipsTextPos.first, placingShipsTextPos.second);
	window.draw(text);
}

void Game::drawPlacingShipsControl(sf::RenderWindow& window) const {
	sf::Text placingShipsControlText;
	placingShipsControlText.setFont(font);
	placingShipsControlText.setCharacterSize(placingShipsControlTextCharacterSize);
	placingShipsControlText.setPosition(placingShipsControlTextPos.first, placingShipsControlTextPos.second);
	placingShipsControlText.setString("Control:\n\ns - place ship\n\nr - rotate ship\n\nesc - exit"
		"\n\nKey Up - move up your current position\n\nKey Down - move down your current position"
		"\n\nKey Left - move left your current position\n\nKey Rigth - move right your current position");
	window.draw(placingShipsControlText);
}

void Game::drawGameControl(sf::RenderWindow& window) const {
	sf::Text gameControlText;
	gameControlText.setFont(font);
	gameControlText.setCharacterSize(gameControlTextCharacterSize);
	gameControlText.setPosition(gameControlTextPos.first, gameControlTextPos.second);
	gameControlText.setString("Control:\n\nf - shoot field\n\nesc - exit"
		"\n\nKey Up - move up\n\nKey Down - move down"
		"\n\nKey Left - move left\n\nKey Rigth - move");
	window.draw(gameControlText);

}

void Game::drawTurnTransitionMenu() const {
	std::string turnTransitionStr = "Press Enter for the next turn";
	sf::Vector2f centerPos = sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2);
	sf::Text turnTransitionText;

	turnTransitionText.setFont(font);
	turnTransitionText.setString(turnTransitionStr);
	turnTransitionText.setCharacterSize(turnTransitionCharacterSize);
	turnTransitionText.setPosition(centerPos.x - turnTransitionText.getGlobalBounds().width / 2, centerPos.y - turnTransitionText.getGlobalBounds().height / 2);

	window.draw(turnTransitionText);
}

void Game::processResultScreen(sf::Keyboard::Key k) {
	switch (k) {
	case sf::Keyboard::F1 : {
		nextGameInSeries = true;
		break;
	}
	case sf::Keyboard::F2: {
		newSeries = true;
		break;
	}
	case sf::Keyboard::Escape: {
		exit(0);
	}
	default:
		break;
	}
}

void Game::menu(sf::RenderWindow& window) {
	seriesScore = std::make_pair(0, 0);
	bool isMenu = true;
	sf::Event event;
	while (isMenu) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
				std::cout << "normal exit" << std::endl;
				exit(0);
			}
			if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code) {
				case sf::Keyboard::Escape: {
					std::cout << "normal exit" << std::endl;
					exit(0);
				}
				case sf::Keyboard::F1: {
					if (choosingFirstPlayer) {
						choosingFirstPlayer = false;
						playerA = new RandomGamer(std::string("fictive name"));
					}
					else {
						playerB = new RandomGamer(std::string("fictive name"));
						isMenu = false;
						choosingFirstPlayer = true; // for the next series
					}
					break;
				}
				case sf::Keyboard::F2: {
					if (choosingFirstPlayer) {
						playerA = new ConsoleGamer(std::string("fictive name"));
						choosingFirstPlayer = false;
					}
					else {
						playerB = new ConsoleGamer(std::string("fictive name"));
						isMenu = false;
						choosingFirstPlayer = true; // for the next series
					}
					break;
				}
				case sf::Keyboard::F3: {
					if (choosingFirstPlayer) {
						playerA = new OptimalGamer(std::string("fictive name"));
						choosingFirstPlayer = false;
					}
					else {
						playerB = new OptimalGamer(std::string("fictive name"));
						isMenu = false;
						choosingFirstPlayer = true; // for the next series
					}
					break;
				}
				default: break;
				}
			}
		}
		drawMenu(window);
	}
}

void Game::init() {

	if (dynamic_cast<ConsoleGamer*>(playerA) != nullptr) {
		delete playerA;
		playerA = new ConsoleGamer(std::string("A"));
	}
	else {
		if (dynamic_cast<RandomGamer*>(playerA) != nullptr) {
			delete playerA;
			playerA = new RandomGamer(std::string("A"));
		}
		else {
			delete playerA;
			playerA = new OptimalGamer(std::string("A"));
		}
	}

	if (dynamic_cast<ConsoleGamer*>(playerB) != nullptr) {
		delete playerB;
		playerB = new ConsoleGamer(std::string("B"));
	}
	else {
		if (dynamic_cast<RandomGamer*>(playerB) != nullptr) {
			delete playerB;
			playerB = new RandomGamer(std::string("B"));
		}
		else {
			delete playerB;
			playerB = new OptimalGamer(std::string("B"));
		}
	}

	winner = nullptr;
	stage = SeaBattle::GameStage::PlacingShips;
	turnTransition = false;
	playersTurn = nullptr; 
	message = SeaBattle::MessageType::EmptyMessage;
	nextGameInSeries = false;
	newSeries = false;
	drawTurnTransition = false;

	playerA->setEnemy(playerB);
	playerB->setEnemy(playerA);
	playersTurn = playerA;
}

void Game::processStage() {
	if (stage == SeaBattle::GameStage::PlacingShips && playerA->getPlacedShips() == SeaBattle::TotalShips &&
		playerB->getPlacedShips() == SeaBattle::TotalShips) {
		stage = SeaBattle::GameStage::Game;
	}
	else {
		if (stage == SeaBattle::GameStage::Game && (playerA->allShipsDestroyed() || playerB->allShipsDestroyed())) {
			stage = SeaBattle::GameStage::ResultScreen;
			setWinner();
		}
	}
}

void Game::processKey(sf::Keyboard::Key k) {
	 //firstPlayer is ConsoleGamer
	message = SeaBattle::MessageType::EmptyMessage;
	bool f = false;
	if (dynamic_cast<ConsoleGamer*>(playerB) != nullptr) {
		f = true;
	}
	if (stage == SeaBattle::GameStage::PlacingShips || stage == SeaBattle::GameStage::Game) {
		 switch (k) {
		 case sf::Keyboard::Up:
		 case sf::Keyboard::Down:
		 case sf::Keyboard::Left:
		 case sf::Keyboard::Right: {
			 if (playersTurn->canMove(getDirection(k), stage)) {
				 playersTurn->makeMove(getDirection(k));
			 }
			 turnTransition = false;
			 break;
		 }
		 case sf::Keyboard::F: {
			 if (stage == SeaBattle::GameStage::Game) {
				 bool tmp = true;
				 message = playersTurn->makeTurn(&tmp);
				 turnTransition = tmp;
				 if (f) {
					 drawTurnTransition = tmp;
				 }
			 }
			 break;
		 }
		 case sf::Keyboard::S: {
			 if (stage == SeaBattle::GameStage::PlacingShips) {
				 message = playersTurn->setShip();
				 if (playersTurn->getPlacedShips() == SeaBattle::TotalShips) {
					 turnTransition = true;
					 if (f) {
						 drawTurnTransition = true;
					 }
				 }
				 else {
					 turnTransition = false;
				 }
			 }
			 break;
		 }
		 case sf::Keyboard::R: {
			 if (stage == SeaBattle::GameStage::PlacingShips) {
				 if (playersTurn->canRotate()) {
					 playersTurn->rotate();
				 }
				 else {
					 message = SeaBattle::MessageType::CantRotate;
				 }
				 turnTransition = false;
			 }
			 break;
		 }
		 case sf::Keyboard::Escape: {
			 std::cout << "normal exit" << std::endl;
			 exit(0);
		 }
		 default:
			 break;
		 }
	 }
	else {
		processResultScreen(k);
	}
}

void Game::processTurnTransition() {
	if (turnTransition) {
		if (dynamic_cast<ConsoleGamer*>(playerB) != nullptr && dynamic_cast<ConsoleGamer*>(playerA) != nullptr) {
			if (drawTurnTransition) {
				processKeyTurnTransition();
				return;
			}
		}
		if (playersTurn == playerA) {
			playersTurn = playerB;
		}
		else {
			playersTurn = playerA;
		}
	}
	turnTransition = false;
}

void Game::drawGame(sf::RenderWindow& window) {
	if (dynamic_cast<ConsoleGamer*>(playersTurn) == nullptr && stage != SeaBattle::GameStage::ResultScreen) {
		return;
	}

	window.clear();
	if (dynamic_cast<ConsoleGamer*>(playerB) != nullptr && dynamic_cast<ConsoleGamer*>(playerA) != nullptr) {
		if (turnTransition && drawTurnTransition) {
			drawTurnTransitionMenu();
			window.display();
			return;
		}
	}

	switch (stage) {
	case SeaBattle::GameStage::PlacingShips: {
		drawMainFieldBorder(window, mainFieldStartPos.first, mainFieldStartPos.second);
		drawSeriesScore(window);
		drawPlacingShipsField(window);
		drawPlacingShipsControl(window);
		break;
	}
	case SeaBattle::GameStage::Game: {
		drawMainFieldBorder(window, mainFieldStartPos.first, mainFieldStartPos.second);
		drawMainFieldBorder(window, mainFieldStartPos.first + MainFieldSize + deltaFields, mainFieldStartPos.second);

		double start_x = mainFieldStartPos.first + 5.f, start_y = mainFieldStartPos.second + 5.f;
		double start_x1 = mainFieldStartPos.first + 5.f + deltaFields + MainFieldSize, start_y1 = mainFieldStartPos.second + 5.f;

		drawEnemyPlayField(window, start_x, start_y);
		drawPlayerPlayField(window, start_x1, start_y1);
		drawGameControl(window);

		std::string toWrite;
		processMessage(toWrite);

		sf::Text text;
		text.setFont(font);
		text.setCharacterSize(gameDescriptionTextCharacteSize);
		text.setPosition(gameDescriptionTextPos.first, gameDescriptionTextPos.second);

		text.setString(toWrite);
		window.draw(text);
		break;
	}
	case SeaBattle::GameStage::ResultScreen: {
		sf::Text text;
		text.setFont(font);
		text.setCharacterSize(30);

		std::string toWrite;
		if (winner == playerA) {
			toWrite += "First player won!";
		}
		else {
			toWrite += "Second player won!";
		}

		text.setString(toWrite);

		sf::Vector2f centerPos = sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2);
		text.setPosition(centerPos.x - text.getGlobalBounds().width / 2, centerPos.y - text.getGlobalBounds().height / 2 - 100);
		window.draw(text);

		text.setCharacterSize(20);
		text.move(0, 30);
		toWrite = "\n\nSeries score:   " + std::to_string(seriesScore.first) + " - " + std::to_string(seriesScore.second);
		text.setString(toWrite);
		window.draw(text);

		text.move(0, 40);
		toWrite = "\n\nf1 - play next game in series\n\nf2 - play new series\n\nesc - exit";
		text.setString(toWrite);
		window.draw(text);
		
		break;
	}
	}

	window.display();

}

IGamer* Game::getTurnPlayer() const {
	return playersTurn;
}

IGamer* Game::getWinner() const {
	return winner;
}


void Game::processTurn() {
	switch (stage) {
	case SeaBattle::GameStage::PlacingShips: {
		playersTurn->setShip();
		turnTransition = true;
		return;
	}
	case SeaBattle::GameStage::Game: {
		bool tmp = true;
		playersTurn->makeTurn(&tmp);
		turnTransition = tmp;
		return;
	}
	case SeaBattle::GameStage::ResultScreen: {
		processResultScreen(sf::Keyboard::A);
		return;
	}
	}
}