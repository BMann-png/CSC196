#pragma once
#include "Base/Actor.h"

class Enemy : public pbls::Actor
{
public:
	Enemy(const pbls::Transform& transform, std::shared_ptr<pbls::Shape> shape, float speed) : pbls::Actor{ transform, shape }, speed{ speed } {}


	void Update(float dt) override;
	void OnCollision(Actor* actor) override;

private:
	float speed{ 15 };
	float fireTimer{ 0 };
	float fireRate{ 3.0f };
};