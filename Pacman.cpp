#include "Pacman.h"
#include <sstream>
#include <iostream>

Pacman::Pacman() : GameObject(), _cPacmanSpeed(0.2f), _cPacmanFrameTime(250)
{
	LoadContent();
}

Pacman::~Pacman()
{
}

void Pacman::LoadContent()
{
	// Load Pacman
	_texture = new Texture2D();
	_texture->Load("Textures/Pacman.png", false);
	_position = new Vector2(0, 0);
	_originalPos = new Vector2(0, 0);
	_sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_colliderRect = new Rect(0.0f, 0.0f, 28, 28);

	//vars to animate pacman
	_pacmanDirection = 0;

	//speed vars
	_speedIncreaser = 1.0f;
	_speedDecreaser = 1.0f;
	_powerUpTimeLimit = 10000;
	_iSpeedChanged = false;
	_dSpeedChanged = false;

	//other vars
	_blocked = false;
	_bonusLevel = false;
	_playerKilled = false;
	_points = 0;
}

void Pacman::Update(int elapsedTime, Input::MouseState* mouseState)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* state = Input::Keyboard::GetState();
		
	if (!_paused)
	{
		//call Input method
		Input(elapsedTime, state, mouseState);

		//call UpdatePacman method
		Animate(elapsedTime);

		//not needed for program, barrier blocks (used for module marks)
		//CheckViewPortCollision();
	}

}

void Pacman::Draw(int elapsedTime)
{
	// Draws Pacman
	SpriteBatch::Draw(_texture, _position, _sourceRect);
}

void Pacman::Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState)
{
	float pacmanSpeed = 0;

	if (_bonusLevel && !_playerKilled)
	{
		//cout << _blocked << endl;
		if (mouseState->LeftButton == Input::ButtonState::PRESSED && !_blocked)
		{
			_position->X = mouseState->X;
			_position->Y = mouseState->Y;
		}
		else if (mouseState->LeftButton == Input::ButtonState::PRESSED && _blocked)
		{
			//play crash sound
		}
	}
	else
	{
		if (_player == 1)
		{
			/*Sprint, kept for module marks
			if (state->IsKeyDown(Input::Keys::LEFTSHIFT))
			{
				_pacmanSpeedMultiplier = 2.0f;
				pacmanSpeed = _cPacmanSpeed * elapsedTime * _pacmanSpeedMultiplier;
			}
			else
			{
				_pacmanSpeedMultiplier = 1.0f;
				pacmanSpeed = _cPacmanSpeed * elapsedTime * _pacmanSpeedMultiplier;
			}
			*/

			//return speed increaser to normal
			if (_iSpeedChanged)
			{
				_iSpeedCount += elapsedTime;
				
				if (_iSpeedCount >= _powerUpTimeLimit)
				{
					_speedIncreaser = 1.0f;
				}
			}
			//return speed decreaser to normal
			if (_dSpeedChanged)
			{
				_dSpeedCount += elapsedTime;

				if (_dSpeedCount >= _powerUpTimeLimit)
				{
					_speedDecreaser = 1.0f;
				}
			}

			pacmanSpeed = _cPacmanSpeed * elapsedTime * _speedIncreaser * _speedDecreaser;

			// Checks if D key is pressed
			if (state->IsKeyDown(Input::Keys::D))
			{
				_position->X += pacmanSpeed; //Moves Pacman across X axis
				_pacmanDirection = 0;
			}

			// Checks if s key is pressed
			else if (state->IsKeyDown(Input::Keys::S))
			{
				_position->Y += pacmanSpeed; //Moves Pacman across y axis
				_pacmanDirection = 1;
			}

			// Checks if A key is pressed
			else if (state->IsKeyDown(Input::Keys::A))
			{
				_position->X -= pacmanSpeed; //Moves Pacman across X axis
				_pacmanDirection = 2;
			}

			// Checks if W key is pressed
			else if (state->IsKeyDown(Input::Keys::W))
			{
				_position->Y -= pacmanSpeed; //Moves Pacman across y axis
				_pacmanDirection = 3;
			}
		}
		else if (_player == 2)
		{
			/*Sprint, kept for module marks
			if (state->IsKeyDown(Input::Keys::LEFTSHIFT))
			{
			_pacmanSpeedMultiplier = 2.0f;
			pacmanSpeed = _cPacmanSpeed * elapsedTime * _pacmanSpeedMultiplier;
			}
			else
			{
			_pacmanSpeedMultiplier = 1.0f;
			pacmanSpeed = _cPacmanSpeed * elapsedTime * _pacmanSpeedMultiplier;
			}
			*/

			//return speed increaser to normal
			if (_iSpeedChanged)
			{
				_iSpeedCount += elapsedTime;

				if (_iSpeedCount >= _powerUpTimeLimit)
				{
					_speedIncreaser = 1.0f;
				}
			}
			//return speed decreaser to normal
			if (_dSpeedChanged)
			{
				_dSpeedCount += elapsedTime;

				if (_dSpeedCount >= _powerUpTimeLimit)
				{
					_speedDecreaser = 1.0f;
				}
			}

			pacmanSpeed = _cPacmanSpeed * elapsedTime * _speedIncreaser * _speedDecreaser;

			// Checks if D key is pressed
			if (state->IsKeyDown(Input::Keys::RIGHT))
			{
				_position->X += pacmanSpeed; //Moves Pacman across X axis
				_pacmanDirection = 0;
			}

			// Checks if s key is pressed
			else if (state->IsKeyDown(Input::Keys::DOWN))
			{
				_position->Y += pacmanSpeed; //Moves Pacman across y axis
				_pacmanDirection = 1;
			}

			// Checks if A key is pressed
			else if (state->IsKeyDown(Input::Keys::LEFT))
			{
				_position->X -= pacmanSpeed; //Moves Pacman across X axis
				_pacmanDirection = 2;
			}

			// Checks if W key is pressed
			else if (state->IsKeyDown(Input::Keys::UP))
			{
				_position->Y -= pacmanSpeed; //Moves Pacman across y axis
				_pacmanDirection = 3;
			}
		}
	}
}

