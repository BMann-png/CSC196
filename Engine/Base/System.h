#pragma once
#include "core.h"

namespace pbls
{
	class System
	{
	public:
		virtual void Startup() = 0;
		virtual void Shutdown() = 0;
		virtual void Update(float dt) = 0;
	};

	class GraphicSystem : public System
	{
	public:
		virtual void Draw(Core::Graphics& graphics) = 0;
	};
}