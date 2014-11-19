#include "Config.h"

Config::Config() : reader("conf/frebons.conf") {
	// Construct
}

Config::~Config() {
	// Garbage
}

bool Config::load() {
	if (reader.ParseError() < 0)
		return false;
	
	return true;
}

::std::string Config::get(const char* section, const char* key) {
	return reader.Get(section, key, "UNKNOWN");
}