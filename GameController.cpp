#include "GameController.h"
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <ctime>

GameController::GameController(int argc, char* argv[], int levelToLoad, int pointsGoal) : Game(argc, argv)
{
	SetLevelToLoad(levelToLoad);
	SetPointsGoal(pointsGoal);

	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1920, 1080, false, 0, 0, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

GameController::~GameController()
{
	//things to delete when class closes
	delete _pacman1;
	delete _pacman2;

	//delete all objects in arrays
	_munchieArray.clear();
	_cherryArray.clear();
	_speedUpArray.clear();
	_speedDownArray.clear();

	delete _stringPlayer1Points;
	delete _stringPlayer2Points;

	delete _menuBackground;
	delete _menuRectangle;
	delete _centrePos;
}

void GameController::LoadContent()
{
	//Spawn Level
	switch (_levelToLoad)
	{
	case 1:
		Level("Level1");
		break;
	case 2:
		Level("Level2");
		break;
	case 3:
		Level("Level3");
		break;
	}

	// Set string position
	_stringPlayer1Points = new Vector2(10.0f, 25.0f);
	_stringPlayer2Points = new Vector2((Graphics::GetViewportWidth() / 2.0f) + 10.0f, 25.0f);

	//Set Pause Menu Parameters
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

	_centrePos = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

	//create other classes as objects
	_pacman1 = new Pacman();
	_pacman2 = new Pacman();

	//spawn munchies for first time
	SpawnMunchie(1); SpawnMunchie(2);

	//set player 1 vars
	_pacman1->SetPlayerNum(1);
	_pacman1->SetPosition(480, 540);
	_pacman1->SetOriginalPosition(480, 540);

	//set player 2 vars
	_pacman2->SetPlayerNum(2);
	_pacman2->SetPosition(1440, 540);
	_pacman2->SetOriginalPosition(1440, 540);

	//menu bools
	_paused = false;
	_pKeyDown = false;
	_gameStarted = false;
	_cherryEaten1 = true;
	_cherryEaten2 = true;

	//PowerUp's
	_currentPower;

	//sound vars
	_munchieEatSound = new SoundEffect();
	_munchieEatSound->Load("Sounds/pop.wav");
	_cherryEatSound = new SoundEffect();
	_cherryEatSound->Load("Sounds/CherryPickUp.wav");
}

void GameController::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* state = Input::Keyboard::GetState();
	_mouseState = Input::Mouse::GetState();

	//call methods
	Input(elapsedTime, state);

	//check game is playing and then update other classes
	if (_gameStarted)
	{
		if (!_paused)
		{
			//call update methods for other classes
			_pacman1->Update(elapsedTime, _mouseState);
			_pacman2->Update(elapsedTime, _mouseState);
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

			for (int i = 0; i < _speedUpArray.size(); i++)
			{
				_speedUpArray[i]->Update(elapsedTime);
			}

			for (int i = 0; i < _speedDownArray.size(); i++)
			{
				_speedDownArray[i]->Update(elapsedTime);
			}

			CheckCollisions(elapsedTime);

			//player 1's cherry
			if (_cherryEaten1)
			{
				_cherryFramesWaited1 += elapsedTime;
				SpawnCherry(1, _cherryFramesWaited1);
			}

			//player 2's cherry
			if (_cherryEaten2)
			{
				_cherryFramesWaited2 += elapsedTime;
				SpawnCherry(2, _cherryFramesWaited2);
			}

			_powerFramesWaited += elapsedTime;
			SpawnPowerUp(_powerFramesWaited);
		}
	}
}

