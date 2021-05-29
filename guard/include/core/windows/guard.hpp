#if defined(_WIN32)
#include <Windows.h>
#include <core/guardbase.hpp>

namespace Instance
{
    class Guard : public GuardBase
    {
        HANDLE mutexHandle;
        bool wasCreated = false;

      public:
        Guard(std::string);
        ~Guard();

      public:
        bool isAnotherRunning() override;
    };
} // namespace Instance

#endif