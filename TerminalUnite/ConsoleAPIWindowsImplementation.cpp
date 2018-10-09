#if defined(_WIN32)

/**
	@file ConsoleAPIWindowsImplementation
	@author Alexander Poganatz
	@author a_poganatz@outlook.com
	@author poganatz.ca
	@date 2018-04-28
	@brief The windows implementation of IConsoleAPI
*/

#include <ConsoleAPI.hpp>

#include <memory>
#include <Windows.h>
#include <algorithm>

namespace apoganatz
{
	void (*ctrlFuncToExecute)(void) = nullptr;
	BOOL SimpleCntlHandler(DWORD ctrlType)
	{
		switch (ctrlType)
		{
		case CTRL_C_EVENT:
		case CTRL_LOGOFF_EVENT:
		case CTRL_SHUTDOWN_EVENT:
		case CTRL_BREAK_EVENT:
		case CTRL_CLOSE_EVENT:
			if (*ctrlFuncToExecute != nullptr)
			{
				ctrlFuncToExecute();
				return TRUE;
			}
			
		}
		return FALSE;
	}

	namespace colors
	{
	    const int RED_TEXT =  FOREGROUND_RED | FOREGROUND_INTENSITY;
		const int GREEN_TEXT =  FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		const int BLUE_TEXT = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		const int WHITE_TEXT = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;

		const int RED_BACKGROUND = BACKGROUND_RED | BACKGROUND_INTENSITY;
		const int GREEN_BACKGROUND = BACKGROUND_GREEN | BACKGROUND_INTENSITY;
		const int BLUE_BACKGROUND = BACKGROUND_BLUE | BACKGROUND_INTENSITY;
		const int WHITE_BACKGROUND = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
	}

	class WindowsConsole : public IConsoleAPI
	{
		HANDLE consoleOutputHandle;
		HANDLE consoleInputHandle;

		// information to save the state of the console.
		std::vector<CHAR_INFO> consoleOutput;
		CONSOLE_SCREEN_BUFFER_INFO ConsoleScreenBufferInfo;
		COORD OriginalBufferCoordinates;
		CONSOLE_CURSOR_INFO CursorInformation;
		UINT codePage;
		DWORD ConsoleMode;

		// Creating a buffer so memory doesn't have to be allocated for each call to writeOutput
		std::vector<CHAR_INFO> outputBuffer;

		short consoleWidth;
		short consoleHeight;

		std::vector<INPUT_RECORD> inputBuffer;
		bool leftMousePressed = false;
		bool scrollWheelPressed = false;
		bool rightMousePressed = false;

	public:
		WindowsConsole()
		{
			consoleOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
			consoleInputHandle = GetStdHandle(STD_INPUT_HANDLE);

			// Save console information

			// Buffer information
			GetConsoleScreenBufferInfo(consoleOutputHandle, &ConsoleScreenBufferInfo);
			OriginalBufferCoordinates.X = 0;
			OriginalBufferCoordinates.Y = 0;

			// Contents of the buffer
			SMALL_RECT bufferRect{ 0 };
			bufferRect.Right = ConsoleScreenBufferInfo.dwSize.X - 1;
			bufferRect.Bottom = ConsoleScreenBufferInfo.dwSize.Y - 1;
			COORD startCoord{ 0 };
			consoleOutput.resize(ConsoleScreenBufferInfo.dwSize.X * ConsoleScreenBufferInfo.dwSize.Y);
			ReadConsoleOutputW(consoleOutputHandle, consoleOutput.data(), ConsoleScreenBufferInfo.dwSize, startCoord, &bufferRect);

			// Curser, codepage, and mode
			GetConsoleCursorInfo(consoleOutputHandle, &CursorInformation);
			codePage = GetConsoleOutputCP();
			::GetConsoleMode(consoleInputHandle, &ConsoleMode);

			// Change buffer size
			this->setSize(ConsoleScreenBufferInfo.srWindow.Right - ConsoleScreenBufferInfo.srWindow.Left, ConsoleScreenBufferInfo.srWindow.Bottom
				- ConsoleScreenBufferInfo.srWindow.Top);

			// Clear the Screen
			this->writeCharactors(CharInfo(' ', colors::WHITE_TEXT),
				consoleWidth * consoleHeight, Coordinate(0, 0));

			// Get mouse and keyboard input
			::SetConsoleMode(consoleInputHandle, ENABLE_WINDOW_INPUT | ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_PROCESSED_INPUT);

			this->setCursorVisibility(false);

			// Set the size of the input buffer to be big enough
			inputBuffer.resize(128);
		}

