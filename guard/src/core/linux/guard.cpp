#if defined(__linux__)
#include <core/linux/guard.hpp>
#include <fcntl.h>
#include <stdexcept>
#include <unistd.h>

namespace Instance
{
    Guard::Guard(std::string name) : GuardBase(std::move(name)), shm_fd(shm_open(this->name.c_str(), O_RDWR, 0660))
    {
        if (shm_fd < 0)
        {
            if (errno == ENOENT)
            {
                wasCreated = true;
                shm_fd = shm_open(this->name.c_str(), O_RDWR | O_CREAT, 0660);
            }
        }

        if (shm_fd < 0)
        {
            throw std::runtime_error("Failed to create shared memory object");
        }

        if (ftruncate(shm_fd, sizeof(pthread_mutex_t)) != 0)
        {
            throw std::runtime_error("ftruncate failed");
        }

        auto *mappedMutex = mmap(nullptr, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if (mappedMutex == reinterpret_cast<void *>(-1) /*MAP_FAILED*/)
        {
            throw std::runtime_error("Failed to map mutex");
        }

        mutex = reinterpret_cast<pthread_mutex_t *>(mappedMutex);

        if (wasCreated)
        {
            pthread_mutexattr_t attr;
            if (pthread_mutexattr_init(&attr) != 0)
            {
                throw std::runtime_error("Failed to initialize mutexattr");
            }
            if (pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED) != 0)
            {
                throw std::runtime_error("Failed to make mutexattr shared");
            }
            if (pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST) != 0)
            {
                throw std::runtime_error("Failed to make mutexattr robust");
            }
            if (pthread_mutex_init(mutex, &attr))
            {
                throw std::runtime_error("Failed to initialize mutex");
            }
        }
    }
    Guard::~Guard()
    {
        if (wasCreated)
        {
            pthread_mutex_destroy(mutex);
            munmap(reinterpret_cast<void *>(mutex), sizeof(pthread_mutex_t));

            close(shm_fd);
            shm_unlink(name.c_str());
        }
        else
        {
            munmap(reinterpret_cast<void *>(mutex), sizeof(pthread_mutex_t));
            close(shm_fd);
        }
    }
    bool Guard::isAnotherRunning()
    {
        auto lockResult = pthread_mutex_trylock(mutex);

        if (lockResult == EOWNERDEAD)
        {
            lockResult = pthread_mutex_consistent(mutex);
            if (lockResult != 0)
            {
                wasCreated = true;
                return true;
            }
        }
        else if (lockResult != 0)
        {
            return true;
        }

        return false;
    }
} // namespace Instance
#endif