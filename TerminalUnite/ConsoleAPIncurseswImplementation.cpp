#include "../output/include/ConsoleAPI.hpp"

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
		}

		void writeString(std::wstring const& str, int color, Coordinate pos)
		{	
			std::string copy(str.begin(), str.end());
			move(pos.y, pos.x);
			colors::setAttrColor(color);
			addstr(copy.c_str());
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
		}

		size_t getInput(std::vector<InputEvent> & buffer)
		{
			
			return 1;
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
