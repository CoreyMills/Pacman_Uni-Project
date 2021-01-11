#include "Barrier.h"
#include <sstream>
#include <iostream>
#include <ctime>

Barrier::Barrier() : GameObject()
{
	LoadContent();
}

Barrier::~Barrier()
{
}

void Barrier::LoadContent()
{
	//Set Pause Menu Parameters
	_texture = new Texture2D();
	_texture->Load("Textures/Barrier.png", false);
	_sourceRect = new Rect(0.0f, 0.0f, 40, 40);
	_colliderRect = new Rect(0.0f, 0.0f, 40, 40);
	_position = new Vector2(0, 0);
}

void Barrier::Update(int elapsedTime)
{
}

void Barrier::Draw(int elapsedTime)
{
	// Draws String
	SpriteBatch::Draw(_texture, _position, _sourceRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
}