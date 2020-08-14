#pragma once

#include <string>
#include <optional>

class OpenFileDialog {
public:
	static std::optional<std::string> open();
private:
	static std::string getCurrentDirectory();
};