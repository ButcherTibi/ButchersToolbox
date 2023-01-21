#pragma once

// Standard
#include <cstdint>
#include <cstdio>
#include <string>


class Console {
public:
	struct Color {
		uint8_t red;
		uint8_t green;
		uint8_t blue;
	};

	struct Style {
		Color foreground_color = { 255, 255, 255 };
		Color background_color = { 0, 0, 0 };
		bool enable_underline = false;
	};

	inline static Style current_formating;

private:

	static void _setForegroundColor(uint8_t red, uint8_t green, uint8_t blue);
	static void _setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue);
	static void _setUnderline(bool enable_underline);
	
public:
	/**
	 * @brief Configures the terminal output to support UTF8 encoding.
	*/
	static void configureForUTF8();

	static void enableVirtualProcessing();

	/**
	 * @brief Set output text color.
	*/
	static void setForegroundColor(uint8_t red, uint8_t green, uint8_t blue);
	/**
	 * @brief Set output text background color.
	*/
	static void setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue);
	/**
	 * @brief Enable/ disable displaying text underline.
	*/
	static void setUnderline(bool enable_underline = true);
	/**
	 * @brief Reset formating to default for the terminal.
	*/
	static void resetFormatting();


	/**
	 * @brief Write to output, C printf style.
	 * @param msg = Message to write to output.
	 * @param formatting = Formating to apply to message.
	*/
	template<typename... Args>
	static void out(const char* msg, Style formatting, Args... args)
	{
		_setForegroundColor(
			formatting.foreground_color.red, 
			formatting.foreground_color.green, 
			formatting.foreground_color.blue
		);
		_setBackgroundColor(
			formatting.background_color.red,
			formatting.background_color.green,
			formatting.background_color.blue
		);
		_setUnderline(formatting.enable_underline);

		printf(msg, args...);
	}

	/**
	 * @brief Write line to output, C printf style.
	 * @param msg = Message to write to output.
	 * @param formatting = Formating to apply to message.
	*/
	template<typename... Args>
	static void log(const char* msg, Style formatting, Args... args)
	{
		Console::out(msg, formatting, args...);
		printf("\n");
	}

	/**
	 * @brief Get current folder from de console.
	*/
	static void getCurrentFolder(std::string& current_folder);
};
