#include "imgui_impl_raylib.h"
#include "rlcimgui.h"
#include "rlgl.h"
#include <string.h>
#include <math.h>

#define FLT_MAX 3.402823466e+38F

#define SET_MOUSE_EVENT(io, rayMouse, imGuiMouse) \
    if (IsMouseButtonPressed(rayMouse)) \
        ImGuiIO_AddMouseButtonEvent(io, imGuiMouse, true); \
    else if (IsMouseButtonReleased(rayMouse)) \
        ImGuiIO_AddMouseButtonEvent(io, imGuiMouse, false)

static ImGuiMouseCursor CurrentMouseCursor = ImGuiMouseCursor_COUNT;
ImGuiContext* GlobalContext = NULL;
static bool LastFrameFocused = false;
static bool LastControlPressed = false;
static bool LastShiftPressed = false;
static bool LastAltPressed = false;
static bool LastSuperPressed = false;

bool rligIsControlDown() { 
    return IsKeyDown(KEY_RIGHT_CONTROL) || IsKeyDown(KEY_LEFT_CONTROL);
}

bool rligIsShiftDown() { 
    return IsKeyDown(KEY_RIGHT_SHIFT) || IsKeyDown(KEY_LEFT_SHIFT);
}

bool rligIsAltDown() {
    return IsKeyDown(KEY_RIGHT_ALT) || IsKeyDown(KEY_LEFT_ALT);
}

bool rligIsSuperDown() {
    return IsKeyDown(KEY_RIGHT_SUPER) || IsKeyDown(KEY_LEFT_SUPER);
}

void ReloadFonts(void)
{
	ImGuiIO* io = igGetIO();
	unsigned char* pixels = NULL;

	int width;
	int height;
    ImFontAtlas_GetTexDataAsRGBA32(io->Fonts, &pixels, &width, &height, NULL);

	Image image = GenImageColor(width, height, BLANK);
	memcpy(image.data, pixels, width * height * 4);

	Texture2D* fontTexture = (Texture2D*)io->Fonts->TexID;
	if (fontTexture && fontTexture->id != 0)
	{
		UnloadTexture(*fontTexture);
		MemFree(fontTexture);
	}

	fontTexture = (Texture2D*)MemAlloc(sizeof(Texture2D));
	*fontTexture = LoadTextureFromImage(image);
	UnloadImage(image);
	io->Fonts->TexID = fontTexture;
}

static const KeyboardKey RaylibKeys[] = {
    KEY_APOSTROPHE,
    KEY_COMMA,
    KEY_MINUS,
    KEY_PERIOD,
    KEY_SLASH,
    KEY_ZERO,
    KEY_ONE,
    KEY_TWO,
    KEY_THREE,
    KEY_FOUR,
    KEY_FIVE,
    KEY_SIX,
    KEY_SEVEN,
    KEY_EIGHT,
    KEY_NINE,
    KEY_SEMICOLON,
    KEY_EQUAL,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_SPACE,
    KEY_ESCAPE,
    KEY_ENTER,
    KEY_TAB,
    KEY_BACKSPACE,
    KEY_INSERT,
    KEY_DELETE,
    KEY_RIGHT,
    KEY_LEFT,
    KEY_DOWN,
    KEY_UP,
    KEY_PAGE_UP,
    KEY_PAGE_DOWN,
    KEY_HOME,
    KEY_END,
    KEY_CAPS_LOCK,
    KEY_SCROLL_LOCK,
    KEY_NUM_LOCK,
    KEY_PRINT_SCREEN,
    KEY_PAUSE,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    KEY_LEFT_SHIFT,
    KEY_LEFT_CONTROL,
    KEY_LEFT_ALT,
    KEY_LEFT_SUPER,
    KEY_RIGHT_SHIFT,
    KEY_RIGHT_CONTROL,
    KEY_RIGHT_ALT,
    KEY_RIGHT_SUPER,
    KEY_KB_MENU,
    KEY_LEFT_BRACKET,
    KEY_BACKSLASH,
    KEY_RIGHT_BRACKET,
    KEY_GRAVE,
    KEY_KP_0,
    KEY_KP_1,
    KEY_KP_2,
    KEY_KP_3,
    KEY_KP_4,
    KEY_KP_5,
    KEY_KP_6,
    KEY_KP_7,
    KEY_KP_8,
    KEY_KP_9,
    KEY_KP_DECIMAL,
    KEY_KP_DIVIDE,
    KEY_KP_MULTIPLY,
    KEY_KP_SUBTRACT,
    KEY_KP_ADD,
    KEY_KP_ENTER,
    KEY_KP_EQUAL
};
static int RaylibKeyCount = sizeof(RaylibKeys)/sizeof(RaylibKeys[0]);

