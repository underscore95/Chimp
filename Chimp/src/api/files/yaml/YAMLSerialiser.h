#pragma once

#include "YAMLWriter.h"
#include "Loggers.h"
#include "api/utils/preprocessor/Casting.h"
#include "YAMLSerialisable.h"
#include "stdafx.h"

namespace Chimp {
	class YAMLSerialiser {
	public:
		YAMLSerialiser() = default;

		// Register a serialisable type
		// id - unique id for the type, should never be changed or yaml will fail to deserialise
		// deserialiser - function that takes a YAMLBlock and returns a new instance of the type
		template <typename T>
		void RegisterSerialisable(const std::string &id, std::function<std::unique_ptr<T>(const YAMLBlock&, const YAMLSerialisable::DeserialiseChildFunc&)> deserialiser) {
			static_assert(std::is_base_of_v<YAMLSerialisable, T>, "Type must be derived from YAMLSerialisable");

			auto iter = m_Deserialisers.find(id);
			if (iter != m_Deserialisers.end()) {
				Loggers::YAML().Error(std::format("Failed to register serialisable type, id '{}' already registered", id));
				return;
			}

			auto typeIdIter = m_TypeIds.find(typeid(T));
			if (typeIdIter != m_TypeIds.end()) {
				Loggers::YAML().Error(std::format("Failed to register serialisable type, type with id {} has already been registered under a different id.", typeIdIter->second));
				return;
			}

			m_Deserialisers[id] = [deserialiser](const YAMLBlock& block, const YAMLSerialisable::DeserialiseChildFunc& deserialiseChild) {
				return UNIQUE_PTR_CAST_AND_MOVE(YAMLSerialisable, deserialiser(block, deserialiseChild));
				};
			m_TypeIds[typeid(T)] = id;
		}

		// Deserialise a YAMLBlock into a type, type must be YAMLSerialisable and must be registered
		[[nodiscard]] std::unique_ptr<YAMLSerialisable> Deserialise(const YAMLBlock& block) const;

		// Serialise a YAMLSerialisable into a YAMLBlock
		[[nodiscard]] YAMLBlock Serialise(const YAMLSerialisable& serialisable) const;

		// Write a YAMLSerialisable to a file, overwrites existing contents
		void WriteToFile(const YAMLSerialisable& serialisable, const std::filesystem::path& path) const;

		// Writes the YAMLBlock to file, overwrites existing contents
		// path - path to write to
		void Write(const YAMLBlock& root, const std::filesystem::path& path);

		// Read a YAMLSerialisable from a file
		[[nodiscard]] std::unique_ptr<YAMLSerialisable> ReadFromFile(const std::filesystem::path& path) const;

	public:
		static constexpr const char* TYPE_KEY = "_type";
	private:
		std::unordered_map<std::type_index, std::string> m_TypeIds;
		std::unordered_map<std::string, std::function<std::unique_ptr<YAMLSerialisable>(const YAMLBlock&, const YAMLSerialisable::DeserialiseChildFunc&)>> m_Deserialisers;
	};
}