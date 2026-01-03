typedef struct menu_file
{

    void display(ImGui::FileBrowser &fileDialog, loader &loader, exporter &exporter, message_state &message_vstate, editor_state &editor_vstate, bool &done)
    {
        if (ImGui::BeginMenu("File"))
        {

            if (ImGui::MenuItem("New"))
            {
                editor_vstate.new_file.create = true;
            }

            if (ImGui::MenuItem("Open"))
            {
                fileDialog.Open();
            }

            if (ImGui::MenuItem("Save", "Default ( PNG )"))
            {

                if (loader.is_texture)
                {
                    message_vstate.init = exporter.toPNG(loader.filename_path);
                    message_vstate.message = "Successfully saved image!";
                }
            }

            if (ImGui::MenuItem("Export"))
            {
                if (loader.is_texture)
                    editor_vstate.export_state.open_modal = true;
            }

            if (ImGui::MenuItem("Close"))
            {
                done = true;
            }

            ImGui::EndMenu();
        }
    }

    void exporting(editor_state &editor_vstate, loader &loader, Caretaker *caretaker, Originator *originator, message_state &message_vstate, exporter &exporter)
    {
        if (editor_vstate.export_state.open_modal && loader.is_texture)
        {
            editor_vstate.is_processing = true;

            ImGui::OpenPopup("Export");

            if (ImGui::BeginPopupModal("Export", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Select your preferred format");
                ImGui::Separator();

                static int format_idx{0};

                ImGui::Combo("Let's doeit", &format_idx, "PNG\0JPEG\0BMP\0\0");

                if (ImGui::Button("Export", ImVec2(120, 0)))
                {
                    exporter.apply(format_idx, &loader, editor_vstate.edit.scale);

                    editor_vstate.export_state.open_modal = false;
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
                    editor_vstate.export_state.open_modal = false;
                    editor_vstate.is_processing = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
    }

    void create_new(editor_state &editor_vstate, loader &loader, Caretaker *caretaker, Originator *originator, message_state &message_vstate, sdl_state &sdl_vstate)
    {
        if (editor_vstate.new_file.create)
        {
            editor_vstate.is_processing = true;

            ImGui::OpenPopup("New File");

            if (ImGui::BeginPopupModal("New File", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Create New Texture/Image");
                ImGui::Separator();

                static int size[2] = {512, 512};
                static color color;
                static float fill[3] = {1.0f, 1.0f, 1.0f};
                ImGui::SliderInt2("Size", size, 0, 2400);

                static char filename[64] = {"new_texture"};

                ImGui::InputText("Filename", filename, sizeof(filename));
                ImGui::ColorPicker3("Fill", fill);

                if (ImGui::Button("Create", ImVec2(120, 0)))
                {
                    editor_vstate.is_processing = false;

                    message_vstate.init = true; // Success on export
                    message_vstate.message = "Successful create!";

                    caretaker->backup();
                    originator->save_action("new image/texture");



                    color.r = (unsigned char)(fill[0] * 255);
                    color.g = (unsigned char)(fill[1] * 255);
                    color.b = (unsigned char)(fill[2] * 255);

                    for (int i = 0; i < 3; i++)
                    {
                        std::cout << "value: " << (fill[i]) << std::endl;
                    }

                    std::cout << "Colors: \n\n red: " << (int)color.r << " green: " << (int)color.g << " blue: " << (int)color.b << "\n\n";  

                    std::cout << filename << std::endl;
                    std::cout << "size[0]: " << size[0] << " size[1]: " << size[1] << std::endl;

                    std::string file(filename);

                    loader.texture = loader.create_texture(color, file, size[0], size[1], sdl_vstate);

                    editor_vstate.new_file.create = false;
                    editor_vstate.is_processing = false;

                    ImGui::CloseCurrentPopup();
                }

                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();

                if (ImGui::Button("Cancel", ImVec2(120, 0)))
                {
                    editor_vstate.new_file.create = false;
                    editor_vstate.is_processing = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
    }

} menu_file;