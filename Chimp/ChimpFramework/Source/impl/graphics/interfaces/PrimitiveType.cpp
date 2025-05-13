#include "api/graphics/PrimitiveType.h"

namespace Chimp {
    unsigned int PrimitiveType::NumberTypes = 0;

    const PrimitiveType PrimitiveType::POINTS = PrimitiveType(++PrimitiveType::NumberTypes);
    const PrimitiveType PrimitiveType::LINES = PrimitiveType(++PrimitiveType::NumberTypes);
    const PrimitiveType PrimitiveType::LINE_STRIP = PrimitiveType(++PrimitiveType::NumberTypes);
    const PrimitiveType PrimitiveType::TRIANGLES = PrimitiveType(++PrimitiveType::NumberTypes);
    const PrimitiveType PrimitiveType::TRIANGLE_STRIP = PrimitiveType(++PrimitiveType::NumberTypes);

    PrimitiveType::PrimitiveType(unsigned int id)
        : Id(id) {}

    bool PrimitiveType::operator==(const PrimitiveType& other) const {
        return this->Id == other.Id;
    }
}