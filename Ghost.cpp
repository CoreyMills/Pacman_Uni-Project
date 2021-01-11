#include "Ghost.h"
#include <iostream>

Ghost::Ghost() : GameObject(), _cGhostSpeed(0.2f), _cGhostFrameTime(300)
{
	LoadContent();
}

Ghost::~Ghost()
{
}

void Ghost::LoadContent()
{
	// Load Ghost
	_texture = new Texture2D();
	_texture->Load("Textures/GhostBlue.png", false);
	_position = new Vector2(0, 0);
	_originalPos = new Vector2(0, 0);
	_sourceRect = new Rect(0.0f, 0.0f, 20, 20);
	_colliderRect = new Rect(0.0f, 0.0f, 20, 20);

	//random move vars
	_direction = 0;
	_randTime = 100;
	_randDir = 0;
	_changeDirTime = 0;

	//route vars
	_chaseDist = 0.0f;
	_target = new Vector2(0, 0);
	_routeType = 0;
	_currentTarget = 0;
	_routeArray.push_back(new Vector2(480, 270));
	_routeArray.push_back(new Vector2(480, 810));
	_routeArray.push_back(new Vector2(1440, 810));
	_routeArray.push_back(new Vector2(1440, 270));

	//other vars
	_blocked = false;

	//sound vars
	_ghostNoise = new SoundEffect();
	_ghostNoise->Load("Sounds/GhostlyNoise.wav");
}

void Ghost::Update(int elapsedTime)
{
	if (!_paused)
	{
		if (_routeType == 1)
		{
			PathPatrol(elapsedTime);
		}
		else if (_routeType == 2)
		{
			_changeDirTime += elapsedTime;
			RandPatrol(elapsedTime);
			if (_changeDirTime > _randTime)
			{
				_direction = _randDir;

				//reset vars
				_changeDirTime = 0;
				_randTime = rand() % 150 + 200;
				_randDir = rand() % 4;
			}
		}
	}
}

void Ghost::Draw(int elapsedTime)
{
	//increase frameCount
	_currentFrameTime += elapsedTime;

	if (_currentFrameTime > _cGhostFrameTime)
	{
		_animationCount++;

		if (_animationCount >= 2)
		{
			_animationCount = 0;
		}

		_currentFrameTime = 0;
	}

	//change ghost sprite to move ghost tails
	//change X source rect
	_sourceRect->X = _sourceRect->Width * _animationCount;

	//change ghost sprite to correct sprite direction
	//change Y source rect
	_sourceRect->Y = _sourceRect->Height * _direction;

	// Draws Pacman
	SpriteBatch::Draw(_texture, _position, _sourceRect);
}

void Ghost::PathPatrol(int elapsedTime)
{
	Vector2 dist;
	float distFloat1;
	float distFloat2;

	//if no target, set to be closest
	if (_target->X == 0 && _target->Y == 0)
	{
		for (int i = 0; i < _routeArray.size(); i++)
		{

			Vector2 route1Pos; route1Pos.X = _routeArray[i]->X; route1Pos.Y = _routeArray[i]->Y;
			Vector2 route2Pos; route2Pos.X = _routeArray[_currentTarget]->X; route2Pos.Y = _routeArray[_currentTarget]->Y;
			Vector2 ghostPos; ghostPos.X = _position->X; ghostPos.Y = _position->Y;

			//get compare distances between current target and next possible target
			distFloat1 = dist.Distance(route1Pos, ghostPos);
			distFloat2 = dist.Distance(route2Pos, ghostPos);

			if (distFloat1 <= distFloat2)
			{
				_currentTarget = i;
				_target = _routeArray[_currentTarget];
			}
		}
	}

	//if reached target move to next
	if (_position->X < (_routeArray[_currentTarget]->X + 1.5f) &&
		_position->X >(_routeArray[_currentTarget]->X - 1.5f) &&
		_position->Y < (_routeArray[_currentTarget]->Y + 1.5f) &&
		_position->Y >(_routeArray[_currentTarget]->Y - 1.5f))
	{
		if (_currentTarget < 3)
		{
			_currentTarget++;
			_target = _routeArray[_currentTarget];
		}
		else
		{
			_currentTarget = 0;
			_target = _routeArray[_currentTarget];
		}
	}

	Vector2 playerPos; playerPos.X = _player->GetPosition()->X; playerPos.Y = _player->GetPosition()->Y;
	Vector2 ghostPos; ghostPos.X = _position->X; ghostPos.Y = _position->Y;

	distFloat1 = dist.Distance(playerPos, ghostPos);
	if (distFloat1 <= _chaseDist)
	{
		Audio::Play(_ghostNoise);
		//give the player as the target
		ChaseTarget(_player->GetPosition(), elapsedTime);
	}
	else
	{
		//give the next next point in array as the target
		ChaseTarget(_target, elapsedTime);
	}
}

