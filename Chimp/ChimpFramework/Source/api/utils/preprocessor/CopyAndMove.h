#pragma once

// Disable copy constructor and copy assignment operator
#define DISABLE_COPY(ClassName) \
    ClassName(const ClassName&) = delete; \
    ClassName& operator=(const ClassName&) = delete

// Disable move constructor and move assignment operator
#define DISABLE_MOVE(ClassName) \
    ClassName(ClassName&&) = delete; \
    ClassName& operator=(ClassName&&) = delete

// Disable both copy and move constructors and assignment operators
#define DISABLE_COPY_AND_MOVE(ClassName) \
    DISABLE_COPY(ClassName); \
    DISABLE_MOVE(ClassName)
