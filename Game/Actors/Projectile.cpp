#include "Projectile.h"
#include "Math/MathUtils.h"
#include "Engine.h"
#include "Enemy.h"
#include "Player.h"

void Projectile::Update(float dt)
{
	lifetime -= dt;
	destroy =  (lifetime <= 0);
	
	transform.position += pbls::Vector2::Rotate(pbls::Vector2::right, transform.rotation) * speed * dt;

	if (tag == "Player")
	{
		transform.position.x = pbls::Wrap(transform.position.x, 0.0f, 800.0f);
		transform.position.y = pbls::Wrap(transform.position.y, 0.0f, 600.0f);
	}
	std::vector<pbls::Color> colors = { pbls::Color::white, pbls::Color::red, pbls::Color::blue };
	scene->engine->Get<pbls::ParticleSystem>()->Create(transform.position, 3, 0.5f, colors[pbls::RandomRangeInt(0, colors.size())], 10);
	
	Actor::Update(dt);
}

void Projectile::OnCollision(Actor* actor)
{
	if ((dynamic_cast<Enemy*>(actor) && tag == "Player") || (dynamic_cast<Player*>(actor) && tag == "Enemy"))
	{
		destroy = true;
	}
}
