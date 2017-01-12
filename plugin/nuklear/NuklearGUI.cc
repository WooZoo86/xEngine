#include "NuklearGUI.h"

#include <window/Window.h>

#define NK_IMPLEMENTATION
#include <nuklear.h>

#include <gtc/matrix_transform.hpp>

namespace xEngine {

#if X_OPENGL

static const char *vertex_shader =
    "#version 410 core\n"
    "uniform mat4 uProjectionMatrix;\n"
    "in vec2 aPosition;\n"
    "in vec2 aTexcoord0;\n"
    "in vec4 aColor0;\n"
    "out vec2 Texcoord;\n"
    "out vec4 Color;\n"
    "void main() {\n"
    "   Texcoord = aTexcoord0;\n"
    "   Color = aColor0;\n"
    "   gl_Position = uProjectionMatrix * vec4(aPosition, 0, 1);\n"
    "}\n";

static const char *fragment_shader =
    "#version 410 core\n"
    "precision mediump float;\n"
    "uniform sampler2D uTexture;\n"
    "in vec2 Texcoord;\n"
    "in vec4 Color;\n"
    "out vec4 outColor;\n"
    "void main(){\n"
    "   outColor = Color * texture(uTexture, Texcoord);\n"
    "}\n";

#elif X_D3D11

static const uchar vertex_shader[] =
{
  0x44, 0x58, 0x42, 0x43, 0x3D, 0x38, 0x11, 0x7C, 0x5D, 0x89, 0x17, 0x1D, 0x88, 0x76, 0xCC, 0xA5, 0xEC, 0x0E, 0x1C, 0x62, 0x01, 0x00, 0x00, 0x00, 0xD8, 0x03, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x4C, 0x01, 0x00, 0x00, 0xBC, 0x01, 0x00, 0x00, 0x30, 0x02, 0x00, 0x00, 0x3C, 0x03, 0x00, 0x00, 0x52, 0x44, 0x45, 0x46, 0x10, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x05, 0xFE, 0xFF, 0x00, 0x01, 0x00, 0x00, 0xE8, 0x00, 0x00, 0x00, 0x52, 0x44, 0x31, 0x31, 0x3C, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x47, 0x6C, 0x6F, 0x62, 0x61, 0x6C, 0x73, 0x00, 0xAB, 0xAB, 0xAB, 0x5C, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0xC4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x75, 0x50, 0x72, 0x6F, 0x6A, 0x65, 0x63, 0x74, 0x69, 0x6F, 0x6E, 0x4D, 0x61, 0x74, 0x72, 0x69, 0x78, 0x00, 0x66, 0x6C, 0x6F, 0x61, 0x74, 0x34, 0x78, 0x34, 0x00, 0xAB, 0x03, 0x00, 0x03, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBA, 0x00, 0x00, 0x00, 0x4D, 0x69, 0x63, 0x72, 0x6F, 0x73, 0x6F, 0x66, 0x74, 0x20, 0x28, 0x52, 0x29, 0x20, 0x48, 0x4C, 0x53, 0x4C, 0x20, 0x53, 0x68, 0x61, 0x64, 0x65, 0x72, 0x20, 0x43, 0x6F, 0x6D, 0x70, 0x69, 0x6C, 0x65, 0x72, 0x20, 0x31, 0x30, 0x2E, 0x31, 0x00, 0x49, 0x53, 0x47, 0x4E, 0x68, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x59, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x5F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x50, 0x4F, 0x53, 0x49, 0x54, 0x49, 0x4F, 0x4E, 0x00, 0x43, 0x4F, 0x4C, 0x4F, 0x52, 0x00, 0x54, 0x45, 0x58, 0x43, 0x4F, 0x4F, 0x52, 0x44, 0x00, 0x4F, 0x53, 0x47, 0x4E, 0x6C, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x5C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x53, 0x56, 0x5F, 0x50, 0x4F, 0x53, 0x49, 0x54, 0x49, 0x4F, 0x4E, 0x00, 0x43, 0x4F, 0x4C, 0x4F, 0x52, 0x00, 0x54, 0x45, 0x58, 0x43, 0x4F, 0x4F, 0x52, 0x44, 0x00, 0xAB, 0x53, 0x48, 0x45, 0x58, 0x04, 0x01, 0x00, 0x00, 0x50, 0x00, 0x01, 0x00, 0x41, 0x00, 0x00, 0x00, 0x6A, 0x08, 0x00, 0x01, 0x59, 0x00, 0x00, 0x04, 0x46, 0x8E, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x5F, 0x00, 0x00, 0x03, 0x32, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5F, 0x00, 0x00, 0x03, 0xF2, 0x10, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x5F, 0x00, 0x00, 0x03, 0x32, 0x10, 0x10, 0x00, 0x02, 0x00, 0x00, 0x00, 0x67, 0x00, 0x00, 0x04, 0xF2, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x03, 0xF2, 0x20, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x03, 0x32, 0x20, 0x10, 0x00, 0x02, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x08, 0xF2, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x56, 0x15, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x8E, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x0A, 0xF2, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x8E, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x0E, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xF2, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x0E, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x8E, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x05, 0xF2, 0x20, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x46, 0x1E, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x05, 0x32, 0x20, 0x10, 0x00, 0x02, 0x00, 0x00, 0x00, 0x46, 0x10, 0x10, 0x00, 0x02, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x01, 0x53, 0x54, 0x41, 0x54, 0x94, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const size_t vertex_length = 984;

static const uchar fragment_shader[] =
{
  0x44, 0x58, 0x42, 0x43, 0x08, 0x48, 0x3F, 0xE0, 0x98, 0x96, 0x43, 0x89, 0x9C, 0x21, 0x21, 0xC8, 0x30, 0x73, 0x57, 0xD4, 0x01, 0x00, 0x00, 0x00, 0xE0, 0x02, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0xF4, 0x00, 0x00, 0x00, 0x68, 0x01, 0x00, 0x00, 0x9C, 0x01, 0x00, 0x00, 0x44, 0x02, 0x00, 0x00, 0x52, 0x44, 0x45, 0x46, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x05, 0xFF, 0xFF, 0x00, 0x01, 0x00, 0x00, 0x8E, 0x00, 0x00, 0x00, 0x52, 0x44, 0x31, 0x31, 0x3C, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x85, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x75, 0x53, 0x61, 0x6D, 0x70, 0x6C, 0x65, 0x72, 0x00, 0x75, 0x54, 0x65, 0x78, 0x74, 0x75, 0x72, 0x65, 0x00, 0x4D, 0x69, 0x63, 0x72, 0x6F, 0x73, 0x6F, 0x66, 0x74, 0x20, 0x28, 0x52, 0x29, 0x20, 0x48, 0x4C, 0x53, 0x4C, 0x20, 0x53, 0x68, 0x61, 0x64, 0x65, 0x72, 0x20, 0x43, 0x6F, 0x6D, 0x70, 0x69, 0x6C, 0x65, 0x72, 0x20, 0x31, 0x30, 0x2E, 0x31, 0x00, 0xAB, 0xAB, 0x49, 0x53, 0x47, 0x4E, 0x6C, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x5C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x53, 0x56, 0x5F, 0x50, 0x4F, 0x53, 0x49, 0x54, 0x49, 0x4F, 0x4E, 0x00, 0x43, 0x4F, 0x4C, 0x4F, 0x52, 0x00, 0x54, 0x45, 0x58, 0x43, 0x4F, 0x4F, 0x52, 0x44, 0x00, 0xAB, 0x4F, 0x53, 0x47, 0x4E, 0x2C, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x53, 0x56, 0x5F, 0x54, 0x61, 0x72, 0x67, 0x65, 0x74, 0x00, 0xAB, 0xAB, 0x53, 0x48, 0x45, 0x58, 0xA0, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x6A, 0x08, 0x00, 0x01, 0x5A, 0x00, 0x00, 0x03, 0x00, 0x60, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 0x18, 0x00, 0x04, 0x00, 0x70, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x55, 0x00, 0x00, 0x62, 0x10, 0x00, 0x03, 0xF2, 0x10, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x62, 0x10, 0x00, 0x03, 0x32, 0x10, 0x10, 0x00, 0x02, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x03, 0xF2, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00, 0x45, 0x00, 0x00, 0x8B, 0xC2, 0x00, 0x00, 0x80, 0x43, 0x55, 0x15, 0x00, 0xF2, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x10, 0x10, 0x00, 0x02, 0x00, 0x00, 0x00, 0x46, 0x7E, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x07, 0xF2, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x0E, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x1E, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x01, 0x53, 0x54, 0x41, 0x54, 0x94, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const size_t fragment_length = 736;

#endif

struct vertex_struct {
  float32 position[2];
  float32 texcoord[2];
  nk_byte color[4];
};

void NuklearGUI::Initialize(NuklearGUIConfig config) {
  x_assert(!Available());

  gui_config_ = config;

  x_assert(gui_config_.window != kInvalidResourceID);
  window_ = Window::GetInstance().Get(gui_config_.window).get();
  x_assert(window_ && window_->graphics());
  graphics_ = window_->graphics().get();
  graphics_->renderer()->MakeCurrent();

  static const nk_draw_vertex_layout_element vertex_layout[] = {
      {NK_VERTEX_POSITION, NK_FORMAT_FLOAT, offsetof(vertex_struct, position)},
      {NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, offsetof(vertex_struct, texcoord)},
      {NK_VERTEX_COLOR, NK_FORMAT_R8G8B8A8, offsetof(vertex_struct, color)},
      {NK_VERTEX_LAYOUT_END}
  };

  memset(&config_, 0, sizeof(config_));
  config_.global_alpha = gui_config_.global_alpha;
  config_.line_AA = gui_config_.line_AA ? NK_ANTI_ALIASING_ON : NK_ANTI_ALIASING_OFF;
  config_.shape_AA = gui_config_.shape_AA ? NK_ANTI_ALIASING_ON : NK_ANTI_ALIASING_OFF;
  config_.circle_segment_count = gui_config_.circle_segment_count;
  config_.arc_segment_count = gui_config_.arc_segment_count;
  config_.curve_segment_count = gui_config_.curve_segment_count;
  config_.vertex_layout = vertex_layout;
  config_.vertex_size = sizeof(vertex_struct);
  config_.vertex_alignment = NK_ALIGNOF(vertex_struct);

  nk_init_default(&context_, nullptr);

  nk_font_atlas_init_default(&font_atlas_);
  nk_font_atlas_begin(&font_atlas_);
  TextureConfig texture_config;
  texture_config.color_format = PixelFormat::RGBA8;
  auto image_data =
      nk_font_atlas_bake(&font_atlas_, &texture_config.width, &texture_config.height, NK_FONT_ATLAS_RGBA32);
  texture_config.data[0][0] = Data::Create(reinterpret_cast<const char *>(image_data),
                                           static_cast<size_t>(texture_config.width * texture_config.height * 4));
  auto image_id = graphics_->resource_manager()->Create(texture_config);
  image_.push_back(image_id);
  auto image = nk_image_id(static_cast<int32>(image_.size() - 1));
  nk_font_atlas_end(&font_atlas_, image.handle, &config_.null);

  nk_style_set_font(&context_, &font_atlas_.default_font->handle);

  sampler_ = graphics_->resource_manager()->Create(SamplerConfig());

  auto shader_config = ShaderConfig::FromBuffer(vertex_shader, vertex_length, fragment_shader, fragment_length);
  shader_ = graphics_->resource_manager()->Create(shader_config);

  MeshConfig mesh_config;
  mesh_config.index_usage = BufferUsage::kStream;
  mesh_config.index_type = IndexFormat::kUint16;
  mesh_config.index_count = gui_config_.max_index_count;
  mesh_config.vertex_usage = BufferUsage::kStream;
  mesh_config.vertex_count = gui_config_.max_vertex_count;
  mesh_config.layout.AddElement(VertexElementSemantic::kPosition, VertexElementFormat::kFloat2).
      AddElement(VertexElementSemantic::kTexcoord0, VertexElementFormat::kFloat2).
      AddElement(VertexElementSemantic::kColor0, VertexElementFormat::kUnsignedByte4Normalized);
  mesh_ = graphics_->resource_manager()->Create(mesh_config);

  auto pipeline_config = PipelineConfig::ShaderWithLayout(shader_, mesh_config.layout);

  pipeline_config.blend_state.enable = true;
  pipeline_config.blend_state.src_rgb_factor = BlendFactor::kSrcAlpha;
  pipeline_config.blend_state.dst_rgb_factor = BlendFactor::kOneMinusSrcAlpha;
  pipeline_config.blend_state.color_format = window_->graphics()->config().color_format;
  pipeline_config.blend_state.depth_format = window_->graphics()->config().depth_format;
  pipeline_config.blend_state.color_mask = PixelChannel::kRGB;

  pipeline_config.depth_stencil_state.depth_enable = false;

  pipeline_config.rasterizer_state.scissor_test_enable = true;
  pipeline_config.rasterizer_state.cull_face_enable = false;
  pipeline_config.rasterizer_state.sample = window_->graphics()->config().sample_count;

  pipeline_ = graphics_->resource_manager()->Create(pipeline_config);

  auto vertex_size = mesh_config.vertex_count * mesh_config.layout.size;
  auto index_size = mesh_config.index_count * SizeOfIndexFormat(mesh_config.index_type);

  vertex_data = Data::Create(vertex_size);
  index_data = Data::Create(index_size);

  nk_buffer_init_default(&command);
  nk_buffer_init_fixed(&vertex_buffer, vertex_data->buffer(), vertex_data->size());
  nk_buffer_init_fixed(&index_buffer, index_data->buffer(), index_data->size());
}

void NuklearGUI::Finalize() {
  x_assert(Available());
  graphics_->renderer()->MakeCurrent();
  graphics_->resource_manager()->Destroy(sampler_);
  graphics_->resource_manager()->Destroy(shader_);
  graphics_->resource_manager()->Destroy(mesh_);
  graphics_->resource_manager()->Destroy(pipeline_);
  for (auto &id : image_) {
    graphics_->resource_manager()->Destroy(id);
  }
  image_.clear();
  nk_font_atlas_clear(&font_atlas_);
  nk_free(&context_);
  graphics_ = nullptr;
}

void NuklearGUI::BeginFrame() {
  nk_input_begin(&context_);
  auto &position = window_->GetMousePosition();
  auto x = static_cast<int32>(position.x);
  auto y = static_cast<int32>(position.y);
  nk_input_motion(&context_, x, y);
  nk_input_scroll(&context_, window_->GetMouseScroll().y);
  nk_input_button(&context_, NK_BUTTON_LEFT, x, y, window_->IsMouseButtonDown(MouseButtonType::kLeft) || window_->IsMouseButtonPressed(MouseButtonType::kLeft));
  nk_input_button(&context_, NK_BUTTON_RIGHT, x, y, window_->IsMouseButtonDown(MouseButtonType::kRight) || window_->IsMouseButtonPressed(MouseButtonType::kRight));
  nk_input_button(&context_, NK_BUTTON_MIDDLE, x, y, window_->IsMouseButtonDown(MouseButtonType::kMiddle) || window_->IsMouseButtonPressed(MouseButtonType::kMiddle));
  nk_input_end(&context_);
}

void NuklearGUI::EndFrame() {
  auto matrix = glm::ortho(0.0f,
                           static_cast<float32>(window_->config().width),
                           static_cast<float32>(window_->config().height),
                           0.0f,
                           -1.0f,
                           1.0f);
  auto scale_x = static_cast<float32>(window_->config().frame_buffer_width) / window_->config().width;
  auto scale_y = static_cast<float32>(window_->config().frame_buffer_height) / window_->config().height;
  nk_buffer_clear(&vertex_buffer);
  nk_buffer_clear(&index_buffer);
  nk_convert(&context_, &command, &vertex_buffer, &index_buffer, &config_);
  graphics_->renderer()->MakeCurrent();
  graphics_->renderer()->ApplyPipeline(pipeline_);
  graphics_->renderer()->UpdateMesh(mesh_, vertex_data, index_data);
  graphics_->renderer()->UpdateShaderResourceData(shader_, "uProjectionMatrix", Data::Create(glm::value_ptr(matrix), sizeof(matrix)));
  const struct nk_draw_command *cmd = nullptr;
  ResourceID current_texture = kInvalidResourceID;
  size_t element_offset = 0;
  nk_draw_foreach(cmd, &context_, &command) {
    auto texture = image_[cmd->texture.id];
    if (current_texture != texture) {
      current_texture = texture;
      graphics_->renderer()->UpdateShaderResourceTexture(shader_, "uTexture", current_texture);
      graphics_->renderer()->UpdateShaderResourceSampler(shader_, "uSampler", sampler_);
    }
    graphics_->renderer()->ApplyScissor(static_cast<int32>(cmd->clip_rect.x * scale_x),
                                        static_cast<int32>((window_->config().height - (cmd->clip_rect.y + cmd->clip_rect.h)) * scale_y),
                                        static_cast<int32>(cmd->clip_rect.w * scale_x),
                                        static_cast<int32>(cmd->clip_rect.h * scale_y));
    draw_state_.first = element_offset;
    draw_state_.count = cmd->elem_count;
    graphics_->renderer()->Draw(draw_state_);
    element_offset += cmd->elem_count;
  }
  nk_clear(&context_);
}

} // namespace xEngine