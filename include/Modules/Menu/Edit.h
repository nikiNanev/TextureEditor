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

            ImGui::Separator();

            if (ImGui::MenuItem("Flip"))
            {
                editor_vstate.edit.is_flip = true;
            }

            if (ImGui::MenuItem("Rotate"))
            {
                editor_vstate.edit.is_rotate = true;
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

    void flip(editor_state &editor_vstate, Loader &loader, message_state &message_vstate, SDL_FlipMode *mode)
    {
        if (editor_vstate.edit.is_flip && loader.is_texture)
        {
            editor_vstate.is_processing = true;

            ImGui::OpenPopup("Flip");

            if (ImGui::BeginPopupModal("Flip", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Flipping of the main texture");
                ImGui::Separator();

                static bool is_horizontal{false};
                static bool is_vertical{false};
                static bool is_default{false};

                if (ImGui::Checkbox("Horizontally", &is_horizontal))
                {
                    *mode = (is_horizontal ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
                }

                if (ImGui::Checkbox("Vertically", &is_vertical))
                {
                    *mode = (is_vertical ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE);
                }

                if (ImGui::Checkbox("Default", &is_default))
                {
                    *mode = SDL_FLIP_NONE;
                }

                if (ImGui::Button("Ok", ImVec2(60, 0)))
                {
                    editor_vstate.edit.is_flip = false;
                    editor_vstate.is_processing = false;

                    message_vstate.init = true;
                    message_vstate.message = "Applied!";

                    ImGui::CloseCurrentPopup();
                }

                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();

                if (ImGui::Button("Cancel", ImVec2(120, 0)))
                {
                    editor_vstate.edit.is_flip = false;
                    editor_vstate.is_processing = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
    }

    void rotate(editor_state &editor_vstate, Loader &loader, message_state &message_vstate, center *center, sdl_state &sdl_vstate)
    {
        if (editor_vstate.edit.is_rotate && loader.is_texture)
        {
            editor_vstate.is_processing = true;

            ImGui::OpenPopup("Rotate");

            if (ImGui::BeginPopupModal("Rotate", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Rotating of the main texture");
                ImGui::Separator();

                static float v[2] = {center->point.x, center->point.y};


                std::cout << "floats: (x: " << v[0] << ") (y: " << v[1] << ")" << std::endl;
                std::cout << "floats: (x: " << center->point.x << ") (y: " << center->point.y << ")" << std::endl;

                ImGui::SliderFloat2("Center Point of rotation", v, -400, 400, "%.2f");

                center->point.x = v[0];
                center->point.y = v[1];
                
                center->rect.x = sdl_vstate.dst.x + center->point.x;
                center->rect.y = sdl_vstate.dst.y + center->point.y;

                static float rad{0.0f};

                ImGui::SliderAngle("Angle", &rad);
                float angle = rad * 180 / M_PI;
                center->angle = (double)angle;

                if (ImGui::Button("Ok", ImVec2(60, 0)))
                {
                    editor_vstate.edit.is_rotate = false;
                    editor_vstate.is_processing = false;

                    message_vstate.init = true;
                    message_vstate.message = "Applied!";

                    ImGui::CloseCurrentPopup();
                }

                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();

                if (ImGui::Button("Cancel", ImVec2(120, 0)))
                {
                    editor_vstate.edit.is_rotate = false;
                    editor_vstate.is_processing = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }

    }

} menu_edit;