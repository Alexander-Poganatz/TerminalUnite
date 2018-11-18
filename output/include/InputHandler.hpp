#ifndef PANEL_INPUT_HANDLER_HPP
#define PANEL_INPUT_HANDLER_HPP
#include <ConsoleAPI.hpp>
namespace ca_poganatz 
{
	struct InputHandlerData {
		bool stopBubbling = false;
		bool preventDefault = false;
	};

	struct InputHandler 
	{
		/**
			@fn handleMouseInput
			@brief Handles the mouse input
			@param InputEvent [in] The mouse event data to process
			@param eventOptions [mutable] The event flow options
		*/
		virtual void handleMouseInput(InputEvent const & input, InputHandlerData & eventOptions) { };

		/**
			@fn handleKeyboardInput
			@brief Handles the keyboard input
			@param InputEvent [in] The keyboard event data to process
			@param eventOptions [mutable] The event flow options
		*/
		virtual void handleKeyboardInput(InputEvent const & input, InputHandlerData & eventOptions) { };
	};

	extern InputHandler DefaultInputHandler;


}

#endif