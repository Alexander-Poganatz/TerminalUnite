/**
	@file TextBox.cpp
	@brief Implementation of TextInput
	@author Alexander Poganatz
	@author a_poganatz@outlook.com
	@date 2018-11-19
*/

#include <TextBox.hpp>
#include <algorithm>
namespace ca_poganatz 
{
	void TextBox::handleMouseInput(InputEvent const& input, InputHandlerData eventOptions)
	{
		this->inputHandler.handleMouseInput(input, eventOptions);
		if (!eventOptions.preventDefault) {
			if (input.eventCode == EventCode::MOUSE_LEFT_CLICK)
			{
				Coordinate mousePosition{ input.x, input.y };

				xOffset = std::min((short)(mousePosition.x - this->x + editControlAperture), (short)privateContent.size());

				if (mousePosition.x > this->x + (short)privateContent.size())
					mousePosition.x = this->x + (short)privateContent.size();

				consoleRef.setCursorPosition(mousePosition.x, mousePosition.y);
			}
		}
	}

	void TextBox::handleKeyboardInput(InputEvent const& input, InputHandlerData eventOptions)
	{
		this->inputHandler.handleMouseInput(input, eventOptions);
		if (!eventOptions.preventDefault) {
			if (this->state != nullptr)
			{
				switch (input.eventCode)
				{
				case EventCode::BACKSPACE:
					if (0 < xOffset && xOffset <= (short)privateContent.size())
					{
						--xOffset;
						privateContent.erase(xOffset, 1);
					}
					break;
				case EventCode::DELETE:
					if (0 <= xOffset && xOffset < (short)privateContent.size())
					{
						privateContent.erase(xOffset, 1);
					}
					break;
				case EventCode::LEFT:
					if (xOffset > 0)
					{
						--xOffset;
					}
					break;
				case EventCode::RIGHT:
					if (xOffset < (short)privateContent.size())
					{
						++xOffset;
					}
					break;
					//Todo: VK_END, HOME, and RETURN
				default:
					if (iswprint(input.ch))
					{
						privateContent.insert(xOffset + privateContent.begin(), input.ch);
						++xOffset;
					}
					break;
				}//End switch

				short practicalSize = (short)privateContent.size() + 1;

				//I just copied you Garth, I have no idea how it works but it works.
				while (xOffset < editControlAperture)
					--editControlAperture;
				while (xOffset - editControlAperture >= this->width)
					++editControlAperture;

				while (practicalSize - editControlAperture<this->width && practicalSize > this->width)
					--editControlAperture;

				auto s = privateContent.substr(editControlAperture, this->width);
				s += std::wstring(this->width - s.size(), L' ');

				//OutputString(EDIT_CONTROL_START.X, EDIT_CONTROL_START.Y, s, EDIT_CONTROL_ATTR);
				Coordinate outputPosition{ this->x, this->y };
				consoleRef.writeString(s, this->color, outputPosition);
				// place cursor in the control

				Coordinate currentMousePosition{ this->x + xOffset - editControlAperture, this->y };
				consoleRef.setCursorPosition(currentMousePosition.x, currentMousePosition.y);

			}//End checking  if we have a state.
		}
	}

	void TextBox::handleFocusGain()
	{
		consoleRef.setCursorVisibility(true);
		this->inputHandler.gotFocus();
	}

	void TextBox::handleFocusLost()
	{
		consoleRef.setCursorVisibility(false);
		this->update();
		this->inputHandler.lostFocus();
	}

	void TextBox::update() 
	{
		auto newState = state->getState();
		if (privateContent != newState)
		{
			privateContent = newState;
			this->editControlAperture = (short)privateContent.size() - 1;
		}
		this->writeToConsoleBuffer();
	}

	void TextBox::writeToConsoleBuffer() 
	{
		privateContent = this->state->getState();
		std::wstring outputString;
		if ((short)privateContent.size() >= this->width - 1)
			outputString.append(privateContent.substr(privateContent.size() - this->width + 1));
		else
			outputString.append(privateContent);
		outputString.push_back(' ');

		consoleRef.writeString(outputString, this->color, Coordinate{ this->x, this->y });
	}
}