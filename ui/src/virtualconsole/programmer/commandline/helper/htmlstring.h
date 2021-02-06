#ifndef HTMLSTRING_H
#define HTMLSTRING_H

#include <string>

class HtmlString : public std::string
{
public:
	HtmlString();
	HtmlString(std::string string);
	virtual ~HtmlString();
	size_t visibleCharCount();
};

#endif