void GameController::Draw(int elapsedTime)
{
	string startGame1 = "Press SPACE to Start!";
	string startGame2 = "Player 1 use:WASD, Player 2 use : Arrows";
	string startGame3 = "First to 300 points";
	string paused = "PAUSED!";
	string p1Win = "Player 1 Wins!!";
	string p2Win = "Player 2 Wins!!";
	string endingMessage = "Escape to Quit, R to restart";

	//Show Start Menu
	std::stringstream menuStream;

	SpriteBatch::BeginDraw();

	if (_pacman1->GetPoints() >= _pointsGoal)
	{
		// Allows us to easily display a string
		std::stringstream endGame;
		endGame << p1Win << '\n' << endingMessage;
		SpriteBatch::DrawString(endGame.str().c_str(), _centrePos, Color::Yellow);
	}
	else if (_pacman2->GetPoints() >= _pointsGoal)
	{
		// Allows us to easily display a string
		std::stringstream endGame;
		endGame << p2Win << '\n' << endingMessage;
		SpriteBatch::DrawString(endGame.str().c_str(), _centrePos, Color::Yellow);
	}
	else
	{ 
		if (!_gameStarted)
		{
			menuStream << startGame1 << '\n' << startGame2 << '\n' << startGame3;
			SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
			SpriteBatch::DrawString(menuStream.str().c_str(), _centrePos, Color::Red);
		}
		else
		{
			//call draw methods for other classes
			_pacman1->Draw(elapsedTime);
			_pacman2->Draw(elapsedTime);

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

			for (int i = 0; i < _speedUpArray.size(); i++)
			{
				_speedUpArray[i]->Draw(elapsedTime);
			}

			for (int i = 0; i < _speedDownArray.size(); i++)
			{
				_speedDownArray[i]->Draw(elapsedTime);
			}

			// Allows us to easily display a string
			std::stringstream player1PointsStream;
			std::stringstream player2PointsStream;

			//get player 1 points
			player1PointsStream << "Player1 Score: " << _pacman1->GetPoints();
			player2PointsStream << "Player2 Score: " << _pacman2->GetPoints();

			// Draws String
			SpriteBatch::DrawString(player1PointsStream.str().c_str(), _stringPlayer1Points, Color::Green);
			SpriteBatch::DrawString(player2PointsStream.str().c_str(), _stringPlayer2Points, Color::Green);

			//show pause menu
			if (_paused)
			{
				menuStream << paused;
				SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
				SpriteBatch::DrawString(menuStream.str().c_str(), _centrePos, Color::Red);
			}
		}
	}

	SpriteBatch::EndDraw();
}

void GameController::Input(int elapsedTime, Input::KeyboardState* state)
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

bool GameController::Collision(Rect* colliderRect1, Rect* colliderRect2)
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

void GameController::Collision(GameObject* movingObj, GameObject* immovableObj)
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

