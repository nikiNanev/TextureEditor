#include "Modules/Textures/Exporter.h"
#include "Modules/Textures/Loader.h"
#include "Modules/Textures/State.h"

#include "Modules/Effects/Blur.h"

#include "EditorState.h"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

#include "imfilebrowser.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <iostream>

bool is_exported = false;
float alpha = 0.0f;
bool done_message = false;
bool is_texture_dragging = false;
bool is_exporting = false;
float drag_offset_x = 0.0f, drag_offset_y = 0.0f;

// Filters
// Gaussian
float sigma{0.0f};
bool is_sigma_set{false};

typedef enum
{
    PNG_FORMAT,
    JPEG_FORMAT,
    BMP_FORMAT,
    WEBP_FORMAT,
    DNG_FORMAT,
} formats;

// Message export
bool is_showed{false};

// Timing is everything
float lastTime = 0.0f, currentTime = 0.0f;
int seconds = 0;

void edit_items()
{
    if (ImGui::MenuItem("Undo", "Ctrl+Z"))
    {
    }
    if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false))
    {
    }
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
}

typedef struct sdl_state
{

    SDL_Window *window;
    SDL_Renderer *renderer;
    int width, height;

} sdl_state;

typedef struct imgui_state
{
    int init_window_flags;
    ImVec4 clear_color;
} imgui_state;

int init_sdl(sdl_state *state)
{
    // Setup SDL
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "Error: SDL_Init(): " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create window with SDL_Renderer graphics context
    state->window = SDL_CreateWindow("Texture Editor v0.0.1",
                                     1280, 720,
                                     SDL_WINDOW_RESIZABLE);
    if (!state->window)
    {
        std::cout << "Error: SDL_CreateWindow(): " << SDL_GetError() << std::endl;
        return 1;
    }

    state->renderer = SDL_CreateRenderer(state->window, NULL);
    if (!state->renderer)
    {
        std::cout << "Error: SDL_CreateRenderer(): " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_GetWindowSize(state->window, &state->width, &state->height);

    return 0;
}

void init_imgui(imgui_state *imgui_vstate, sdl_state *sdl_vstate)
{
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
    ImGui_ImplSDL3_InitForSDLRenderer(sdl_vstate->window, sdl_vstate->renderer);
    ImGui_ImplSDLRenderer3_Init(sdl_vstate->renderer);

    // Load Fonts
    ImGuiIO &io_fonts = ImGui::GetIO();
    io_fonts.Fonts->AddFontDefault();

    imgui_vstate->init_window_flags |= ImGuiWindowFlags_NoMove;
    imgui_vstate->init_window_flags |= ImGuiWindowFlags_NoCollapse;
    imgui_vstate->init_window_flags |= ImGuiWindowFlags_NoResize;

    imgui_vstate->clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
}

void mouse_controls(SDL_Event *event)
{
    switch (event->type)
    {
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            float mouse_x = event->button.x;
            float mouse_y = event->button.y;
            is_texture_dragging = !is_exporting && is_dragging(&dst_texture, &mouse_x, &mouse_y, &drag_offset_x, &drag_offset_y);
        }
        break;

    case SDL_EVENT_MOUSE_MOTION:
        if (is_texture_dragging)
        {
            // Update dst_rect position based on mouse motion
            dst_texture.x = event->motion.x - drag_offset_x;
            dst_texture.y = event->motion.y - drag_offset_y;
        }
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            is_texture_dragging = false; // Release the texture
        }
        break;
    }
}

