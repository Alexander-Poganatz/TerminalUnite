#ifndef PANEL_HPP
#define PANEL_HPP

/**
	@file Panel.hpp
	@author Alexander Poganatz
	@author a_poganatz@outlook.com
	@date 2018-04-27
	@brief Generic panel to be drawn to the console
*/

#include <ConsoleAPI.hpp>

namespace apoganatz
{
	class Panel
	{
	protected:
		IConsoleAPI& consoleRef;

	public:
		short x;
		short y;
		short height;
		short width;
		int color;
		short priority;

		Panel() : x(0), y(0), height(1), width(1), priority(0), color(colors::WHITE_TEXT),  consoleRef(getConsoleInstance()) {}
		virtual ~Panel() {}
		
		/**
			@fn doesIntersect
			@brief Checks if the x,y coordinate intersects with the panel.
			@param sx [in] the x coordinate on the console
			@param sy [in] the y coordinate on the console
			@return true if the x,y values intersect. false otherwise
		*/
		inline bool doesIntersect(short sx, short sy) { return sx >= x && sx <= x + width && sy >= y && sy <= y + height; }

		/**
			@fn handleInput
			@brief Handles the input given, used for subclasses to override.
			@param InputEvent [in] The mouse/keyboard data from the event.
		*/
		virtual void handleInput(InputEvent input) {}

		/**
			@fn render
			@brief Displays a blank panel with the given color to the console.
		*/
		virtual void render() 
		{
			for (short h = 0; h < height; ++h)
			{
				consoleRef.writeCharactors(CharInfo(L' ', color), width, Coordinate(x, y + h));
			}
		}
	};
}



#endif