void GameController::CheckCollisions(int elapsedTime)
{
	//check if any pacman players collided with munchies
	for (int i = 0; i < _munchieArray.size(); i++)
	{
		bool colliderHit = false;

		colliderHit = Collision(_pacman1->GetColliderRect(), _munchieArray[i]->GetColliderRect());
		if (colliderHit)
		{
			_pacman1->ItemCollected(_munchieArray[i]->GetPointsValue());
			_munchieArray.erase(_munchieArray.begin() + i);
			Audio::Play(_munchieEatSound);
			colliderHit = false;
			SpawnMunchie(1);
			continue;
		}
		colliderHit = Collision(_pacman2->GetColliderRect(), _munchieArray[i]->GetColliderRect());
		if (colliderHit)
		{
			_pacman2->ItemCollected(_munchieArray[i]->GetPointsValue());
			_munchieArray.erase(_munchieArray.begin() + i);
			Audio::Play(_munchieEatSound);
			colliderHit = false;
			SpawnMunchie(2);
			continue;
		}
	}

	//check if any pacman players collided with cherrys
	for (int i = 0; i < _cherryArray.size(); i++)
	{
		bool colliderHit = false;

		colliderHit = Collision(_pacman1->GetColliderRect(), _cherryArray[i]->GetColliderRect());
		if (colliderHit)
		{
			_pacman1->ItemCollected(_cherryArray[i]->GetPointsValue());
			_cherryArray.erase(_cherryArray.begin() + i);
			Audio::Play(_cherryEatSound);
			colliderHit = false;
			_cherryEaten1 = true;
			continue;
		}

		colliderHit = Collision(_pacman2->GetColliderRect(), _cherryArray[i]->GetColliderRect());
		if (colliderHit)
		{
			_pacman2->ItemCollected(_cherryArray[i]->GetPointsValue());
			_cherryArray.erase(_cherryArray.begin() + i);
			Audio::Play(_cherryEatSound);
			colliderHit = false;
			_cherryEaten2 = true;
			continue;
		}
	}

	//check if any pacman players collided with barriers
	for (int i = 0; i < _barrierArray.size(); i++)
	{
		bool colliderHit = false;

		colliderHit = Collision(_pacman1->GetColliderRect(), _barrierArray[i]->GetColliderRect());
		if (colliderHit)
		{
			Collision(_pacman1, _barrierArray[i]);
			_pacman1->SetBlocked(true);
			colliderHit = false;
			continue;
		}

		colliderHit = Collision(_pacman2->GetColliderRect(), _barrierArray[i]->GetColliderRect());
		if (colliderHit)
		{
			Collision(_pacman2, _barrierArray[i]);
			_pacman2->SetBlocked(true);
			colliderHit = false;
			continue;
		}
	}

	//check if any pacman players collided with munchies
	for (int i = 0; i < _speedUpArray.size(); i++)
	{
		bool colliderHit = false;

		colliderHit = Collision(_pacman1->GetColliderRect(), _speedUpArray[i]->GetColliderRect());
		if (colliderHit)
		{
			_pacman1->SetSpeedIncreaser(_speedUpArray[i]->GetEffect());
			_speedUpArray.erase(_speedUpArray.begin() + i);
			//Audio::Play(_pop);
			colliderHit = false;
			continue;
		}
		colliderHit = Collision(_pacman2->GetColliderRect(), _speedUpArray[i]->GetColliderRect());
		if (colliderHit)
		{
			_pacman2->SetSpeedIncreaser(_speedUpArray[i]->GetEffect());
			_speedUpArray.erase(_speedUpArray.begin() + i);
			//Audio::Play(_pop);
			colliderHit = false;
			continue;
		}
	}

	//check if any pacman players collided with munchies
	for (int i = 0; i < _speedDownArray.size(); i++)
	{
		bool colliderHit = false;

		colliderHit = Collision(_pacman1->GetColliderRect(), _speedDownArray[i]->GetColliderRect());
		if (colliderHit)
		{
			_pacman2->SetSpeedDecreaser(_speedDownArray[i]->GetEffect());
			_speedDownArray.erase(_speedDownArray.begin() + i);
			//Audio::Play(_pop);
			colliderHit = false;
			continue;
		}
		colliderHit = Collision(_pacman2->GetColliderRect(), _speedDownArray[i]->GetColliderRect());
		if (colliderHit)
		{
			_pacman1->SetSpeedDecreaser(_speedDownArray[i]->GetEffect());
			_speedDownArray.erase(_speedDownArray.begin() + i);
			//Audio::Play(_pop);
			colliderHit = false;
			continue;
		}
	}
}

void GameController::SpawnMunchie(int playerNum)
{
	if (playerNum == 1)
	{
		int randChoice = rand() % _freeSpaceArray1.size();;
			
		float freeSpaceX = _freeSpaceArray1[randChoice]->GetPosition()->X;
		float freeSpaceY = _freeSpaceArray1[randChoice]->GetPosition()->Y;

		_munchie1 = new Munchies();
		_munchie1->SetPosition(freeSpaceX + 10, freeSpaceY + 10);
		_munchieArray.push_back(_munchie1);
	}
	else if (playerNum == 2)
	{
		int randChoice = rand() % _freeSpaceArray2.size();;
			
		float freeSpaceX = _freeSpaceArray2[randChoice]->GetPosition()->X;
		float freeSpaceY = _freeSpaceArray2[randChoice]->GetPosition()->Y;

		_munchie2 = new Munchies();
		_munchie2->SetPosition(freeSpaceX + 10, freeSpaceY + 10);
		_munchieArray.push_back(_munchie2);
	}
}

void GameController::SpawnCherry(int playerNum, int framesWaited)
{
	//random spawn time
	int randTime = rand() % 8000 + 12000;

	if (framesWaited >= randTime)
	{
		if (playerNum == 1)
		{
			int randChoice = rand() % _freeSpaceArray1.size();;

			float freeSpaceX = _freeSpaceArray1[randChoice]->GetPosition()->X;
			float freeSpaceY = _freeSpaceArray1[randChoice]->GetPosition()->Y;

			_cherry = new Cherry();
			_cherry->SetPosition(freeSpaceX, freeSpaceY + 5);
			_cherryArray.push_back(_cherry);
			_cherryEaten1 = false;
			_cherryFramesWaited1 = 0;
		}
		else if (playerNum == 2)
		{
			int randChoice = rand() % _freeSpaceArray2.size();;

			float freeSpaceX = _freeSpaceArray2[randChoice]->GetPosition()->X;
			float freeSpaceY = _freeSpaceArray2[randChoice]->GetPosition()->Y;

			_cherry = new Cherry();
			_cherry->SetPosition(freeSpaceX, freeSpaceY + 5);
			_cherryArray.push_back(_cherry);
			_cherryEaten2 = false;
			_cherryFramesWaited2 = 0;
		}
	}
}

