#include "Engine/MyClass.h"
#include "Engine/Utility/Log.h"

#include <iostream>

int main( int /*argc*/, char* /*argv*/[] )
{
	gp1::initLog();

	MyClass a( 50 );

	std::cout << "Hello, world!\n";
	std::cout << "A: " << a.Value() << "\n\n";

	gp1::log(gp1::Severity::Trace, "This is a trace message.");
	gp1::log(gp1::Severity::Debug, "Another string: '%s'", "Hello, World!");
	gp1::log(gp1::Severity::Warning, "Oh no!");
	gp1::log(gp1::Severity::Error, "Code: %d.", 1);


	gp1::log(gp1::LogGroup::TestGroup, gp1::Severity::Trace, "Trace message from test group");
	gp1::disableLogGroup(gp1::LogGroup::TestGroup, gp1::Severity::Warning);
	gp1::log(gp1::LogGroup::TestGroup, gp1::Severity::Warning, "This message should be ignored!");
	gp1::enableLogGroup(gp1::LogGroup::TestGroup, gp1::Severity::Warning);
	gp1::log(gp1::LogGroup::TestGroup, gp1::Severity::Warning, "Warning message from test group");

	return 0;
}

#if defined( _WIN32 )

#include <Windows.h>

int WINAPI WinMain( HINSTANCE /*instance*/, HINSTANCE /*prev_instance*/, LPSTR /*cmd_line*/, int /*cmd_show*/ )
{
	return main( __argc, __argv );
}

#endif // _WIN32
