#include "Interface/MUtilityFile.h"
#include "Interface/MUtilityLog.h"
#include "Interface/MUtilityPlatformDefinitions.h"
#include "Interface/MUtilityWindowsInclude.h"
#include <direct.h>
#include <fstream>

#define LOG_CATEGORY_FILE "MUtilityFile"

bool MUtility::CreateDir(const char* directoryPath)
{
	int result;
	
	#if PLATFORM == PLATFORM_WINDOWS
		result = _mkdir(directoryPath);
	#else
		result = mkdir(directoryPath.c_str(), 0777);
	#endif
	if (result != 0)
	{
		switch (errno)
		{
		case EEXIST:
			return true;

		case ENOENT:
			MLOG_WARNING("Unable to create diretory on path \"" << directoryPath << "\" since the path is invalid", LOG_CATEGORY_FILE);
			return false;

		default:
			MLOG_WARNING("Unable to create directory at path \"" << directoryPath << "\" - Unknown error", LOG_CATEGORY_FILE);
			return false;
			break;
		}
	}
		
	return result == 0;
}

bool MUtility::DirectoryExists(const char* folderPath)
{
	struct stat info;
	if (stat(folderPath, &info) != 0)
		return false;

	return ((info.st_mode & S_IFDIR) != 0);
}

bool MUtility::FileExists(const char* filePath)
{
	bool toReturn = false;
	std::ifstream file(filePath);
	if (file.good())
	{
		toReturn = true;
		file.close();
	}

	return toReturn;
}

std::string MUtility::GetDirectoryPathFromFilePath(const std::string& filePath)
{
	std::string toReturn = "";
	const size_t last_slash_idx = filePath.find_last_of("\\/");
	if (std::string::npos != last_slash_idx)
	{
		toReturn = filePath.substr(0, last_slash_idx);
	}
	return toReturn;
}

std::string	MUtility::GetFileContentAsString(const std::string& filePath)
{
	std::string toReturn = "";
	std::ifstream file = std::ifstream(filePath, std::ios_base::in | std::ios_base::binary);
	if (file.is_open())
	{
		uint64_t fileContentSize = GetFileContentSize(filePath);
		char* fileContent = static_cast<char*>(malloc(fileContentSize + 1)); // +1 For Null terminator
		fileContent[fileContentSize] = '\0';
		file.read(fileContent, fileContentSize);
		file.close();

		toReturn = std::string(fileContent);

		if (fileContent != nullptr)
		{
			toReturn = std::string(fileContent);
			free(fileContent);
		}
	}
	else
		MLOG_WARNING("Failed to find file; Path = " << filePath, LOG_CATEGORY_FILE);

	return toReturn;
}

uint64_t MUtility::GetFileContentSize(const std::string& filePath)
{
	uint64_t toReturn = 0;
	std::ifstream file = std::ifstream(filePath, std::ios_base::binary | std::ios_base::ate);
	if (file.is_open())
	{
		toReturn = static_cast<uint64_t>(file.tellg());
		file.close();
	}
	else
		MLOG_WARNING("Failed to find file; Path = " << filePath, LOG_CATEGORY_FILE);

	return toReturn;
}