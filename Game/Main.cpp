#include "core.h"
#include <iostream>

bool Update(float dt)
{ 

	return false; 

}

void Draw(Core::Graphics& graphics) 
{ 
	graphics.SetBackgroundColor(RGB(rand() % 256, rand() % 256, rand() % 256));
	for (int i = 0; i < 1000; i++) {
		graphics.SetColor(RGB(rand() % 256, rand() % 256, rand() % 256));
		graphics.DrawLine(static_cast<float>(rand() % 801), static_cast<float>(rand() % 601), static_cast<float>(rand() % 801), static_cast<float>(rand() % 601));
	}
}

int main()
{
	char name[] = "CSC196";
	Core::Init(name, 800, 600);
	Core::RegisterUpdateFn(Update);
	Core::RegisterDrawFn(Draw);


	Core::GameLoop();
	Core::Shutdown();
}