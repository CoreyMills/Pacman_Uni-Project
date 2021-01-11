#include "BonusController.h"
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>
#include <vector>

BonusController::BonusController(int argc, char* argv[]) : Game(argc, argv)
{
	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1920, 1080, false, 0, 0, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

BonusController::~BonusController()
{
	//things to delete when class closes
	delete _pacman;

	//delete all objects in arrays
	_munchieArray.clear();
	_cherryArray.clear();
	_barrierArray.clear();
	_ghostArray.clear();

	delete _stringPointsPos;

	delete _menuBackground;
	delete _menuRectangle;
	delete _centrePos;
}

void BonusController::LoadContent()
{
	// Set string position
	_stringPointsPos = new Vector2(10.0f, 25.0f);

	//Set Pause Menu Parameters
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

	_centrePos = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

	//create player class
	_pacman = new Pacman();

	//array of objects
	vector<Munchies*> _munchieArray;
	vector<Cherry*> _cherryArray;
	vector<Barrier*> _barrierArray;
	vector<Ghost*> _ghostArray;

	//spawn items for first time
	SpawnMunchie();
	SpawnGhost(480, 270, 1, 200, 1); SpawnGhost(1440, 810, 1, 200, 2); SpawnGhost(200, 880, 2, 200, 3); SpawnGhost(1720, 200, 2, 200, 4);

	//set player vars
	_pacman->SetBonusLevel(true);
	_pacman->SetPosition(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
	_pacman->SetOriginalPosition(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

	//bools
	_paused = false;
	_pKeyDown = false;
	_gameStarted = false;
	_cherryEaten = true;
	_playerKilled = false;
	_justDied = false;
	_justWon = false;

	//sound vars
	_munchieEatSound = new SoundEffect();
	_munchieEatSound->Load("Sounds/pop.wav");
	_cherryEatSound = new SoundEffect();
	_cherryEatSound->Load("Sounds/CherryPickUp.wav");
	_deathSound = new SoundEffect();
	_deathSound->Load("Sounds/Dead.wav");

	//Spawn Level
	Level("BonusLevel");

	//score
	_pointsGoal = 200;
	currentHighscore = 0;

	//get highscore to display
	currentHighscore = HighScore(0);
}

void BonusController::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* state = Input::Keyboard::GetState();
	_mouseState = Input::Mouse::GetState();

	//call methods
	Input(elapsedTime, state);

	if (!_playerKilled)
	{
		//set new highscore display, if new is greater
		if (_pacman->GetPoints() > currentHighscore)
		{
			currentHighscore = _pacman->GetPoints();
		}

		if (_gameStarted)
		{
			//check that game hasnt paused and player hasnt won or lost
			if (!_paused && _pacman->GetPoints() < _pointsGoal && !_playerKilled)
			{
				for (int i = 0; i < _munchieArray.size(); i++)
				{
					_munchieArray[i]->Update(elapsedTime);
				}

				for (int i = 0; i < _cherryArray.size(); i++)
				{
					_cherryArray[i]->Update(elapsedTime);
				}

				for (int i = 0; i < _barrierArray.size(); i++)
				{
					_barrierArray[i]->Update(elapsedTime);
				}

				for (int i = 0; i < _ghostArray.size(); i++)
				{
					_ghostArray[i]->Update(elapsedTime);
				}

				CheckCollisions();

				//player 1's cherry
				if (_cherryEaten)
				{
					_framesWaited += elapsedTime;
					SpawnCherry();
				}

				if (_munchieArray.size() == 0)
				{
					SpawnMunchie();
				}
			}
		}
	}

	//update pacman even after death
	if (_gameStarted)
	{
		if (!_paused)
		{
			//call update methods for other classes
			_pacman->Update(elapsedTime, _mouseState);
		}
	}
}

void BonusController::Draw(int elapsedTime)
{
	string inGameHighscore = "Highscore = ";
	string endGameHighScore = "HighScore";
	string startGame1 = "Press SPACE to Start!";
	string startGame2 = "Use the left mouse button to move";
	string paused = "PAUSED!"; 
	string pLost = "YOU LOSE!";
	string pWin = "You Win!!";
	string score = "Your Score was: ";
	string endingMessage = "Escape to Quit, R to restart";

	//Show Start Menu
	stringstream menuStream;

	SpriteBatch::BeginDraw();
	if (_playerKilled)
	{
		menuStream << pLost << '\n' << score << _pacman->GetPoints() << '\n' << endingMessage;
		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _centrePos, Color::Red);

		if (_justDied)
		{
			_justDied = false;

			//add score to highscore
			HighScore(_pacman->GetPoints());
		}

		//display highscore
		DisplayHighscore(endGameHighScore);
	}
	else if (_pacman->GetPoints() >= _pointsGoal)
	{
		// Allows us to easily display a string

		stringstream endGame;
		endGame << pWin<< '\n' << score << _pacman->GetPoints() << '\n' << endingMessage;
		SpriteBatch::DrawString(endGame.str().c_str(), _centrePos, Color::Yellow);

		if (!_justWon)
		{
			_justWon = true;
			//add score then display
			HighScore(_pacman->GetPoints());
		}

		//display highscore
		DisplayHighscore(endGameHighScore);
	}
	else
	{
		if (!_gameStarted)
		{
			menuStream << startGame1 << '\n' << startGame2;
			SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
			SpriteBatch::DrawString(menuStream.str().c_str(), _centrePos, Color::Red);

			//display hihscore table
			DisplayHighscore(endGameHighScore);
		}
		else
		{
			for (int i = 0; i < _munchieArray.size(); i++)
			{
				_munchieArray[i]->Draw(elapsedTime);
			}

			for (int i = 0; i < _cherryArray.size(); i++)
			{
				_cherryArray[i]->Draw(elapsedTime);
			}

			for (int i = 0; i < _barrierArray.size(); i++)
			{
				_barrierArray[i]->Draw(elapsedTime);
			}

			for (int i = 0; i < _ghostArray.size(); i++)
			{
				_ghostArray[i]->Draw(elapsedTime);
			}

			// Allows us to easily display a string
			stringstream pointsStream;

			//write to stream player points
			pointsStream << "Player Score: " << _pacman->GetPoints();
			SpriteBatch::DrawString(pointsStream.str().c_str(), _stringPointsPos, Color::Green);

			//show pause menu
			if (_paused)
			{
				menuStream << paused;
				SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
				SpriteBatch::DrawString(menuStream.str().c_str(), _centrePos, Color::Red);
			}
		}
	}

	//draw pacman even after death
	if (_gameStarted)
	{
		_pacman->Draw(elapsedTime);
	}

	//draw highscore at all times
	stringstream highscoreStream;
	highscoreStream << inGameHighscore << currentHighscore;
	SpriteBatch::DrawString(highscoreStream.str().c_str(), new Vector2(Graphics::GetViewportWidth() / 2.0f, 25), Color::Red);

	SpriteBatch::EndDraw();
}

void BonusController::Input(int elapsedTime, Input::KeyboardState* state)
{
	//pause the game
	if (state->IsKeyDown(Input::Keys::P) && !_pKeyDown && _gameStarted)
	{
		_pKeyDown = true;
		_paused = !_paused;
	}

	//unpause the game
	if (state->IsKeyUp(Input::Keys::P))
	{
		_pKeyDown = false;
	}

	//start the game 
	if (state->IsKeyDown(Input::Keys::SPACE))
	{
		_gameStarted = true;
	}

	//exit the game
	if (state->IsKeyDown(Input::Keys::ESCAPE))
	{
		exit(0);
	}

	//restart the game
	if (state->IsKeyDown(Input::Keys::R))
	{
		Restart();
	}
}

bool BonusController::Collision(Rect* colliderRect1, Rect* colliderRect2)
{
	//did research into this and found this piece of code
	if ((colliderRect1->X < colliderRect2->X + colliderRect2->Width) &&
		(colliderRect2->X < colliderRect1->X + colliderRect1->Width) &&
		(colliderRect1->Y < colliderRect2->Y + colliderRect2->Height) &&
		(colliderRect2->Y < colliderRect1->Y + colliderRect1->Height))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void BonusController::Collision(GameObject* movingObj, GameObject* immovableObj)
{
	Rect* immovableRect = immovableObj->GetColliderRect();
	Rect* movingRect = movingObj->GetColliderRect();

	//1D point centres
	float immovableXCentre = (immovableRect->X + (immovableRect->Width / 2));
	float immovableYCentre = (immovableRect->Y + (immovableRect->Height / 2));
	float movingXCentre = (movingRect->X + (movingRect->Width / 2));
	float movingYCentre = (movingRect->Y + (movingRect->Height / 2));

	//2D point centres
	//X centers
	Vector2 immovableNCentre; immovableNCentre.X = immovableXCentre; immovableNCentre.Y = immovableRect->Y;
	Vector2 immovableSCentre; immovableSCentre.X = immovableXCentre; immovableSCentre.Y = immovableRect->Y + immovableRect->Height;
	//Y centres
	Vector2 immovableWCentre; immovableWCentre.X = immovableRect->X; immovableWCentre.Y = immovableYCentre;
	Vector2 immovableECentre; immovableECentre.X = immovableRect->X + immovableRect->Width; immovableECentre.Y = immovableYCentre;
	//pacman centre
	Vector2 movingCentre; movingCentre.X = movingXCentre; movingCentre.Y = movingYCentre;

	Vector2 distX;
	float distXFloat;
	Vector2 distY;
	float distYFloat;

	bool top;
	bool left;

	//horizontal
	if (movingXCentre < immovableXCentre)
	{
		//cout << "on left" << endl;
		distXFloat = distX.Distance(movingCentre, immovableWCentre);
		left = true;
	}
	else
	{
		//cout << "on right" << endl;
		distXFloat = distX.Distance(movingCentre, immovableECentre);
		left = false;
	}

	//vertical
	if (movingYCentre < immovableYCentre)
	{
		//cout << "on top" << endl;
		distYFloat = distY.Distance(movingCentre, immovableNCentre);
		top = true;
	}
	else
	{
		//cout << "on bottom" << endl;
		distYFloat = distY.Distance(movingCentre, immovableSCentre);
		top = false;
	}

	//find specific side
	if (distYFloat < distXFloat)
	{
		if (top)
			movingObj->SetPosition(movingObj->GetPosition()->X, immovableRect->Y - movingRect->Height);
		else
			movingObj->SetPosition(movingObj->GetPosition()->X, immovableRect->Y + immovableRect->Height);
	}
	else
	{
		if (left)
			movingObj->SetPosition(immovableRect->X - movingRect->Width, movingObj->GetPosition()->Y);
		else
			movingObj->SetPosition(immovableRect->X + immovableRect->Width, movingObj->GetPosition()->Y);
	}
}

void BonusController::CheckCollisions()
{
	//check if any pacman players collided with munchies
	for (int i = 0; i < _munchieArray.size(); i++)
	{
		bool colliderHit = false;

		colliderHit = Collision(_pacman->GetColliderRect(), _munchieArray[i]->GetColliderRect());
		if (colliderHit)
		{
			_pacman->ItemCollected(_munchieArray[i]->GetPointsValue());
			_munchieArray.erase(_munchieArray.begin() + i);
			Audio::Play(_munchieEatSound);
			colliderHit = false;
			continue;
		}
	}

	//check if any pacman players collided with cherrys
	for (int i = 0; i < _cherryArray.size(); i++)
	{
		bool colliderHit = false;

		colliderHit = Collision(_pacman->GetColliderRect(), _cherryArray[i]->GetColliderRect());
		if (colliderHit)
		{
			_pacman->ItemCollected(_cherryArray[i]->GetPointsValue());
			_cherryArray.erase(_cherryArray.begin() + i);
			Audio::Play(_cherryEatSound);
			colliderHit = false;
			_cherryEaten = true;
			continue;
		}
	}

	//check if any pacman players collided with ghosts
	for (int i = 0; i < _ghostArray.size(); i++)
	{
		bool colliderHit = false;

		colliderHit = Collision(_pacman->GetColliderRect(), _ghostArray[i]->GetColliderRect());
		if (colliderHit)
		{
			_justDied = true;
			_playerKilled = true;
			_pacman->SetPlayerKilled(true);
			Audio::Play(_deathSound);
			colliderHit = false;
			continue;
		}
	}

	//check if any ghosts collided with barriers
	for (int i = 0; i < _barrierArray.size(); i++)
	{
		bool colliderHit = false;

		for (int j = 0; j < _ghostArray.size(); j++)
		{
			colliderHit = Collision(_ghostArray[j]->GetColliderRect(), _barrierArray[i]->GetColliderRect());
			if (colliderHit)
			{
				Collision(_ghostArray[j], _barrierArray[i]);
				colliderHit = false;
				continue;
			}
		}
	}

	_pacman->SetBlocked(false);

	//check if pacman mouse goes out of a certain area
	if(_mouseState->X < 0 + _barrier->GetColliderRect()->Width ||
		_mouseState->X > (Graphics::GetViewportWidth() - _barrier->GetColliderRect()->Width - _pacman->GetColliderRect()->Width) ||
		_mouseState->Y < 0 + _barrier->GetColliderRect()->Height || 
		_mouseState->Y >(Graphics::GetViewportHeight() - _barrier->GetColliderRect()->Height - _pacman->GetColliderRect()->Height))
	{
		_pacman->SetBlocked(true);
	}
}

void BonusController::SpawnMunchie()
{
	int randX = rand() % 1820 + 50;
	int randY = rand() % 980 + 50;

	_munchie = new Munchies();
	_munchie->SetPosition(randX, randY);
	_munchieArray.push_back(_munchie);
}

void BonusController::SpawnCherry()
{
	//random spawn time
	int randTime = rand() % 8000 + 12000;

	if (_framesWaited >= randTime)
	{
		//random position
		int randX = rand() % 1820 + 50;
		int randY = rand() % 980 + 50;

		_cherry = new Cherry();
		_cherry->SetPosition(randX, randY);
		_cherryArray.push_back(_cherry);
		_cherryEaten = false;
		_framesWaited = 0;
	}
}

void BonusController::SpawnBarrier(float x, float y)
{
	_barrier = new Barrier();
	_barrier->SetPosition(x * _barrier->GetColliderRect()->Height, y * _barrier->GetColliderRect()->Width);
	_barrierArray.push_back(_barrier);
}

void BonusController::SpawnGhost(float xPos, float yPos, int routeType, int chaseDist, int ghostVersion)
{
	_ghost = new Ghost();
	_ghost->SetGhost(ghostVersion);
	_ghost->SetPosition(xPos, yPos);
	_ghost->SetOriginalPosition(xPos, yPos);
	_ghost->SetRouteType(routeType);
	_ghost->SetChaseDist(chaseDist);
	_ghost->SetPlayerObject(_pacman);
	_ghostArray.push_back(_ghost);
}

void BonusController::Level(string levelF)
{
	int width;
	vector<string>* lines = new vector<string>();

	fstream stream;
	stringstream ss;
	ss << "Levels/" << levelF << ".txt";
	stream.open(ss.str(), fstream::in);

	char* line = new char[256];
	stream.getline(line, 256);
	string* sline = new string(line);
	width = sline->size();

	while (!stream.eof())
	{
		lines->push_back(*sline);

		//error catcher
		if (sline->size() != width)
		{
			cout << "Bad Level Load\n";
		}
		stream.getline(line, 256);

		//reset vars
		delete sline;
		sline = new string(line);
	}

	delete[] line;
	delete sline;

	// Loop over every tile position,
	for (int y = 0; y < lines->size(); ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			// to load each tile.
			char tileType = lines->at(y)[x];
			if (tileType == '1')
			{
				SpawnBarrier(x, y);
			}
		}
	}

	delete lines;
}

int BonusController::HighScore(int playerScore)
{
	int scores[5], currentHighest = playerScore, flag = 1, size = 0;

	ifstream highscoreFileIn;
	highscoreFileIn.open("HighScore/Highscore.txt");

	//Check for Error
	if (highscoreFileIn.fail())
	{
		cerr << "Bad File" << endl;
		exit(1);
	}

	while (!highscoreFileIn.eof())
	{
		highscoreFileIn >> scores[size];
		size++;
	}

	//place playerscore into the array to get sorted
	if(currentHighest > scores[4])
	{
		scores[4] = currentHighest;
	}

	highscoreFileIn.close();

	//sort the array
	for (int i = 0; i <= size; i++)
	{
		for (int i = 0; i <= size && flag; i++)
		{
			flag = 0;

			for (int j = 0; j < (size - 1); j++)
			{
				if (scores[j + 1] > scores[j])
				{
					currentHighest = scores[j];
					scores[j] = scores[j + 1];
					scores[j + 1] = currentHighest;
					flag = 1;
				}
			}
		}
	}
	
	ofstream highscoreFileOut;
	highscoreFileOut.open("HighScore/Highscore.txt");

	for (int i = 0; i < 5; i++)
	{
		if (scores[i] < 0)
		{
			highscoreFileOut << "0" << endl;
		}
		else
		{
			highscoreFileOut << scores[i] << endl;
		}
	}
	
	highscoreFileOut.close();

	return scores[0];
}

void BonusController::DisplayHighscore(string title)
{
	int score, count = 0;

	stringstream highscoreStream;
	ifstream highscoreFileIn;
	highscoreFileIn.open("HighScore/Highscore.txt");

	//Check for Error
	if (highscoreFileIn.fail())
	{
		cerr << "Bad File" << endl;
		exit(1);
	}
	
	//output to screen
	highscoreStream << "\n \n \n \n" << title << '\n';

	//read from file and display
	for(int i = 0; i< 5; i++)
	{
		highscoreFileIn >> score;

		count++;
		highscoreStream << count << ": " << score << endl;
	}

	highscoreFileIn.close();
	SpriteBatch::DrawString(highscoreStream.str().c_str(), _centrePos, Color::Red);
}

void BonusController::Restart()
{
	//delete all objects in arrays
	_munchieArray.clear();
	_cherryArray.clear();

	for (int i = 0; i < _ghostArray.size(); i++)
	{
		_ghostArray[i]->Restart();
	}

	//reset pacman vars
	_pacman->Restart();

	//reset ints
	_framesWaited = 0;

	//reset bools
	_paused = false;
	_pKeyDown = false;
	_gameStarted = false;
	_cherryEaten = true;
	_playerKilled = false;
	_justDied = false;
	_justWon = false;
}