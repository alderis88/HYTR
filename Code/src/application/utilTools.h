#pragma once

enum DebugType
{
	Message,
	Warning,
	Error,
};

void DebugLog(const std::string& message, DebugType type = DebugType::Message);
