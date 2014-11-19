#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <iostream>
#include "INIReader.h"

class Config {
public:
	Config();
	~Config();
	bool load();

	::std::string get(const char* section, const char* key);

private:
	INIReader reader;
};