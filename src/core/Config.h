#ifndef XENGINE_CORE_CONFIG_H
#define XENGINE_CORE_CONFIG_H

#if X_FORCE_NO_THREADS
# define X_HAS_THREADS (0)
# define X_COMPILER_HAS_THREADLOCAL (0)
#elif X_EMSCRIPTEN
# define X_HAS_THREADS (0)
# define X_COMPILER_HAS_THREADLOCAL (0)
#elif X_IOS
# define X_HAS_THREADS (1)
# define X_COMPILER_HAS_THREADLOCAL (0)
#else
# define X_HAS_THREADS (0)
# define X_COMPILER_HAS_THREADLOCAL (1)
#endif // X_FORCE_NO_THREADS

#define X_HAS_ATOMIC (1)

#endif // XENGINE_CORE_CONFIG_H