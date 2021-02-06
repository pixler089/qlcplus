#include "htmlstring.h"

HtmlString::HtmlString()
{
}

HtmlString::HtmlString(std::string string)
	: std::string(string)
{
}

HtmlString::~HtmlString()
{
}

size_t HtmlString::visibleCharCount()
{
	size_t charCount=0;
	bool isInTag=false;
	for (char c : *this)
	{
		switch (c)
		{
			case '<':
				isInTag=true;
				break;
			case '>':
				if (isInTag)
					isInTag=false;
				else
					++charCount;
				break;
			default:
				if (!isInTag)
					++charCount;
				break;
		}
	}
	return charCount;
}

