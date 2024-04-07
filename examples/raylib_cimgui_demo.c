/*******************************************************************************************
*
*   raylib cimgui example
*
*   Migrated to raylib cimgui by Alfred Reinold Baudisch
*   from the official samples from dear imgui.
*
*   Heavily inspired by the original "rlImGui" C++
*   implementation Copyright (c) 2024 Jeffery Myers (LICENSE: ZLIB).
*
********************************************************************************************/

#include "raylib.h"
#include "../imgui_impl_raylib.h"
#include "../rlcimgui.h"
#include <stdlib.h>
#include <math.h>

// Main code
int main(int, char**)
{
    // Setup raylib window
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
    InitWindow(1280, 720, "raylib CIMGUI PURE C");
    SetTargetFPS(144);
 
    // Setup Dear ImGui context
    igCreateContext(NULL);
    ImGuiIO *ioptr = igGetIO();
    ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    
    // set docking
#ifdef IMGUI_HAS_DOCK
    ioptr->ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
    ioptr->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows
#endif
    
    // Setup Dear ImGui style
    igStyleColorsDark(NULL);

    // Setup Platform/Renderer backends
    ImGui_ImplRaylib_Init();

    ImFontAtlas_AddFontDefault(ioptr->Fonts, NULL);
    rligSetupFontAwesome();

    // required to be called to cache the font texture with raylib
    ImGui_ImplRaylib_BuildFontAtlas();
   
    // Our state
    bool showDemoWindow = true;
    bool showAnotherWindow = false;
    ImVec4 clearColor = {0.45f, 0.55f, 0.60f, 1.00f};

    bool myToolActive = false;

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        ImGui_ImplRaylib_ProcessEvents();

        // Start the Dear ImGui frame
        ImGui_ImplRaylib_NewFrame();
        igNewFrame();

        // 1. Show the big demo window (Most of the sample code is in igShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (showDemoWindow)
            igShowDemoWindow(&showDemoWindow);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            igBegin("Hello, world!", NULL, 0); // Create a window called "Hello, world!" and append into it.

            igText("This is some useful text."); // Display some text (you can use a format strings too)
            igCheckbox("Demo Window", &showDemoWindow); // Edit bools storing our window open/close state
            igCheckbox("Another Window", &showAnotherWindow);

            igSliderFloat("Float", &f, 0.0f, 1.0f, "%.3f", 0); // Edit 1 float using a slider from 0.0f to 1.0f
            igColorEdit3("clear color", (float*)&clearColor, 0); // Edit 3 floats representing a color

            ImVec2 buttonSize;
            buttonSize.x = 0;
            buttonSize.y = 0;
            if (igButton("Button", buttonSize))  // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            igSameLine(0.0f, -1.0f);
            igText("counter = %d", counter);

            igText("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / igGetIO()->Framerate, igGetIO()->Framerate);
            igEnd();
        }

        // 3. Show another simple window.
        if (showAnotherWindow)
        {
            igBegin("Another Window", &showAnotherWindow, 0);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            igText("Hello from another window!");
            ImVec2 buttonSize;
            buttonSize.x = 0;
            buttonSize.y = 0;
            if (igButton("Close me", buttonSize)) {
                showAnotherWindow = false;
            }
            igEnd();
        }

        // Create a window called "My First Tool", with a menu bar.
        igBegin("My First Tool", &myToolActive, ImGuiWindowFlags_MenuBar);
        if (igBeginMenuBar())
        {
            if (igBeginMenu("File", true))
            {
                if (igMenuItem_Bool("Open..", "Ctrl+Q", false, true)) {  }
                if (igMenuItem_Bool("Save", "Ctrl+S", false, false)) { /* Do stuff */ }
                if (igMenuItem_Bool("Close", "Ctrl+W", false, true)) { 
                    myToolActive = false;
                }
                igEndMenu();
            }
            igEndMenuBar();
        }

        // Edit a color stored as 4 floats
        igColorEdit4("Color", (float*)&clearColor, 0);

        // Mix icon with text
        igText("%s among %d items", ICON_FA_BOOK, 10);

        igSetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_C);
        if(igButton(ICON_FA_MUSIC " Search", (ImVec2){0, 0})) {
            printf("Music button!");
        }

        if(igButton("Add Bunnies", (ImVec2){0, 0})) {
           printf("Bunnies!");
        }

        // Generate samples and plot them
        float samples[100];
        for (int n = 0; n < 100; n++)
            samples[n] = sinf(n * 0.2f + igGetTime() * 1.5f);
        igPlotLines_FloatPtr("Samples", samples, 100, 10, "Overlay", 1.0, 2.0, (ImVec2){100, 100}, 0);

        // Display contents in a scrolling region
        igTextColored((ImVec4){1,1,0,1}, "Important Stuff");
        igBeginChild_Str("Scrolling", (ImVec2){200, 200}, 0, 0);
        for (int n = 0; n < 50; n++)
            igText("%04d: Some text", n);
        igEndChild();
        igEnd();

        // Rendering
        igRender();
        BeginDrawing();
        ClearBackground((Color){ (unsigned char)(clearColor.x * 255), (unsigned char)(clearColor.y * 255),(unsigned char)(clearColor.z * 255),(unsigned char)(clearColor.w * 255) });
        ImGui_ImplRaylib_RenderDrawData(igGetDrawData());
        EndDrawing();

        done = WindowShouldClose();
    }

    // Cleanup
    ImGui_ImplRaylib_Shutdown();
    igDestroyContext(NULL);

    CloseWindow();

    return 0;
}
