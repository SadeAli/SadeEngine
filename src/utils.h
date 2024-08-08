#pragma once

#define RETURN_VAL_IF_NULL(x, val) if ((x) == nullptr) return (val)
#define RETURN_NULL_IF_NULL(x) if ((x) == nullptr) return nullptr
#define RETURN_IF_NULL(x) if ((x) == nullptr) return