#ifndef PANEL_HPP
#define PANEL_HPP

/**
	@file Panel.hpp
	@author Alexander Poganatz
	@author a_poganatz@outlook.com
	@date 2018-04-27
	@brief Generic panel to be drawn to the console
*/
#include <InputHandler.hpp>

namespace ca_poganatz
{
	/*
		Text alignment enums.
	*/
	enum HORIZONTAL_ALIGNMENT { H_ALIGN_LEFT, H_ALIGN_CENTER };
	enum VERTICAL_ALIGNMENT { V_ALIGN_TOP, V_ALIGN_CENTER };

	class Panel
	{
	private: 
		InputHandler& inputHandler;
	protected:
		IConsoleAPI& consoleRef;
	public:
		short x;
		short y;
		short height;
		short width;
		int color;
		short z_index;

		Panel() : x(0), y(0), height(1), width(1), z_index(0), color(colors::WHITE_TEXT), consoleRef(getConsoleInstance()), inputHandler(DefaultInputHandler){}
		Panel(short x, short y, short width, short height, int color, short z_index = 0) : Panel()
		{
			this->x = x; this->y = y; this->width = width; this->height = height; this->color = color; this->z_index = z_index;
		}
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
			@fn handleMouseInput
			@brief Handles the mouse input given
			@param InputEvent [in] The mouse data from the event.
			@param eventOptions [mutable] The event flow options
		*/
		virtual void handleMouseInput(InputEvent const& input, InputHandlerData eventOptions) {
			this->inputHandler.handleMouseInput(input, eventOptions);
		}

		/**
			@fn handleKeyboardInput
			@brief Handles the keyboard input given
			@param InputEvent [in] The mouse data from the event.
			@param eventOptions [mutable] The event flow options
		*/
		virtual void handleKeyboardInput(InputEvent const& input, InputHandlerData eventOptions) {
			this->inputHandler.handleMouseInput(input, eventOptions);
		}

		/**
			@fn setInputHandler
			@brief sets the input handler to be processed
			@param InputHandler [in] The handler to set
		*/
		inline void setInputHandler(InputHandler& handler) {
			this->inputHandler = handler;
		}

		/**
			@fn render
			@brief Displays a blank panel with the given color to the console.
		*/
		virtual void writeToConsoleBuffer() 
		{
			for (short h = 0; h < height; ++h)
			{
				consoleRef.writeCharInfo(CharInfo(L' ', color), width, Coordinate(x, y + h));
			}
		}
	};
}



#endif