#pragma once

// nlohmann json
// not sure why but it does not like the ifdef
//#ifdef CHIMP_NLOHMANN_JSON
#include <nlohmann/json.hpp>
namespace Chimp {
	using Json = nlohmann::json;
}
//#endif