static ImGuiKey RaylibKeyToImGui(KeyboardKey key)
{
    switch(key) {
    case KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
    case KEY_COMMA: return ImGuiKey_Comma;
    case KEY_MINUS: return ImGuiKey_Minus;
    case KEY_PERIOD: return ImGuiKey_Period;
    case KEY_SLASH: return ImGuiKey_Slash;
    case KEY_ZERO: return ImGuiKey_0;
    case KEY_ONE: return ImGuiKey_1;
    case KEY_TWO: return ImGuiKey_2;
    case KEY_THREE: return ImGuiKey_3;
    case KEY_FOUR: return ImGuiKey_4;
    case KEY_FIVE: return ImGuiKey_5;
    case KEY_SIX: return ImGuiKey_6;
    case KEY_SEVEN: return ImGuiKey_7;
    case KEY_EIGHT: return ImGuiKey_8;
    case KEY_NINE: return ImGuiKey_9;
    case KEY_SEMICOLON: return ImGuiKey_Semicolon;
    case KEY_EQUAL: return ImGuiKey_Equal;
    case KEY_A: return ImGuiKey_A;
    case KEY_B: return ImGuiKey_B;
    case KEY_C: return ImGuiKey_C;
    case KEY_D: return ImGuiKey_D;
    case KEY_E: return ImGuiKey_E;
    case KEY_F: return ImGuiKey_F;
    case KEY_G: return ImGuiKey_G;
    case KEY_H: return ImGuiKey_H;
    case KEY_I: return ImGuiKey_I;
    case KEY_J: return ImGuiKey_J;
    case KEY_K: return ImGuiKey_K;
    case KEY_L: return ImGuiKey_L;
    case KEY_M: return ImGuiKey_M;
    case KEY_N: return ImGuiKey_N;
    case KEY_O: return ImGuiKey_O;
    case KEY_P: return ImGuiKey_P;
    case KEY_Q: return ImGuiKey_Q;
    case KEY_R: return ImGuiKey_R;
    case KEY_S: return ImGuiKey_S;
    case KEY_T: return ImGuiKey_T;
    case KEY_U: return ImGuiKey_U;
    case KEY_V: return ImGuiKey_V;
    case KEY_W: return ImGuiKey_W;
    case KEY_X: return ImGuiKey_X;
    case KEY_Y: return ImGuiKey_Y;
    case KEY_Z: return ImGuiKey_Z;
    case KEY_SPACE: return ImGuiKey_Space;
    case KEY_ESCAPE: return ImGuiKey_Escape;
    case KEY_ENTER: return ImGuiKey_Enter;
    case KEY_TAB: return ImGuiKey_Tab;
    case KEY_BACKSPACE: return ImGuiKey_Backspace;
    case KEY_INSERT: return ImGuiKey_Insert;
    case KEY_DELETE: return ImGuiKey_Delete;
    case KEY_RIGHT: return ImGuiKey_RightArrow;
    case KEY_LEFT: return ImGuiKey_LeftArrow;
    case KEY_DOWN: return ImGuiKey_DownArrow;
    case KEY_UP: return ImGuiKey_UpArrow;
    case KEY_PAGE_UP: return ImGuiKey_PageUp;
    case KEY_PAGE_DOWN: return ImGuiKey_PageDown;
    case KEY_HOME: return ImGuiKey_Home;
    case KEY_END: return ImGuiKey_End;
    case KEY_CAPS_LOCK: return ImGuiKey_CapsLock;
    case KEY_SCROLL_LOCK: return ImGuiKey_ScrollLock;
    case KEY_NUM_LOCK: return ImGuiKey_NumLock;
    case KEY_PRINT_SCREEN: return ImGuiKey_PrintScreen;
    case KEY_PAUSE: return ImGuiKey_Pause;
    case KEY_F1: return ImGuiKey_F1;
    case KEY_F2: return ImGuiKey_F2;
    case KEY_F3: return ImGuiKey_F3;
    case KEY_F4: return ImGuiKey_F4;
    case KEY_F5: return ImGuiKey_F5;
    case KEY_F6: return ImGuiKey_F6;
    case KEY_F7: return ImGuiKey_F7;
    case KEY_F8: return ImGuiKey_F8;
    case KEY_F9: return ImGuiKey_F9;
    case KEY_F10: return ImGuiKey_F10;
    case KEY_F11: return ImGuiKey_F11;
    case KEY_F12: return ImGuiKey_F12;
    case KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
    case KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
    case KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
    case KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
    case KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
    case KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
    case KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
    case KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
    case KEY_KB_MENU: return ImGuiKey_Menu;
    case KEY_LEFT_BRACKET: return ImGuiKey_LeftBracket;
    case KEY_BACKSLASH: return ImGuiKey_Backslash;
    case KEY_RIGHT_BRACKET: return ImGuiKey_RightBracket;
    case KEY_GRAVE: return ImGuiKey_GraveAccent;
    case KEY_KP_0: return ImGuiKey_Keypad0;
    case KEY_KP_1: return ImGuiKey_Keypad1;
    case KEY_KP_2: return ImGuiKey_Keypad2;
    case KEY_KP_3: return ImGuiKey_Keypad3;
    case KEY_KP_4: return ImGuiKey_Keypad4;
    case KEY_KP_5: return ImGuiKey_Keypad5;
    case KEY_KP_6: return ImGuiKey_Keypad6;
    case KEY_KP_7: return ImGuiKey_Keypad7;
    case KEY_KP_8: return ImGuiKey_Keypad8;
    case KEY_KP_9: return ImGuiKey_Keypad9;
    case KEY_KP_DECIMAL: return ImGuiKey_KeypadDecimal;
    case KEY_KP_DIVIDE: return ImGuiKey_KeypadDivide;
    case KEY_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
    case KEY_KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
    case KEY_KP_ADD: return ImGuiKey_KeypadAdd;
    case KEY_KP_ENTER: return ImGuiKey_KeypadEnter;
    case KEY_KP_EQUAL: return ImGuiKey_KeypadEqual;
    default: return ImGuiKey_None;
    };
}

