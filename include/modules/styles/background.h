#include "imgui.h"

// Backgrounds
// 6 Warm background themes
enum class WarmBackgrounds : int
{
    LatteCream = 0,   // Soft creamy beige
    Caramel = 1,      // Warm caramel brown
    SunsetOrange = 2, // Soft sunset glow
    PumpkinSpice = 3, // Cozy autumn orange
    Cinnamon = 4,     // Rich cinnamon red-brown
    HoneyGold = 5,    // Warm golden honey

    Count = 6
};

// 6 Hand-picked warm ImVec4 background colors (all slightly desaturated for comfort)
static const ImVec4 WarmBackgroundColors[(int)WarmBackgrounds::Count] = {
    ImVec4(0.98f, 0.95f, 0.90f, 1.00f), // LatteCream     - #FAF3E6
    ImVec4(0.76f, 0.60f, 0.42f, 1.00f), // Caramel        - #C29A6B
    ImVec4(0.95f, 0.70f, 0.50f, 1.00f), // SunsetOrange   - #F2B384
    ImVec4(0.85f, 0.55f, 0.35f, 1.00f), // PumpkinSpice   - #D98C59
    ImVec4(0.70f, 0.40f, 0.30f, 1.00f), // Cinnamon       - #B3664D
    ImVec4(0.95f, 0.75f, 0.45f, 1.00f), // HoneyGold      - #F2BF74
};

// Pretty names for the combo/menu
static const char *WarmBackgroundNames[(int)WarmBackgrounds::Count] = {
    "Latte Cream",
    "Caramel",
    "Sunset Orange",
    "Pumpkin Spice",
    "Cinnamon",
    "Honey Gold"};

// Current selected theme (persist this if you want)
static WarmBackgrounds currentWarmBackground = WarmBackgrounds::LatteCream;

// Get color by enum
ImVec4 GetWarmBackgroundColor(WarmBackgrounds back)
{
    return WarmBackgroundColors[(int)back];
}

// Enum for the 5 background themes
enum class BgTheme : int
{
    Dark = 0,
    Classic = 1,
    Light = 2,
    Cherry = 3,
    Ocean = 4,

    Count = 5
};

// Array of background colors (you can tweak these to your liking)
static const ImVec4 BackgroundColors[(int)BgTheme::Count] = {
    ImVec4(0.10f, 0.10f, 0.10f, 1.00f), // Dark
    ImVec4(0.06f, 0.06f, 0.10f, 1.00f), // Classic ImGui dark
    ImVec4(0.95f, 0.75f, 0.96f, 1.00f), // Light
    ImVec4(0.20f, 0.05f, 0.10f, 1.00f), // Cherry / Deep red-ish
    ImVec4(0.00f, 0.18f, 0.24f, 1.00f), // Ocean blue
};

// Names for the combo box
static const char *BackgroundThemeNames[(int)BgTheme::Count] = {
    "Dark",
    "Classic",
    "Light",
    "Cherry",
    "Ocean"};

// Global or static variable to store current selection
static BgTheme currentBgTheme = BgTheme::Dark;

// Helper function to get color by theme
ImVec4 GetBackgroundColor(BgTheme theme)
{
    return BackgroundColors[(int)theme];
}

// Optional: direct index access (safer than raw array)
ImVec4 GetBackgroundColorByIndex(int index)
{
    if (index < 0 || index >= (int)BgTheme::Count)
        return BackgroundColors[0];
    return BackgroundColors[index];
}

// Function to apply the selected background color to ImGui style
void ApplyBackgroundColor()
{
    ImGuiStyle &style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg] = GetBackgroundColor(currentBgTheme);
    // Optional: also change other related colors for better contrast
    style.Colors[ImGuiCol_ChildBg] = style.Colors[ImGuiCol_WindowBg];
    style.Colors[ImGuiCol_PopupBg] = ImVec4(style.Colors[ImGuiCol_WindowBg].x * 0.9f,
                                            style.Colors[ImGuiCol_WindowBg].y * 0.9f,
                                            style.Colors[ImGuiCol_WindowBg].z * 0.9f, 1.0f);
}