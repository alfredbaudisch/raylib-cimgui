/**********************************************************************************************
*
*   dear ImGui - cimgui C bindings Platform Backend for Raylib
*
*   (Info: Raylib is a cross-platform general purpose library for handling windows,
*   inputs, graphics context creation, etc. using OpenGL)
*   This is is the low level cimgui backend for raylib, a higher level API that
*    matches the raylib API can be found in rlcimgui.h
*
*   Migrated to C and cimgui by Alfred Reinold Baudisch from the original "rlImGui" C++
*   implementation Copyright (c) 2024 Jeffery Myers (LICENSE: ZLIB).
*
**********************************************************************************************/

// You can use unmodified imgui_impl_* files in your project. See examples/ folder for examples of using this.
// Prefer including the entire imgui/ repository into your project (either as a copy or as a submodule), and only build the backends you need.
// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp
// - cimgui: https://github.com/cimgui/cimgui

#ifndef IMGUI_DISABLE
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui.h"

typedef struct ImDrawData ImDrawData;
CIMGUI_API bool ImGui_ImplRaylib_Init(void);
CIMGUI_API void ImGui_ImplRaylib_BuildFontAtlas(void);
CIMGUI_API void ImGui_ImplRaylib_Shutdown(void);
CIMGUI_API void ImGui_ImplRaylib_NewFrame(void);
CIMGUI_API void ImGui_ImplRaylib_RenderDrawData(ImDrawData* draw_data);
CIMGUI_API bool ImGui_ImplRaylib_ProcessEvents(void);

#endif // #ifndef IMGUI_DISABLE
