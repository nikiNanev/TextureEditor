typedef struct _menu_image
{
    // utilities

    static bool compare_colors(ImVec4 &prevColor, ImVec4 color)
    {
        if (prevColor.x == color.x && prevColor.y == color.y && prevColor.z == color.z)
            return true;
        return false;
    }

    void display(Caretaker *caretaker, Originator *originator, editor_state &editor_vstate)
    {
        if (ImGui::BeginMenu("Image"))
        {
            ImGui::SeparatorText("Filters");

            if (ImGui::MenuItem("Borders"))
            {
                editor_vstate.filter.borders = true;

                caretaker->backup();
                originator->save_action("Edge Enhancement filter");
            }

            if (ImGui::MenuItem("Colorize"))
            {
                editor_vstate.filter.colorize = true;

                caretaker->backup();
                originator->save_action("Edge Enhancement filter");
            }

            ImGui::Separator();

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

            if (ImGui::MenuItem("High Pass"))
            {
                editor_vstate.filter.high_pass = true;

                caretaker->backup();
                originator->save_action("High pass filter");
            }

            if (ImGui::MenuItem("Brightness Adjustment"))
            {
                editor_vstate.filter.brightness_adjustment = true;

                caretaker->backup();
                originator->save_action("Brightness Adjustment");
            }

            if (ImGui::MenuItem("Contrast Adjustment"))
            {
                editor_vstate.filter.contrast_adjustment = true;

                caretaker->backup();
                originator->save_action("Contrast Adjustment");
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Binary Thresholds"))
            {
                editor_vstate.filter.binary_thresholds = true;
            }

            if (ImGui::MenuItem("Grayscale"))
            {
                editor_vstate.filter.grayscale = true;
            }

            if (ImGui::MenuItem("Invert"))
            {
                editor_vstate.filter.invert = true;
            }

            if (ImGui::MenuItem("Film Grain"))
            {
                editor_vstate.filter.film_grain = true;
            }

            if (ImGui::MenuItem("Emboss"))
            {
                editor_vstate.filter.emboss = true;
            }

            if (ImGui::MenuItem("Gamma Correction"))
            {
                editor_vstate.filter.gamma_correction = true;
            }

            if (ImGui::MenuItem("Posterization"))
            {
                editor_vstate.filter.posterization = true;
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

    void high_pass(editor_state &editor_vstate, loader &loader, Caretaker *caretaker, Originator *originator, message_state &message_vstate, sdl_state &sdl_vstate)
    {
        if (editor_vstate.filter.high_pass && loader.is_texture)
        {
            _high_pass high_pass;
            caretaker->backup();
            originator->save_snapshot(loader.texture, loader.filename_path);

            if (high_pass.load(loader.filename_path, loader))
            {
                high_pass.apply(loader, &sdl_vstate);

                message_vstate.init = true;
                message_vstate.message = "Applied & Exported! ( Edge high_pass )";

                editor_vstate.filter.high_pass = false;
            }

            editor_vstate.filter.high_pass = false;
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

    void contrast_adjustment(editor_state &editor_vstate, loader &loader, Caretaker *caretaker, Originator *originator, message_state &message_vstate, sdl_state &sdl_vstate)
    {
        if (editor_vstate.filter.contrast_adjustment && loader.is_texture)
        {
            ImGui::OpenPopup("Contrast Adjustment");

            if (ImGui::BeginPopupModal("Contrast Adjustment", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Contrast Adjustment");
                ImGui::Separator();

                _contrast_adjustment contrast_adjustment;

                static float factor{0.0f};

                ImGui::SliderFloat("Contrast Factor", &factor, -255.0f, 255.0f);

                if (ImGui::Button("Ok", ImVec2(60, 0)))
                {

                    if (contrast_adjustment.load(loader.filename_path, loader))
                    {

                        factor = (259.0f * (factor + 255)) / (255.0f * (259 - factor));

                        contrast_adjustment.apply(factor, loader, &sdl_vstate);

                        caretaker->backup(); // Success on export
                        originator->save_snapshot(loader.texture, loader.filename_path);

                        message_vstate.init = true;
                        message_vstate.message = "Applied & Exported! ( Contrast Adjustment )";

                        editor_vstate.filter.contrast_adjustment = false;
                        editor_vstate.is_processing = false;
                    }
                    ImGui::CloseCurrentPopup();
                }

                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();

                if (ImGui::Button("Cancel", ImVec2(120, 0)))
                {
                    editor_vstate.filter.contrast_adjustment = false;
                    editor_vstate.is_processing = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
    }

    void binary_thresholds(editor_state &editor_vstate, loader &loader, Caretaker *caretaker, Originator *originator, message_state &message_vstate, sdl_state &sdl_vstate)
    {

        if (editor_vstate.filter.binary_thresholds && loader.is_texture)
        {

            _binary_thresholds bt;

            ImGui::OpenPopup("Binary Thresholds");

            if (ImGui::BeginPopupModal("Binary Thresholds", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Binary thresholds");
                ImGui::Separator();

                // Channels
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
                    if (bt.load(loader.filename_path, 4))
                    {
                        bt.apply(r, g, b, &loader, sdl_vstate);
                    }

                    message_vstate.init = true;
                    message_vstate.message = " Applied & Exported! ( Binary Thresholds )";

                    ImGui::CloseCurrentPopup();
                }

                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();

                if (ImGui::Button("Cancel", ImVec2(120, 0)))
                {
                    editor_vstate.filter.binary_thresholds = false;
                    editor_vstate.is_processing = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
    }

    void film_grain(editor_state &editor_vstate, loader &loader, Caretaker *caretaker, Originator *originator, message_state &message_vstate, sdl_state &sdl_vstate)
    {
        if (editor_vstate.filter.film_grain && loader.is_texture)
        {
            ImGui::OpenPopup("Film Grain");

            if (ImGui::BeginPopupModal("Film Grain", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Film Grain");
                ImGui::Separator();

                static int strength = 0;

                ImGui::InputInt("Strength: ", &strength, 0, 100);

                if (ImGui::Button("Ok", ImVec2(60, 0)))
                {
                    editor_vstate.filter.film_grain = false;
                    editor_vstate.is_processing = false;

                    caretaker->backup();
                    originator->save_snapshot(loader.texture, loader.filename_path);

                    _film_grain fg;

                    // apply
                    if (fg.load(loader.filename_path, loader))
                    {
                        fg.apply(loader, &sdl_vstate, strength);
                    }

                    message_vstate.init = true;
                    message_vstate.message = " Applied & Exported! ( Film Grain )";

                    ImGui::CloseCurrentPopup();
                }

                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();

                if (ImGui::Button("Cancel", ImVec2(120, 0)))
                {
                    editor_vstate.filter.film_grain = false;
                    editor_vstate.is_processing = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
    }

    void emboss(editor_state &editor_vstate, loader &loader, Caretaker *caretaker, Originator *originator, message_state &message_vstate, sdl_state &sdl_vstate)
    {
        if (editor_vstate.filter.emboss && loader.is_texture)
        {
            _emboss emboss;
            caretaker->backup();
            originator->save_snapshot(loader.texture, loader.filename_path);

            if (emboss.load(loader.filename_path, loader))
            {
                emboss.apply(loader, &sdl_vstate);

                message_vstate.init = true;
                message_vstate.message = "Applied & Exported! ( Emboss )";

                editor_vstate.filter.emboss = false;
            }

            editor_vstate.filter.emboss = false;
        }
    }

    void gamma_correction(editor_state &editor_vstate, loader &loader, Caretaker *caretaker, Originator *originator, message_state &message_vstate, sdl_state &sdl_vstate)
    {
        if (editor_vstate.filter.gamma_correction && loader.is_texture)
        {
            ImGui::OpenPopup("Gamma Correction");

            static int flags = ImGuiWindowFlags_AlwaysAutoResize;

            if (ImGui::BeginPopup("Gamma Correction", flags))
            {
                ImGui::Text("Gamma Correction");
                ImGui::Separator();

                static float gamma = 0;
                static float prevGamma = gamma;
                static bool is_applied = false;

                ImGui::SliderFloat("Gamma", &gamma, 1, 30, "%.3f");

                if (ImGui::Button("Ok", ImVec2(60, 0)))
                {

                    editor_vstate.filter.gamma_correction = false;
                    editor_vstate.is_processing = false;

                    if (prevGamma != gamma)
                        is_applied = false;

                    if (!is_applied)
                    {
                        caretaker->backup();
                        originator->save_snapshot(loader.texture, loader.filename_path);

                        _gamma_correction gc;

                        // apply
                        if (gc.load(loader.filename_path, loader))
                        {
                            gc.apply(loader, &sdl_vstate, gamma);
                        }

                        message_vstate.init = true;
                        message_vstate.message = " Applied & Exported! ( Gamma Correction )";
                        prevGamma = gamma;
                        is_applied = true;
                    }

                    ImGui::CloseCurrentPopup();
                }

                // ImGui::SetItemDefaultFocus();
                ImGui::SameLine();

                if (ImGui::Button("Apply", ImVec2(120, 0)))
                {
                    if (prevGamma != gamma)
                        is_applied = false;

                    if (!is_applied)
                    {
                        caretaker->backup();
                        originator->save_snapshot(loader.texture, loader.filename_path);

                        _gamma_correction gc;

                        // apply
                        if (gc.load(loader.filename_path, loader))
                        {
                            gc.apply(loader, &sdl_vstate, gamma);
                        }

                        message_vstate.init = true;
                        message_vstate.message = " Applied & Exported! ( Gamma Correction )";

                        is_applied = true;
                        prevGamma = gamma;
                    }
                }

                ImGui::SameLine();

                if (ImGui::Button("Cancel", ImVec2(120, 0)))
                {
                    editor_vstate.filter.gamma_correction = false;
                    editor_vstate.is_processing = false;
                    is_applied = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
    }

    void borders(editor_state &editor_vstate, loader &loader, Caretaker *caretaker, Originator *originator, message_state &message_vstate, sdl_state &sdl_vstate)
    {
        if (editor_vstate.filter.borders && loader.is_texture)
        {
            ImGui::OpenPopup("Borders");

            if (ImGui::BeginPopup("Borders", ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Borders");
                ImGui::Separator();

                static ImVec4 color;
                static ImVec4 prevColor = color;
                static int border_width = 0;
                static bool is_applied = false;

                static float selectedColor[3] = {0, 0, 0};

                ImGui::ColorPicker3("Border Color", selectedColor);

                ImGui::SliderInt("Width", &border_width, 1, 300);

                color.x = selectedColor[0];
                color.y = selectedColor[1];
                color.z = selectedColor[2];

                if (ImGui::Button("Ok", ImVec2(60, 0)))
                {
                    editor_vstate.filter.borders = false;
                    editor_vstate.is_processing = false;

                    if (!compare_colors(prevColor, color))
                        is_applied = false;

                    if (!is_applied)
                    {
                        caretaker->backup();
                        originator->save_snapshot(loader.texture, loader.filename_path);

                        _borders b;

                        // apply
                        if (b.load(loader.filename_path, loader))
                        {
                            b.apply(loader, &sdl_vstate, color, border_width);
                        }

                        message_vstate.init = true;
                        message_vstate.message = " Applied & Exported! ( Borders )";
                        prevColor = color;
                        is_applied = false;
                    }

                    ImGui::CloseCurrentPopup();
                }

                // ImGui::SetItemDefaultFocus();
                ImGui::SameLine();

                if (ImGui::Button("Apply", ImVec2(120, 0)))
                {
                    if (compare_colors(prevColor, color))
                        is_applied = false;

                    if (!is_applied)
                    {
                        caretaker->backup();
                        originator->save_snapshot(loader.texture, loader.filename_path);

                        _borders b;

                        // apply
                        if (b.load(loader.filename_path, loader))
                        {
                            b.apply(loader, &sdl_vstate, color, border_width);
                        }

                        message_vstate.init = true;
                        message_vstate.message = " Applied & Exported! ( Borders )";
                    }

                    if (!compare_colors(prevColor, color))
                    {
                        is_applied = true;
                        prevColor = color;
                    }
                }
                ImGui::SameLine();

                if (ImGui::Button("Cancel", ImVec2(120, 0)))
                {
                    editor_vstate.filter.borders = false;
                    editor_vstate.is_processing = false;
                    is_applied = false;
                    ImGui::CloseCurrentPopup();
                }
            }

            ImGui::EndPopup();
        }
    }

    void colorize(editor_state &editor_vstate, loader &loader, Caretaker *caretaker, Originator *originator, message_state &message_vstate, sdl_state &sdl_vstate)
    {
        if (editor_vstate.filter.colorize && loader.is_texture)
        {
            ImGui::OpenPopup("Colorize");

            if (ImGui::BeginPopup("Colorize", ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Colorize");
                ImGui::Separator();

                static ImVec4 red_channel;
                static ImVec4 green_channel;
                static ImVec4 blue_channel;

                static float sum = 0.0f;
                static float prev_sum = sum;
                static bool is_applied = false;

                static float selectedColor_red[3] = {0, 0, 0};
                static float selectedColor_green[3] = {0, 0, 0};
                static float selectedColor_blue[3] = {0, 0, 0};

                ImGui::ColorPicker3("Red Channel", selectedColor_red);
                ImGui::ColorPicker3("Green Channel", selectedColor_green);
                ImGui::ColorPicker3("Blue Channel", selectedColor_blue);

                red_channel.x = selectedColor_red[0];
                red_channel.y = selectedColor_red[1];
                red_channel.z = selectedColor_red[2];

                green_channel.x = selectedColor_green[0];
                green_channel.y = selectedColor_green[1];
                green_channel.z = selectedColor_green[2];

                blue_channel.x = selectedColor_blue[0];
                blue_channel.y = selectedColor_blue[1];
                blue_channel.z = selectedColor_blue[2];

                if (ImGui::Button("Ok", ImVec2(60, 0)))
                {
                    editor_vstate.filter.colorize = false;
                    editor_vstate.is_processing = false;

                    if (prev_sum != sum)
                        is_applied = false;

                    if (!is_applied)
                    {
                        caretaker->backup();
                        originator->save_snapshot(loader.texture, loader.filename_path);

                        _colorize c;

                        c.rgb[0] = (red_channel.x + red_channel.y + red_channel.z) / 3;
                        c.rgb[1] = (green_channel.x + green_channel.y + green_channel.z) / 3;
                        c.rgb[2] = (blue_channel.x + blue_channel.y + blue_channel.z) / 3;

                        sum = c.rgb[0] + c.rgb[1] + c.rgb[2];

                        // apply
                        if (c.load(loader.filename_path, loader))
                        {
                            c.apply(loader, &sdl_vstate);
                        }

                        message_vstate.init = true;
                        message_vstate.message = " Applied & Exported! ( Colorize )";

                        is_applied = false;
                        prev_sum = sum;
                    }

                    ImGui::CloseCurrentPopup();
                }

                // ImGui::SetItemDefaultFocus();
                ImGui::SameLine();

                if (ImGui::Button("Apply", ImVec2(120, 0)))
                {

                    if (prev_sum != sum)
                        is_applied = false;

                    if (!is_applied)
                    {
                        caretaker->backup();
                        originator->save_snapshot(loader.texture, loader.filename_path);

                        _colorize c;

                        c.rgb[0] = (red_channel.x + red_channel.y + red_channel.z) / 3;
                        c.rgb[1] = (green_channel.x + green_channel.y + green_channel.z) / 3;
                        c.rgb[2] = (blue_channel.x + blue_channel.y + blue_channel.z) / 3;

                        sum = c.rgb[0] + c.rgb[1] + c.rgb[2];

                        // apply
                        if (c.load(loader.filename_path, loader))
                        {
                            c.apply(loader, &sdl_vstate);
                        }

                        message_vstate.init = true;
                        message_vstate.message = " Applied & Exported! ( Colorize )";

                        is_applied = false;
                        prev_sum = sum;
                    }
                }
                ImGui::SameLine();

                if (ImGui::Button("Cancel", ImVec2(120, 0)))
                {
                    editor_vstate.filter.colorize = false;
                    editor_vstate.is_processing = false;
                    is_applied = false;
                    ImGui::CloseCurrentPopup();
                }
            }

            ImGui::EndPopup();
        }
    }

    void invert(editor_state &editor_vstate, loader &loader, Caretaker *caretaker, Originator *originator, message_state &message_vstate, sdl_state &sdl_vstate)
    {
        if (editor_vstate.filter.invert && loader.is_texture)
        {
            _invert invert;
            caretaker->backup();
            originator->save_snapshot(loader.texture, loader.filename_path);

            if (invert.load(loader.filename_path, loader))
            {
                invert.apply(loader, &sdl_vstate);

                message_vstate.init = true;
                message_vstate.message = "Applied & Exported! ( Inverted )";

                editor_vstate.filter.invert = false;
            }

            editor_vstate.filter.invert = false;
        }
    }

    void grayscale(editor_state &editor_vstate, loader &loader, Caretaker *caretaker, Originator *originator, message_state &message_vstate, sdl_state &sdl_vstate)
    {
        if (editor_vstate.filter.grayscale && loader.is_texture)
        {
            _grayscale grayscale;
            caretaker->backup();
            originator->save_snapshot(loader.texture, loader.filename_path);

            if (grayscale.load(loader.filename_path, loader))
            {
                grayscale.apply(loader, &sdl_vstate);

                message_vstate.init = true;
                message_vstate.message = "Applied & Exported! ( Grayscale )";

                editor_vstate.filter.grayscale = false;
            }

            editor_vstate.filter.grayscale = false;
        }
    }

    void posterization(editor_state &editor_vstate, loader &loader, Caretaker *caretaker, Originator *originator, message_state &message_vstate, sdl_state &sdl_vstate)
    {
        if (editor_vstate.filter.posterization && loader.is_texture)
        {
            ImGui::OpenPopup("Posterization");

            static int flags = ImGuiWindowFlags_AlwaysAutoResize;

            if (ImGui::BeginPopup("Posterization", flags))
            {
                ImGui::Text("Posterization");
                ImGui::Separator();

                static int levels = 0;
                static int prev_levels = levels;
                static bool is_applied = false;

                ImGui::SliderInt("Levels", &levels, 2, 256);

                if (ImGui::Button("Ok", ImVec2(60, 0)))
                {

                    editor_vstate.filter.posterization = false;
                    editor_vstate.is_processing = false;

                    if (prev_levels != levels)
                        is_applied = false;

                    if (!is_applied)
                    {
                        caretaker->backup();
                        originator->save_snapshot(loader.texture, loader.filename_path);

                        _posterization p;

                        // apply
                        if (p.load(loader.filename_path, loader))
                        {
                            p.apply(levels, loader, &sdl_vstate);
                        }

                        message_vstate.init = true;
                        message_vstate.message = " Applied & Exported! ( Posterization )";
                        prev_levels = levels;
                        is_applied = true;
                    }

                    ImGui::CloseCurrentPopup();
                }

                ImGui::SameLine();

                if (ImGui::Button("Apply", ImVec2(120, 0)))
                {
                    if (prev_levels != levels)
                        is_applied = false;

                    if (!is_applied)
                    {
                        caretaker->backup();
                        originator->save_snapshot(loader.texture, loader.filename_path);

                        _posterization p;

                        // apply
                        if (p.load(loader.filename_path, loader))
                        {
                            p.apply(levels, loader, &sdl_vstate);
                        }

                        message_vstate.init = true;
                        message_vstate.message = " Applied & Exported! ( Posterization )";

                        is_applied = true;
                        prev_levels = levels;
                    }
                }

                ImGui::SameLine();

                if (ImGui::Button("Cancel", ImVec2(120, 0)))
                {
                    editor_vstate.filter.posterization = false;
                    editor_vstate.is_processing = false;
                    is_applied = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
    }

} menu_image;