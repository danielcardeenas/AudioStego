#pragma once

class CustomHeader
{
public:
	CustomHeader(char* header);
	CustomHeader()	= default;

	int		Step();
	string	Extension();
	char	Type();

protected:
	int		step;
	string	extension;
	char	type;
};