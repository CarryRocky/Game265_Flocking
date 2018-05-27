#include "SFMLHeader.h"
#include "flockingSystem.h"
#include "propertyHeader.h"

FlockingSystem fSys;
RenderWindow window;

void update_state(float dt)
{
	fSys.updateSystem(dt);
	if (Mouse::isButtonPressed(Mouse::Left))
	{
		fSys.addNewFlocking();
	}
	if (Mouse::isButtonPressed(Mouse::Right))
	{
		fSys.decreaseFlocking();
	}
}

void render_frame()
{
	window.clear();
	vector<FlockingElement*> allFlocking = fSys.getAllFlocking();
	for (int i = 0; i < allFlocking.size(); i++)
	{
		window.draw(*allFlocking[i]);
	}
}

int main()
{
	srand((unsigned)time(0));

	window.create(VideoMode(WIN_WIDTH, WIN_HEIGHT), "Flocking !");
	
	Clock clock;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		float dt = clock.restart().asSeconds();
		update_state(dt);
		render_frame();
		window.display();
	}

	return 0;
}

