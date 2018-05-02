
#include <ConsoleAPI.hpp>
#include <iostream>
#include <Panel.hpp>
#include <thread>
namespace a = apoganatz;

volatile bool cntrlHit = false;
void cntrlIsHit()
{
	cntrlHit = true;
}

struct GlobalInstanceTester
{
	apoganatz::IConsoleAPI& ref;
	GlobalInstanceTester() : ref(apoganatz::getConsoleInstance()) {};
};

GlobalInstanceTester instance;

void testInput()
{
	std::vector<apoganatz::InputEvent> events(128);
	size_t numOfEvents;
	bool notQuiting = true;
	std::cout << "Beginning input test, press Esc to exit." << std::endl;
	while (notQuiting)
	{
		numOfEvents = instance.ref.getInput(events);


		for (size_t x = 0; x < numOfEvents; ++x)
		{
			switch (events[x].eventCode)
			{
			case a::EventCode::ESC:
				std::cout << "ESC pressed" << std::endl;
				notQuiting = false;
				break;
			case a::EventCode::F1:
				std::cout << "F1 pressed" << std::endl;
				break;
			case a::EventCode::F2:
				std::cout << "F2 pressed" << std::endl;
				break;
			case a::EventCode::F3:
				std::cout << "F3 pressed" << std::endl;
				break;
			case a::EventCode::F4:
				std::cout << "F4 pressed" << std::endl;
				break;
			case a::EventCode::F5:
				std::cout << "F5 pressed" << std::endl;
				break;
			case a::EventCode::F6:
				std::cout << "F6 pressed" << std::endl;
				break;
			case a::EventCode::F7:
				std::cout << "F7 pressed" << std::endl;
				break;
			case a::EventCode::F8:
				std::cout << "F8 pressed" << std::endl;
				break;
			case a::EventCode::F9:
				std::cout << "F9 pressed" << std::endl;
				break;
			case a::EventCode::F10:
				std::cout << "F10 pressed" << std::endl;
				break;
			case a::EventCode::F11:
				std::cout << "F11 pressed" << std::endl;
				break;
			case a::EventCode::F12:
				std::cout << "F12 pressed" << std::endl;
				break;
			case a::EventCode::ENTER:
				std::cout << "ENTER pressed" << std::endl;
				break;
			case a::EventCode::UP:
				std::cout << "UP pressed" << std::endl;
				break;
			case a::EventCode::DOWN:
				std::cout << "DOWN pressed" << std::endl;
				break;
			case a::EventCode::LEFT:
				std::cout << "LEFT pressed" << std::endl;
				break;
			case a::EventCode::RIGHT:
				std::cout << "RIGHT pressed" << std::endl;
				break;
			case a::EventCode::BACKSPACE:
				std::cout << "Backspace pressed" << std::endl;
				break;
			case a::EventCode::DELETE:
				std::cout << "DELETE pressed" << std::endl;
				break;
			case a::EventCode::INSERT:
				std::cout << "INSERT pressed" << std::endl;
				break;
			case a::EventCode::PAGE_UP:
				std::cout << "PAGE_UP pressed" << std::endl;
				break;
			case a::EventCode::PAGE_DOWN:
				std::cout << "PAGE_DOWN pressed" << std::endl;
				break;
			case a::EventCode::HOME:
				std::cout << "HOME pressed" << std::endl;
				break;
			case a::EventCode::NUM_0:
				std::cout << "NUM_0 pressed" << std::endl;
				break;
			case a::EventCode::NUM_1:
				std::cout << "NUM_1 pressed" << std::endl;
				break;
			case a::EventCode::NUM_2:
				std::cout << "NUM_2 pressed" << std::endl;
				break;
			case a::EventCode::NUM_3:
				std::cout << "NUM_3 pressed" << std::endl;
				break;
			case a::EventCode::NUM_4:
				std::cout << "NUM_4 pressed" << std::endl;
				break;
			case a::EventCode::NUM_5:
				std::cout << "NUM_5 pressed" << std::endl;
				break;
			case a::EventCode::NUM_6:
				std::cout << "NUM_6 pressed" << std::endl;
				break;
			case a::EventCode::NUM_7:
				std::cout << "NUM_7 pressed" << std::endl;
				break;
			case a::EventCode::NUM_8:
				std::cout << "NUM_8 pressed" << std::endl;
				break;
			case a::EventCode::NUM_9:
				std::cout << "NUM_9 pressed" << std::endl;
				break;
			case a::EventCode::END:
				std::cout << "END pressed" << std::endl;
				break;

			case a::EventCode::ANY:
				std::cout << "Any: " << events[x].ch << std::endl;
				break;

			case a::EventCode::MOUSE_LEFT_CLICK:
				std::cout << "MOUSE_LEFT_CLICK: " << " X: " << events[x].x << " Y: " << events[x].y << std::endl;
				break;

			case a::EventCode::MOUSE_LEFT_RELEASE:
				std::cout << "MOUSE_LEFT_RELEASE: " << " X: " << events[x].x << " Y: " << events[x].y << std::endl;
				break;

			case a::EventCode::MOUSE_RIGHT_CLICK:
				std::cout << "MOUSE_RIGHT_CLICK: " << " X: " << events[x].x << " Y: " << events[x].y << std::endl;
				break;

			case a::EventCode::MOUSE_RIGHT_RELEASE:
				std::cout << "MOUSE_RIGHT_RELEASE: " << " X: " << events[x].x << " Y: " << events[x].y << std::endl;
				break;

			case a::EventCode::MOUSE_SCROLLWHEEL_PRESS:
				std::cout << "MOUSE_SCROLLWHEEL_PRESS: " << " X: " << events[x].x << " Y: " << events[x].y << std::endl;
				break;

			case a::EventCode::MOUSE_SCROLLWHEEL_RELEASE:
				std::cout << "MOUSE_SCROLLWHEEL_RELEASE: " << " X: " << events[x].x << " Y: " << events[x].y << std::endl;
				break;

			case a::EventCode::MOUSE_SCROLL_DOWN:
				std::cout << "MOUSE_SCROLL_DOWN: " << " X: " << events[x].x << " Y: " << events[x].y << std::endl;
				break;
			case a::EventCode::MOUSE_SCROLL_UP:
				std::cout << "MOUSE_SCROLL_UP: " << " X: " << events[x].x << " Y: " << events[x].y << std::endl;
				break;
			case a::EventCode::MOUSE_DOUBLE_CLICK:
				std::cout << "MOUSE_DOUBLE_CLICK: " << " X: " << events[x].x << " Y: " << events[x].y << std::endl;
				break;
			}
		}
	}
	
}

