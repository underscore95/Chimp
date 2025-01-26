#include "api/files/yaml/YAMLSerialiser.h"
#include "api/files/yaml/YAMLBlockParser.h"

namespace Chimp {
	std::unique_ptr<YAMLSerialisable> YAMLSerialiser::Deserialise(const YAMLBlock& block) const
	{
		auto typeIdIter = block.Strings.find(TYPE_KEY);
		if (typeIdIter == block.Strings.end()) {
			Loggers::YAML().Error("Failed to deserialise YAMLBlock into a type, no type id found");
			return nullptr;
		}
		auto iter = m_Deserialisers.find(typeIdIter->second);
		if (iter == m_Deserialisers.end()) {
			Loggers::YAML().Error(std::format("Failed to deserialise YAMLBlock into a type, type id '{}' not registered", typeIdIter->second));
			return nullptr;
		}

		const auto& deserialiseFunc = iter->second;
		YAMLSerialisable::DeserialiseChildFunc deserialiseChildFunc = [this, &block, &deserialiseFunc](std::string_view key)-> std::unique_ptr<YAMLSerialisable> {
			auto childIter = block.Blocks.find(std::string(key));
			if (childIter == block.Blocks.end()) {
				return nullptr;
			}
			return Deserialise(childIter->second);
			};

		return deserialiseFunc(block, deserialiseChildFunc);
	}

	YAMLBlock YAMLSerialiser::Serialise(const YAMLSerialisable& serialisable) const
	{
		auto iter = m_TypeIds.find(typeid(serialisable));
		if (iter == m_TypeIds.end()) {
			Loggers::YAML().Error(std::format("Failed to serialise YAMLSerialisable, type id '{}' not registered", typeid(serialisable).name()));
			return {};
		}

		YAMLBlock block;
		block.Strings.insert({ TYPE_KEY, iter->second });
		serialisable.Serialise(block, [this, &block](std::string_view key, const YAMLSerialisable& child) {
			block.Blocks.insert({ std::string(key), Serialise(child) });
			});

		return block;
	}

	void YAMLSerialiser::WriteToFile(const YAMLSerialisable& serialisable, const std::filesystem::path& path) const
	{
		YAMLBlock block = Serialise(serialisable);
		YAMLWriter::Write(block, path);
	}

	void YAMLSerialiser::Write(const YAMLBlock& root, const std::filesystem::path& path)
	{
		YAMLWriter::Write(root, path);
	}

	std::unique_ptr<YAMLSerialisable> YAMLSerialiser::ReadFromFile(const std::filesystem::path& path) const
	{
		YAMLBlock block = YAMLBlockParser::Parse(path).Data;
		return Deserialise(block);
	}
}