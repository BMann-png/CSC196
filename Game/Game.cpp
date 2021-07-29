#include "Game.h"
#include "Actors/Player.h"
#include "Actors/Enemy.h"
#include "Actors/Projectile.h"

void Game::Initialize()
{
	engine = std::make_unique<pbls::Engine>();
	engine->Startup();
	scene = std::make_unique<pbls::Scene>();
	scene->engine = engine.get();

	engine->Get<pbls::AudioSystem>()->AddAudio("explosion", "Explosion.wav");
	engine->Get<pbls::AudioSystem>()->AddAudio("playerFire", "playerFire.wav");
	engine->Get<pbls::AudioSystem>()->AddAudio("enemyFire", "enemyFire.wav");
	engine->Get<pbls::AudioSystem>()->AddAudio("backgroundMusic", "GameLoop.mid");
	
	engine->Get<pbls::EventSystem>()->Subscribe("AddPoints", std::bind(&Game::OnAddPoints, this, std::placeholders::_1));
	engine->Get<pbls::EventSystem>()->Subscribe("PlayerHit", std::bind(&Game::OnPlayerHit, this, std::placeholders::_1));
//	stateFunction = &Game::UpdateTitle;

}

void Game::Shutdown()
{
	scene->RemoveAllActors();
	engine->Shutdown();
}

void Game::Update(float dt)
{
	stateTimer += dt;

//	(this->*stateFunction)(dt);

	switch (state)
	{
	case Game::eState::Title:
		UpdateTitle(dt);
		break;
	case Game::eState::StartGame:
		score = 0;
		lives = 3;

		engine->Get<pbls::AudioSystem>()->PlayAudio("backgroundMusic");

		state = eState::StartLevel;
		break;
	case Game::eState::StartLevel:
		UpdateLevelStart(dt);
		break;
	case Game::eState::Game:
		GameLoop();

		if (lives <= 0)
		{
			state = eState::GameOver;
		}
		break;
	case Game::eState::GameOver:

		scene->RemoveAllActors();
		
		if (Core::Input::IsPressed('N'))
		{
			state = eState::Title;
		}

		break;
	default:
		break;
	}

	engine->Update(dt);
	scene->Update(dt);
}

void Game::Draw(Core::Graphics& graphics)
{
	switch (state)
	{
	case Game::eState::Title:
		graphics.SetColor(pbls::Color::cyan);
		graphics.DrawString(30, 20, highscore.c_str());
		graphics.DrawString(360, 300 + static_cast<int>(std::sin(stateTimer * 6.0f) * 10.0f), "Vectrex");

		graphics.SetColor(pbls::Color::white);
		graphics.DrawString(320, 400, "Press n to Start");
		break;
	case Game::eState::StartGame:
		break;
	case Game::eState::StartLevel:
		break;
	case Game::eState::Game:
		roundDisplay = "Round";
		graphics.SetColor(pbls::Color::white);
		graphics.DrawString(30, 40, roundDisplay.c_str());
		graphics.DrawString(80, 40, std::to_string(round).c_str());

		graphics.DrawString(770, 40, std::to_string(scene->GetActor<Player>()->getBombs()).c_str());
		break;
	case Game::eState::GameOver:
		graphics.SetColor(pbls::Color::white);
		graphics.DrawString(360, 400, "Game Over");

		graphics.SetColor(pbls::Color::white);
		graphics.DrawString(320, 440, "Press n to Start Again");
		break;
	default:
		break;
	}

	graphics.SetColor(pbls::Color::white);
	graphics.DrawString(30, 20, std::to_string(score).c_str());
	graphics.DrawString(770, 20, std::to_string(lives).c_str());

	scene->Draw(graphics);
	engine->Draw(graphics);
}

void Game::OnAddPoints(const pbls::Event& event)
{
	score += std::get<int>(event.data);
}
void Game::OnPlayerHit(const pbls::Event& event)
{
	lives--;
	std::cout << std::get<std::string>(event.data) << std::endl;
}

void Game::GameLoop()
{
	if (scene->GetActors<Enemy>().size() == 0)
	{
		int numberOfEnemies = pbls::RandomRangeInt(1, 10);
		for (size_t i = 0; i < numberOfEnemies; i++)
		{
			scene->AddActor(std::make_unique <Enemy>(pbls::Transform{ { pbls::RandomRange(0.0f, 800.0f), pbls::RandomRange(0.0f, 10.0f) }, pbls::RandomRange(0.0f, pbls::TwoPi), 2 }, engine->Get<pbls::ResourceSystem>()->Get<pbls::Shape>("Enemy.txt"), 200));
		}
		round++;
	}
}

void Game::UpdateTitle(float dt)
{
	if (Core::Input::IsPressed('N'))
	{
		//stateFunction = &Game::UpdateLevelStart;
		state = eState::StartGame;
	}
}

void Game::UpdateLevelStart(float dt)
{
	{
		

		//std::vector<pbls::Vector2> points = { {-5 , -5}, {5 , -5}, {0, 5}, {-5, -5} };
		//std::shared_ptr<pbls::Shape> shape1 = std::make_shared<pbls::Shape>(points, pbls::Color{ 0, 1, 0 });
		//std::shared_ptr <pbls::Shape> shape2 = std::make_shared<pbls::Shape>(points, pbls::Color{ 0.2f, 1, 1 });


		scene->AddActor(std::make_unique<Player>(pbls::Transform{ { 400, 300 }, 0, 3 }, engine->Get<pbls::ResourceSystem>()->Get<pbls::Shape>("Spaceship.txt"), 300));
		for (size_t i = 0; i < 3; i++)
		{
			scene->AddActor(std::make_unique <Enemy>(pbls::Transform{ { pbls::RandomRange(0.0f, 800.0f), pbls::RandomRange(0.0f, 600.0f) }, pbls::RandomRange(0.0f, pbls::TwoPi), 2 }, engine->Get<pbls::ResourceSystem>()->Get<pbls::Shape>("Enemy.txt"), 200));
		}

		state = eState::Game;
	}
}
