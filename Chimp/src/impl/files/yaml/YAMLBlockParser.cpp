#include "api/files/yaml/YAMLBlockParser.h"
#include "Loggers.h"

namespace Chimp {
	YAMLBlockParser::YAMLBlockParser(
		const std::vector<std::string>& lines,
		std::vector<std::string>::const_iterator begin,
		int indentsThisBlock
	) : m_IndentsThisBlock(indentsThisBlock),
		m_IsValid(true),
		m_End(lines.end())
	{
		auto& logger = Loggers::YAML();
		if (m_IndentsThisBlock < 0) [[unlikely]] {
			m_IsValid = false;
			logger.Error(std::format("Invalid indents for block: {}", m_IndentsThisBlock));
			return;
			}

		auto iter = begin;
		while (iter < lines.end()) {
			std::string_view key = GetKey(*iter);
			std::string_view value = GetValue(*iter);
			int indentsThisLine = GetIndentCount(*iter);

			// Did we reach end of block?
			if (indentsThisLine < m_IndentsThisBlock) {
				m_End = iter;
				return;
			}

			auto nextIter = iter + 1;
			int indentsNextLine = nextIter < lines.end() ? GetIndentCount(*nextIter) : -1;
			if (indentsNextLine <= m_IndentsThisBlock || indentsNextLine == -1) {
				// Parsing normal value or a list
				if (indentsNextLine == m_IndentsThisBlock && IsListElement(*nextIter)) {
					iter = ParseList(key, nextIter, lines.end());
				}
				else {
					ParseValue(key, value);
					++iter;
				}
			}
			else {
				// Parsing a block
				if (indentsThisLine + 1 < indentsNextLine) {
					logger.Error(std::format("Invalid indents for new block {}: {} -> {}", key, indentsThisLine, indentsNextLine));
					m_IsValid = false;
					return;
				}

				YAMLBlockParser parsedBlockYamlBlockParser(lines, iter + 1, indentsNextLine);
				Data.Blocks.emplace(std::string(key), parsedBlockYamlBlockParser.Data);
				iter = parsedBlockYamlBlockParser.m_End;
				m_IsValid = m_IsValid && parsedBlockYamlBlockParser.IsValid();
			}
		}
	}

	YAMLBlockParser YAMLBlockParser::Parse(const std::vector<std::string>& lines)
	{
		return YAMLBlockParser(lines, lines.begin());
	}

	YAMLBlockParser YAMLBlockParser::Parse(const std::filesystem::path& path)
	{
		std::vector<std::string> lines;
		try {
			std::ifstream file(path);
			if (file.is_open()) {
				std::string line;
				while (std::getline(file, line)) {
					lines.push_back(line);
				}
			}
			else {
				Loggers::YAML().Error(std::format("Failed to open file '{}' for reading", path.string()));
			}
		}
		catch (const std::exception& e) {
			Loggers::YAML().Error(std::format("Failed to read file '{}': {}", path.string(), e.what()));
		}

		return YAMLBlockParser(lines, lines.begin());
	}

	bool YAMLBlockParser::IsValid() const
	{
		return m_IsValid;
	}

	std::vector<std::string>::const_iterator YAMLBlockParser::ParseList(std::string_view keyView, std::vector<std::string>::const_iterator begin, std::vector<std::string>::const_iterator end)
	{
		// This is the most unreadable code ever, I'm practicing if I ever have to write the standard library

		// Attempt to set the map and parse function variables, and then insert the first value
		// TODO HANDLE RETURN END BETTER
#define FIND_MAP_AND_INSERT_FIRST_IF_FOUND(parseFunction, arraysMap, key, value, valueType) { \
    const auto v = parseFunction(value); \
    if (v.HasValue()) { \
        map = &arraysMap; \
		parseFunctions = [this](std::string_view view) -> InPlaceOptional<valueType> { return parseFunction(view); }; \
        auto boolMap = std::get_if<std::unordered_map<std::string, std::vector<valueType>>*>(&map); \
		assert(boolMap);\
        if ((*boolMap)->find(key) != (*boolMap)->end()) { \
            Loggers::YAML().Warning(std::format("List {} already exists in block", key)); \
            return end; \
        } \
        (*boolMap)->insert({ key, std::vector<valueType>{ v.Value() } }); \
    } \
}