		// Restore console state
		~WindowsConsole()
		{
			// Set the screen size to small
			this->setSize(0, 0);
			SetConsoleScreenBufferSize(consoleOutputHandle, ConsoleScreenBufferInfo.dwSize);
			
			SetConsoleWindowInfo(consoleOutputHandle, TRUE, &ConsoleScreenBufferInfo.srWindow);

			// Write original output back
			SMALL_RECT bufferRect = { 0,0,0,0 };
			bufferRect.Right = ConsoleScreenBufferInfo.dwSize.X - 1;
			bufferRect.Bottom = ConsoleScreenBufferInfo.dwSize.Y - 1;
			
			WriteConsoleOutputW(consoleOutputHandle, consoleOutput.data(), ConsoleScreenBufferInfo.dwSize, OriginalBufferCoordinates, &bufferRect);
			
			// Restore cursor, codepage, and mode
			SetConsoleCursorInfo(consoleOutputHandle, &CursorInformation);

			::SetConsoleCursorPosition(consoleOutputHandle, ConsoleScreenBufferInfo.dwCursorPosition);
			::SetConsoleOutputCP(codePage);
			::SetConsoleMode(consoleInputHandle, ConsoleMode);
		}

		virtual void setCursorVisibility(bool isVisible) override
		{
			CONSOLE_CURSOR_INFO info;
			GetConsoleCursorInfo(consoleOutputHandle, &info);
			info.bVisible = isVisible ? TRUE : FALSE;
			SetConsoleCursorInfo(consoleOutputHandle, &info);
		};

		virtual void setCursorPosition(short x, short y) override
		{
			::SetConsoleCursorPosition(consoleOutputHandle, COORD{ x, y });
		};

		virtual Coordinate getCursorPosition() override
		{
			CONSOLE_SCREEN_BUFFER_INFO info;
			GetConsoleScreenBufferInfo(consoleOutputHandle, &info);
			return Coordinate(info.dwCursorPosition.X, info.dwCursorPosition.Y);
		}

		virtual void setSize(short width, short height) override
		{
			// make the window size as small as possible before changing the size of the buffer
			SMALL_RECT rect{ 0 };
			rect.Right = 0;
			rect.Bottom = 0;
			SetConsoleWindowInfo(consoleOutputHandle, TRUE, &rect);

			//adjust buffer size then the window
			SetConsoleScreenBufferSize(consoleOutputHandle, COORD{ width, height });

			CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
			GetConsoleScreenBufferInfo(consoleOutputHandle, &bufferInfo);
			consoleWidth = (std::min)(width, bufferInfo.dwMaximumWindowSize.X);
			consoleHeight = (std::min)(height, bufferInfo.dwMaximumWindowSize.Y);

			rect.Top = rect.Left = 0;
			rect.Right = consoleWidth - 1;
			rect.Bottom = consoleHeight - 1;
			SetConsoleWindowInfo(consoleOutputHandle, TRUE, &rect);

			outputBuffer.resize(consoleWidth * consoleHeight);
		}

		virtual void writeCharactors(CharInfo ch, int num, Coordinate pos) override
		{
			short writePosition = (pos.y * consoleWidth) + pos.x;
			for (int x = 0; x < num; ++x) 
			{
				outputBuffer[x + writePosition].Char.UnicodeChar = ch.ch;
				outputBuffer[x + writePosition].Attributes = ch.color;
			}
		};

		virtual void writeString(std::wstring const& str, int color, Coordinate pos) override
		{

			short writePosition = (pos.y * consoleWidth) + pos.x;
			for (size_t x = 0; x < str.size(); ++x)
			{
				outputBuffer[x + writePosition].Char.UnicodeChar = str[x];
			}
		};

