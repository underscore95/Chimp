#pragma once

#include "api/utils/preprocessor/CopyAndMove.h"

namespace Chimp {
    struct PrimitiveType {
        DISABLE_MOVE(PrimitiveType);

    private:
        PrimitiveType(unsigned int id);

    public:
        ~PrimitiveType() = default;

        PrimitiveType(const PrimitiveType& other) = default;

        PrimitiveType& operator=(const PrimitiveType& other) = default;

    public:
        bool operator==(const PrimitiveType& other) const;

    public:
        unsigned int Id; // ID representing this primitive type

    private:
        static unsigned int NumberTypes;

    public:
        static const PrimitiveType POINTS;
        static const PrimitiveType LINES;
        static const PrimitiveType LINE_STRIP;
        static const PrimitiveType TRIANGLES;
        static const PrimitiveType TRIANGLE_STRIP;
    };
}
