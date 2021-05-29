#pragma once
#include <string>

namespace Instance
{
    class GuardBase
    {
      protected:
        std::string name;

      public:
        GuardBase(std::string);

      public:
        virtual bool isAnotherRunning() = 0;
    };
} // namespace Instance