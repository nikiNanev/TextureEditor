typedef struct _menu_about
{
    void display(editor_state &editor_vstate)
    {
        if (ImGui::BeginMenu("About"))
        {

            // About the Authors/Editors/Feedbackers

            if (ImGui::MenuItem("Info"))
            {
                editor_vstate.info.desc = true;
            }

            ImGui::EndMenu();
        }
    }

    void info(editor_state &editor_vstate)
    {
        if (editor_vstate.info.desc)
        {
            ImGui::OpenPopup("Info", ImGuiPopupFlags_AnyPopup);

            if (ImGui::BeginPopupModal("Info", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::BulletText("Developer & Design by: Niki Nanev");
                ImGui::Separator();

                ImGui::BulletText("Repo: ");
                ImGui::SameLine(0, 0);
                ImGui::TextLinkOpenURL("https://github.com/nikiNanev/TextureEditor.git");

                if (ImGui::Button("Okay", ImVec2(120, 0)))
                {
                    editor_vstate.export_st.open_modal = false;
                    editor_vstate.is_processing = false;
                    editor_vstate.info.desc = false;

                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }
        }
    }

} menu_about;