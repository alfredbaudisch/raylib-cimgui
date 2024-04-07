/**********************************************************************************************
*
*   rlcimgui - raylib dear ImGui cimgui C bindings integration
*
*   Migrated to C and cimgui by Alfred Reinold Baudisch from the original "rlImGui" C++
*   implementation Copyright (c) 2024 Jeffery Myers (LICENSE: ZLIB).
*
**********************************************************************************************/

#include "raylib.h"

#ifndef NO_FONT_AWESOME
    #include "extras/FA6FreeSolidFontData.h"
    #include "extras/IconsFontAwesome6.h"
    #define FONT_AWESOME_ICON_SIZE 13
#endif

#ifdef __cplusplus
extern "C" {
#endif

// High level API. This API is designed in the style of raylib and meant to work with reaylib code.
// It will manage it's own ImGui context and call common ImGui functions (like NewFrame and Render) for you
// for a lower level API that matches the other ImGui platforms, please see imgui_impl_raylib.h

/**
 * Helper to load and add Font Awesome to ImGui as a merged font.
 */
void rligSetupFontAwesome(void);

/**
 * Sets up ImGui, loads fonts and themes
 * Calls ImGui_ImplRaylib_Init and sets the theme. Will install Font awesome by default
 *
 * @param darkTheme when true(default) the dark theme is used, when false the light theme is used
 */
void rligSetup(bool darkTheme);

/**
 * Starts a new ImGui Frame
 * Calls ImGui_ImplRaylib_NewFrame, ImGui_ImplRaylib_ProcessEvents, and ImGui::NewFrame together
 */
void rligBegin(void);

/**
 * Ends an ImGui frame and submits all ImGui drawing to raylib for processing.
 * Calls ImGui:Render, an d ImGui_ImplRaylib_RenderDrawData to draw to the current raylib render target
 */
void rligEnd(void);

/**
 * Cleanup ImGui and unload font atlas
 * Calls ImGui_ImplRaylib_Shutdown
 */
void rligShutdown(void);

// Advanced StartupAPI

/**
 * Custom initialization. Not needed if you call rlImGuiSetup. Only needed if you want to add custom setup code.
 * must be followed by rlImGuiEndInitImGui
 * Called by ImGui_ImplRaylib_Init, and does the first part of setup, before fonts are rendered
 */
void rligBeginInitImGui(void);

/**
 * End Custom initialization. Not needed if you call rlImGuiSetup. Only needed if you want to add custom setup code.
 * must be proceeded by rlImGuiBeginInitImGui
 * Called by ImGui_ImplRaylib_Init and does the second part of setup, and renders fonts.
 */
void rligEndInitImGui(void);

/**
 * Forces the font texture atlas to be recomputed and re-cached
 */
void rligReloadFonts(void);

// Advanced Update API

/**
 * Starts a new ImGui Frame with a specified delta time
 *
 * @param dt delta time, any value < 0 will use raylib GetFrameTime
 */
void rligBeginDelta(float deltaTime);

/**
 * Draw a portion texture as an image in an ImGui Context at a defined size
 * Uses the current ImGui Cursor position and the specified size
 * The image will be scaled up or down to fit as needed
 *
 * @param image The raylib texture to draw
 * @param destWidth The width of the drawn image
 * @param destHeight The height of the drawn image
 * @param sourceRect The portion of the texture to draw as an image. Negative values for the width and height will flip the image
 */
void rligImageRect(const Texture* image, int destWidth, int destHeight, Rectangle sourceRect);

/**
 * Draws a render texture as an image an ImGui Context, automatically flipping the Y axis so it will show correctly on screen
 *
 * @param image The render texture to draw
 */
void rligImageRenderTexture(const RenderTexture* image);

/**
 * Draws a render texture as an image an ImGui Context, automatically flipping the Y axis so it will show correctly on screen
 * Fits the render texture to the available content area
 *
 * @param image The render texture to draw
 * @param center When true the image will be centered in the content area
 */
void rligImageRenderTextureFit(const RenderTexture* image, bool center);

/**
 * Draws a texture as an image button in an ImGui context. Uses the current ImGui cursor position and the full size of the texture
 *
 * @param name The display name and ImGui ID for the button
 * @param image The texture to draw
 * @return True if the button was clicked
 */
bool rligImageButton(const char* name, const Texture* image);

#ifdef __cplusplus
}
#endif
