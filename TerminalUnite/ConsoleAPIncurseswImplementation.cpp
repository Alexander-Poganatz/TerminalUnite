#include <ConsoleAPI.hpp>

#include <ncursesw/ncurses.h>
#include <locale>
#include <memory>
#include <signal.h>
namespace apoganatz
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
		}

		~NCursesWConsole()
		{
			endwin();	
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

		void setSize(short width, short height)
		{
			resizeterm(height, width);
		}

		void writeCharactors(CharInfo ch, int num, Coordinate pos)
		{
			move(pos.y, pos.x);
			colors::setAttrColor(ch.color);
			for(int x = 0; x < num; ++x)
				addch(ch.ch);
			refresh();
		}

		void writeString(std::wstring const& str, int color, Coordinate pos)
		{	
			std::string copy(str.begin(), str.end());
			move(pos.y, pos.x);
			colors::setAttrColor(color);
			addstr(copy.c_str());
			refresh();
		}

		void writeOutput(std::vector<CharInfo> const& buffer, Rectangle area)
		{
			int index = 0;
			for(short y = area.y; y < (area.y + area.height); ++y)
			{
				for(short x = area.x; x < (area.x + area.width); ++x)
				{
					colors::setAttrColor(buffer[index].color);
					mvaddch(y, x, buffer[index].ch);
					++index;
				}
			}
			refresh();
		}

		size_t getInput(std::vector<InputEvent> & buffer)
		{
			int ch = getch();
			if(ch == ERR)
				return 0;
			if(buffer.size() < 2)
				buffer.resize(2);

			// todo check for utf-8 input

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
						if(event.bstate&BUTTON1_RELEASED)
							buffer[0].eventCode = EventCode::MOUSE_LEFT_RELEASE;
						if(event.bstate&BUTTON2_PRESSED)
							buffer[0].eventCode = EventCode::MOUSE_SCROLLWHEEL_PRESS;
						if(event.bstate&BUTTON2_RELEASED)
							buffer[0].eventCode = EventCode::MOUSE_SCROLLWHEEL_RELEASE;
						if(event.bstate&BUTTON3_PRESSED)
							buffer[0].eventCode = EventCode::MOUSE_RIGHT_CLICK;
						if(event.bstate&BUTTON3_RELEASED)
							buffer[0].eventCode = EventCode::MOUSE_RIGHT_RELEASE;
						if(event.bstate&BUTTON1_CLICKED)
						{
							buffer[0].eventCode = EventCode::MOUSE_LEFT_CLICK;
							buffer[1].eventCode = EventCode::MOUSE_LEFT_RELEASE;
							numToReturn = 2;
						}
						if(event.bstate&BUTTON2_CLICKED)
						{
							buffer[0].eventCode = EventCode::MOUSE_SCROLLWHEEL_PRESS;
							buffer[1].eventCode = EventCode::MOUSE_SCROLLWHEEL_RELEASE;
							numToReturn = 2;
						}
						if(event.bstate&BUTTON3_CLICKED)
						{
							buffer[0].eventCode = EventCode::MOUSE_RIGHT_CLICK;
							buffer[1].eventCode = EventCode::MOUSE_RIGHT_RELEASE;
							numToReturn = 2;
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
	}; // end class	
			
	IConsoleAPI& getConsoleInstance()
	{
		static std::shared_ptr<NCursesWConsole> c = std::make_shared<NCursesWConsole>();
		return *c;
	}
	
}
