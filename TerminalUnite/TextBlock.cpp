/**
	@file TextBox.cpp
	@brief Impelmentation of TextPanel
	@author Alexander Poganatz
	@author a_poganatz@outlook.com
	@date 2018-11-18
*/
#include <TextBlock.hpp>
#include <locale>
#include <Utility.hpp>

namespace ca_poganatz
{
	// Shoved this here for now
	InputHandler DefaultInputHandler;

	void TextBlock::updateOutputBuffer()
	{
		std::wstring textStateCopy(state->getState());
		CharInfo defaultCharInfo;
		defaultCharInfo.ch = ' ';
		defaultCharInfo.color = this->color;

		//Adding lines.
		std::vector<std::wstring> lines;
		std::wstring currentLine;
		std::locale local;
		size_t sinceLastWhiteSpace = 0;
		size_t lineCount = 0;
		for (size_t x = 0; x < textStateCopy.size(); ++x)
		{

			currentLine.push_back(textStateCopy[x]);

			if (std::isspace(textStateCopy[x], local))
			{
				sinceLastWhiteSpace = 0;
				if (L'\n' == textStateCopy[x])
				{
					currentLine.pop_back();
					while (currentLine.size() < (size_t)this->width)
						currentLine.push_back(L' ');
				}
			}
			else
				++sinceLastWhiteSpace;

			if (currentLine.size() >= (size_t)this->width || (textStateCopy.size() - 1) == x)
			{
				while (sinceLastWhiteSpace > 0 && sinceLastWhiteSpace != (size_t)this->width && (textStateCopy.size() - 1) != x)
				{
					--sinceLastWhiteSpace;
					--x;
					currentLine.pop_back();
				}
				lines.push_back(currentLine);
				currentLine.clear();
				++lineCount;
				sinceLastWhiteSpace = 0;
			}
		}

		//Finished parsing lines

	   //Fill the output buffer.
		if (lineCount < (size_t)this->height)
			buffer.assign(this->width * this->height, defaultCharInfo);
		else
			buffer.assign(this->width * lineCount, defaultCharInfo);

		size_t xOffset = 0;
		int yOffset = 0;
		if (VERTICAL_ALIGNMENT::V_ALIGN_CENTER == vAlignment && lines.size() < (size_t)this->height)
		{
			yOffset = (this->height - (short)lines.size()) / 2;
		}
		if (this->hAlignment == HORIZONTAL_ALIGNMENT::H_ALIGN_CENTER)
		{
			for (size_t lineIndex = 0; lineIndex < lines.size(); ++lineIndex)
			{
				utility::trimString(lines.at(lineIndex));
				xOffset = (this->width - lines.at(lineIndex).size()) / 2;
				for (size_t x = 0; x < lines.at(lineIndex).size(); ++x)
				{
					buffer[xOffset + ((lineIndex + yOffset) * this->width) + x].ch = lines.at(lineIndex).at(x);
				}
			}
		}//End center alignment.

		if (this->hAlignment == HORIZONTAL_ALIGNMENT::H_ALIGN_LEFT)
		{
			for (size_t lineIndex = 0; lineIndex < lines.size(); ++lineIndex)
			{
				for (size_t x = 0; x < lines.at(lineIndex).size(); ++x)
				{
					buffer[xOffset + ((lineIndex + yOffset) * this->width) + x].ch = lines.at(lineIndex).at(x);
				}
			}
		}
	}
}