#pragma once

#include <functional>
#include <memory>
#include <string_view>
#include "YAMLBlock.h"

namespace Chimp {
	class YAMLSerialisable {
	public:
		typedef std::function<void(std::string_view, const YAMLSerialisable&)> SerialiseChildFunc;
		typedef std::function<std::unique_ptr<YAMLSerialisable>(std::string_view)> DeserialiseChildFunc;

		virtual void Serialise(YAMLBlock& block, const SerialiseChildFunc& serialiseChild) const = 0;
	};
}