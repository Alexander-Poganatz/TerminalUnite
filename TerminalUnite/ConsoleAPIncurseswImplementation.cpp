#include <ConsoleAPI.hpp>
#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED
#endif
#include <ncursesw/ncurses.h>
#include <locale>
#include <memory>
#include <signal.h>
#include <bitset>
namespace ca_poganatz
{
	namespace colors
	{

		const int RED_TEXT = 0;
		const int GREEN_TEXT = 1;
		const int BLUE_TEXT = 2;
		const int WHITE_TEXT = 3;

		const int RED_BACKGROUND = 4;
		const int GREEN_BACKGROUND = 5;
		const int BLUE_BACKGROUND = 6;
		const int WHITE_BACKGROUND = 7;

		static void initConsoleColors()
		{
			if(has_colors() == false)
				return;

			start_color();
			init_pair(0, COLOR_RED, COLOR_BLACK);
			init_pair(1, COLOR_GREEN, COLOR_BLACK);
			init_pair(2, COLOR_BLUE, COLOR_BLACK);
			init_pair(3, COLOR_WHITE, COLOR_BLACK);
			init_pair(4, COLOR_BLACK, COLOR_RED);
			init_pair(5, COLOR_BLACK, COLOR_GREEN);
			init_pair(6, COLOR_BLACK, COLOR_BLUE);
			init_pair(7, COLOR_BLACK, COLOR_WHITE);
		}

		static void setAttrColor(int colorNum)
		{
			if(has_colors() == false)
				return;
			attrset(COLOR_PAIR(colorNum));
		}
	}
	static void(*f)(void) = nullptr;
	static void ctrlCHandler(int signum)
	{
		if(SIGINT == signum) // Ctrl-c
		{
			if(nullptr != f)
				f();
		}
	}

	class NCursesWConsole : public IConsoleAPI
	{
		private:
		const int CHAR_ARRAY_SIZE = 5;
		char charArray[5];
		std::bitset<sizeof(wchar_t)*8> bitField;
		struct sigaction sa;
		public:

		NCursesWConsole()
		{
			std::locale::global(std::locale(""));
			initscr();
			colors::initConsoleColors();
			clear();

			noecho();
			cbreak();
			keypad(stdscr, TRUE);
			mousemask(ALL_MOUSE_EVENTS, NULL);
			timeout(10);
			setCursorVisibility(false);			
		}

		~NCursesWConsole()
		{
			endwin();	
		}

		void printCharAsUTF8String(wchar_t ch)
		{
			
			// Values gotten from wikipedia
			if(ch < 0x0080)
			{
				addch(ch);
				return;
			}
			
			int numOfBytesNeeded = 2;
			// An invalid unicode value, requires more than 21 bits
			if(ch > 0x10FFFF)
			{
				addch('?');
				return;
			}
			else if(ch >= 0x10000)
				numOfBytesNeeded = 4;
			else if(ch >= 0x0800)
				numOfBytesNeeded = 3;

			for(int i = 0; i < CHAR_ARRAY_SIZE; ++i)
				charArray[i] = 0;
			bitField.reset();
			std::bitset<sizeof(wchar_t)*8> charBitField(ch);
			
			size_t conversionOffset = 0;
			for(int x = 0; x < numOfBytesNeeded*8; ++x)
			{
				for(int inner = 0; inner < 6; ++inner)
				{
					bitField[x] = charBitField[x-conversionOffset];
					++x;
				}
				bitField.reset(x);
				++x;
				bitField.set(x);
				conversionOffset += 2;
			}

			int headerIndex = (numOfBytesNeeded*8)-numOfBytesNeeded;
			--headerIndex;
			
			bitField.reset(headerIndex);
			for(int x = 1; x <= numOfBytesNeeded; ++x)
			{
				bitField.set(headerIndex+x);
			}
			unsigned long i = bitField.to_ulong();
			for(int x = numOfBytesNeeded-1; x >= 0; --x)
			{
				charArray[x] = i >> (8 * (numOfBytesNeeded-1-x));
			}
			printw(charArray);

		}

		void setCursorPosition(short x, short y) override
		{
			move(y, x);
		}

		Coordinate getCursorPosition() override
		{
			int x = 0;
			int y = 0;
			getyx(stdscr, y,x);
			return Coordinate(x,y);	
		}

