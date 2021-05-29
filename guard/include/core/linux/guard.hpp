#if defined(__linux__)
#pragma once
#include <core/guardbase.hpp>
#include <pthread.h>
#include <sys/mman.h>

namespace Instance
{
    class Guard : public GuardBase
    {
        std::int32_t shm_fd;
        pthread_mutex_t *mutex;
        bool wasCreated = false;

      public:
        Guard(std::string);
        ~Guard();

      public:
        bool isAnotherRunning() override;
    };
} // namespace Instance

#endif