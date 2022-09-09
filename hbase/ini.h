#pragma once
#include "gta/natives.h"

namespace INI {
	void WriteString(std::string file, std::string string, std::string app, std::string key);
	std::string GetString(std::string file, std::string app, std::string key);
	void WriteInt(std::string file, int value, std::string app, std::string key);
	int GetInt(std::string file, std::string app, std::string key);
	void WriteFloat(std::string file, float value, std::string app, std::string key);
	float GetFloat(std::string file, std::string app, std::string key);
	void WriteVector3(std::string file, Vector3 value, std::string app);
	Vector3 GetVector3(std::string file, std::string app);
	void WriteChar(std::string file, char* value, std::string app, std::string key);
	char* GetChar(std::string file, std::string app, std::string key);
	void WriteBool(std::string file, bool value, std::string app, std::string key);
	bool GetBool(std::string file, std::string app, std::string key);
}
