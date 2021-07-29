#pragma once
#include "Engine.h"

class Game
{
public:
	enum class eState
	{
		Title,
		StartGame,
		StartLevel,
		Game,
		GameOver,
	};
public:
	void Initialize();
	void Shutdown();
	
	void Update(float dt);
	void Draw(Core::Graphics& graphics);

private:
	void UpdateTitle(float dt);
	void UpdateLevelStart(float dt);
	void OnAddPoints(const pbls::Event& event);
	void OnPlayerHit(const pbls::Event& event);
	void GameLoop();

public:
	std::unique_ptr<pbls::Engine> engine;
	std::unique_ptr<pbls::Scene> scene;

private:
	eState state = eState::Title;
	float stateTimer{ 0.0f };

	void (Game::*stateFunction)(float) = nullptr;

	size_t score{ 0 };
	size_t lives{ 0 };
	size_t round{ 0 };

	std::string roundDisplay{};
	std::string highscore{};
};