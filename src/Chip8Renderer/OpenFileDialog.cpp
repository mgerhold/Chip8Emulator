#include "Chip8Renderer/OpenFileDialog.hpp"

#include <iostream>
#include <nfd.h>
#ifdef _MSC_VER
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

std::optional<std::string> OpenFileDialog::open() {
	nfdchar_t* outPath = nullptr;
#ifdef _MSC_VER
	const nfdresult_t result = NFD_OpenDialog(nullptr, getCurrentDirectory().c_str(), &outPath);
#else
	// on linux it doesn't seem to work to set a starting directory
	const nfdresult_t result = NFD_OpenDialog(nullptr, nullptr, &outPath);
#endif

	if (result == NFD_OKAY) {
		const std::string path = outPath;
#ifdef _MSC_VER
		// suppress warning because of free
#pragma warning( push , 0 )
#pragma warning( disable : 26408)
#endif
		free(outPath);
#ifdef _MSC_VER
#pragma warning( pop )
#endif
		return path;
	} else if (result == NFD_CANCEL) {
		return {};
	} else {
		std::cout << "Error while opening file dialog!\n";
		return {};
	}
}

std::string OpenFileDialog::getCurrentDirectory() {
	return fs::current_path().string();
}
