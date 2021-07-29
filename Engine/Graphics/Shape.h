#pragma once
#include "Math/Vector2.h"
#include "Math/Color.h"
#include "Math/Transform.h"
#include "Framework/ResourceSystem.h"
#include <vector>
#include <string>

namespace pbls
{
	class Shape : public Resource
	{
	public:
		Shape() {}
		Shape(std::vector<Vector2> points, Color color) : points{ points }, color{ color } 
		{
			ComputeRadius();
		}

		void Draw(Core::Graphics& graphics, const Vector2 position,float angle, float scale = 1);
		void Draw(Core::Graphics& graphics, const Transform& transform);
		
		bool Load(const std::string& filename);

	private:
		void ComputeRadius();

	public:
		std::vector<Vector2> points;
		Color color;
		float radius{ 0 };

	};
}