#include <core/guardbase.hpp>

namespace Instance
{
    GuardBase::GuardBase(std::string name) : name(std::move(name)) {}
} // namespace Instance