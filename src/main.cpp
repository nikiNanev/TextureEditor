#include "Modules/States/EditorState.h"
#include "Modules/States/Memento/Caretaker.h"

#include "Modules/Controls/Mouse.h"
#include "Modules/Textures/Exporter.h"
#include "Modules/Textures/Loader.h"

#include "Modules/Effects/Filters/Blur.h"
#include "Modules/Effects/Filters/Edge_Enhancement.h"

#include "Modules/Messaging/Messenger.h"

void edit_items(Caretaker *caretaker, Loader *loader)
{
    if (ImGui::MenuItem("Undo", "Ctrl+Z"))
    {
        caretaker->undo(loader);
    }
    if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false))
    {
    }
}

void memento_cleanup(Originator *originator, Caretaker *caretaker)
{

    delete originator;
    delete caretaker;
}

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

int main(int, char **)
{
    sdl_state sdl_vstate = {
        .src = {.x = 0.0f,
                .y = 0.0f,
                .w = 0.0f,
                .h = 0.0f},
        .dst = {.x = 400.0f, .y = 200.0f, .w = 400.0f, .h = 400.0f}};
    imgui_state imgui_vstate;

    init_sdl(&sdl_vstate);
    init_imgui(&imgui_vstate, &sdl_vstate);

    // Message states
    message_state message_vstate = {
        .message = "Successfully saved Image!" // Default
    };

    // Editor State init
    editor_state editor_vstate;

    // Memento
    std::srand(static_cast<unsigned int>(std::time(NULL)));

    Originator *originator = new Originator("Init memento in texture editor");
    Caretaker *caretaker = new Caretaker(originator);

    // File Dialog for textures
    ImGui::FileBrowser fileDialog;

    // (optional) set browser properties
    fileDialog.SetTitle("Choose Texture");
    fileDialog.SetTypeFilters({".png", ".jpeg", ".jpg", ".bmp"});

    // Load Modules
    Loader loader;
    Exporter exporter;

    Mouse mouse;
    mouse_controls mouse_vcontrols;

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

            mouse.dragging(&event, sdl_vstate, &mouse_vcontrols, editor_vstate.is_processing);
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

                    if (loader.is_texture_loaded())
                    {
                        message_vstate.init = exporter.toPNG(loader.get_filename_path());
                        message_vstate.message = "Successfully saved image!";
                    }
                }

                if (ImGui::MenuItem("Export"))
                {
                    if (loader.is_texture_loaded())
                        editor_vstate.export_st.open_modal = true;
                }

                if (ImGui::MenuItem("Close"))
                {
                    done = true;
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit"))
            {
                edit_items(caretaker, &loader);
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Image"))
            {
                ImGui::SeparatorText("Filters");
                if (ImGui::MenuItem("Blur"))
                {
                    editor_vstate.filter.blur = true;

                    caretaker->backup();
                    originator->save_action("Blur filter");
                }

                if (ImGui::MenuItem("Edge Enhancement"))
                {
                    editor_vstate.filter.edge_enhancement = true;

                    caretaker->backup();
                    originator->save_action("Edge Enhancement filter");
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Settings"))
            {

                if (ImGui::MenuItem("show history ( actions )"))
                {
                    caretaker->show_history();
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
                loader.texture_load(selected.c_str(), sdl_vstate.renderer, &sdl_vstate.src);
            }

            fileDialog.ClearSelected();
        }

        // Messaging
        if (message_vstate.init)
        {
            Messenger message;
            message.display(&sdl_vstate, &imgui_vstate, &message_vstate);
        }

        // Filtering

        if (editor_vstate.filter.blur && loader.is_texture_loaded())
        {
            Blur blur;
            editor_vstate.is_processing = true;

            ImGui::OpenPopup("Blur");

            if (ImGui::BeginPopupModal("Blur", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Select your preferred format");
                ImGui::Separator();

                static float sigma{0.0f};

                ImGui::SliderFloat("Sigma value", &sigma, 0.0f, 10.0f);

                if (ImGui::Button("Ok", ImVec2(60, 0)))
                {
                    editor_vstate.filter.blur = false;
                    editor_vstate.is_processing = false;
                    caretaker->backup();
                    originator->save_snapshot(loader.get_texture());

                    if (blur.load(loader.get_filename_path()))
                        blur.apply(sigma, loader, &sdl_vstate);

                    message_vstate.init = true;
                    message_vstate.message = " Applied & Exported! ( Blur )";

                    ImGui::CloseCurrentPopup();
                }

                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();

                if (ImGui::Button("Cancel", ImVec2(120, 0)))
                {
                    editor_vstate.filter.blur = false;
                    editor_vstate.is_processing = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }

        if (editor_vstate.filter.edge_enhancement && loader.is_texture_loaded())
        {
            Edge_Enhancement edge_enhance;
            if (edge_enhance.load(loader.get_filename_path()))
            {
                caretaker->backup();
                originator->save_snapshot(loader.get_texture());

                edge_enhance.apply(loader, &sdl_vstate);

                message_vstate.init = true;
                message_vstate.message = "Applied & Exported! ( Edge Enhancement )";

                editor_vstate.filter.edge_enhancement = false;
            }

            editor_vstate.filter.edge_enhancement = false;
        }

        // Exporting ...
        if (editor_vstate.export_st.open_modal && loader.is_texture_loaded())
        {
            editor_vstate.is_processing = true;

            ImGui::OpenPopup("Export");

            if (ImGui::BeginPopupModal("Export", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Select your preferred format");
                ImGui::Separator();

                static int format_idx{0};

                ImGui::Combo("Let's doeit", &format_idx, "PNG\0JPEG\0BMP\0WEBP\0DNG\0\0");

                if (ImGui::Button("Export", ImVec2(120, 0)))
                {
                    exporter.dlib_exporter(format_idx, &loader);

                    editor_vstate.export_st.open_modal = false;
                    editor_vstate.is_processing = false;

                    message_vstate.init = true; // Success on export
                    message_vstate.message = "Successful export!";

                    caretaker->backup();
                    originator->save_action("Export image");

                    ImGui::CloseCurrentPopup();
                }

                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();

                if (ImGui::Button("Cancel", ImVec2(120, 0)))
                {
                    editor_vstate.export_st.open_modal = false;
                    editor_vstate.is_processing = false;
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
            SDL_RenderTexture(sdl_vstate.renderer, loader.get_texture(), &sdl_vstate.src, &sdl_vstate.dst);
        }

        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), sdl_vstate.renderer);

        SDL_RenderPresent(sdl_vstate.renderer);
    }

    // Cleanup
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    // Memento
    memento_cleanup(originator, caretaker);

    loader.cleanup();
    SDL_DestroyRenderer(sdl_vstate.renderer);
    SDL_DestroyWindow(sdl_vstate.window);
    SDL_Quit();

    return 0;
}