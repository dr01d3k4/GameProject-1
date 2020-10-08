#pragma once

namespace gp1
{

enum class LogGroup
{
	Default,
	TestGroup,
	Count,
};

enum class Severity
{
	Trace = 0b0001,
	Debug = 0b0010,
	Warning = 0b0100,
	Error = 0b1000,
};

void initLog();

// Will log with the default group
void log(Severity severity, const char* message, ...);
void log(LogGroup group, Severity severity, const char* message, ...);

void enableLogGroup(LogGroup group);
void enableLogGroup(LogGroup group, Severity severity);
void disableLogGroup(LogGroup group);
void disableLogGroup(LogGroup group, Severity severity);

}
