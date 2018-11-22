#ifndef TEXT_INPUT_HPP
#define TEXT_INPUT_HPP

/**
	@file TextBox.hpp
	@brief Panel for text input
	@author Alexander Poganatz
	@author a_poganatz@outlook.com
	@date 2018-11-19
*/

#include <Panel.hpp>
#include <ObserverPattern.hpp>

namespace ca_poganatz
{
	class TextBox : public Panel, public Observer
	{
	private:
		TSubject<std::wstring>* state;
		std::wstring privateContent; //! The purpose of the private content is to check against the updated content before refreshing.
		short editControlAperture = 0;
		short xOffset = 0;
		bool oldCurserState;
	public:

		virtual void update() override;

		/**
			@fn handleMouseInput
			@brief Handles the mouse input given
			@param InputEvent [in] The mouse data from the event.
			@param eventOptions [mutable] The event flow options
		*/
		virtual void handleMouseInput(InputEvent const& input, InputHandlerData eventOptions) override;

		/**
			@fn handleKeyboardInput
			@brief Handles the keyboard input given
			@param InputEvent [in] The mouse data from the event.
			@param eventOptions [mutable] The event flow options
		*/
		virtual void handleKeyboardInput(InputEvent const& input, InputHandlerData eventOptions) override;

		/**
			@fn handleFocusGain
			@brief Handles the focus gain event
		*/
		virtual void handleFocusGain() override;

		/**
			@fn handleFocusLost
			@brief Handles the focus lost event
		*/
		virtual void handleFocusLost() override;

		/**
			@fn writeToConsoleBuffer
			@brief writes the panel buffer to the console buffer
		*/
		virtual void writeToConsoleBuffer() override;
	};
}

#endif