void Ghost::RandPatrol(int elapsedTime)
{
	Vector2 dist;
	float distFloat1;

	Vector2 playerPos; playerPos.X = _player->GetPosition()->X; playerPos.Y = _player->GetPosition()->Y;
	Vector2 ghostPos; ghostPos.X = _position->X; ghostPos.Y = _position->Y;

	distFloat1 = dist.Distance(playerPos, ghostPos);
	if (distFloat1 <= _chaseDist)
	{
		Audio::Play(_ghostNoise);
		//give the player as the target
		ChaseTarget(_player->GetPosition(), elapsedTime);
	}
	else
	{
		//right
		if (_direction == 0)
		{
			_position->X += _cGhostSpeed * elapsedTime;
		}
		//left
		else if (_direction == 1)
		{
			_position->X -= _cGhostSpeed * elapsedTime;
		}
		//down
		else if (_direction == 2)
		{
			_position->Y += _cGhostSpeed * elapsedTime;
		}
		//up
		else if (_direction == 3)
		{
			_position->Y -= _cGhostSpeed * elapsedTime;
		}
	}
}

void Ghost::ChaseTarget(Vector2* target, int elapsedTime)
{
	//help
	Vector2 targetPos; targetPos.X = target->X; targetPos.Y = target->Y;
	
	if (abs(targetPos.X - _position->X) > abs(targetPos.Y - _position->Y))
	{
		if (targetPos.X > _position->X)
		{
			//right
			_direction = 0;
			_position->X += _cGhostSpeed * elapsedTime;
		}
		else
		{
			//left
			_direction = 1;
			_position->X -= _cGhostSpeed * elapsedTime;
		}
	}
	else
	{
		if (targetPos.Y > _position->Y)
		{
			//down
			_direction = 2;
			_position->Y += _cGhostSpeed * elapsedTime;
		}
		else
		{
			//up
			_direction = 3;
			_position->Y -= _cGhostSpeed * elapsedTime;
		}
	}
	round(_position->X); round(_position->Y);
}

Vector2* Ghost::GetPosition()
{
	return _position;
}

void Ghost::SetBlocked(bool blocked)
{
	_blocked = blocked;
}

void Ghost::SetGhost(int ghostNum)
{
	switch (ghostNum)
	{
	case 1:
		_texture->Load("Textures/GhostBlue.png", false);
		break;
	case 2:
		_texture->Load("Textures/GhostPink.png", false);
		break;
	case 3:
		_texture->Load("Textures/GhostGrey.png", false);
		break;
	case 4:
		_texture->Load("Textures/GhostGreen.png", false);
		break;
	}
}

void Ghost::SetRouteType(int routeNum)
{
	_routeType = routeNum;
}

void Ghost::SetChaseDist(int chaseDist)
{
	_chaseDist = chaseDist;
}

void Ghost::SetPlayerObject(GameObject* player)
{
	_player = player;
}

void Ghost::SetOriginalPosition(float x, float y)
{
	_originalPos->X = x; _originalPos->Y = y;
}

void Ghost::Restart()
{
	//reset vars
	*_position = *_originalPos;

	//random move vars
	_direction = 0;
	_randTime = 0;
	_randDir = 0;
	_changeDirTime = 0;

	//route vars
	_target = new Vector2(0, 0);
	_currentTarget = 0;

	//other vars
	_blocked = false;
}