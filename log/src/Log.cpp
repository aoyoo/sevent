#include "Log.h"
#include <string>
using namespace std;

bool init_logger(const string &path, const string &level)
{
	if (path.empty() || level.empty())
		return false;
	int i;
	for (i = 0; i< 7; i++)
		if (CLevel::Log_Level_Name[i] == level)
			break;
	if (i == 7)
		return false;

	SharedFilterPtr filter;
	SharedFormatterPtr formatter;
	SharedHandlerPtr handler;
	CLevelFilter *ptr = new CLevelFilter();
	ptr->SetLevel(CLevel((Log_Level_T)i));
	filter = ptr;
	formatter = new FileFormatter();
	handler = new FileHandler();
	handler->SetFilter(filter);
	handler->SetFormatter(formatter);
	static_cast<FileHandler *>(handler.Get())->SetLogPath(path);
	CLogManager::Instance()->GetLogger().AddHandler(handler);
	return true;
}