void GameController::SpawnBarrier(float x, float y)
{
	_barrier = new Barrier();
	_barrier->SetPosition(x * _barrier->GetColliderRect()->Height, y * _barrier->GetColliderRect()->Width);
	_barrierArray.push_back(_barrier);
}

void GameController::SpawnPowerUp(int framesWaited)
{
	//random spawn time
	int randTime = rand() % 4000 + 8000;
	int randPower = rand() % 2 + 1;

	if (framesWaited >= randTime)
	{
		/*KEY:
		1 = Speed Up
		2 = Speed Down
		*/

		for (int i = 0; i < _speedUpArray.size(); i++)
		{
			_speedUpArray.clear();
		}

		for (int i = 0; i < _speedDownArray.size(); i++)
		{
			_speedDownArray.clear();
		}
		
		switch (randPower)
		{
		case 1:
			_speedUp = new Speed_PowerUp();
			SetPowerPosition(_speedUp);
			_speedUpArray.push_back(_speedUp);
			_currentPower = randPower;
			break;
		case 2:
			_speedDown = new Slowness_PowerUp();
			SetPowerPosition(_speedDown);
			_speedDownArray.push_back(_speedDown);
			_currentPower = randPower;
			break;
		}

		_powerFramesWaited = 0;
	}
}

void GameController::SetPowerPosition(GameObject* powerUp)
{
	int randPlayer = rand() % 100;

	if (randPlayer <= 50)
	{
		int randChoice = rand() % _freeSpaceArray1.size();;

		float freeSpaceX = _freeSpaceArray1[randChoice]->GetPosition()->X;
		float freeSpaceY = _freeSpaceArray1[randChoice]->GetPosition()->Y;

		powerUp->SetPosition(freeSpaceX, freeSpaceY);
	}
	else if (randPlayer > 50)
	{
		int randChoice = rand() % _freeSpaceArray2.size();;

		float freeSpaceX = _freeSpaceArray2[randChoice]->GetPosition()->X;
		float freeSpaceY = _freeSpaceArray2[randChoice]->GetPosition()->Y;

		powerUp->SetPosition(freeSpaceX, freeSpaceY);
	}
}

void GameController::SetLevelToLoad(int levelToLoad)
{
	//set correct level
	_levelToLoad = levelToLoad;
}

void GameController::SetPointsGoal(int pointsGoal)
{
	_pointsGoal = pointsGoal;
}

void GameController::Level(string levelF)
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
		if (sline->size() != width)
			cout << "Bad Level Load\n";
		stream.getline(line, 256);
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
			else if (tileType == '0')
			{
				_freeSpace = new GameObject();
				_freeSpace->SetPosition(x * _barrier->GetColliderRect()->Height, y * _barrier->GetColliderRect()->Width);
				_freeSpaceArray1.push_back(_freeSpace);
			}
			else if (tileType == '2')
			{
				_freeSpace = new GameObject();
				_freeSpace->SetPosition(x * _barrier->GetColliderRect()->Height, y * _barrier->GetColliderRect()->Width);
				_freeSpaceArray2.push_back(_freeSpace);
			}
		}
	}

	delete lines;
}

void GameController::Restart()
{
	//delete all objects in arrays
	_munchieArray.clear();
	_cherryArray.clear();
	_speedUpArray.clear();
	_speedDownArray.clear();

	//spawn munchies for first time
	SpawnMunchie(1);
	SpawnMunchie(2);

	//reset pacman vars
	_pacman1->Restart();
	_pacman2->Restart();

	//reset ints
	_cherryFramesWaited1 = 0;
	_cherryFramesWaited2 = 0;

	//reset bools
	_paused = false;
	_pKeyDown = false;
	_gameStarted = false;
	_cherryEaten1 = true;
	_cherryEaten2 = true;
}