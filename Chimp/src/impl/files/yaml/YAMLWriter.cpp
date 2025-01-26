#include "api/files/yaml/YAMLWriter.h"
#include "api/files/yaml/YAMLBlockParser.h"
#include "Loggers.h"

namespace Chimp {
	void YAMLWriter::Write(const YAMLBlock& root, std::vector<std::string>& out)
	{
		Write(root, [&out](std::string_view line) {
			out.push_back(std::string(line));
			});
	}

	void YAMLWriter::Write(const YAMLBlock& root, const std::filesystem::path& path)
	{
		try {
			if (path.has_parent_path() && !std::filesystem::exists(path.parent_path())) {
				std::filesystem::create_directories(path.parent_path());
			}
		}
		catch (const std::exception& e) {
			Loggers::YAML().Error(std::format("Failed to create directories for file '{}': {}", path.string(), e.what()));
			return;
		}

		std::ofstream file(path);

		try {
			if (!file.is_open()) {
				Loggers::YAML().Error(std::format("Failed to open file '{}' for writing", path.string()));
				return;
			}

			Write(root, [&file](std::string_view line) {
				file << line << '\n';
				});
		}
		catch (const std::exception& e) {
			Loggers::YAML().Error(std::format("Failed to write to file '{}': {}", path.string(), e.what()));
		}
	}

	void YAMLWriter::Write(const YAMLBlock& root, std::function<void(std::string_view)> writeFunc, int numIndents) {
		const std::string indents = std::string(numIndents, YAMLBlockParser::INDENT_CHAR);
		for (const auto& [key, value] : root.Bools) {
			writeFunc(std::format("{}{}: {}", indents, key, value));
		}
		for (const auto& [key, value] : root.Ints) {
			writeFunc(std::format("{}{}: {}", indents, key, value));
		}
		for (const auto& [key, value] : root.Floats) {
			// Ensure that floats aren't misintrepreted as ints when deserialising
			const std::string stringValue = FloatEqual(value, std::floor(value)) ? std::format("{:.1f}", value) : std::format("{}", value);
			writeFunc(std::format("{}{}: {}", indents, key, stringValue));
		}
		for (const auto& [key, value] : root.Strings) {
			writeFunc(std::format("{}{}: {}", indents, key, value));
		}
		for (const auto& [key, value] : root.StringArrays) {
			writeFunc(std::format("{}{}:", indents, key));
			for (const auto& v : value) {
				writeFunc(std::format("{} - {}", indents, v));
			}
		}
		for (const auto& [key, value] : root.IntArrays) {
			writeFunc(std::format("{}{}:", indents, key));
			for (const auto& v : value) {
				writeFunc(std::format("{} - {}", indents, v));
			}
		}
		for (const auto& [key, value] : root.FloatArrays) {
			writeFunc(std::format("{}{}:", indents, key));
			for (const auto& v : value) {
				writeFunc(std::format("{} - {}", indents, v));
			}
		}
		for (const auto& [key, value] : root.BoolArrays) {
			writeFunc(std::format("{}{}:", indents, key));
			for (const auto& v : value) {
				writeFunc(std::format("{} - {}", indents, v));
			}
		}
		for (const auto& [key, value] : root.Blocks) {
			writeFunc(std::format("{}{}:", indents, key));
			Write(value, writeFunc, numIndents + 1);
		}
	}
}