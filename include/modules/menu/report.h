typedef struct _menu_report
{

    void display(editor_state &editor_vstate)
    {
        if (ImGui::BeginMenu("Report"))
        {

            if (ImGui::MenuItem("Add Report"))
            {
                editor_vstate.report_state.init = true;
            }

            ImGui::EndMenu();
        }
    }

    void report(editor_state &editor_vstate, message_state &message_vstate)
    {

        if (editor_vstate.report_state.init)
        {
            ImGui::OpenPopup("Report", ImGuiPopupFlags_AnyPopup);

            if (ImGui::BeginPopupModal("Report", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Creating a report ...");
                ImGui::Separator();

                ImGui::BulletText("Category: ");
                const char *items[] = {"Issues", "Wishlist", "Feedback"};
                static int item = 0;
                ImGui::Combo("Category", &item, items, IM_ARRAYSIZE(items));
                static char text[1024 * 16] =
                    {
                        "Write your report here ...\n"};

                ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;

                ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags);

                ImGui::BulletText("More info: ");
                ImGui::SameLine(0, 0);
                ImGui::TextLinkOpenURL("https://github.com/nikiNanev/TextureEditor.git");

                if (ImGui::Button("Report", ImVec2(120, 0)))
                {
                    // To Do (Receiving the report on email, site, database, something )

                    message_vstate.init = true;
                    message_vstate.message = " The report was created!";

                    editor_vstate.export_state.open_modal = false;
                    editor_vstate.is_processing = false;
                    editor_vstate.report_state.init = false;

                    ImGui::CloseCurrentPopup();
                }

                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();

                if (ImGui::Button("Cancel", ImVec2(120, 0)))
                {
                    editor_vstate.report_state.init = false;
                    editor_vstate.is_processing = false;
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }
        }
    }

} menu_report;