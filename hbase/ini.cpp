#pragma once
#include "pch.h"
#include "ini.h"

using namespace std;
void INI::WriteString(std::string file, std::string string, std::string app, std::string key) {
	WritePrivateProfileStringA(app.c_str(), key.c_str(), string.c_str(), file.c_str());
}
std::string INI::GetString(std::string file, std::string app, std::string key) {
	char buf[100];
	GetPrivateProfileStringA(app.c_str(), key.c_str(), "NULL", buf, 100, file.c_str());
	return (std::string)buf;
}
void INI::WriteInt(std::string file, int value, std::string app, std::string key) {
	INI::WriteString(file, std::to_string(value), app, key);
}
int INI::GetInt(std::string file, std::string app, std::string key) {
	string buf = INI::GetString(file, app, key);
	if (buf == "NULL")
	{
		return 0;
	}
	return std::stoi(buf);
}
void INI::WriteFloat(std::string file, float value, std::string app, std::string key) {
	INI::WriteString(file, std::to_string(value), app, key);
}
float INI::GetFloat(std::string file, std::string app, std::string key) {
	string buf = INI::GetString(file, app, key);
	if (buf == "NULL")
	{
		return 0.f;
	}
	return std::stof(buf);
}
void INI::WriteVector3(std::string file, Vector3 value, std::string app) {
	INI::WriteString(file, std::to_string(value.x), app, "X");
	INI::WriteString(file, std::to_string(value.y), app, "Y");
	INI::WriteString(file, std::to_string(value.z), app, "Z");
}
Vector3 INI::GetVector3(std::string file, std::string app) {
	Vector3 out;
	out.x = stof(INI::GetString(file, app, "X"));
	out.y = stof(INI::GetString(file, app, "Y"));
	out.z = stof(INI::GetString(file, app, "Z"));
	return out;
}
void INI::WriteChar(std::string file, char* value, std::string app, std::string key) {
	std::string Value0(value);
	INI::WriteString(file, Value0, app, key);
}
char* INI::GetChar(std::string file, std::string app, std::string key) {
	std::string Inistring = INI::GetString(file, app, key);
	char* strLeng = new char[Inistring.length() + 1];
	return strcpy(strLeng, Inistring.c_str());
}
void INI::WriteBool(std::string file, bool value, std::string app, std::string key) {
	value ? INI::WriteString(file, "true", app, key) : INI::WriteString(file, "false", app, key);
}
bool INI::GetBool(std::string file, std::string app, std::string key) {
	if (INI::GetString(file, app, key) == "true") {
		return true;
	}
	else {
		return false;
	}
}