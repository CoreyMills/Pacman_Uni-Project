#include "Slowness_PowerUp.h"
#include <sstream>

Slowness_PowerUp::Slowness_PowerUp() : GameObject()
{
	LoadContent();
}

Slowness_PowerUp::~Slowness_PowerUp()
{
}

void Slowness_PowerUp::LoadContent()
{
	// Load Munchie
	_texture = new Texture2D();
	_texture->Load("Textures/SpeedDown.png", true);
	_position = new Vector2(240, 320);
	_sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_colliderRect = new Rect(0.0f, 0.0f, 18, 28);

	//declare vars to animate munchie
	_frameTime = 150;

	//effect vars
	_effect = 0.5f;
}

void Slowness_PowerUp::Update(int elapsedTime)
{
	Animate(elapsedTime);
}

void Slowness_PowerUp::Draw(int elapsedTime)
{
	//animate munchie
	//move source rect position
	_sourceRect->X = _sourceRect->Width * _animationCount;

	// Draws Red Munchie
	SpriteBatch::Draw(_texture, _position, _sourceRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
}

void Slowness_PowerUp::Animate(int elapsedTime)
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

float Slowness_PowerUp::GetEffect()
{
	return _effect;
}