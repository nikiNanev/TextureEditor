typedef struct menu_file
{

    void display(ImGui::FileBrowser &fileDialog, loader &loader, exporter &exporter, message_state &message_vstate, editor_state &editor_vstate, bool &done)
    {
        if (ImGui::BeginMenu("File"))
        {

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

                ImGui::Combo("Let's doeit", &format_idx, "PNG\0JPEG\0BMP\0WEBP\0DNG\0\0");

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

} menu_file;