#if X_MACOS

#include "MacOSWindow.h"

#include "application/Application.h"

#import <Cocoa/Cocoa.h>

using namespace xEngine;

@interface MacOSWindowDelegate : NSObject <NSWindowDelegate> {
  MacOSWindow *window;
}

- (id) initWithMacOSWindow: (MacOSWindow *) initWindow;

@end

@implementation MacOSWindowDelegate

- (id) initWithMacOSWindow: (MacOSWindow *) initWindow {
  self = [super init];
  if (self != nil) window = initWindow;
  return self;
}

- (BOOL) windowShouldClose: (id) sender {
  InvokeWindowClose(window);
  return NO;
}

@end

@interface MacOSWindowView : NSView {
  MacOSWindow *window;
}

- (id) initWithMacOSWindow: (MacOSWindow *) initWindow;

@end

@implementation MacOSWindowView

- (id) initWithMacOSWindow: (MacOSWindow *) initWindow {
  self = [super init];
  if (self != nil) window = initWindow;
  return self;
}

- (BOOL) isOpaque {
  return YES;
}

- (BOOL) canBecomeKeyView {
  return YES;
}

- (BOOL) acceptsFirstResponder {
  return YES;
}

- (void) viewDidChangeBackingProperties {
  @autoreleasepool {
    const NSRect content_rect = [self frame];
    const NSRect frame_buffer_rect = [self convertRectToBacking:content_rect];

    auto &config = GetConfig(window);
    config.frame_buffer_width = frame_buffer_rect.size.width;
    config.frame_buffer_height = frame_buffer_rect.size.height;

    InvokeWindowResize(window);
  }
}

- (void) mouseMoved: (NSEvent *) event {
  @autoreleasepool {
    const NSRect content_rect = [self frame];
    const NSPoint position = [event locationInWindow];
    SetMousePosition(window, position.x, content_rect.size.height - position.y);
   }
}

- (void) mouseDragged: (NSEvent *) event {
  [self mouseMoved:event];
}

- (void) mouseDown: (NSEvent *) event {
  SetMouseButtonDown(window, MouseButtonType::kLeft);
}

- (void) mouseUp: (NSEvent *) event {
  SetMouseButtonUp(window, MouseButtonType::kLeft);
}

- (void) rightMouseDown: (NSEvent *) event {
  SetMouseButtonDown(window, MouseButtonType::kRight);
}

- (void) rightMouseUp: (NSEvent *) event {
  SetMouseButtonUp(window, MouseButtonType::kRight);
}

- (void) otherMouseDown: (NSEvent *) event {
  SetMouseButtonDown(window, MouseButtonType::kMiddle);
}

- (void) otherMouseUp: (NSEvent *) event {
  SetMouseButtonUp(window, MouseButtonType::kMiddle);
}

- (void) scrollWheel: (NSEvent *) event {
  @autoreleasepool {
    auto x = [event scrollingDeltaX];
    auto y = [event scrollingDeltaY];
    if ([event hasPreciseScrollingDeltas]) {
        x *= 0.1;
        y *= 0.1;
    }
    if (fabs(x) > 0.0 || fabs(y) > 0.0) {
      SetMouseScroll(window, x, y);
    }
  }
}

@end

namespace xEngine {

void MacOSWindow::Create(const WindowConfig &config) {
  @autoreleasepool {
	x_assert(config.delegate != nullptr);
    config_ = config;
    NSWindow *window = [[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, config.width, config.height)
                                         styleMask:(NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable)
                                         backing:NSBackingStoreBuffered
                                         defer:YES];
    [window setTitle:[NSString stringWithCString:config.title.c_str() encoding:[NSString defaultCStringEncoding]]];
    [window setDelegate:[[MacOSWindowDelegate alloc] initWithMacOSWindow:this]];
    MacOSWindowView *view = [[MacOSWindowView alloc] initWithMacOSWindow:this];
    [view setWantsBestResolutionOpenGLSurface:YES];
    [window makeFirstResponder:view];
    [window setContentView:view];
    [window setAcceptsMouseMovedEvents:YES];
    [window setRestorable:NO];
    [window orderFront:nil];
    [[NSApplication sharedApplication] activateIgnoringOtherApps:YES];
    [window makeKeyAndOrderFront:nil];
    [window center];
    window_ = window;
    loop_id_ = Application::GetInstance().AddLoop([this](){
      if (this->config_.delegate != nullptr) {
        Reset();
        config_.delegate->OnWindowUpdate();
      }
    });
  }
}

void MacOSWindow::Destroy() {
  @autoreleasepool {
    NSWindow *window = static_cast<NSWindow *>(window_);
    [[window contentView] release];
    [[window delegate] release];
    [window release];
    window_ = nullptr;
    Application::GetInstance().RemoveLoop(loop_id_);
    loop_id_ = kInvalidLoopID;
  }
}

void MacOSWindow::SetTitle(const eastl::string &name) {
  @autoreleasepool {
    NSWindow *window = static_cast<NSWindow *>(window_);
    [window setTitle:[NSString stringWithCString:name.c_str() encoding:[NSString defaultCStringEncoding]]];
  }
}

} // namespace xEngine

#endif