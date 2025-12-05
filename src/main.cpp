#include "States/Memento/Caretaker.h"

#include "Messaging/Messenger.h"
#include "Styles/Background.h"

#include "Menu/Menu.h"

#include "Controls/Controls.h"

#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_PATCH 0

#include "Initializers.h"

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

    // Controls
    Mouse mouse;
    mouse_controls mouse_vcontrols;
    bool left_cntrl_holded{false};

    // Main Menu items
    menu_file menu_file;
    menu_edit menu_edit;
    menu_image menu_image;
    menu_settings menu_settings;
    menu_report menu_report;
    menu_about menu_about;
    menu_help menu_help;

    bool done{false};

    // Control objects
    rotations rotations;
    flip flip;
    center center;

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

            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.key == SDLK_ESCAPE)
                {
                    done = true;
                }

                if (event.key.key == SDLK_LCTRL)
                {
                    left_cntrl_holded = true;
                }

                // Controls

                if (loader.is_texture)
                {
                    // Rotations
                    rotations.controls(event, angle, factor_angle, add_wait_seconds, message_vstate);
                    flip.controls(event, message_vstate, add_wait_seconds);
                    center.controls(event, message_vstate, &center.point, loader.texture, &sdl_vstate);
                }

                if (event.key.key == SDLK_Z && left_cntrl_holded)
                {
                    caretaker->undo(&loader);
                }
            }

            mouse.dragging(&event, sdl_vstate, &mouse_vcontrols, center.rect, center.point, editor_vstate.is_processing);
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        if (ImGui::BeginMainMenuBar())
        {

            menu_file.display(fileDialog, loader, exporter, message_vstate, editor_vstate, done);
            menu_edit.display(caretaker, loader, editor_vstate);
            menu_image.display(caretaker, originator, editor_vstate);
            menu_settings.display(caretaker, editor_vstate);
            menu_report.display(editor_vstate);
            menu_about.display(editor_vstate);
            menu_help.display(editor_vstate);

            ImGui::EndMainMenuBar();
        }

        fileDialog.Display();

        if (fileDialog.HasSelected())
        {
            loader.texture_load(fileDialog.GetSelected().c_str(), sdl_vstate.renderer, &sdl_vstate.src);

            //Init parameters to the texture
            center.init(&sdl_vstate);

            fileDialog.ClearSelected();
        }

        // Messaging
        if (message_vstate.init)
        {
            Messenger message;
            message.display(&sdl_vstate, &imgui_vstate, &message_vstate);
        }

        // Filtering
        menu_image.filter_blur(editor_vstate, loader, caretaker, originator, message_vstate, sdl_vstate);
        menu_image.edge_enhancement(editor_vstate, loader, caretaker, originator, message_vstate, sdl_vstate);
        menu_image.brightness_adjustment(editor_vstate, loader, caretaker, originator, message_vstate, sdl_vstate);

        // Editing ( Resize )
        menu_edit.resize(editor_vstate, loader, caretaker, originator, message_vstate, sdl_vstate);
        menu_edit.scale(editor_vstate, loader, message_vstate);

        // Styles ( Themes )
        menu_settings.themes(editor_vstate, caretaker, originator, message_vstate);
        menu_settings.backgrounds(editor_vstate, caretaker, originator, message_vstate, imgui_vstate);

        // Adding Report
        menu_report.report(editor_vstate, message_vstate);

        // Info ( Author and Repo )
        menu_about.info(editor_vstate);

        // Exporting ...
        menu_file.exporting(editor_vstate, loader, caretaker, originator, message_vstate, exporter);

        // Rendering
        ImGui::Render();

        ImGui::EndFrame();

        SDL_SetRenderDrawColor(sdl_vstate.renderer, (Uint8)(imgui_vstate.clear_color.x * 255), (Uint8)(imgui_vstate.clear_color.y * 255), (Uint8)(imgui_vstate.clear_color.z * 255), (Uint8)(imgui_vstate.clear_color.w * 255));
        SDL_RenderClear(sdl_vstate.renderer);

        if (loader.texture)
        {
            // SDL_RenderTexture(sdl_vstate.renderer, loader.texture, &sdl_vstate.src, &sdl_vstate.dst);
            SDL_RenderTextureRotated(sdl_vstate.renderer, loader.texture, &sdl_vstate.src, &sdl_vstate.dst, angle, &center.point, flip.flag);
            SDL_SetRenderDrawColor(sdl_vstate.renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(sdl_vstate.renderer, &center.rect);            
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