#pragma once

#include <vector>

static std::vector<std::string> splitString(const std::string& str, const char delim)
{
	int size = str.size();

	std::vector<std::string> res;

	char currStr[100];
	int currSize = 0;
	for (int i = 0; i < size; ++i)
	{
		if (str[i] == delim)
		{
			currStr[currSize] = '\0';
			res.push_back(currStr);
			currSize = 0;
		}
		else if (i == size - 1)
		{
			currStr[currSize] = str[i];
			currStr[currSize + 1] = '\0';
			res.push_back(currStr);
		}
		else
		{
			currStr[currSize] = str[i];
			++currSize;
		}
	}

	return res;
}

static std::string getFileName(const std::string& path)
{
	int dotInd = 0;
	for (int i = path.size() - 1; i >= 0; --i)
	{
		if (path[i] == '.')
			dotInd = i;
		if (path[i] == '/' || path[i] == '\\')
		{
			if (dotInd > 0)
				return path.substr(i + 1, dotInd - i-1);
			else
				return path.substr(i + 1);
		}
	}

	if (dotInd > 0)
		return path.substr(0, dotInd);
	else
		return path;
}

static std::string removePathExt(const std::string& path)
{
	for (int i = path.size() - 1; i >= 0; --i)
	{
		if (path[i] == '.' || path[i] == '.')
		{
			return path.substr(0, i);
		}
	}

	return path;
}

static std::string getPathDir(const std::string& path)
{
	for (int i = path.size() - 1; i >= 0; --i)
	{
		if (path[i] == '/' || path[i] == '\\')
		{
			return path.substr(0, i+1);
		}
	}

	return path;
}