
#include <ConsoleAPI.hpp>
#include <iostream>
#include <Panel.hpp>
#include <thread>
#include <list>
#include <TextBlock.hpp>
#include <App.hpp>
#include <TextBox.hpp>
namespace a = ca_poganatz;

// Testing to see what happens when an instance is created before main() is even ran
struct GlobalInstanceTester
{
	ca_poganatz::IConsoleAPI& ref;
	GlobalInstanceTester() : ref(ca_poganatz::getConsoleInstance()) {};
};

GlobalInstanceTester instance;

int printTestInputX = 15;
std::list<std::wstring> list;
void printTestInput(std::wstring const& s)
{
	list.push_back(s);
	if (list.size() > 10)
		list.pop_front();
	auto iter = list.begin();
	for (size_t y = 0; y < list.size(); ++y)
	{
		instance.ref.writeCharInfo(ca_poganatz::CharInfo(L' ', ca_poganatz::colors::WHITE_BACKGROUND), 40, ca_poganatz::Coordinate(printTestInputX, (short)y));
		instance.ref.writeString(*iter, ca_poganatz::colors::WHITE_BACKGROUND, ca_poganatz::Coordinate(printTestInputX, (short)y));
		++iter;
	}
	instance.ref.refresh();
}

void testInput()
{
	std::vector<ca_poganatz::InputEvent> events(128);
	size_t numOfEvents;
	bool notQuiting = true;
	printTestInput(L"Beginning input test, press Esc to exit.");
	while (notQuiting)
	{
		numOfEvents = instance.ref.getInput(events);


		for (size_t x = 0; x < numOfEvents; ++x)
		{
			switch (events[x].eventCode)
			{
			case a::EventCode::ESC:
				printTestInput(L"ESC pressed");
				notQuiting = false;
				break;
			case a::EventCode::F1:
				printTestInput(L"F1 pressed");
				break;
			case a::EventCode::F2:
				printTestInput(L"F2 pressed");
				break;
			case a::EventCode::F3:
				printTestInput(L"F3 pressed");
				break;
			case a::EventCode::F4:
				printTestInput(L"F4 pressed");
				break;
			case a::EventCode::F5:
				printTestInput(L"F5 pressed");
				break;
			case a::EventCode::F6:
				printTestInput(L"F6 pressed");
				break;
			case a::EventCode::F7:
				printTestInput(L"F7 pressed");
				break;
			case a::EventCode::F8:
				printTestInput(L"F8 pressed");
				break;
			case a::EventCode::F9:
				printTestInput(L"F9 pressed");
				break;
			case a::EventCode::F10:
				printTestInput(L"F10 pressed");
				break;
			case a::EventCode::F11:
				printTestInput(L"F11 pressed");
				break;
			case a::EventCode::F12:
				printTestInput(L"F12 pressed");
				break;
			case a::EventCode::ENTER:
				printTestInput(L"ENTER pressed");
				break;
			case a::EventCode::UP:
				printTestInput(L"UP pressed");
				break;
			case a::EventCode::DOWN:
				printTestInput(L"DOWN pressed");
				break;
			case a::EventCode::LEFT:
				printTestInput(L"LEFT pressed");
				break;
			case a::EventCode::RIGHT:
				printTestInput(L"RIGHT pressed");
				break;
			case a::EventCode::BACKSPACE:
				printTestInput(L"Backspace pressed");
				break;
			case a::EventCode::DELETE:
				printTestInput(L"DELETE pressed");
				break;
			case a::EventCode::INSERT:
				printTestInput(L"INSERT pressed");
				break;
			case a::EventCode::PAGE_UP:
				printTestInput(L"PAGE_UP pressed");
				break;
			case a::EventCode::PAGE_DOWN:
				printTestInput(L"PAGE_DOWN pressed");
				break;
			case a::EventCode::HOME:
				printTestInput(L"HOME pressed");
				break;
			case a::EventCode::NUM_0:
				printTestInput(L"NUM_0 pressed");
				break;
			case a::EventCode::NUM_1:
				printTestInput(L"NUM_1 pressed");
				break;
			case a::EventCode::NUM_2:
				printTestInput(L"NUM_2 pressed");
				break;
			case a::EventCode::NUM_3:
				printTestInput(L"NUM_3 pressed");
				break;
			case a::EventCode::NUM_4:
				printTestInput(L"NUM_4 pressed");
				break;
			case a::EventCode::NUM_5:
				printTestInput(L"NUM_5 pressed");
				break;
			case a::EventCode::NUM_6:
				printTestInput(L"NUM_6 pressed");
				break;
			case a::EventCode::NUM_7:
				printTestInput(L"NUM_7 pressed");
				break;
			case a::EventCode::NUM_8:
				printTestInput(L"NUM_8 pressed");
				break;
			case a::EventCode::NUM_9:
				printTestInput(L"NUM_9 pressed");
				break;
			case a::EventCode::END:
				printTestInput(L"END pressed");
				break;

			case a::EventCode::ANY:
				printTestInput(std::wstring(L"Any: ").append(std::to_wstring( events[x].ch)));
				break;

			case a::EventCode::MOUSE_LEFT_CLICK:
				printTestInput(L"MOUSE_LEFT_CLICK: X: " + std::to_wstring(events[x].x) + L" Y: " + std::to_wstring(events[x].y));
				break;

			case a::EventCode::MOUSE_LEFT_RELEASE:
				printTestInput(L"MOUSE_LEFT_RELEASE: X: " + std::to_wstring(events[x].x) + L" Y: " + std::to_wstring(events[x].y));
				break;

			case a::EventCode::MOUSE_RIGHT_CLICK:
				printTestInput(L"MOUSE_RIGHT_CLICK: X: " + std::to_wstring(events[x].x) + L" Y: " + std::to_wstring(events[x].y));
				break;

			case a::EventCode::MOUSE_RIGHT_RELEASE:
				printTestInput(L"MOUSE_RIGHT_RELEASE: X: " + std::to_wstring(events[x].x) + L" Y: " + std::to_wstring(events[x].y));
				break;

			case a::EventCode::MOUSE_SCROLLWHEEL_PRESS:
				printTestInput(L"MOUSE_SCROLLWHEEL_PRESS: X: " + std::to_wstring(events[x].x) + L" Y: " + std::to_wstring(events[x].y));
				break;

			case a::EventCode::MOUSE_SCROLLWHEEL_RELEASE:
				printTestInput(L"MOUSE_SCROLLWHEEL_RELEASE: X: " + std::to_wstring(events[x].x) + L" Y: " + std::to_wstring(events[x].y));
				break;

			case a::EventCode::MOUSE_SCROLL_DOWN:
				printTestInput(L"MOUSE_SCROLL_DOWN: X: " + std::to_wstring(events[x].x) + L" Y: " + std::to_wstring(events[x].y));
				break;
			case a::EventCode::MOUSE_SCROLL_UP:
				printTestInput(L"MOUSE_SCROLL_UP: X: " + std::to_wstring(events[x].x) + L" Y: " + std::to_wstring(events[x].y));
				break;
			case a::EventCode::MOUSE_DOUBLE_CLICK:
				printTestInput(L"MOUSE_DOUBLE_CLICK: X: " + std::to_wstring(events[x].x) + L" Y: " + std::to_wstring(events[x].y));
				break;
			}
		}
	}
	
}

