#ifndef CONSOLEAPI_HPP
#define CONSOLEAPI_HPP

/**
	@fn ConsoleAPI
	@author Alexander Poganatz
	@author a_poganatz@outlook.com
	@author poganatz.ca
	@date 2018-04-26
	@brief Generic stuff for a console api that can be used multiplatform.
*/

// For linux wncurses compilation for wide char

#include <string>
#include <vector>

namespace ca_poganatz
{

	/**
		Data to be written to the console.
	*/ 
	struct CharInfo
	{
		wchar_t ch; // The char to write to the console.
		int color; // The color from the color namespace.

		CharInfo(wchar_t c = L' ', int co = 0) : ch(c), color(co) {}
	};

	// Specifies the x,y coordinate on the screen to start writing info.
	struct Coordinate
	{
		short x;
		short y;
		Coordinate(short xCoord, short yCoord) : x(xCoord), y(yCoord) {}
	};

	// An area on the screen to draw to.
	struct Rectangle
	{
		short x;
		short y;
		short width;
		short height;
		Rectangle(short xCoord, short yCoord, short w, short h) : x(xCoord), y(yCoord), width(w), height(h) {}
	};

	// A list of colors to be used on multiple patforms.
	namespace colors
	{
		extern const int RED_TEXT;
		extern const int GREEN_TEXT;
		extern const int BLUE_TEXT;
		extern const int WHITE_TEXT;
		
		extern const int RED_BACKGROUND;
		extern const int GREEN_BACKGROUND;
		extern const int BLUE_BACKGROUND;
		extern const int WHITE_BACKGROUND;
	}

	// The event that occured. ANY means that the event is not handled or it is a charactor input.
	enum EventCode {
		ANY,
		ESC,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		ENTER,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		BACKSPACE,
		DELETE,
		INSERT,
		PAGE_UP,
		PAGE_DOWN,
		HOME,
		END,
		NUM_0,
		NUM_1,
		NUM_2,
		NUM_3,
		NUM_4,
		NUM_5,
		NUM_6,
		NUM_7,
		NUM_8,
		NUM_9,
		MOUSE_LEFT_CLICK,
		MOUSE_LEFT_RELEASE,
		MOUSE_RIGHT_CLICK,
		MOUSE_RIGHT_RELEASE,
		MOUSE_SCROLLWHEEL_PRESS,
		MOUSE_SCROLLWHEEL_RELEASE,
		MOUSE_SCROLL_UP,
		MOUSE_SCROLL_DOWN,
		MOUSE_DOUBLE_CLICK
	};

	// Data to hold input event data.
	struct InputEvent
	{
		wchar_t ch;
		EventCode eventCode;
		short x;
		short y;
		InputEvent(wchar_t c, EventCode ec, short xC, short yC) : ch(c), eventCode(ec), x(xC), y(yC) {}
		InputEvent() : ch(0), eventCode(EventCode::ANY), x(0), y(0) {}
	};

	struct IConsoleAPI
	{
		/**
		Constructors Should save the console, clear the screen, and set things up to recieve user input and handle unicode.
		*/

		/**
			Restore the console state when done.
		*/
		virtual ~IConsoleAPI() {};

		/**
			@fn getCursorVisibility
			@brief gets a bool to determine if the console cursor is blinking
			@return true if visible, false otherwise
		*/
		virtual bool getCursorVisibility() = 0;

		/**
			@fn setCurserVisibility
			@brief sets the visiblity on the console to either visible or invisible
			@param visibility [in] true is visible, false is to hide it
		*/
		virtual void setCursorVisibility(bool visiblity) = 0;
		/**
			@fn setCursorPosition
			@brief Sets the position of the cursor
			@param x [in] the x positon to set
			@param y [in] the y position to set
		*/
		virtual void setCursorPosition(short x, short y) = 0;

		/**
			@fn getCursorPosition
			@brief gets the position of the cursor on the console
			@return the x,y coordinate of the cursor
		*/
		virtual Coordinate getCursorPosition() = 0;

		/**
			@fn setSize
			@brief Sets the size of the console screen and buffers
			@param width [in] the width to set
			@param height [in] the height to set
		*/
		virtual void setSize(short width, short height) = 0;

		/**
			@fn writeCharactors
			@brief writes a number of CharInfo to the buffer
			@param ch [in] the char and color to write
			@param num [in] the numer of ch to write
			@param pos [in] the position to start writing
		*/
		virtual void writeCharInfo(CharInfo ch, int num, Coordinate pos) = 0;
		/**
			@fn writeString
			@brief writes a string to the buffer
			@param str [in] the string to write
			@param color [in] the color to write
			@param pos [in] the position to start writing
		*/
		virtual void writeString(std::wstring const& str, int color, Coordinate pos) = 0;
		/**
			@fn writeOutput
			@brief writes a vector of CharInfo to the buffer
			@param buffer [in] the values to write
			@param pos [in] the area to write to in the console. The width * height of the area should equal the buffer size.
		*/
		virtual void writeCharInfo(std::vector<CharInfo> const& buffer, Rectangle area) = 0;

		/**
			@fn getInput
			@brief Gets input from the console.
			@param buffer [out] the buffer to fill with console events. It should be resized to be big enough.
			@return The number of events gotten.
		*/
		virtual size_t getInput(std::vector<InputEvent>& buffer) = 0;
		/**
			@fn setCTRLCHandler
			@brief Sets the function to handle when ctrl-c is pressed
			@param fn [in] the function to execute
		*/
		virtual void setCTRLCHandler(void(*fn)(void)) = 0;

		/**
			Prints the buffer to the console.
		*/
		virtual void refresh() = 0;
	};

	// To be implemented in the platform specific cpp
	IConsoleAPI& getConsoleInstance();
}


#endif
