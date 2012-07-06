#include "Log.h"

int main(int argc,char *argv[])
{
	CLogManager* pManager=CLogManager::Instance();

	//filter
	SharedFilterPtr filter_ptr(new CLevelFilter());

	static_cast<CLevelFilter*>(filter_ptr.Get())->SetLevel(CLevel(TRACE_LOG_LEVEL));

	//console formatter
	SharedFormatterPtr console_formatter(new ConsoleFormatter());
	
	//console handler
	SharedHandlerPtr console_ptr(new ConsoleHandler());
	
	//console:set filter
	console_ptr->SetFilter(filter_ptr);
	
	//console:set formatter
	console_ptr->SetFormatter(console_formatter);
	
	//add console handler
	pManager->AddHandler(console_ptr);

	//file formatter
	SharedFormatterPtr file_formatter(new FileFormatter());
	//file handler
	SharedHandlerPtr file_ptr(new FileHandler());
	//file:set filter
	file_ptr->SetFilter(filter_ptr);
	//file:set formatter
	file_ptr->SetFormatter(file_formatter);
	pManager->AddHandler(file_ptr);
	

	CLogger logger=pManager->GetLogger();

	LOG_DEBUG(logger,"this is a deubg"<<endl);
	LOG_ERROR(logger,"this is a error"<<endl);
	LOG_FATAL(logger,"this is fatal"<<endl);
	LOG_INFO(logger,"this is info"<<endl);
	LOG_TRACE(logger,"this is trace"<<endl);
	LOG_WARN(logger,"this is warn"<<endl);
	LOG_INFO(logger,"this is a bool:"<<true<<endl);
	LOG_INFO(logger,"this is a char:"<<'x'<<endl);
	LOG_INFO(logger,"this is a string:"<<string("string")<<endl);
	LOG_INFO(logger,"this is a short:"<<(short)-100<<endl);
	LOG_INFO(logger,"this is a int:"<<(int)1000<<endl);
	LOG_INFO(logger,"this is a unsigned int:"<<(unsigned int)1000<<endl);
	LOG_INFO(logger,"this is a long(hex):0x"<<hex<<(long)1000000<<endl);
	LOG_INFO(logger,"this is a unsigned long:"<<(unsigned long)100000000<<endl);
	LOG_INFO(logger,"this is a float:"<<(float)1.2345<<endl);
	LOG_INFO(logger,"this is a double:"<<setprecision(15)<<(double)1.2345234234<<endl);
	LOG_INFO(logger,"this is a double:"<<setprecision(15)<<(long double)123452342342.342<<endl);

	LOG_FATAL_VA("hello, s,%s d,%d llu,%llu",  "taotaosou", 3101, 21444444444343);
	LOG_ERROR_VA("hello, s,%s d,%d llu,%llu",  "taotaosou", 3101, 21444444444343);
	LOG_WARN_VA("hello, s,%s d,%d llu,%llu",  "taotaosou", 3101, 21444444444343);
	LOG_INFO_VA("hello, s,%s d,%d llu,%llu",  "taotaosou", 3101, 21444444444343);
	LOG_DEBUG_VA("hello, s,%s d,%d llu,%llu",  "taotaosou", 3101, 21444444444343);
	LOG_TRACE_VA("hello, s,%s d,%d llu,%llu",  "taotaosou", 3101, 21444444444343);
	LOG_TRACE_VA("hello, s,");
	

	//CLogger l_logger=logger;
	//LOG_INFO(l_logger,"this is a double:"<<setprecision(15)<<(long double)123452342342.342<<endl);

	//CLogger l_logger1(logger);
	//LOG_INFO(l_logger1,"this is a double:"<<setprecision(15)<<(long double)123452342342.342<<endl);

	//CLogManager::DestroyInstance();
	pManager->DestroyInstance();
}