namespace ca_poganatz {
	struct ExitHandler : public InputHandler {
		void handleMouseInput(InputEvent const& input, InputHandlerData& eventOptions) override {
			a::App::SetDefaultRunFlagToFalse();
		}
	};

	class Test : public App {
	public:
		int Execute() override {
			ca_poganatz::IConsoleAPI& console = ca_poganatz::getConsoleInstance();

			testInput();

			console.writeString(L"Hello World!", ca_poganatz::colors::BLUE_BACKGROUND, ca_poganatz::Coordinate(0, 0));

			console.writeCharInfo(ca_poganatz::CharInfo(L'A', ca_poganatz::colors::WHITE_BACKGROUND), 5, ca_poganatz::Coordinate(1, 1));

			ca_poganatz::Panel p;
			p.x = 2; p.y = 5; p.height = 3; p.width = 5; p.color = ca_poganatz::colors::RED_BACKGROUND;
			p.writeToConsoleBuffer();


			std::vector<ca_poganatz::CharInfo> info;
			info.resize(16, ca_poganatz::CharInfo(L'木', ca_poganatz::colors::GREEN_BACKGROUND));
			console.writeCharInfo(info, ca_poganatz::Rectangle(10, 10, 4, 4));

			// I expect giberish on 16 bit wchar_t compilers like windows. Also a compile warning.
			info.assign(16, ca_poganatz::CharInfo(L'☠', ca_poganatz::colors::GREEN_BACKGROUND));
			console.writeCharInfo(info, ca_poganatz::Rectangle(15, 15, 4, 4));
			//Don't exit
			console.writeString(L"Press CNTRL-C to exit", a::colors::BLUE_TEXT, a::Coordinate(0, 12));
			console.writeCharInfo(CharInfo(0xd83c, colors::BLUE_BACKGROUND), 1, Coordinate(0, 13));
			console.writeCharInfo(CharInfo(0xdf55, colors::BLUE_BACKGROUND), 1, Coordinate(1, 13));
			a::TSubject<std::wstring> state(L"Exit");
			a::TextBlock textPanel(20, 15, 6, 3, a::colors::WHITE_BACKGROUND, 0, &state, a::H_ALIGN_CENTER, a::V_ALIGN_CENTER);
			textPanel.writeToConsoleBuffer();

			a::TextBox textBox(30, 20, 10, a::colors::WHITE_BACKGROUND, 10, &state);
			this->AddPanel(&textBox);
			
			this->AddPanel(&textPanel);
			ExitHandler exitHandler;
			textPanel.setInputHandler(&exitHandler);

			Panel defaultPanel(30, 10, 10, 10, colors::WHITE_BACKGROUND, -1);
			this->AddPanel(&defaultPanel);

			console.refresh();
			console.setCTRLCHandler(App::SetDefaultRunFlagToFalse);
			state.attach(&textBox);
			state.attach(&textPanel);
			while (this->GetDefaultRunFlag()) 
			{
				this->ReadAndHandleConsoleInput();
				this->PaintPanels();
			}

			console.writeString(L"CNTRL-C hit", a::colors::BLUE_TEXT, a::Coordinate(0, 13));
			std::this_thread::sleep_for(std::chrono::seconds(2));
			return 0;
		}
	} myAppInstance;
}