static MouseCursor ImGuiCursorToRaylib(ImGuiMouseCursor cursor)
{
    switch(cursor) {
    case ImGuiMouseCursor_Arrow: return MOUSE_CURSOR_ARROW;
    case ImGuiMouseCursor_TextInput: return MOUSE_CURSOR_IBEAM;
    case ImGuiMouseCursor_Hand: return MOUSE_CURSOR_POINTING_HAND;
    case ImGuiMouseCursor_ResizeAll: return MOUSE_CURSOR_RESIZE_ALL;
    case ImGuiMouseCursor_ResizeEW: return MOUSE_CURSOR_RESIZE_EW;
    case ImGuiMouseCursor_ResizeNESW: return MOUSE_CURSOR_RESIZE_NESW;
    case ImGuiMouseCursor_ResizeNS: return MOUSE_CURSOR_RESIZE_NS;
    case ImGuiMouseCursor_ResizeNWSE: return MOUSE_CURSOR_RESIZE_NWSE;
    case ImGuiMouseCursor_NotAllowed: return MOUSE_CURSOR_NOT_ALLOWED;
    default: return MOUSE_CURSOR_DEFAULT;
    };
}

static const char* GetClipTextCallback(void*) 
{
    return GetClipboardText();
}

static void SetClipTextCallback(void*, const char* text)
{
    SetClipboardText(text);
}

void HandleGamepadButtonEvent(ImGuiIO* io, GamepadButton button, ImGuiKey key)
{
    if (IsGamepadButtonPressed(0, button))
        ImGuiIO_AddKeyEvent(io, key, true);
    else if (IsGamepadButtonReleased(0, button))
        ImGuiIO_AddKeyEvent(io, key, false);
}

void HandleGamepadStickEvent(ImGuiIO* io, GamepadAxis axis, ImGuiKey negKey, ImGuiKey posKey)
{
    static float deadZone = 0.20f;

    float axisValue = GetGamepadAxisMovement(0, axis);

    ImGuiIO_AddKeyAnalogEvent(io, negKey, axisValue < -deadZone, axisValue < -deadZone ? -axisValue : 0);
    ImGuiIO_AddKeyAnalogEvent(io, posKey, axisValue > deadZone, axisValue > deadZone ? axisValue : 0);
}

