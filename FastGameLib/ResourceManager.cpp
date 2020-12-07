#include "ResourceManager.h"

std::string GetMimeType(const std::string& szExtension)
{
	// return mime type for extension
	HKEY hKey = nullptr;
	std::string szResult = "application/unknown";

	// open registry key
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, szExtension.c_str(),
		0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		// define buffer
		char szBuffer[256] = { 0 };
		DWORD dwBuffSize = sizeof(szBuffer);

		// get content type
		if (RegQueryValueEx(hKey, "Content Type", nullptr, nullptr,
			(LPBYTE)szBuffer, &dwBuffSize) == ERROR_SUCCESS)
		{
			// success
			szResult = szBuffer;
		}

		// close key
		RegCloseKey(hKey);
	}

	// return result
	return szResult;
}

long long getFileSize(const std::string& filename)
{
	struct stat stat_buf;
	int rc = stat(filename.c_str(), &stat_buf);
	return rc == 0 ? stat_buf.st_size : -1;
}

bool ResourceManager::load(const std::string& key, const std::string& file)
{
	if (file.find(".") == std::string::npos) return false;
	std::string type = GetMimeType(file.substr(file.find_last_of('.'), file.size() - file.find_last_of('.')));

	if(type.substr(0, type.find("/")) == "image")
	{
		textureHolder.emplace(key, std::make_shared<sf::Texture>());
		textureHolder[key]->loadFromFile(file);
	}
	else if(type == "application/json")
	{
		std::ifstream jFile(file);
		if(jFile.is_open())
		{
			nlohmann::json j;
			jFile >> j;
			jsonHolder.emplace(key, std::make_shared<nlohmann::json>(j));
			return true;
		}
		else 
		{
			std::cout << "Failed to load json " << file << ". Reason: Unable to open file" << std::endl;
		}
	}
	else if(type.substr(0, type.find("/")) == "audio")
	{
		long long size = getFileSize(file);
		if(size < 0)
		{
			std::cout << "Failed to load audio " << file << ". Reason: Unable to open file" << std::endl;
		}
		else if(static_cast<unsigned long long>(size) <= audioLimit)
		{
			soundHolder.emplace(key, std::make_shared<sf::SoundBuffer>());
			soundHolder[key]->loadFromFile(file);
			return true;
		}
		else
		{
			musicHolder.emplace(key, std::make_shared<sf::Music>());
			musicHolder[key]->openFromFile(file);
			return true;
		}
	}
	else if(file.substr(file.find_last_of('.'), file.size() - file.find_last_of('.')) == ".ttf")
	{
		fontHolder.emplace(key, std::make_shared<sf::Font>());
		fontHolder[key]->loadFromFile(file);
		return true;
	}

	return false;
}

GVariant ResourceManager::get(const std::string& key)
{
	if(musicHolder.find(key) != musicHolder.end())
	{
		return musicHolder[key];
	}
	else if (textureHolder.find(key) != textureHolder.end())
	{
		return textureHolder[key];
	}
	else if (jsonHolder.find(key) != jsonHolder.end())
	{
		return jsonHolder[key];
	}
	else if (soundHolder.find(key) != soundHolder.end())
	{
		return soundHolder[key];
	}
	else if (fontHolder.find(key) != fontHolder.end())
	{
		return fontHolder[key];
	}
	return GVariant();
}

GVariant ResourceManager::operator[](const std::string& key)
{
	return get(key);
}

void ResourceManager::remove(const std::string& key)
{
	if (musicHolder.find(key) != musicHolder.end())
	{
		musicHolder.erase(key);
	}
	else if (textureHolder.find(key) != textureHolder.end())
	{
		textureHolder.erase(key);
	}
	else if (jsonHolder.find(key) != jsonHolder.end())
	{
		jsonHolder.erase(key);
	}
	else if (soundHolder.find(key) != soundHolder.end())
	{
		soundHolder.erase(key);
	}
	else if (fontHolder.find(key) != fontHolder.end())
	{
		fontHolder.erase(key);
	}
}

void ResourceManager::setAudioLimit(unsigned long long limit)
{
	audioLimit = limit;
}

unsigned long long ResourceManager::getAudioLimit() const
{
	return audioLimit;
}
