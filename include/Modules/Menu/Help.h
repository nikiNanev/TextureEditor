typedef struct _menu_help
{
    void display(editor_state &editor_vstate)
    {
        if (ImGui::BeginMenu("Help"))
        {

            if (ImGui::MenuItem("Manual"))
            {
                editor_vstate.help.manual = true;

            }

            ImGui::EndMenu();
        }
    }
} menu_help;