static void ImGuiNewFrame(float deltaTime)
{
    ImGuiIO* io = igGetIO();

    if (IsWindowFullscreen())
    {
        int monitor = GetCurrentMonitor();
        io->DisplaySize.x = (float)GetMonitorWidth(monitor);
        io->DisplaySize.y = (float)GetMonitorHeight(monitor);
    }
    else
    {
        io->DisplaySize.x = (float)GetScreenWidth();
        io->DisplaySize.y = (float)GetScreenHeight();
    }
    
    Vector2 resolutionScale = GetWindowScaleDPI();
    
#if !defined(__APPLE__)
    if (!IsWindowState(FLAG_WINDOW_HIGHDPI))
        resolutionScale = (Vector2){ 1.0f, 1.0f };
#endif

    io->DisplayFramebufferScale = (ImVec2){resolutionScale.x, resolutionScale.y};

    io->DeltaTime = deltaTime;

    if (io->WantSetMousePos)
    {
        SetMousePosition((int)io->MousePos.x, (int)io->MousePos.y);
    }
    else
    {
        ImGuiIO_AddMousePosEvent(io, (float)GetMouseX(), (float)GetMouseY());
    }

    SET_MOUSE_EVENT(io, MOUSE_BUTTON_LEFT, ImGuiMouseButton_Left);
    SET_MOUSE_EVENT(io, MOUSE_BUTTON_RIGHT, ImGuiMouseButton_Right);
    SET_MOUSE_EVENT(io, MOUSE_BUTTON_MIDDLE, ImGuiMouseButton_Middle);
    SET_MOUSE_EVENT(io, MOUSE_BUTTON_FORWARD, ImGuiMouseButton_Middle+1);
    SET_MOUSE_EVENT(io, MOUSE_BUTTON_BACK, ImGuiMouseButton_Middle+2);

    {
        Vector2 mouseWheel = GetMouseWheelMoveV();
        ImGuiIO_AddMouseWheelEvent(io, mouseWheel.x, mouseWheel.y);
    }

    if ((io->ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) == 0)
    {
        ImGuiMouseCursor imgui_cursor = igGetMouseCursor();
        if (imgui_cursor != CurrentMouseCursor || io->MouseDrawCursor)
        {
            CurrentMouseCursor = imgui_cursor;
            if (io->MouseDrawCursor || imgui_cursor == ImGuiMouseCursor_None)
            {
                HideCursor();
            }
            else
            {
                ShowCursor();

                if (!(io->ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange))
                {
                    SetMouseCursor((imgui_cursor > -1 && imgui_cursor < ImGuiMouseCursor_COUNT) ? ImGuiCursorToRaylib(imgui_cursor) : MOUSE_CURSOR_DEFAULT);
                }
            }
        }
    }
}

static void ImGuiTriangleVert(ImDrawVert* idx_vert)
{
    Color* c;
    c = (Color*)&idx_vert->col;
    rlColor4ub(c->r, c->g, c->b, c->a);
    rlTexCoord2f(idx_vert->uv.x, idx_vert->uv.y);
    rlVertex2f(idx_vert->pos.x, idx_vert->pos.y);
}

static void ImGuiRenderTriangles(unsigned int count, int indexStart, const ImVector_ImDrawIdx* indexBuffer, const ImVector_ImDrawVert* vertBuffer, void* texturePtr)
{
    if (count < 3)
        return;

    Texture* texture = (Texture*)texturePtr;

    unsigned int textureId = (texture == NULL) ? 0 : texture->id;

    rlBegin(RL_TRIANGLES);
    rlSetTexture(textureId);

    for (unsigned int i = 0; i <= (count - 3); i += 3)
    {
        ImDrawIdx indexA = indexBuffer->Data[indexStart + i];
        ImDrawIdx indexB = indexBuffer->Data[indexStart + i + 1];
        ImDrawIdx indexC = indexBuffer->Data[indexStart + i + 2];

        ImDrawVert vertexA = vertBuffer->Data[indexA];
        ImDrawVert vertexB = vertBuffer->Data[indexB];
        ImDrawVert vertexC = vertBuffer->Data[indexC];

        ImGuiTriangleVert(&vertexA);
        ImGuiTriangleVert(&vertexB);
        ImGuiTriangleVert(&vertexC);
    }
    rlEnd();
}

