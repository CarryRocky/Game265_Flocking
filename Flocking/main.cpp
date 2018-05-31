#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC_NEW
#include <cstdlib>
#include <crtdbg.h>
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif

#include "SFMLHeader.h"
#include "flockingSystem.h"
#include "propertyHeader.h"

FlockingSystem _fSys;
RenderWindow _window;
float _pressGap;
Font _font;
Text _numText;
Text _seperationText;
Text _alignmentText;
Text _cohesionText;

void updateText()
{
	_numText.setString("Flocking Number : " + to_string(_fSys.getSysSize()));
	_seperationText.setString("Seperation Factor : " + to_string(_fSys.getSeperationFactor()));
	_alignmentText.setString("Alignment Factor : " + to_string(_fSys.getAlignmentFactor()));
	_cohesionText.setString("Cohesion Factor : " + to_string(_fSys.getCohesionFactor()));
}

void update_state(float dt)
{
	_fSys.updateSystem(dt);
	updateText();
	_pressGap -= dt;

	if (_pressGap <= 0)
	{
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			_fSys.addNewFlocking();
			_pressGap = GAP_TIME;
		}
		if (Mouse::isButtonPressed(Mouse::Right))
		{
			_fSys.decreaseFlocking();
			_pressGap = GAP_TIME;
		}

		if (Keyboard::isKeyPressed(Keyboard::Q))
		{
			_fSys.addSeperationFactor(1);
			_pressGap = GAP_TIME;
		}

		if (Keyboard::isKeyPressed(Keyboard::A))
		{
			_fSys.addSeperationFactor(-1);
			_pressGap = GAP_TIME;
		}

		if (Keyboard::isKeyPressed(Keyboard::W))
		{
			_fSys.addAlignmentFactor(1);
			_pressGap = GAP_TIME;
		}

		if (Keyboard::isKeyPressed(Keyboard::S))
		{
			_fSys.addAlignmentFactor(-1);
			_pressGap = GAP_TIME;
		}

		if (Keyboard::isKeyPressed(Keyboard::E))
		{
			_fSys.addCohesionFactor(1);
			_pressGap = GAP_TIME;
		}

		if (Keyboard::isKeyPressed(Keyboard::D))
		{
			_fSys.addCohesionFactor(-1);
			_pressGap = GAP_TIME;
		}
	}
}

void render_frame()
{
	_window.clear();
	_fSys.renderAllFlocking(&_window);
	_window.draw(_numText);
	_window.draw(_seperationText);
	_window.draw(_alignmentText);
	_window.draw(_cohesionText);
}

void initText()
{
	Vector2f initPos = Vector2f(300, 200);
	int gapInt = 50;
	int fontSize = 30;
	_numText.setFont(_font);
	_numText.setCharacterSize(fontSize);
	_numText.setFillColor(Color::Yellow);
	_numText.setPosition(initPos);

	_seperationText.setFont(_font);
	_seperationText.setCharacterSize(fontSize);
	_seperationText.setFillColor(Color::Yellow);
	_seperationText.setPosition(Vector2f(initPos.x,initPos.y + gapInt));

	_alignmentText.setFont(_font);
	_alignmentText.setCharacterSize(fontSize);
	_alignmentText.setFillColor(Color::Yellow);
	_alignmentText.setPosition(Vector2f(initPos.x, initPos.y + 2 * gapInt));

	_cohesionText.setFont(_font);
	_cohesionText.setCharacterSize(fontSize);
	_cohesionText.setFillColor(Color::Yellow);
	_cohesionText.setPosition(Vector2f(initPos.x, initPos.y + 3 * gapInt));
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	srand((unsigned)time(0));

	_window.create(VideoMode(WIN_WIDTH, WIN_HEIGHT), "Flocking !");
	
	Clock clock;

	_pressGap = 0;

	_font.loadFromFile("calibri.ttf");

	initText();

	while (_window.isOpen())
	{
		Event event;
		while (_window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				_window.close();
		}

		float dt = clock.restart().asSeconds();
		update_state(dt);
		render_frame();
		_window.display();
	}

	return 0;
}

