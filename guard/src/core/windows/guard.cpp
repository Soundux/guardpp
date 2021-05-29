#if defined(_WIN32)
#include <core/windows/guard.hpp>
#include <stdexcept>

namespace Instance
{
    Guard::Guard(std::string name) : GuardBase(std::move(name))
    {
        mutexHandle = CreateMutex(nullptr, false, this->name.c_str());
        if (!mutexHandle)
        {
            throw std::runtime_error("Mutex creation failed");
        }

        if (GetLastError() != ERROR_ALREADY_EXISTS)
        {
            wasCreated = true;
        }
    }

    Guard::~Guard()
    {
        ReleaseMutex(mutexHandle);

        if (wasCreated)
        {
            CloseHandle(mutexHandle);
        }
    }

    bool Guard::isAnotherRunning()
    {
        auto status = WaitForSingleObject(mutexHandle, 0);

        if (status == WAIT_ABANDONED)
        {
            wasCreated = true;
            return false;
        }

        return status != WAIT_OBJECT_0;
    }
} // namespace Instance
#endif