static void EnableScissor(float x, float y, float width, float height)
{
    rlEnableScissorTest();
    ImGuiIO* io = igGetIO();

    ImVec2 scale = io->DisplayFramebufferScale;
#if !defined(__APPLE__)
    if (!IsWindowState(FLAG_WINDOW_HIGHDPI))
    {
        scale.x = 1;
        scale.y = 1;
    }
#endif

    rlScissor((int)(x * scale.x),
        (int)((io->DisplaySize.y - (int)(y + height)) * scale.y),
        (int)(width * scale.x),
        (int)(height * scale.y));
}

static void SetupGlobals(void)
{
	LastFrameFocused = IsWindowFocused();
	LastControlPressed = false;
	LastShiftPressed = false;
	LastAltPressed = false;
	LastSuperPressed = false;
}

void SetupBackend(void)
{
    ImGuiIO* io = igGetIO();
	io->BackendPlatformName = "imgui_impl_raylib";

	io->BackendFlags |= ImGuiBackendFlags_HasMouseCursors | ImGuiBackendFlags_HasGamepad | ImGuiBackendFlags_HasSetMousePos;

	io->MousePos = (ImVec2){0, 0};

	io->SetClipboardTextFn = SetClipTextCallback;
	io->GetClipboardTextFn = GetClipTextCallback;

	io->ClipboardUserData = NULL;
}

void rligSetupFontAwesome(void)
{
#ifndef NO_FONT_AWESOME
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig icons_config = {
        .FontDataOwnedByAtlas = true,
        .SizePixels = FONT_AWESOME_ICON_SIZE,
        .OversampleH = 1,
        .OversampleV = 1,
        .PixelSnapH = true,
        .GlyphMinAdvanceX = 1.0f,
        .GlyphMaxAdvanceX = FLT_MAX,
        .MergeMode = true,
        .RasterizerMultiply = 1.0f,
        .RasterizerDensity = 1.0f
    };

    ImGuiIO* io = igGetIO();
    ImFontAtlas_AddFontFromMemoryCompressedTTF(io->Fonts, (void*)fa_solid_900_compressed_data, fa_solid_900_compressed_size, FONT_AWESOME_ICON_SIZE, &icons_config, icons_ranges); // FA 6
#endif
}

void rligEndInitImGui(void)
{
    igSetCurrentContext(GlobalContext);

    rligSetupFontAwesome();

    SetupBackend();

    ReloadFonts();
}

void rligBeginInitImGui(void)
{
    SetupGlobals();
    if (GlobalContext == NULL) {
        GlobalContext = igCreateContext(NULL);
    }

	ImGuiIO* io = igGetIO();
    ImFontAtlas_AddFontDefault(io->Fonts, NULL);
}

void rligSetup(bool dark)
{
    rligBeginInitImGui();

    if (dark)
        igStyleColorsDark(NULL);
    else
        igStyleColorsDark(NULL);

    rligEndInitImGui();
}

void rligBeginDelta(float deltaTime)
{
    igSetCurrentContext(GlobalContext);
	ImGuiNewFrame(deltaTime);
    ImGui_ImplRaylib_ProcessEvents();
	igNewFrame();
}

void rligReloadFonts(void)
{
    igSetCurrentContext(GlobalContext);

    ReloadFonts();
}

void rligBegin(void)
{
    igSetCurrentContext(GlobalContext);
    rligBeginDelta(GetFrameTime());
}

void rligEnd(void)
{
    igSetCurrentContext(GlobalContext);
    igRender();
    ImGui_ImplRaylib_RenderDrawData(igGetDrawData());
}

void rligShutdown(void)
{
    if (GlobalContext != NULL) {
        return;
    }

	igSetCurrentContext(GlobalContext);
    ImGui_ImplRaylib_Shutdown();

    igDestroyContext(GlobalContext);
    GlobalContext = NULL;
}

// raw ImGui backend API
bool ImGui_ImplRaylib_Init(void)
{
    SetupGlobals();

    SetupBackend();

    return true;
}

void ImGui_ImplRaylib_BuildFontAtlas(void)
{
    ReloadFonts();
}

void ImGui_ImplRaylib_Shutdown()
{
    ImGuiIO* io = igGetIO();
    Texture2D* fontTexture = (Texture2D*)io->Fonts->TexID;

    if (fontTexture)
    {
        UnloadTexture(*fontTexture);
        MemFree(fontTexture);
    }

    io->Fonts->TexID = 0;
}

