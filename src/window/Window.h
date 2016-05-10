#ifndef XENGINE_WINDOW_WINDOW_H
#define XENGINE_WINDOW_WINDOW_H

#if X_WINDOWS || X_OSX || X_LINUX
# include "window/glfw/GLFWWindow.h"
#endif // X_WINDOWS || X_OSX || X_LINUX

namespace xEngine {

#if X_WINDOWS || X_OSX || X_LINUX
class Window: public GLFWWindow { };
#endif // X_WINDOWS || X_OSX || X_LINUX

} // namespace xEngine

#endif // XENGINE_WINDOW_WINDOW_H