		void setCursorVisibility(bool visibility) override
		{
			curs_set(visibility);
		}

		bool getCursorVisibility() override 
		{
			int visibility = curs_set(0);
			curs_set(visibility);
			return visibility == ERR ? 0 : visibility == 0 
		}

		void setSize(short width, short height)
		{
			resizeterm(height, width);
		}

		void writeCharInfo(CharInfo ch, int num, Coordinate pos)
		{
			auto cursPos = getCursorPosition();
			move(pos.y, pos.x);
			colors::setAttrColor(ch.color);
			for(int x = 0; x < num; ++x)
				printCharAsUTF8String(ch.ch);
			setCursorPosition(cursPos.x, cursPos.y);
		}

		void writeString(std::wstring const& str, int color, Coordinate pos)
		{
			auto cursPos = getCursorPosition();
			
			move(pos.y, pos.x);
			colors::setAttrColor(color);
			for(auto c : str)
				printCharAsUTF8String(c);
			setCursorPosition(cursPos.x, cursPos.y);
		}

		void writeCharInfo(std::vector<CharInfo> const& buffer, Rectangle area)
		{
			auto cursPos = getCursorPosition();
			int index = 0;
			for(short y = area.y; y < (area.y + area.height); ++y)
			{
				move(y, area.x + area.width);
				for(short x = area.x; x < (area.x + area.width); ++x)
				{
					colors::setAttrColor(buffer[index].color);
					printCharAsUTF8String(buffer[index].ch);
					++index;
				}
			}
			setCursorPosition(cursPos.x, cursPos.y);
		}