void Pacman::Animate(int elapsedTime)
{
	//increase frameCount
	_currentFrameTime += elapsedTime;

	//moving sourceRect.X
	if (_currentFrameTime > _cPacmanFrameTime)
	{
		_animationCount++;

		if (!_playerKilled)
		{
			if (_animationCount >= 2)
			{
				_animationCount = 0;
			}
		}
		else
		{
			if (_animationCount > 5)
			{
				_texture->Load("Textures/Transparency.png", false);
			}
		}
		_currentFrameTime = 0;
	}

	//change pacmans sprite to open and close mouth
	//change X source rect
	_sourceRect->X = _sourceRect->Width * _animationCount;

	//change pacmans sprite to correct sprite direction
	//change Y source rect
	_sourceRect->Y = _sourceRect->Height * _pacmanDirection;
}

void Pacman::CheckViewPortCollision()
{
	//checks if Pacman is trying to disappear
	if (_position->X + _sourceRect->Width > Graphics::GetViewportWidth())
	{
		//Pacman hit right wall - reset his position to left wall
		_position->X = 0;
	}

	//checks if Pacman is trying to disappear
	if (_position->X < 0)
	{
		//Pacman hit laft wall - reset his position to right wall
		_position->X = Graphics::GetViewportWidth() - _sourceRect->Width;;
	}

	//checks if Pacman is trying to disappear
	if (_position->Y + _sourceRect->Height > Graphics::GetViewportHeight())
	{
		//Pacman hit bottom wall - reset his position to top wall
		_position->Y = 0;
	}

	//checks if Pacman is trying to disappear
	if (_position->Y < 0)
	{
		//Pacman hit top wall - reset his position to bottom wall
		_position->Y = Graphics::GetViewportHeight() - _sourceRect->Height;
	}
}

void Pacman::ItemCollected(int pointsGained)
{
	_points += pointsGained;
}

int Pacman::GetPoints()
{
	return _points;
}

void Pacman::SetPoints(int newPoints)
{
	_points = newPoints;
}

void Pacman::SetPlayerNum(int playerNum)
{
	_player = playerNum;
}

void Pacman::SetBlocked(bool blocked)
{
	_blocked = blocked;
}

void Pacman::SetBonusLevel(bool bonusLevel)
{
	_bonusLevel = bonusLevel;
}

void Pacman::SetSpeedIncreaser(float speedChange)
{
	_speedIncreaser = 1.0f;
	_speedDecreaser = 1.0f;
	_speedIncreaser *= speedChange;
	_iSpeedCount = 0;
	_iSpeedChanged = true;
}

void Pacman::SetSpeedDecreaser(float speedChange)
{
	_speedDecreaser = 1.0f;
	_speedIncreaser = 1.0f;
	_speedDecreaser *= speedChange;
	_dSpeedCount = 0;
	_dSpeedChanged = true;
}

void Pacman::SetPlayerKilled(bool playerKilled)
{
	_playerKilled = playerKilled;
}

void Pacman::SetOriginalPosition(float x, float y)
{
	_originalPos->X = x; _originalPos->Y = y;
}

void Pacman::Restart()
{
	//reset pos
	*_position = *_originalPos;

	//vars to animate pacman
	_pacmanDirection = 0;

	//speed vars
	_speedIncreaser = 1.0f;
	_speedDecreaser = 1.0f;
	_iSpeedChanged = false;
	_dSpeedChanged = false;

	//other vars
	_blocked = false;
	SetPlayerKilled(false);
	_points = 0;
	_animationCount = 0;
	_texture->Load("Textures/Pacman.png", false);
}