		// Early exit for empty list
		if (begin == end) {
			Loggers::YAML().Warning(std::format("List {} is empty", keyView));
			return end;
		}

		std::string key(keyView);
		std::string_view value = GetValue(*begin);

		// Map will point to the correct map, and first value will be added into the map
		std::variant<
			std::monostate,
			std::unordered_map<std::string, std::vector<bool>>*,
			std::unordered_map<std::string, std::vector<int>>*,
			std::unordered_map<std::string, std::vector<float>>*,
			std::unordered_map<std::string, std::vector<std::string>>*
		> map;

		// Holds a function to parse into the correct type
		std::variant<
			std::function<InPlaceOptional<bool>(std::string_view)>,
			std::function<InPlaceOptional<int>(std::string_view)>,
			std::function<InPlaceOptional<float>(std::string_view)>,
			std::function<InPlaceOptional<std::string>(std::string_view)>
		> parseFunctions;

		// Set the map and parse function variants
		FIND_MAP_AND_INSERT_FIRST_IF_FOUND(ParseBool, Data.BoolArrays, key, value, bool);
		if (std::holds_alternative<std::monostate>(map)) [[likely]] {
			FIND_MAP_AND_INSERT_FIRST_IF_FOUND(ParseInt, Data.IntArrays, key, value, int);
			if (std::holds_alternative<std::monostate>(map)) [[likely]] {
				FIND_MAP_AND_INSERT_FIRST_IF_FOUND(ParseFloat, Data.FloatArrays, key, value, float);
				if (std::holds_alternative<std::monostate>(map)) {
					FIND_MAP_AND_INSERT_FIRST_IF_FOUND(ParseString, Data.StringArrays, key, value, std::string);
					assert(!std::holds_alternative<std::monostate>(map));
				}
				}
			}

			// Parse remaining items now we know the map to use
		for (auto iter = begin + 1; iter < end; ++iter) {
			if (!IsListElement(*iter)) {
				return iter;
			}
			std::string_view value = GetValue(*iter);

			std::visit([&](auto&& parseFn) {
				auto parsedValue = parseFn(value); // Parse value
				if (parsedValue.HasValue()) {
					// Push into vector
					using Type = typename decltype(parsedValue)::ValueType;
					auto correctMap = std::get_if<std::unordered_map<std::string, std::vector<Type>>*>(&map);
					assert(correctMap);
					(*correctMap)->at(key).push_back(parsedValue.Value());
				}
				else {
					Loggers::YAML().Warning(std::format("Failed to parse value: {}", value));
				}
				}, parseFunctions);
		}

