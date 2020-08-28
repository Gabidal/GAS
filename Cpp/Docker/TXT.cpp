#include "../../H/Docker/TXT.h"

void TXT::TXT_Analyzer(vector<string>& Output)
{
	ifstream file(DOCKER::Working_Dir + DOCKER::FileName.back(), std::ios::binary);
	file.seekg(0, SEEK_END);
	long long size = file.tellg();
	char* Buffer = new char[size + 1];
	file.seekg(0, SEEK_SET);
	file.read(Buffer, size);
	Buffer[size] = '\0';
	file.close();
	Output.push_back(string(Buffer));
	return;
}