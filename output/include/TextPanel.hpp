#ifndef TEXT_PANEL_HPP
#define TEXT_PANEL_HPP

/**
	@fn TextPanel.hpp
	@brief Implementation of a TextPanel object
	@author Alexander Poganatz
	@author a_poganatz@outlook.com
	@date 2018-11-18
*/

#include <Panel.hpp>
#include <ObserverPattern.hpp>

namespace ca_poganatz 
{
	class TextPanel : public Panel, public Observer
	{
	private:
		TSubject<std::wstring>* state;
		HORIZONTAL_ALIGNMENT hAlignment; //! The horizontal alignment of the text
		VERTICAL_ALIGNMENT vAlignment; //! The vertical alignment of the text
		std::vector<CharInfo> buffer;
	public:
		TextPanel() = delete;
		TextPanel(short x, short y, short width, short height, int color, short z_index, TSubject<std::wstring>* pSubject, 
			HORIZONTAL_ALIGNMENT hAlign = H_ALIGN_LEFT, VERTICAL_ALIGNMENT vAlign= V_ALIGN_TOP) : Panel(x, y, width, height, color, z_index) 
		{
			this->state = pSubject;
			this->hAlignment = hAlign;
			this->vAlignment = vAlign;
			this->updateOutputBuffer();
		}
	private:
		/**
			@fn updateOutputBuffer
			@brief Converts the string to a output buffer representation
		*/
		void updateOutputBuffer();

	public:
		/**
			@fn writeToConsoleBuffer
			@brief writes the panel buffer to the console buffer
		*/
		virtual void writeToConsoleBuffer() override
		{
			Rectangle rect(this->x, this->y, this->width, this->height);
			this->consoleRef.writeCharInfo(this->buffer, rect);
		}

		/**
			@fn update
			@brief Creates a buffer based on the state and writes it to the console buffer
		*/
		virtual void update() override 
		{
			this->updateOutputBuffer();
			this->writeToConsoleBuffer();
		}
	};
}


#endif