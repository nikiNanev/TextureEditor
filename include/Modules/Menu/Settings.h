typedef struct _menu_settings
{
    void display(Caretaker *caretaker, editor_state &editor_vstate)
    {
        if (ImGui::BeginMenu("Settings"))
        {

            if (ImGui::MenuItem("history of actions"))
            {
                caretaker->show_history();
            }

            ImGui::SeparatorText("Styles");

            if (ImGui::MenuItem("Themes"))
            {
                editor_vstate.styles.theme = true;
            }

            if (ImGui::MenuItem("Background"))
            {
                editor_vstate.styles.background = true;
            }

            ImGui::EndMenu();
        }
    }

    void themes(editor_state &editor_vstate, Caretaker *caretaker, Originator *originator, message_state &message_vstate)
    {
        if (editor_vstate.styles.theme)
        {
            editor_vstate.is_processing = true;

            ImGui::OpenPopup("Themes");

            if (ImGui::BeginPopupModal("Themes", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Select your preferred theme");
                ImGui::Separator();
                ImGui::SetItemDefaultFocus();
                if (ImGui::BeginCombo("Themes", BackgroundThemeNames[(int)currentBgTheme]))
                {
                    for (int i = 0; i < (int)BgTheme::Count; i++)
                    {
                        bool is_selected = (int)currentBgTheme == i;
                        if (ImGui::Selectable(BackgroundThemeNames[i], is_selected))
                        {
                            currentBgTheme = (BgTheme)i;
                        }
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }

                if (ImGui::Button("Okay", ImVec2(120, 0)))
                {
                    editor_vstate.is_processing = false;
                    editor_vstate.styles.theme = false;

                    message_vstate.init = true; // Success on export
                    message_vstate.message = "Theme Changed!";

                    ApplyBackgroundColor();

                    caretaker->backup();
                    originator->save_action("theme changed");

                    ImGui::CloseCurrentPopup();
                }

                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();

                if (ImGui::Button("Cancel", ImVec2(120, 0)))
                {
                    editor_vstate.is_processing = false;
                    editor_vstate.styles.theme = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
    }

    void backgrounds(editor_state &editor_vstate, Caretaker *caretaker, Originator *originator, message_state &message_vstate, imgui_state &imgui_vstate)
    {
        if (editor_vstate.styles.background)
        {
            editor_vstate.is_processing = true;

            ImGui::OpenPopup("Background");

            if (ImGui::BeginPopupModal("Background", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Select your preferred background");
                ImGui::Separator();

                if (ImGui::BeginCombo("Backgrounds", WarmBackgroundNames[(int)currentWarmBackground]))
                {
                    for (int i = 0; i < (int)WarmBackgrounds::Count; i++)
                    {
                        bool is_selected = (int)currentWarmBackground == i;
                        if (ImGui::Selectable(WarmBackgroundNames[i], is_selected))
                        {
                            currentWarmBackground = (WarmBackgrounds)i;
                        }
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }

                if (ImGui::Button("Okay", ImVec2(120, 0)))
                {
                    editor_vstate.is_processing = false;
                    editor_vstate.styles.background = false;

                    message_vstate.init = true;
                    message_vstate.message = "Background Changed!";

                    imgui_vstate.clear_color = GetWarmBackgroundColor(currentWarmBackground);

                    caretaker->backup();
                    originator->save_action("background changed");

                    ImGui::CloseCurrentPopup();
                }

                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();

                if (ImGui::Button("Cancel", ImVec2(120, 0)))
                {
                    editor_vstate.is_processing = false;
                    editor_vstate.styles.background = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
    }

} menu_settings;