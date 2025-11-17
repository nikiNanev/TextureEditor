#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

#include "imfilebrowser.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <iostream>

#include "Modules/Textures/Exporter.h"
#include "Modules/Textures/Loader.h"
#include "EditorState.h"


SDL_FRect src_texture = {
    .x = 0.0f,
    .y = 0.0f,
    .w = 0.0f,
    .h = 0.0f};

SDL_FRect dst_texture = {
    .x = 400.0f,
    .y = 200.0f,
    .w = 400.0f,
    .h = 400.0f};

const char *path;
bool is_exported = false;
float alpha = 0.0f;
bool done_message = false;
bool is_texture_dragging = false;
float drag_offset_x = 0.0f, drag_offset_y = 0.0f;

// Message export
bool is_showed = false;

bool is_dragging(SDL_FRect *dst_rect, const float *mouse_x, const float *mouse_y, float *drag_offset_x, float *drag_offset_y)
{
    // Check if mouse click collides with the texture's dst_rect
    if (*mouse_x >= dst_rect->x && *mouse_x < dst_rect->x + dst_rect->w &&
        *mouse_y >= dst_rect->y && *mouse_y < dst_rect->y + dst_rect->h)
    {
        // Calculate offset to keep the drag relative to click position
        *drag_offset_x = *mouse_x - dst_rect->x;
        *drag_offset_y = *mouse_y - dst_rect->y;
        return true;
    }
    return false;
}

// Timing is everything
float lastTime = 0.0f, currentTime = 0.0f;
int seconds = 0;

// Main code
int main(int, char **)
{
    // Setup SDL
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "Error: SDL_Init(): " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create window with SDL_Renderer graphics context
    SDL_Window *window = SDL_CreateWindow("Texture Editor v0.0.1",
                                          1280, 720,
                                          SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        std::cout << "Error: SDL_CreateWindow(): " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer)
    {
        std::cout << "Error: SDL_CreateRenderer(): " << SDL_GetError() << std::endl;
        return 1;
    }

    int width, height;
    SDL_GetWindowSize(window, &width, &height);

    int init_window_flags = 0;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Dear ImGui style appearance
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    // Load Fonts
    ImGuiIO &io_fonts = ImGui::GetIO();
    io_fonts.Fonts->AddFontDefault();

    init_window_flags |= ImGuiWindowFlags_NoMove;
    init_window_flags |= ImGuiWindowFlags_NoCollapse;
    init_window_flags |= ImGuiWindowFlags_NoResize;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //Editor State init
    editor_state editor_state;

    // File Dialog for textures
    ImGui::FileBrowser fileDialog;

    // (optional) set browser properties
    fileDialog.SetTitle("Choose Texture");
    fileDialog.SetTypeFilters({".png", ".jpeg", ".jpg", ".bmp"});

    // Load Modules

    Loader loader;

    // Textures
    Exporter exporter("../assets/exported.png");

    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)

                done = true;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
                done = true;

            switch (event.type)
            {
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    float mouse_x = event.button.x;
                    float mouse_y = event.button.y;
                    is_texture_dragging = is_dragging(&dst_texture, &mouse_x, &mouse_y, &drag_offset_x, &drag_offset_y);
                }
                break;

            case SDL_EVENT_MOUSE_MOTION:
                if (is_texture_dragging)
                {
                    // Update dst_rect position based on mouse motion
                    dst_texture.x = event.motion.x - drag_offset_x;
                    dst_texture.y = event.motion.y - drag_offset_y;
                }
                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    is_texture_dragging = false; // Release the texture
                }
                break;
            }
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        if (ImGui::BeginMainMenuBar())
        {

            if (ImGui::BeginMenu("File"))
            {

                if (ImGui::MenuItem("Open"))
                {
                    fileDialog.Open();
                }

                if (ImGui::MenuItem("Save"))
                {

                    editor_state.f_opt.save_default = true;

                    is_exported = exporter.toPNG(loader.get_texture(), loader.get_surface());
                }

                if (ImGui::MenuItem("Close"))
                {
                    done = true;
                }

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "Ctrl+Z"))
                {
                }
                if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false))
                {
                } // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "Ctrl+X"))
                {
                }
                if (ImGui::MenuItem("Copy", "Ctrl+C"))
                {
                }
                if (ImGui::MenuItem("Paste", "Ctrl+V"))
                {
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Image"))
            {
                if (ImGui::MenuItem("Blur"))
                {
                }

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        fileDialog.Display();

        if (fileDialog.HasSelected())
        {

            for (auto selected : fileDialog.GetMultiSelected())
            {
                path = selected.c_str();
                loader.texture_load(path, renderer, &src_texture);
            }

            fileDialog.ClearSelected();
        }

        if (is_exported)
        {

            if (!done_message)
            {
                int width, height;
                SDL_GetWindowSize(window, &width, &height);
                const ImVec2 position(300.0f, 20.0f);
                const ImVec2 size(160.0f, 10.0f);

                const ImVec4 green = {0.0f, 180.0f, 0.0f, 255.0f};

                init_window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration;

                currentTime = SDL_GetTicks();

                if (alpha >= 1.0f)
                {
                    alpha = 1.0f;

                    is_showed = true;
                }
                else
                {
                    if (!is_showed)
                    {
                        if (currentTime > lastTime + 30.0f)
                        {
                            alpha += 0.05f;
                            lastTime = currentTime;
                        }
                    }
                }

                if (is_showed)
                {

                    if (currentTime > lastTime + 1000.0f)
                    {
                        seconds++;
                        lastTime = currentTime;
                    }

                    if (seconds == 2)
                    {
                        if (alpha <= 0.0f)
                        {
                            alpha = 0.0f;
                            done_message = true;
                        }
                        else
                        {
                            if (currentTime > lastTime + 30.0f)
                            {
                                // Fade Out
                                alpha -= 0.05f;
                                lastTime = currentTime;
                            }
                        }
                    }
                }
                ImGui::SetWindowPos(position);
                ImGui::SetWindowSize(size);
                ImGui::SetNextWindowBgAlpha(alpha);
                ImGui::Begin("[INFO]", NULL, init_window_flags);

                ImGui::TextColored(green, "Success exported png!");

                ImGui::End();
            }
        }
        // Rendering
        ImGui::Render();

        ImGui::EndFrame();

        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

        if (loader.get_texture())
        {
            SDL_RenderTexture(renderer, loader.get_texture(), &src_texture, &dst_texture);
        }

        SDL_RenderPresent(renderer);
    }

    // Cleanup
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    loader.cleanup();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}