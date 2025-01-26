#pragma once

#include "stdafx.h"
#include "api/utils/InPlaceOptional.h"
#include "api/files/yaml/YAMLBlock.h"

namespace Chimp {
	class YAMLBlockParser {
	private:
		YAMLBlockParser(const std::vector<std::string>& lines, std::vector<std::string>::const_iterator begin, int indentsThisBlock = 0);

	public:
		// Parses a YAML block from a vector of strings
		static [[nodiscard]] YAMLBlockParser Parse(const std::vector<std::string>& lines);

		// Parses a YAML block from a file
		static [[nodiscard]] YAMLBlockParser Parse(const std::filesystem::path& path);

		~YAMLBlockParser() = default;

		[[nodiscard]] bool IsValid() const;

	private:
		// Parses a list, key should be list key, iter should be pointing to the first element of the list, end should be pointing to (1 after) the end of the lines vector
		// List (vector) will be put into the correct map
		// Returns iterator pointing to the next element after the list, this may be equal to end
		[[nodiscard]] std::vector<std::string>::const_iterator ParseList(std::string_view key, std::vector<std::string>::const_iterator begin, std::vector<std::string>::const_iterator end);

		// Parses a key value pair, key should be the key, value should be the value
		// inserts into correct map
		[[nodiscard]] void ParseValue(std::string_view key, std::string_view value);

		// Case insensitive
		[[nodiscard]] InPlaceOptional<bool> ParseBool(std::string_view value) const;
		// Fails to parse floats
		[[nodiscard]] InPlaceOptional<int> ParseInt(std::string_view value) const;
		// Fails to parse ints
		[[nodiscard]] InPlaceOptional<float> ParseFloat(std::string_view value) const;
		// Always returns
		[[nodiscard]] InPlaceOptional<std::string> ParseString(std::string_view value) const;

		// Returns number of indent characters at the start of the string
		[[nodiscard]] int GetIndentCount(std::string_view line) const;

		// Returns key, that is from the first non-indent character until (excluding) the first colon (or until the end of the string if no colon)
		// Undefined behaviour if m_IndentsThisBlock is invalid
		[[nodiscard]] std::string_view GetKey(std::string_view line) const;

		// Returns value, that is everything after the first delim character combination (": " or " - ") until the end of the string, returns empty string if no value found
		// Undefined behaviour if m_IndentsThisBlock is invalid
		[[nodiscard]] std::string_view GetValue(std::string_view line) const;

		// Returns true if line starts with "INDENTATIONS - "
		[[nodiscard]] bool IsListElement(std::string_view line) const;
	public:
		static constexpr char INDENT_CHAR = '\t';

		YAMLBlock Data;

	private:
		std::vector<std::string>::const_iterator m_End; // one past end of this block in the lines vector
		int m_IndentsThisBlock;
		bool m_IsValid;
	};
}