		return end;
	}

	void YAMLBlockParser::ParseValue(std::string_view key, std::string_view value)
	{
#define ATTEMPT_PARSE_AND_RETURN_IF_SUCCESSFUL(parseFunction, value, map, key) {\
	const auto v = parseFunction(value);\
	if (v.HasValue()) {\
		const std::string k = std::string(key);\
		if (map.find(k) == map.end()) {\
			map[k] = v.Value();\
		} else {\
			Loggers::YAML().Warning(std::format("Key {} already exists in block", k));\
		}\
		return;\
	}\
}

		ATTEMPT_PARSE_AND_RETURN_IF_SUCCESSFUL(ParseBool, value, Data.Bools, key);
		ATTEMPT_PARSE_AND_RETURN_IF_SUCCESSFUL(ParseInt, value, Data.Ints, key);
		ATTEMPT_PARSE_AND_RETURN_IF_SUCCESSFUL(ParseFloat, value, Data.Floats, key);
		ATTEMPT_PARSE_AND_RETURN_IF_SUCCESSFUL(ParseString, value, Data.Strings, key);
		assert(false);
	}

	InPlaceOptional<bool> YAMLBlockParser::ParseBool(std::string_view value) const
	{
		if (value.size() == 4
			&& (value.at(0) == 'T' || value.at(0) == 't')
			&& (value.at(1) == 'R' || value.at(1) == 'r')
			&& (value.at(2) == 'U' || value.at(2) == 'u')
			&& (value.at(3) == 'E' || value.at(3) == 'e')) {
			return InPlaceOptional<bool>(true);
		}
		else if (value.size() == 5
			&& (value.at(0) == 'F' || value.at(0) == 'f')
			&& (value.at(1) == 'A' || value.at(1) == 'a')
			&& (value.at(2) == 'L' || value.at(2) == 'l')
			&& (value.at(3) == 'S' || value.at(3) == 's')
			&& (value.at(4) == 'E' || value.at(4) == 'e')) {
			return InPlaceOptional<bool>(false);
		}
		else {
			return InPlaceOptional<bool>();
		}
	}

	InPlaceOptional<int> YAMLBlockParser::ParseInt(std::string_view value) const
	{
		InPlaceOptional<int> result(0);
		int& val = result.UnsafeGet();

		bool negative = !value.empty() && value.at(0) == '-';
		if (negative) {
			value = value.substr(1);
		}

		for (char c : value) {
			if (c < '0' || c > '9') {
				return InPlaceOptional<int>();
			}
			val = val * 10 + (c - '0');
		}

		if (negative) {
			val = -val;
		}
		return result;
	}

	InPlaceOptional<float> YAMLBlockParser::ParseFloat(std::string_view value) const
	{
		InPlaceOptional<float> result(0.0f);
		float& val = result.UnsafeGet();
		int charactersAfterDecimal = 0;

		bool negative = !value.empty() && value.at(0) == '-';
		if (negative) {
			value = value.substr(1);
		}

		for (char c : value) {
			if (c == '.') {
				if (charactersAfterDecimal != 0) return InPlaceOptional<float>();
				charactersAfterDecimal = 1;
			}
			else if (c < '0' || c > '9') {
				return InPlaceOptional<float>();
			}
			else if (charactersAfterDecimal > 0) {
				val += (c - '0') * std::pow(10, -charactersAfterDecimal);
				charactersAfterDecimal++;
			}
			else {
				val = val * 10 + (c - '0');
			}
		}

		if (charactersAfterDecimal == 0) {
			return InPlaceOptional<float>();
		}

		if (negative) {
			val = -val;
		}
		return result;
	}

	InPlaceOptional<std::string> YAMLBlockParser::ParseString(std::string_view value) const
	{
		return InPlaceOptional<std::string>(std::string(value));
	}

	int YAMLBlockParser::GetIndentCount(std::string_view line) const
	{
		int chars = 0;
		for (char c : line) {
			if (c == '\t') {
				chars++;
			}
			else {
				return chars;
			}
		}
		return chars;
	}

	std::string_view YAMLBlockParser::GetKey(std::string_view line) const
	{
		std::string_view noIndents = line.substr(m_IndentsThisBlock);
		size_t colonIndex = noIndents.find(':');
		if (colonIndex == std::string::npos) {
			return noIndents;
		}
		else {
			return noIndents.substr(0, colonIndex);
		}
	}

	std::string_view YAMLBlockParser::GetValue(std::string_view line) const
	{
		std::string_view noIndents = line.substr(m_IndentsThisBlock);
		for (size_t i = 0; i < noIndents.size(); i++) {
			if (noIndents.at(i) == ':' && i + 1 < noIndents.size() && noIndents.at(i + 1) == ' ') {
				return noIndents.substr(i + 2);
			}
			else if (noIndents.at(i) == ' ' && i + 2 < noIndents.size() && noIndents.at(i + 1) == '-' && noIndents.at(i + 2) == ' ') {
				return noIndents.substr(i + 3);
			}
		}
		return noIndents.substr(noIndents.size());
	}

	bool YAMLBlockParser::IsListElement(std::string_view line) const
	{
		if (line.size() - 3 < m_IndentsThisBlock) {
			return false;
		}

		for (int i = 0; i < m_IndentsThisBlock; ++i) {
			if (line.at(i) != INDENT_CHAR) {
				return false;
			}
		}

		if (line.at(m_IndentsThisBlock) == ' ' && line.at(m_IndentsThisBlock + 1) == '-' && line.at(m_IndentsThisBlock + 2) == ' ') {
			return true;
		}

		return false;
	}
}