void dlib_exporter(const int format_idx, Loader *loader)
{
    switch (format_idx)
    {
    case PNG_FORMAT:
    {
        try
        {
            dlib::array2d<dlib::rgb_pixel> image;
            dlib::load_image(image, loader->get_file_path());

            const std::string file_output = "exported_dlib.png";
            dlib::save_png(image, file_output);

            std::cout << "Success export with dlib to png format" << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    break;
    case JPEG_FORMAT:
    {
        try
        {
            dlib::array2d<dlib::rgb_pixel> image;
            dlib::load_image(image, loader->get_file_path());

            const std::string file_output = "exported_dlib.jpeg";
            dlib::save_jpeg(image, file_output);

            std::cout << "Success export with dlib to jpeg format" << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    break;
    case BMP_FORMAT:
    {
        try
        {
            dlib::array2d<dlib::rgb_pixel> image;
            dlib::load_image(image, loader->get_file_path());

            const std::string file_output = "exported_dlib.bmp";
            dlib::save_bmp(image, file_output);

            std::cout << "Success export with dlib to bmp format" << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    break;
    case WEBP_FORMAT:
    {
        try
        {
            dlib::array2d<dlib::rgb_pixel> image;
            dlib::load_image(image, loader->get_file_path());

            const std::string file_output = "exported_dlib.webp";
            dlib::save_webp(image, file_output);

            std::cout << "Success export with dlib to webp format" << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    break;
    case DNG_FORMAT:
    {
        try
        {
            dlib::array2d<dlib::rgb_pixel> image;
            dlib::load_image(image, loader->get_file_path());

            const std::string file_output = "exported_dlib.dng";
            dlib::save_dng(image, file_output);

            std::cout << "Success export with dlib to dng format" << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    break;
    }
}

int main(int, char **)
{
    sdl_state sdl_vstate;
    imgui_state imgui_vstate;

    init_sdl(&sdl_vstate);
    init_imgui(&imgui_vstate, &sdl_vstate);

    // Editor State init
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

    Blur blur;

    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)

                done = true;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(sdl_vstate.window))
                done = true;

            mouse_controls(&event);
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

                if (ImGui::MenuItem("Save", "Default ( PNG )"))
                {

                    editor_state.f_opt.save_default = true;

                    is_exported = exporter.toPNG(loader.get_texture(), loader.get_surface());
                }

                if (ImGui::MenuItem("Export"))
                {
                    if (loader.is_texture_loaded())
                        editor_state.export_st.open_modal = true;
                }

                if (ImGui::MenuItem("Close"))
                {
                    done = true;
                }

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                edit_items();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Image"))
            {
                ImGui::SeparatorText("Filters");
                if (ImGui::MenuItem("Blur"))
                {
                    editor_state.filter.blur = true;
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
                loader.texture_load(selected.c_str(), sdl_vstate.renderer, &src_texture);
            }

            fileDialog.ClearSelected();
        }

        if (is_exported)
        {

            if (!done_message)
            {
                int width, height;
                SDL_GetWindowSize(sdl_vstate.window, &width, &height);
                const ImVec2 position(300.0f, 20.0f);
                const ImVec2 size(160.0f, 10.0f);

                const ImVec4 green = {0.0f, 180.0f, 0.0f, 255.0f};

                imgui_vstate.init_window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration;

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
                ImGui::Begin("[INFO]", NULL, imgui_vstate.init_window_flags);

                ImGui::TextColored(green, "Success exported png!");

                ImGui::End();
            }
        }

        // Filtering
        if (editor_state.filter.blur)
        {
            ImGui::OpenPopup("Blur");

            if (ImGui::BeginPopupModal("Blur", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Select your preferred format");
                ImGui::Separator();

                ImGui::SliderFloat("Sigma value", &sigma, 0.0f, 10.0f);

                if (ImGui::Button("Ok", ImVec2(60, 0)))
                {
                    editor_state.filter.blur = false;

                    if (blur.load(loader.get_file_path()))
                        blur.apply(sigma);

                    std::cout << "applied" << std::endl;

                    ImGui::CloseCurrentPopup();
                }

                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();

                if (ImGui::Button("Cancel", ImVec2(120, 0)))
                {
                    is_sigma_set = false;
                    editor_state.filter.blur = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }

        // Exporting ...
        if (editor_state.export_st.open_modal && loader.is_texture_loaded())
        {
            is_exporting = true;

            ImGui::OpenPopup("Export");

            if (ImGui::BeginPopupModal("Export", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Select your preferred format");
                ImGui::Separator();

                static int format_idx{0};

                ImGui::Combo("Let's doeit", &format_idx, "PNG\0JPEG\0BMP\0WEBP\0DNG\0\0");

                if (ImGui::Button("Export", ImVec2(120, 0)))
                {
                    dlib_exporter(format_idx, &loader);

                    editor_state.export_st.open_modal = false;
                    is_exporting = false;
                    
                    ImGui::CloseCurrentPopup();
                }

                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();

                if (ImGui::Button("Cancel", ImVec2(120, 0)))
                {
                    editor_state.export_st.open_modal = false;
                    is_exporting = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
        // Rendering
        ImGui::Render();

        ImGui::EndFrame();

        SDL_SetRenderDrawColor(sdl_vstate.renderer, (Uint8)(imgui_vstate.clear_color.x * 255), (Uint8)(imgui_vstate.clear_color.y * 255), (Uint8)(imgui_vstate.clear_color.z * 255), (Uint8)(imgui_vstate.clear_color.w * 255));
        SDL_RenderClear(sdl_vstate.renderer);

        if (loader.get_texture())
        {
            SDL_RenderTexture(sdl_vstate.renderer, loader.get_texture(), &src_texture, &dst_texture);
        }

        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), sdl_vstate.renderer);

        SDL_RenderPresent(sdl_vstate.renderer);
    }

    // Cleanup
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    loader.cleanup();
    SDL_DestroyRenderer(sdl_vstate.renderer);
    SDL_DestroyWindow(sdl_vstate.window);
    SDL_Quit();

    return 0;
}