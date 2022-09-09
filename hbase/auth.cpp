#include <iomanip>
#include <iostream>
#include"HTTPRequest.hpp"
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include"cryptopp/include/aes.h"
#include"cryptopp/include/modes.h"
#include"cryptopp/include/filters.h"
#include"cryptopp/include/sha.h"
#include"cryptopp/include/hex.h"
#include"cryptopp/include/base64.h"
#include"ini.h"
#pragma comment(lib,"ws2_32.lib")
#if _DEBUG
#pragma comment(lib,"cryptopp\\Debug\\cryptlib.lib")
#else
#pragma comment(lib,"cryptopp\\Release\\cryptlib.lib")

#endif // _DEBUG
std::string encrypt(const std::string& str_in, const std::string& key, const std::string& iv)
{
	std::string str_out;
	CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption encryption((byte*)key.c_str(), key.length(), (byte*)iv.c_str());

	CryptoPP::StringSource encryptor(str_in, true,
		new CryptoPP::StreamTransformationFilter(encryption,
			new CryptoPP::Base64Encoder(
				new CryptoPP::StringSink(str_out),
				false // do not append a newline
			)
		)
	);
	return str_out;
}

std::string decrypt(const std::string& str_in, const std::string& key, const std::string& iv)
{

	std::string str_out;
	CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption decryption((byte*)key.c_str(), key.length(), (byte*)iv.c_str());

	CryptoPP::StringSource decryptor(str_in, true,
		new CryptoPP::Base64Decoder(
			new CryptoPP::StreamTransformationFilter(decryption,
				new CryptoPP::StringSink(str_out)
			)
		)
	);
	return str_out;
}
std::string get_hwid() {
	char hwid[4096];
	// total physical memory
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	sprintf_s(hwid, "%I64i", statex.ullTotalPhys / 1024);

	// volume information
	TCHAR volumename[MAX_PATH + 1] = { 0 };
	TCHAR filesystemname[MAX_PATH + 1] = { 0 };
	DWORD serialnumber = 0, maxcomponentlen = 0, filesystemflags = 0;
	GetVolumeInformation(L"C:\\", volumename, ARRAYSIZE(volumename), &serialnumber, &maxcomponentlen, &filesystemflags, filesystemname, ARRAYSIZE(filesystemname));
	sprintf_s(hwid, "%s%li%ws%li", hwid, serialnumber, filesystemname, maxcomponentlen);

	// computer name
	TCHAR computerName[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD size = sizeof(computerName) / sizeof(computerName[0]);
	GetComputerName(computerName, &size);
	sprintf_s(hwid, "%s%ws", hwid, computerName);
	return hwid;
}
namespace auth
{
	bool login()
	{
		VMProtectBegin("1");
		try
		{
			int timei = time(0);
			http::Request request{ "http://auth.humanmenu.cn/php/api.php" };
			const std::string body = "username="+INI::GetString(MENU_PATH"\\auth.ini","user","username") + "&password=" + INI::GetString(MENU_PATH"\\auth.ini", "user", "password") + "&hwid=" + get_hwid() + "&time=" + std::to_string(timei);
			const auto response = request.send("POST", body, {
				{"Content-Type", "application/x-www-form-urlencoded"}
				});
			nlohmann::json json= nlohmann::json::parse(decrypt(std::string{ response.body.begin(), response.body.end() }, "huangmo666niubia", "huangmo666niubia"));
			if (json["time"].get<int>() != timei)
			{
				LOG(INFO) <<"数据包造假";
				return false;
			}
			LOG(INFO) << json["msg"].get<std::string>();
			if (json["code"].get<int>()==0)
			{
				if (json["hwid"].get<std::string>()== get_hwid())
				{
					return true;
				}
			}
			return false;
		}
		catch (const std::exception& e)
		{
			LOG(INFO) << "Request failed, error: " << e.what() << '\n';
			return false;
		}
		VMProtectEnd();
	}
}




