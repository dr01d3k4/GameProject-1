#include <ctime>
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <filesystem>

#include "Engine/Utility/Log.h"

#define GREEN "\033[0;32m"
#define WHITE "\033[0;97m"
#define BLUE "\033[0;36m"
#define YELLOW "\033[0;93m"
#define RED "\033[0;91m"
#define DEFAULT_COLOUR "\033[0m"

namespace gp1
{
constexpr uint8_t allSeverities = uint8_t(Severity::Error) | uint8_t(Severity::Warning) | uint8_t(Severity::Debug) | uint8_t(Severity::Trace);

static_assert(int(LogGroup::Count) < 256, "Maximum of 254 log groups allowed");
uint8_t enabledSeverityByLogGroup[uint8_t(LogGroup::Count)]{ 0 };

void initLog()
{
	for (uint8_t i = 0; i < uint8_t(LogGroup::Count); ++i)
	{
		enabledSeverityByLogGroup[i] = allSeverities;
	}
}

inline bool canLog(LogGroup group, Severity severity)
{
	return enabledSeverityByLogGroup[uint8_t(group)] & uint8_t(severity);
}

void enableLogGroup(LogGroup group)
{
	enabledSeverityByLogGroup[uint8_t(group)] = allSeverities;
}

void enableLogGroup(LogGroup group, Severity severity)
{
	enabledSeverityByLogGroup[uint8_t(group)] |= uint8_t(severity);
}

void disableLogGroup(LogGroup group)
{
	enabledSeverityByLogGroup[uint8_t(group)] = 0;
}

void disableLogGroup(LogGroup group, Severity severity)
{
	enabledSeverityByLogGroup[uint8_t(group)] &= ~uint8_t(severity);
}

const char* logGroupToString(LogGroup group)
{
	switch (group)
	{
	case LogGroup::Default:
		return "";
	case LogGroup::TestGroup:
		return "TestGroup";
	}
	return "";
}

bool fileLoggingEnabled = true;
std::string logFilepath = "log.txt";

// TODO(fkp): This currently opens the file each time we log. We
// should instead probably save to a buffer first and flush it every
// so often.
void vlog(LogGroup group, Severity severity, const char* format, va_list args)
{
	if (!canLog(group, severity))
	{
		return;
	}

	FILE* file = nullptr;

	if (fileLoggingEnabled)
	{
		std::filesystem::path filepath { logFilepath };
		std::filesystem::create_directories(filepath.parent_path());

		file = fopen(logFilepath.c_str(), "a");

		if (file)
		{
			static bool hasDone = false;

			if (!hasDone)
			{
				hasDone = true;
				fprintf(file, "\n\n================================================================\n\n");
			}
		}
		else
		{
			fileLoggingEnabled = false;
			log(Severity::Warning, "Failed to open log file '%s' (error: %s).", logFilepath.c_str(), std::strerror(errno));
		}
	}
	
	constexpr unsigned int timeBufferSize = 16;
	std::time_t currentTime = std::time(nullptr);
	char timeBuffer[timeBufferSize];
	
	if (std::strftime(timeBuffer, timeBufferSize, "[%H:%M:%S]", std::localtime(&currentTime)))
	{
		printf("%s ", timeBuffer);

		if (file)
		{
			fprintf(file, "%s ", timeBuffer);
		}
	}

	if (group != LogGroup::Default)
	{
		const char* g = logGroupToString(group);
		printf(GREEN "%s ", g);
		if (file)
		{
			fprintf(file, "%s ", g);
		}
	}
	
	switch (severity)
	{
	case Severity::Trace:
		printf(WHITE "Trace" DEFAULT_COLOUR ": ");

		if (file)
		{
			fprintf(file, "Trace: ");
		}
		break;

	case Severity::Debug:
		printf(BLUE "Debug" DEFAULT_COLOUR ": ");
		if (file)
		{
			fprintf(file, "Debug: ");
		}
		break;

	case Severity::Warning:
		printf(YELLOW "Warning" DEFAULT_COLOUR ": ");
		if (file)
		{
			fprintf(file, "Warning: ");
		}
		break;

	case Severity::Error:
		printf(RED "Error" DEFAULT_COLOUR ": ");
		
		if (file)
		{
			fprintf(file, "Error: ");
		}
		break;
	}
	
	vprintf(format, args);
	printf("\n");

	if (file)
	{
		vfprintf(file, format, args);
		fprintf(file, "\n");

		fclose(file);
	}
}

void log(Severity severity, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	vlog(LogGroup::Default, severity, format, args);
	va_end(args);
}

void log(LogGroup group, Severity severity, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	vlog(group, severity, format, args);
	va_end(args);
}

}

#undef GREEN
#undef WHITE
#undef BLUE
#undef YELLOW
#undef RED
#undef DEFAULT_COLOUR
