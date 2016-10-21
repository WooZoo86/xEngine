#if X_MACOS

#include "MacOSWindow.h"

#import <Cocoa/Cocoa.h>

using namespace xEngine;

@interface AppDelegate : NSObject <NSApplicationDelegate>

@end

@implementation AppDelegate

@end

@interface WindowDelegate : NSObject <NSWindowDelegate> {
  MacOSWindow *window;
}

- (id) initWithMacOSWindow:(MacOSWindow *)initWindow;

@end

@implementation WindowDelegate

- (id) initWithMacOSWindow:(MacOSWindow *)initWindow {
  self = [super init];
  if (self != nil) window = initWindow;
  return self;
}

- (BOOL) windowShouldClose:(id)sender {
  SetShouldClose(window);
  return NO;
}

@end

@interface WindowView : NSView {
  MacOSWindow *window;
}

- (id) initWithMacOSWindow:(MacOSWindow *)initWindow;

@end

@implementation WindowView

- (id) initWithMacOSWindow:(MacOSWindow *)initWindow {
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
  }
}

@end

namespace xEngine {

NSMenu *menu_{nullptr};

void MacOSWindow::Initialize() {
  @autoreleasepool {
    [NSApplication sharedApplication];
    [[NSApplication sharedApplication] setActivationPolicy:NSApplicationActivationPolicyRegular];
    NSMenu *menu = [[NSMenu alloc] init];
    [[NSApplication sharedApplication] setMainMenu:menu];
    NSMenuItem* appMenuItem = [menu addItemWithTitle:@"" action:NULL keyEquivalent:@""];
    menu_ = [[NSMenu alloc] init];
    [appMenuItem setSubmenu:menu_];
    [[NSApplication sharedApplication] performSelector:NSSelectorFromString(@"setAppleMenu:") withObject:menu_];
    [[NSApplication sharedApplication] setDelegate:[[AppDelegate alloc] init]];
  }
}

void MacOSWindow::Finalize() {
  @autoreleasepool {
    if (menu_) {
      [menu_ release];
    }
    id main_menu = [[NSApplication sharedApplication] mainMenu];
    if (main_menu) {
      [main_menu release];
    }
    id delegate = [[NSApplication sharedApplication] delegate];
    [[NSApplication sharedApplication] setDelegate:nil];
    if (delegate) {
      [delegate release];
    }
  }
}

void MacOSWindow::PollEvent() {
  @autoreleasepool {
    NSEvent *event;
    do {
      event = [NSApp nextEventMatchingMask:NSEventMaskAny untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:YES];
      [[NSApplication sharedApplication] sendEvent: event];
    } while(event != nil);
  }
}

void MacOSWindow::Create(const WindowConfig &config) {
  x_assert(!Available());
  @autoreleasepool {
    NSWindow *window = [[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, config.width, config.height)
                                         styleMask:(NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable)
                                         backing:NSBackingStoreBuffered
                                         defer:YES];
    [window setTitle:[NSString stringWithCString:config.title.c_str() encoding:[NSString defaultCStringEncoding]]];
    [window setDelegate:[[WindowDelegate alloc] initWithMacOSWindow:this]];
    WindowView *view = [[WindowView alloc] initWithMacOSWindow:this];
    [view setWantsBestResolutionOpenGLSurface:YES];
    [window makeFirstResponder:view];
    [window setContentView:view];
    [window setAcceptsMouseMovedEvents:YES];
    [window setRestorable:NO];
    [window orderFront:nil];
    [[NSApplication sharedApplication] activateIgnoringOtherApps:YES];
    [window makeKeyAndOrderFront:nil];
    window_ = window;
  }
}

void MacOSWindow::Destroy() {
  x_assert(Available());
  @autoreleasepool {
    NSWindow *window = static_cast<NSWindow *>(window_);
    [[window contentView] release];
    [[window delegate] release];
    [window release];
    window_ = nullptr;
  }
}

bool MacOSWindow::Available() const {
  return window_ != nullptr;
}

bool MacOSWindow::ShouldClose() {
  x_assert(Available());
  return should_close_;
}

void MacOSWindow::SetTitle(const eastl::string &name) {
  x_assert(Available());
  @autoreleasepool {
    NSWindow *window = static_cast<NSWindow *>(window_);
    [window setTitle:[NSString stringWithCString:name.c_str() encoding:[NSString defaultCStringEncoding]]];
  }
}

} // namespace xEngine

#endif