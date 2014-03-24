#include "MultiThreading.h"

THREAD_HANDLE   CreateThreadPortable    (void *(*start_routine) (void *), void *arg)
{
    #if defined _WIN32 || defined _WIN64
        return CreateThread (0, 0, (LPTHREAD_START_ROUTINE)start_routine, (LPVOID)arg, 0, 0);
    #else
        THREAD_HANDLE th = (THREAD_HANDLE)malloc(sizeof(pthread_t));
    
        if (!th)
        {
            std::cerr << "Error allocating thread." << std::endl;
            return NULL;
        }

        if(int err = pthread_create(th, NULL, start_routine, arg))
        {
            std::cerr << strerror(err) << std::endl;
            return NULL;
        }
        return th;
    #endif
}

void DestroyThreadPortable (THREAD_HANDLE handle)
{
    #if !defined _WIN32 && !defined _WIN64
        if(handle)
            free(handle);
    #endif
}