int main()
{
	//for a breakpoint marker;
	int x = 2 + 2;
	x += 2;
	
	std::cout << "Before initialization of console implementation." << std::endl;
	apoganatz::IConsoleAPI& console = apoganatz::getConsoleInstance();

	testInput();

	console.writeString(L"Hello World!", apoganatz::colors::BLUE_BACKGROUND, apoganatz::Coordinate(0,0));

	console.writeCharactors(apoganatz::CharInfo(L'A', apoganatz::colors::WHITE_BACKGROUND), 5, apoganatz::Coordinate(1, 1));

	apoganatz::Panel p;
	p.x = 2; p.y = 5; p.height = 3; p.width = 5; p.color = apoganatz::colors::RED_BACKGROUND;
	p.render();


	std::vector<apoganatz::CharInfo> info;
	info.resize(16, apoganatz::CharInfo(L'木', apoganatz::colors::GREEN_BACKGROUND));
	console.writeOutput(info, apoganatz::Rectangle(10, 10, 4, 4));


	//Don't exit
	std::vector<apoganatz::InputEvent> events;
	events.resize(128);
	while (events[0].eventCode != apoganatz::EventCode::ESC)
	{
		console.getInput(events);
	}
	console.setCTRLCHandler(cntrlIsHit);
	console.writeString(L"Press CNTRL-C to exit", a::colors::BLUE_TEXT, a::Coordinate(0, 12));
	while (cntrlHit == false);
	
	
	console.writeString(L"CNTRL-C hit", a::colors::BLUE_TEXT, a::Coordinate(0, 13));
	std::this_thread::sleep_for(std::chrono::seconds(2));
	return 0;
}