		virtual void writeOutput(std::vector<CharInfo> const& buffer, Rectangle area) override
		{
			size_t bufferIndex = 0;
			for (short y = 0; y < area.height; ++y) 
			{
				for (short x = 0; x < area.width; ++x)
				{
					outputBuffer[((area.y + y) * consoleWidth) + x + area.x].Char.UnicodeChar = buffer[bufferIndex].ch;
					outputBuffer[((area.y + y) * consoleWidth) + x + area.x].Attributes = buffer[bufferIndex].color;
					++bufferIndex;
				}
			}
		};

		virtual size_t getInput(std::vector<InputEvent>& buffer) override
		{
			DWORD numOfEvents = 0;
			
			inputBuffer.resize(buffer.size());

			GetNumberOfConsoleInputEvents(consoleInputHandle, &numOfEvents);

			if (0 == numOfEvents)
				return numOfEvents;

			// do input processing
			::ReadConsoleInput(consoleInputHandle, inputBuffer.data(), (DWORD)inputBuffer.size(), &numOfEvents);

			// Do conversion.
			size_t numOfConvertedEvents = 0;
			InputEvent kEvent(0, EventCode::ANY, 0, 0);

			for (DWORD x = 0; x < numOfEvents; ++x)
			{
				switch (inputBuffer[x].EventType)
				{
				case KEY_EVENT:
					if (inputBuffer[x].Event.KeyEvent.bKeyDown != TRUE)
						break;

					kEvent.ch = inputBuffer[x].Event.KeyEvent.uChar.UnicodeChar;
					kEvent.eventCode = EventCode::ANY;

					switch (inputBuffer[x].Event.KeyEvent.wVirtualKeyCode)
					{
					case VK_BACK:
						kEvent.eventCode = EventCode::BACKSPACE;
						break;
					case VK_RETURN:
						kEvent.eventCode = EventCode::ENTER;
						break;
					case VK_ESCAPE:
						kEvent.eventCode = EventCode::ESC;
						break;
					case VK_PRIOR:
						kEvent.eventCode = EventCode::PAGE_UP;
						break;
					case VK_NEXT:
						kEvent.eventCode = EventCode::PAGE_DOWN;
						break;
					case VK_END:
						kEvent.eventCode = EventCode::END;
						break;
					case VK_HOME:
						kEvent.eventCode = EventCode::HOME;
						break;
					case VK_LEFT:
						kEvent.eventCode = EventCode::LEFT;
						break;
					case VK_UP:
						kEvent.eventCode = EventCode::UP;
						break;
					case VK_RIGHT:
						kEvent.eventCode = EventCode::RIGHT;
						break;
					case VK_DOWN:
						kEvent.eventCode = EventCode::DOWN;
						break;
					case VK_INSERT:
						kEvent.eventCode = EventCode::INSERT;
						break;
					case VK_DELETE:
#undef DELETE
						kEvent.eventCode = EventCode::DELETE;
						break;
					case VK_NUMPAD0:
						kEvent.eventCode = EventCode::NUM_0;
						break;
					case VK_NUMPAD1:
						kEvent.eventCode = EventCode::NUM_1;
						break;
					case VK_NUMPAD2:
						kEvent.eventCode = EventCode::NUM_2;
						break;
					case VK_NUMPAD3:
						kEvent.eventCode = EventCode::NUM_3;
						break;
					case VK_NUMPAD4:
						kEvent.eventCode = EventCode::NUM_4;
						break;
					case VK_NUMPAD5:
						kEvent.eventCode = EventCode::NUM_5;
						break;
					case VK_NUMPAD6:
						kEvent.eventCode = EventCode::NUM_6;
						break;
					case VK_NUMPAD7:
						kEvent.eventCode = EventCode::NUM_7;
						break;
					case VK_NUMPAD8:
						kEvent.eventCode = EventCode::NUM_8;
						break;
					case VK_NUMPAD9:
						kEvent.eventCode = EventCode::NUM_9;
						break;
					case VK_F1:
						kEvent.eventCode = EventCode::F1;
						break;
					case VK_F2:
						kEvent.eventCode = EventCode::F2;
						break;
					case VK_F3:
						kEvent.eventCode = EventCode::F3;
						break;
					case VK_F4:
						kEvent.eventCode = EventCode::F4;
						break;
					case VK_F5:
						kEvent.eventCode = EventCode::F5;
						break;
					case VK_F6:
						kEvent.eventCode = EventCode::F6;
						break;
					case VK_F7:
						kEvent.eventCode = EventCode::F7;
						break;
					case VK_F8:
						kEvent.eventCode = EventCode::F8;
						break;
					case VK_F9:
						kEvent.eventCode = EventCode::F9;
						break;
					case VK_F10:
						kEvent.eventCode = EventCode::F10;
						break;
					case VK_F11:
						kEvent.eventCode = EventCode::F11;
						break;
					case VK_F12:
						kEvent.eventCode = EventCode::F12;
						break;
					} // End specifiying key event type
					

					buffer[numOfConvertedEvents] = kEvent;
					++numOfConvertedEvents;
					
					break;

				case MOUSE_EVENT:
					if (inputBuffer[x].Event.MouseEvent.dwEventFlags == MOUSE_MOVED)
					{
						break;
					}

					InputEvent mEvent(L' ', EventCode::ANY, inputBuffer[x].Event.MouseEvent.dwMousePosition.X, inputBuffer[x].Event.MouseEvent.dwMousePosition.Y);

					switch (inputBuffer[x].Event.MouseEvent.dwEventFlags)
					{
					case DOUBLE_CLICK:
						mEvent.eventCode = EventCode::MOUSE_DOUBLE_CLICK;
						break;
					case MOUSE_WHEELED:
						if ( (long)inputBuffer[x].Event.MouseEvent.dwButtonState > 0)
							mEvent.eventCode = EventCode::MOUSE_SCROLL_UP;
						else
							mEvent.eventCode = EventCode::MOUSE_SCROLL_DOWN;
						break;
					case 0: // a button is pressed or released

						if (inputBuffer[x].Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED && !leftMousePressed)
						{
							mEvent.eventCode = EventCode::MOUSE_LEFT_CLICK;
							leftMousePressed = true;
						}
						else if (!(inputBuffer[x].Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) && leftMousePressed)
						{
							mEvent.eventCode = EventCode::MOUSE_LEFT_RELEASE;
							leftMousePressed = false;
						}
						if (inputBuffer[x].Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED && !rightMousePressed)
						{
							mEvent.eventCode = EventCode::MOUSE_RIGHT_CLICK;
							rightMousePressed = true;
						}
						else if (!(inputBuffer[x].Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED) && rightMousePressed)
						{
							mEvent.eventCode = EventCode::MOUSE_RIGHT_RELEASE;
							rightMousePressed = false;
						}
						if (inputBuffer[x].Event.MouseEvent.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED && !scrollWheelPressed)
						{
							mEvent.eventCode = EventCode::MOUSE_SCROLLWHEEL_PRESS;
							scrollWheelPressed = true;
						}
						else if (!(inputBuffer[x].Event.MouseEvent.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED) && scrollWheelPressed)
						{
							mEvent.eventCode = EventCode::MOUSE_SCROLLWHEEL_RELEASE;
							scrollWheelPressed = false;
						}

						break;

					}

					buffer[numOfConvertedEvents] = mEvent;
					++numOfConvertedEvents;
					break;
				}
			}

			return numOfConvertedEvents;
		}; 

		virtual void setCTRLCHandler(void(*fn)(void))
		{
			if (ctrlFuncToExecute == nullptr)
				::SetConsoleCtrlHandler((PHANDLER_ROUTINE)SimpleCntlHandler, TRUE);
			ctrlFuncToExecute = fn;
		}

		virtual void refresh() 
		{

			COORD bufferSize;
			bufferSize.X = consoleWidth;
			bufferSize.Y = consoleHeight;

			COORD buffStartPos;
			buffStartPos.X = 0;
			buffStartPos.Y = 0;

			SMALL_RECT rect;
			rect.Left = 0;
			rect.Right = consoleWidth;
			rect.Top = 0;
			rect.Bottom = consoleHeight;
			WriteConsoleOutputW(consoleOutputHandle, outputBuffer.data(), bufferSize, buffStartPos, &rect);
		}
	};


	IConsoleAPI& getConsoleInstance()
	{
		static std::shared_ptr<WindowsConsole> c = std::make_shared<WindowsConsole>();
		return *c;
	}
}



#endif