		size_t getInput(std::vector<InputEvent> & buffer)
		{
			int ch = getch();
			if(ch == ERR)
				return 0;
			if(buffer.size() < 2)
				buffer.resize(2);

			// Check if UTF-8 header.
			if(ch > 192 && ch < 248)
			{
				char inputData[CHAR_ARRAY_SIZE] = {0};
				int numRemaining = 1;
				int index = 0;
				inputData[index] = ch;
				// Get number of bytes and chop off header.
				if(ch >= 0xF0)
				{
					numRemaining = 3;
					inputData[index] &= 0x7;
				}
				else if (ch >= 0xE0)
				{
					numRemaining = 2;
					inputData[index] &= 0xF;
				}
				else
				{
					inputData[index] &= 0x1F;
				}
				int numOfBytes = numRemaining + 1;
				
				++index;
				for(; numRemaining > 0; --numRemaining)
				{
					do
					{
						ch = getch();
					}while(ch == ERR);
					//if(ch == ERR)
					{
						// put error handling here.
					}
							//chop off header bits
					inputData[index] = ch;
					inputData[index] &= 0x3F;
					++index;
				}
				//Now to put UTF-8 to UTF-32
				long long utf32Var = 0;
				for(int x = 0; x < numOfBytes; ++x)
				{
					
					utf32Var |= inputData[x];
					utf32Var = utf32Var << 6;
				}
				utf32Var = utf32Var >> 6;
				ch = utf32Var;
				
			}

			int numToReturn = 1;
			buffer[0].ch = ch;
			buffer[0].eventCode = EventCode::ANY;	
			
			switch(ch)
			{
				case 27: // Esc in both ASCII and Unicode
					buffer[0].eventCode = EventCode::ESC; // I hope it is escape
					break;
				case KEY_UP:
					buffer[0].eventCode = EventCode::UP;
					break;
				case KEY_DOWN:
					buffer[0].eventCode = EventCode::DOWN;
					break;
				case KEY_LEFT:
					buffer[0].eventCode = EventCode::LEFT;
					break;
				case KEY_RIGHT:
					buffer[0].eventCode = EventCode::RIGHT;
					break;
				case 10: // Enter
					buffer[0].eventCode = EventCode::ENTER;
					break;
				case KEY_BACKSPACE:
					buffer[0].eventCode = EventCode::BACKSPACE;
					break;
				case KEY_DC:
					buffer[0].eventCode = EventCode::DELETE;
					break;
				case KEY_IC:
					buffer[0].eventCode = EventCode::INSERT;
					break;
				case KEY_NPAGE:
					buffer[0].eventCode = EventCode::PAGE_DOWN;
					break;
				case KEY_PPAGE:
					buffer[0].eventCode = EventCode::PAGE_UP;
					break;
				case KEY_HOME:
					buffer[0].eventCode = EventCode::HOME;
					break;
				case KEY_END: // ???
					buffer[0].eventCode = EventCode::END;
					break;
				case KEY_MOUSE:
					MEVENT event;
					if(getmouse(&event) == OK)
					{
						buffer[0].x = event.x;
						buffer[0].y = event.y;

						// bstate holds lots of information.
						if(event.bstate&BUTTON1_PRESSED)
							buffer[0].eventCode = EventCode::MOUSE_LEFT_CLICK;
						else if(event.bstate&BUTTON1_RELEASED)
							buffer[0].eventCode = EventCode::MOUSE_LEFT_RELEASE;
						else if(event.bstate&BUTTON2_PRESSED)
							buffer[0].eventCode = EventCode::MOUSE_SCROLLWHEEL_PRESS;
						else if(event.bstate&BUTTON2_RELEASED)
							buffer[0].eventCode = EventCode::MOUSE_SCROLLWHEEL_RELEASE;
						else if(event.bstate&BUTTON3_PRESSED)
							buffer[0].eventCode = EventCode::MOUSE_RIGHT_CLICK;
						else if(event.bstate&BUTTON3_RELEASED)
							buffer[0].eventCode = EventCode::MOUSE_RIGHT_RELEASE;
						else if(event.bstate&BUTTON1_CLICKED)
						{
							buffer[0].eventCode = EventCode::MOUSE_LEFT_CLICK;
							buffer[1].eventCode = EventCode::MOUSE_LEFT_RELEASE;
							numToReturn = 2;
						}
						else if(event.bstate&BUTTON2_CLICKED)
						{
							buffer[0].eventCode = EventCode::MOUSE_SCROLLWHEEL_PRESS;
							buffer[1].eventCode = EventCode::MOUSE_SCROLLWHEEL_RELEASE;
							numToReturn = 2;
						}
						else if(event.bstate&BUTTON3_CLICKED)
						{
							buffer[0].eventCode = EventCode::MOUSE_RIGHT_CLICK;
							buffer[1].eventCode = EventCode::MOUSE_RIGHT_RELEASE;
							numToReturn = 2;
						}
						else if(event.bstate&BUTTON1_DOUBLE_CLICKED)
						{
							buffer[0].eventCode = EventCode::MOUSE_DOUBLE_CLICK;
						}
						else
						{
							return 0;
						}
					}
					else
					{
						return 0;
					}
					break;
			}
			if(ch == KEY_F(1))
				buffer[0].eventCode = EventCode::F1;
			else if (ch == KEY_F(2))
				buffer[0].eventCode = EventCode::F2;
			else if(ch == KEY_F(3))
				buffer[0].eventCode = EventCode::F3;
			else if(ch == KEY_F(4))
				buffer[0].eventCode = EventCode::F4; 	
			else if(ch == KEY_F(5))      
				buffer[0].eventCode = EventCode::F5;
       			else if(ch == KEY_F(6))                       		
       				buffer[0].eventCode = EventCode::F6;
			else if(ch == KEY_F(7))      
				buffer[0].eventCode = EventCode::F7;
			else if(ch == KEY_F(8))      
				buffer[0].eventCode = EventCode::F8;
			else if(ch == KEY_F(9))      
				buffer[0].eventCode = EventCode::F9;
			else if(ch == KEY_F(10))      
				buffer[0].eventCode = EventCode::F10;
			else if(ch == KEY_F(11))      
				buffer[0].eventCode = EventCode::F11;
			else if(ch == KEY_F(12))      
				buffer[0].eventCode = EventCode::F12;

			return numToReturn;
		}

		void setCTRLCHandler(void(*fn)(void))
		{
			if(f == nullptr)
			{
				sa.sa_handler = ctrlCHandler;
				sigaction(SIGINT, &sa, NULL);
			}			
			f = fn;
		}
		
		void refresh() {
			::refresh();
		}
	}; // end class	
			
	IConsoleAPI& getConsoleInstance()
	{
		static std::shared_ptr<NCursesWConsole> c = std::make_shared<NCursesWConsole>();
		return *c;
	}
	
}