void ImGui_ImplRaylib_NewFrame(void)
{
    ImGuiNewFrame(GetFrameTime());
}

void ImGui_ImplRaylib_RenderDrawData(ImDrawData* draw_data)
{
	rlDrawRenderBatchActive();
	rlDisableBackfaceCulling();

	for (int l = 0; l < draw_data->CmdListsCount; ++l)
	{
		const ImDrawList* commandList = draw_data->CmdLists.Data[l];

        for (int ci = 0; ci < commandList->CmdBuffer.Size; ++ci)
        {
            const ImDrawCmd cmd = commandList->CmdBuffer.Data[ci];

			EnableScissor(cmd.ClipRect.x - draw_data->DisplayPos.x, cmd.ClipRect.y - draw_data->DisplayPos.y, cmd.ClipRect.z - (cmd.ClipRect.x - draw_data->DisplayPos.x), cmd.ClipRect.w - (cmd.ClipRect.y - draw_data->DisplayPos.y));
			if (cmd.UserCallback != NULL)
			{
				cmd.UserCallback(commandList, &cmd);
				continue;
			}

			ImGuiRenderTriangles(cmd.ElemCount, cmd.IdxOffset, &commandList->IdxBuffer, &commandList->VtxBuffer, cmd.TextureId);
			rlDrawRenderBatchActive();
		}
	}

	rlSetTexture(0);
	rlDisableScissorTest();
	rlEnableBackfaceCulling();
}

bool ImGui_ImplRaylib_ProcessEvents(void)
{
	ImGuiIO* io = igGetIO();

	bool focused = IsWindowFocused();
	if (focused != LastFrameFocused)
		ImGuiIO_AddFocusEvent(io, focused);
	LastFrameFocused = focused;

	// handle the modifyer key events so that shortcuts work
	bool ctrlDown = rligIsControlDown();
	if (ctrlDown != LastControlPressed)
		ImGuiIO_AddKeyEvent(io, ImGuiMod_Ctrl, ctrlDown);
	LastControlPressed = ctrlDown;

	bool shiftDown = rligIsShiftDown();
	if (shiftDown != LastShiftPressed)
		ImGuiIO_AddKeyEvent(io, ImGuiMod_Shift, shiftDown);
	LastShiftPressed = shiftDown;

	bool altDown = rligIsAltDown();
	if (altDown != LastAltPressed)
		ImGuiIO_AddKeyEvent(io, ImGuiMod_Alt, altDown);
	LastAltPressed = altDown;

	bool superDown = rligIsSuperDown();
	if (superDown != LastSuperPressed)
		ImGuiIO_AddKeyEvent(io, ImGuiMod_Super, superDown);
	LastSuperPressed = superDown;

	// get the pressed keys, they are in event order
	int keyId = GetKeyPressed();
	while (keyId != 0)
	{
        ImGuiKey key = RaylibKeyToImGui(keyId);
        if(key != ImGuiKey_None) {
			ImGuiIO_AddKeyEvent(io, key, true);
        }
		keyId = GetKeyPressed();
	}

	// look for any keys that were down last frame and see if they were down and are released
	for (int ki = 0; ki < RaylibKeyCount; ++ki)
	{
		if (IsKeyReleased(RaylibKeys[ki]))
			ImGuiIO_AddKeyEvent(io, RaylibKeyToImGui(RaylibKeys[ki]), false);
	}

	// add the text input in order
	unsigned int pressed = GetCharPressed();
	while (pressed != 0)
	{
		ImGuiIO_AddInputCharacter(io, pressed);
		pressed = GetCharPressed();
	}

    if (io->ConfigFlags & ImGuiConfigFlags_NavEnableGamepad && IsGamepadAvailable(0))
    {
        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_LEFT_FACE_UP, ImGuiKey_GamepadDpadUp);
        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_LEFT_FACE_RIGHT, ImGuiKey_GamepadDpadRight);
        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_LEFT_FACE_DOWN, ImGuiKey_GamepadDpadDown);
        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_LEFT_FACE_LEFT, ImGuiKey_GamepadDpadLeft);

        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_RIGHT_FACE_UP, ImGuiKey_GamepadFaceUp);
        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT, ImGuiKey_GamepadFaceLeft);
        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_RIGHT_FACE_DOWN, ImGuiKey_GamepadFaceDown);
        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_RIGHT_FACE_LEFT, ImGuiKey_GamepadFaceRight);

        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_LEFT_TRIGGER_1, ImGuiKey_GamepadL1);
        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_LEFT_TRIGGER_2, ImGuiKey_GamepadL2);
        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_RIGHT_TRIGGER_1, ImGuiKey_GamepadR1);
        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_RIGHT_TRIGGER_2, ImGuiKey_GamepadR2);
        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_LEFT_THUMB, ImGuiKey_GamepadL3);
        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_RIGHT_THUMB, ImGuiKey_GamepadR3);

        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_MIDDLE_LEFT, ImGuiKey_GamepadStart);
        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_MIDDLE_RIGHT, ImGuiKey_GamepadBack);

        // left stick
        HandleGamepadStickEvent(io, GAMEPAD_AXIS_LEFT_X, ImGuiKey_GamepadLStickLeft, ImGuiKey_GamepadLStickRight);
        HandleGamepadStickEvent(io, GAMEPAD_AXIS_LEFT_Y, ImGuiKey_GamepadLStickUp, ImGuiKey_GamepadLStickDown);

        // right stick
        HandleGamepadStickEvent(io, GAMEPAD_AXIS_RIGHT_X, ImGuiKey_GamepadRStickLeft, ImGuiKey_GamepadRStickRight);
        HandleGamepadStickEvent(io, GAMEPAD_AXIS_RIGHT_Y, ImGuiKey_GamepadRStickUp, ImGuiKey_GamepadRStickDown);
    }

    return true;
}

