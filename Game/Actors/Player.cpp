#include "Player.h"
#include "Math/MathUtils.h"
#include "Projectile.h"
#include "Enemy.h"
#include <memory>

Player::Player(const pbls::Transform& transform, std::shared_ptr<pbls::Shape> shape, float speed) : pbls::Actor{ transform, shape }, speed{ speed }
{
	

}

void Player::Initialize()
{
	invincibilityTimer = invincibilityTimerSet;

	std::unique_ptr locator = std::make_unique<Actor>();
	locator->transform.localPosition = pbls::Vector2{ 8,0 };
	AddChild(std::move(locator));

	locator = std::make_unique<Actor>();
	locator->transform.localPosition = pbls::Vector2{ 0,5 };
	AddChild(std::move(locator));

	locator = std::make_unique<Actor>();
	locator->transform.localPosition = pbls::Vector2{ 0,-5 };
	AddChild(std::move(locator));

	locator = std::make_unique<Actor>();
	locator->transform.localPosition = pbls::Vector2{ -10, 0 };
	locator->transform.localRotation = pbls::DegToRad(180);
	AddChild(std::move(locator));

	std::unique_ptr engine = std::make_unique<Actor>(pbls::Transform{}, scene->engine->Get<pbls::ResourceSystem>()->Get<pbls::Shape>("Engine.txt"));
	engine->transform.localPosition = pbls::Vector2{ -8, 0 };
	engine->transform.localScale = 0.6f;
	AddChild(std::move(engine));
}

void Player::Update(float dt)
{
	if (invincibilityTimer > 0)
	{
		invincibilityTimer -= dt;
		this->shape->color = { 0.9027246f, 0.5453452f, 0.7f };
	}
	else
	{
		this->shape->color = { 0.9027246f, 0.5453452f, 0.8949556f };
	}

	if (Core::Input::IsPressed('A')) { children[4]->transform.localRotation = pbls::QuarterPi; }
	if (Core::Input::IsPressed('D')) { children[4]->transform.localRotation = -pbls::QuarterPi; }
	if (!Core::Input::IsPressed('A') && !Core::Input::IsPressed('D')) { children[4]->transform.localRotation = 0; }

	float thrust = 0;
	if (Core::Input::IsPressed('A')) { transform.rotation -= 5 * dt; }
	if (Core::Input::IsPressed('D')) { transform.rotation += 5 * dt; }
	if (Core::Input::IsPressed('W')) { thrust = speed; }
	//if (Core::Input::IsPressed('S')) { input += pbls::Vector2::down; }

	pbls::Vector2 acceleration;
	acceleration += pbls::Vector2::Rotate(pbls::Vector2::right, transform.rotation) * thrust;
	pbls::Vector2 gravity = (pbls::Vector2{ 400, 300 } - transform.position).Normalized() * 30;
	acceleration += gravity;

	velocity += acceleration * dt;

	transform.position += velocity * dt;

	velocity *= 0.98f;

	transform.position.x = pbls::Wrap(transform.position.x, 0.0f, 800.0f);
	transform.position.y = pbls::Wrap(transform.position.y, 0.0f, 600.0f);

	if (shotsFire % 10 == 1)
	{
		shotsFire++;
		bombs++;
	}

	//fire
	fireTimer -= dt;
	if (fireTimer <= 0 && Core::Input::IsPressed('V'))
	{
		if (weaponType == 0) { weaponType = 1;}
		else { weaponType = 0; }
	}
	if (weaponType == 0)
	{
		fireRate = 0.3f;
		if (fireTimer <= 0 && Core::Input::IsPressed(VK_SPACE))
		{
			shotsFire++;

			fireTimer = fireRate;
			{
				pbls::Transform t = children[1]->transform;
				t.scale = 1.5f;

				std::unique_ptr<Projectile> projectile = (std::make_unique<Projectile>(t, scene->engine->Get<pbls::ResourceSystem>()->Get<pbls::Shape>("Bullet.txt"), 600));
				projectile->tag = "Player";
				scene->AddActor(std::move(projectile));
			}
			{
				pbls::Transform t = children[2]->transform;
				t.scale = 1.5f;

				std::unique_ptr<Projectile> projectile = (std::make_unique<Projectile>(t, scene->engine->Get<pbls::ResourceSystem>()->Get<pbls::Shape>("Bullet.txt"), 600));
				projectile->tag = "Player";
				scene->AddActor(std::move(projectile));
			}
			scene->engine->Get<pbls::AudioSystem>()->PlayAudio("playerFire");

		}
	}
	if (weaponType == 1)
	{
		fireRate = 0.2f;
		if (fireTimer <= 0 && Core::Input::IsPressed(VK_SPACE))
		{
			shotsFire++;

			fireTimer = fireRate;
			{
				pbls::Transform t = children[0]->transform;
				t.scale = 1.5f;

				std::unique_ptr<Projectile> projectile = (std::make_unique<Projectile>(t, scene->engine->Get<pbls::ResourceSystem>()->Get<pbls::Shape>("Bullet.txt"), 600));
				projectile->tag = "Player";
				scene->AddActor(std::move(projectile));
			}
			scene->engine->Get<pbls::AudioSystem>()->PlayAudio("playerFire");

		}
	}
	if (fireTimer <= 0 && Core::Input::IsPressed('C') && bombs > 0)
	{
		bombs--;
		fireTimer = fireRate;

		pbls::Transform t = children[0]->transform;
		t.scale = 1.5f;

		for (int i = 0; i < 12; i++)
		{
			t.rotation += pbls::DegToRad(i * 30);
			std::unique_ptr<Projectile> projectile = (std::make_unique<Projectile>(t, scene->engine->Get<pbls::ResourceSystem>()->Get<pbls::Shape>("Bullet.txt"), 600));
			projectile->tag = "Player";
			scene->AddActor(std::move(projectile));
		}
	}

	std::vector<pbls::Color> smoke = { pbls::Color::white, pbls::Color::yellow, pbls::Color::red, pbls::Color::orange };
	scene->engine->Get<pbls::ParticleSystem>()->Create(children[3]->transform.position, 3, 2, smoke, 40, children[3]->transform.rotation, pbls::DegToRad(15));

	Actor::Update(dt);
}

void Player::OnCollision(Actor* actor)
{

	if (dynamic_cast<Enemy*>(actor) && invincibilityTimer <= 0)
	{
		PlayerHit();
	}
	if (dynamic_cast<Projectile*>(actor) && actor->tag == "Enemy" && invincibilityTimer <= 0)
	{
		PlayerHit();
	}
	
}

void Player::PlayerHit()
{
	scene->engine->Get<pbls::ParticleSystem>()->Create(transform.position, 50, 2, pbls::Color::red, 40);
	scene->engine->Get<pbls::AudioSystem>()->PlayAudio("explosion");

	pbls::Event event;
	event.name = "PlayerHit";
	event.data = std::string("yes I'm dead!");
	scene->engine->Get<pbls::EventSystem>()->Notify(event);

	invincibilityTimer = invincibilityTimerSet;
}
