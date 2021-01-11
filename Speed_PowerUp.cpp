#include "Speed_PowerUp.h"
#include <sstream>

Speed_PowerUp::Speed_PowerUp() : GameObject()
{
	LoadContent();
}

Speed_PowerUp::~Speed_PowerUp()
{
}

void Speed_PowerUp::LoadContent()
{
	// Load Munchie
	_texture = new Texture2D();
	_texture->Load("Textures/SpeedUp.png", true);
	_position = new Vector2(240, 320);
	_sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_colliderRect = new Rect(0.0f, 0.0f, 18, 28);

	//declare vars to animate munchie
	_frameTime = 150;

	//effect vars
	_effect = 2;
}

void Speed_PowerUp::Update(int elapsedTime)
{
	Animate(elapsedTime);
}

void Speed_PowerUp::Draw(int elapsedTime)
{
	//animate munchie
	//move source rect position
	_sourceRect->X = _sourceRect->Width * _animationCount;

	// Draws Red Munchie
	SpriteBatch::Draw(_texture, _position, _sourceRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
}

void Speed_PowerUp::Animate(int elapsedTime)
{
	//increase frameCount
	_currentFrameTime += elapsedTime;

	if (_currentFrameTime > _frameTime)
	{
		_animationCount++;

		if (_animationCount >= 4)
		{
			_animationCount = 0;
		}

		_currentFrameTime = 0;
	}
}

float Speed_PowerUp::GetEffect()
{
	return _effect;
}