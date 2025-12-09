typedef struct _menu_image
{
    void display(Caretaker *caretaker, Originator *originator, editor_state &editor_vstate)
    {
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

            if (ImGui::MenuItem("Brightness Adjustment"))
            {
                editor_vstate.filter.brightness_adjustment = true;

                caretaker->backup();
                originator->save_action("Brightness Adjustment");
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Binary Thresholds"))
            {
                editor_vstate.filter.binary_thresholds = true;
            }

            ImGui::SeparatorText("Stats");

            if (ImGui::MenuItem("General Info"))
            {
                editor_vstate.image_state.general_info = true;
            }

            ImGui::EndMenu();
        }
    }

    void filter_blur(editor_state &editor_vstate, loader &loader, Caretaker *caretaker, Originator *originator, message_state &message_vstate, sdl_state &sdl_vstate)
    {
        if (editor_vstate.filter.blur && loader.is_texture)
        {
            blur blur;
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
                    originator->save_snapshot(loader.texture, loader.filename_path);

                    if (blur.load(loader.filename_path, loader))
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
    }

    void edge_enhancement(editor_state &editor_vstate, loader &loader, Caretaker *caretaker, Originator *originator, message_state &message_vstate, sdl_state &sdl_vstate)
    {
        if (editor_vstate.filter.edge_enhancement && loader.is_texture)
        {
            _edge_enhancement edge_enhance;
            caretaker->backup();
            originator->save_snapshot(loader.texture, loader.filename_path);

            if (edge_enhance.load(loader.filename_path, loader))
            {
                edge_enhance.apply(loader, &sdl_vstate);

                message_vstate.init = true;
                message_vstate.message = "Applied & Exported! ( Edge Enhancement )";

                editor_vstate.filter.edge_enhancement = false;
            }

            editor_vstate.filter.edge_enhancement = false;
        }
    }

    void brightness_adjustment(editor_state &editor_vstate, loader &loader, Caretaker *caretaker, Originator *originator, message_state &message_vstate, sdl_state &sdl_vstate)
    {
        if (editor_vstate.filter.brightness_adjustment && loader.is_texture)
        { // Success on export
            _brightness_adjustment brightness_adjust;

            caretaker->backup(); // Success on export
            originator->save_snapshot(loader.texture, loader.filename_path);

            if (brightness_adjust.load(loader.filename_path, loader))
            {

                float alpha = 2.5;
                int beta = 100;

                brightness_adjust.apply(alpha, beta, loader, &sdl_vstate);

                message_vstate.init = true;
                message_vstate.message = "Applied & Exported! ( Brightness Adjustment )";

                editor_vstate.filter.brightness_adjustment = false;
            }
            editor_vstate.filter.brightness_adjustment = false;
        }
    }

    void binary_thresholds(editor_state &editor_vstate, loader &loader, Caretaker *caretaker, Originator *originator, message_state &message_vstate, sdl_state &sdl_vstate)
    {

        if (editor_vstate.filter.binary_thresholds && loader.is_texture)
        { // Success on export
            _binary_thresholds bt;

            ImGui::OpenPopup("Binary Thresholds");

            if (ImGui::BeginPopupModal("Binary Thresholds", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Binary thresholds");
                ImGui::Separator();

                //Channels
                static float r[2];
                static float g[2];
                static float b[2];

                ImGui::SliderFloat2("Red", r, 0.0f, 255.0f);
                ImGui::SliderFloat2("Green", g, 0.0f, 255.0f);
                ImGui::SliderFloat2("Blue", b, 0.0f, 255.0f);

                if (ImGui::Button("Ok", ImVec2(60, 0)))
                {
                    editor_vstate.filter.binary_thresholds = false;
                    editor_vstate.is_processing = false;

                    caretaker->backup();
                    originator->save_snapshot(loader.texture, loader.filename_path);

                    // apply
                    if(bt.load(loader.filename_path, 4))
                    {
                        bt.apply(r, g, b);
                    }


                    message_vstate.init = true;
                    message_vstate.message = " Applied & Exported! ( Binary Thresholds )";

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
    }

} menu_image;