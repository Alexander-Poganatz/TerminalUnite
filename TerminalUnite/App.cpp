
#include <App.hpp>
#include <iostream>
#include <algorithm>

namespace ca_poganatz
{
	App* App::appInstance = nullptr;

	int App::Execute() 
	{
		std::cout << "Hello! welcome to the console app. Please override the execute method to get started.";
		return EXIT_SUCCESS;
	}

	void SetDefaultRunFlagToFalse() {
		App::SetDefaultRunFlagToFalse();
	}

	void App::AddDefaultCtrlHandler() {
		getConsoleInstance().setCTRLCHandler(SetDefaultRunFlagToFalse);
	}

	void App::sortPanelsForPainting()
	{
		std::sort(panels.begin(), panels.end(), [](Panel* left, Panel* right) { return left->z_index < right->z_index; });
	}

	void App::sortPanelsForInput() 
	{
		std::sort(panels.begin(), panels.end(), [](Panel* left, Panel* right) { return left->z_index > right->z_index; });
	}

	static Panel defaultFocusedPanel;

	App::App() 
	{
		if (appInstance)
		{
			throw "Error! this application is already initialized.";
		}
		inputBuffer.resize(128);

		defaultRunFlag = true;
		focusedPanel = &defaultFocusedPanel;

		appInstance = this;
	}

	void App::PaintPanels() 
	{
		sortPanelsForPainting();
		for (auto panel : this->panels) 
			panel->writeToConsoleBuffer();
		
		getConsoleInstance().refresh();

	}

	static bool isMouseEvent(EventCode code) 
	{
		switch (code) 
		{
		case MOUSE_DOUBLE_CLICK:
		case MOUSE_LEFT_CLICK:
		case MOUSE_LEFT_RELEASE:
		case MOUSE_SCROLLWHEEL_PRESS:
		case MOUSE_SCROLLWHEEL_RELEASE:
		case MOUSE_SCROLL_DOWN:
		case MOUSE_SCROLL_UP:
		case MOUSE_RIGHT_CLICK:
		case MOUSE_RIGHT_RELEASE:
			return true;
		default:
			return false;
		}
	}

	void App::ReadAndHandleConsoleInput()
	{
		sortPanelsForInput();
		size_t numOfEvents = getConsoleInstance().getInput(this->inputBuffer);
		for (size_t index = 0; index < numOfEvents; ++index)
		{
			InputHandlerData data;
			bool focusedIsSet = false;
			if (isMouseEvent(inputBuffer[index].eventCode)) {
				for (auto panel : panels) {
					if (panel->doesIntersect(this->inputBuffer[index].x, this->inputBuffer[index].y)) {
						if (!focusedIsSet && this->focusedPanel != panel)
						{
							this->focusedPanel->handleFocusLost();
							this->focusedPanel = panel;
							panel->handleFocusGain();
						}
						focusedIsSet = true;
						panel->handleMouseInput(this->inputBuffer[index], data);
						if (data.stopBubbling)
							break; // break out of panel loop
					}
				}
			}
			else {
				for (auto panel : panels)
				{
					if (panel->doesIntersect(focusedPanel->x, focusedPanel->y))
					{
						panel->handleKeyboardInput(this->inputBuffer[index], data);
						if (data.stopBubbling)
							break;
					}
				}
			}
		}
	}

	int App::Main(int argc, char* argv[]) 
	{
		for (int x = 0; x < argc; ++x) 
		{
			appInstance->arguments.push_back(argv[x]);
		}
		return appInstance->Execute();
	}
}

int main(int argc, char* argv[]) try
{
	return ca_poganatz::App::appInstance->Main(argc, argv);
}
catch (...) {
	std::cout << "Error occured in program" << std::endl;
}