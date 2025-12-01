typedef struct _menu_edit
{

    void display(Caretaker *caretaker, Loader &loader, editor_state &editor_vstate)
    {
        if (ImGui::BeginMenu("Edit"))
        {

            if (ImGui::MenuItem("Undo", "Ctrl+Z"))
            {
                caretaker->undo(&loader);
            }
            if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false))
            {
            }
            ImGui::Separator();

            if (ImGui::MenuItem("Resize"))
            {
                editor_vstate.edit.is_resize = true;
            }

            if (ImGui::MenuItem("Scale"))
            {
                editor_vstate.edit.is_scale = true;
            }

            ImGui::EndMenu();
        }
    }

    void resize(editor_state &editor_vstate, Loader &loader, Caretaker *caretaker, Originator *originator, message_state &message_vstate, sdl_state &sdl_vstate)
    {
        if (editor_vstate.edit.is_resize && loader.is_texture)
        {
            Resize resize;
            editor_vstate.is_processing = true;

            ImGui::OpenPopup("Resize");

            if (ImGui::BeginPopupModal("Resize", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Resizing of the main texture");
                ImGui::Separator();

                static float width{0.0f}, height{0.0f};

                ImGui::SliderFloat("Width", &width, 0.0f, 2500.0f);
                ImGui::SliderFloat("Height", &height, 0.0f, 2500.0f);

                if (ImGui::Button("Ok", ImVec2(60, 0)))
                {
                    editor_vstate.edit.is_resize = false;
                    editor_vstate.is_processing = false;
                    caretaker->backup();
                    originator->save_snapshot(loader.texture, loader.filename_path);

                    // Apply the Resize
                    resize.apply(&loader, &sdl_vstate, width, height);
                    message_vstate.init = true;
                    message_vstate.message = " Applied & Exported! ( Resized )";

                    ImGui::CloseCurrentPopup();
                }

                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();

                if (ImGui::Button("Cancel", ImVec2(120, 0)))
                {
                    editor_vstate.edit.is_resize = false;
                    editor_vstate.is_processing = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
    }

    void scale(editor_state &editor_vstate, Loader &loader, message_state &message_vstate)
    {
        if (editor_vstate.edit.is_scale && loader.is_texture)
        {
            editor_vstate.is_processing = true;

            ImGui::OpenPopup("Scaling");

            if (ImGui::BeginPopupModal("Scaling", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Scaling of the main texture");
                ImGui::Separator();

                static float scale{1.0f};

                ImGui::SliderFloat("Scale", &scale, 1.0f, 10.0f);

                if (ImGui::Button("Ok", ImVec2(60, 0)))
                {
                    editor_vstate.edit.is_scale = false;
                    editor_vstate.is_processing = false;

                    // Apply the Scaling
                    editor_vstate.edit.scale = scale;

                    message_vstate.init = true;
                    message_vstate.message = " Applied! ( Scaled )";

                    ImGui::CloseCurrentPopup();
                }

                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();

                if (ImGui::Button("Cancel", ImVec2(120, 0)))
                {
                    editor_vstate.edit.is_scale = false;
                    editor_vstate.is_processing = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
    }

} menu_edit;