void rligImageRect(const Texture* image, int destWidth, int destHeight, Rectangle sourceRect)
{
	if (!image)
		return;
	
    if (GlobalContext)
		igSetCurrentContext(GlobalContext);
    
    ImVec2 uv0;
    ImVec2 uv1;

    if (sourceRect.width < 0)
    {
        uv0.x = -((float)sourceRect.x / image->width);
        uv1.x = (uv0.x - (float)(fabs(sourceRect.width) / image->width));
    }
    else
    {
        uv0.x = (float)sourceRect.x / image->width;
        uv1.x = uv0.x + (float)(sourceRect.width / image->width);
    }

    if (sourceRect.height < 0)
    {
        uv0.y = -((float)sourceRect.y / image->height);
        uv1.y = (uv0.y - (float)(fabs(sourceRect.height) / image->height));
    }
    else
    {
        uv0.y = (float)sourceRect.y / image->height;
        uv1.y = uv0.y + (float)(sourceRect.height / image->height);
    }

    ImVec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
    igImage((ImTextureID)image, (ImVec2){(float)destWidth, (float)destHeight}, uv0, uv1, color, color);
}

void rligImageRenderTexture(const RenderTexture* image)
{
    if (!image)
        return;
    
    if (GlobalContext)
        igSetCurrentContext(GlobalContext);
    
    rligImageRect(&image->texture, image->texture.width, image->texture.height, (Rectangle){ 0,0, (float)(image->texture.width), -(float)(image->texture.height) });
}

void rligImageRenderTextureFit(const RenderTexture* image, bool center)
{
	if (!image)
		return;
	
    if (GlobalContext)
		igSetCurrentContext(GlobalContext);

    ImVec2 area;
    igGetContentRegionAvail(&area);

    float scale =  area.x / image->texture.width;

    float y = image->texture.height * scale;
    if (y > area.y)
    {
        scale = area.y / image->texture.height;
    }

    int sizeX = (int)(image->texture.width * scale);
    int sizeY = (int)(image->texture.height * scale);

    if (center)
    {
        igSetCursorPosX(0);
        igSetCursorPosX(area.x/2 - sizeX/2);
        igSetCursorPosY(igGetCursorPosY() + (area.y / 2 - sizeY / 2));
    }

    rligImageRect(&image->texture, sizeX, sizeY, (Rectangle){ 0,0, (float)(image->texture.width), -(float)(image->texture.height) });
}

bool rligImageButton(const char* name, const Texture* image)
{
	if (!image)
		return false;
	
    if (GlobalContext)
		igSetCurrentContext(GlobalContext);
    
    ImVec2 uv = {1.0f, 1.0f};
    ImVec4 color = {1.0f, 1.0f, 1.0f, 1.0f};

    return igImageButton(name, (ImTextureID)image, (ImVec2){(float)(image->width), (float)(image->height)}, uv, uv, color, color);
}
