/** @file
  * @brief Contains the OpenFileDialog class. That class is a wrapper for the nativefiledialog C library.
  */
#pragma once

#include <string>
#include <optional>

/**
 * @brief Helper class to open a dialog to choose a file to open.
*/
class OpenFileDialog {
public:
	/**
	 * @brief Opens the dialog.
	 * @return The filename of the selected file, if any. Otherwise an empty std::optional.
	*/
	static std::optional<std::string> open();

private:
	static std::string getCurrentDirectory();
};