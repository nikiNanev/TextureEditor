#include "common.h"

typedef struct _image_info
{

    void display(editor_state &editor_vstate, loader &loader, Caretaker *caretaker, Originator *originator, message_state &message_vstate, sdl_state &sdl_vstate)
    {

        if (editor_vstate.image_state.general_info && loader.is_texture)
        {

            ImGui::OpenPopup("Image Info");

            if (ImGui::BeginPopupModal("Image Info", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::SetWindowFontScale(1.2f);
                ImGui::Text("General Image Info");
                ImGui::Separator();

                ImGui::Text("Filename: %s", loader.filename_path.c_str());
                ImGui::Text("Dimensions: %dx%d", loader.width, loader.height);
                ImGui::Text("Size: %lu bytes ( %lu Kb)", loader.size, (loader.size / 1024));
                ImGui::Text("Channels: %d", loader.channels);
                ImGui::SameLine();
                ImGui::Text("( %s )", (loader.channels == 1 ? "Gray" : loader.channels == 2 ? "Gray+Alpha"
                                                            : loader.channels == 3   ? "RGB"
                                                                              : "RGBA"));

                if (ImGui::Button("Ok", ImVec2(60, 0)))
                {
                    editor_vstate.image_state.general_info = false;
                    editor_vstate.is_processing = false;

                    caretaker->backup();
                    originator->save_snapshot(loader.texture, loader.filename_path);
                    
                    ImGui::CloseCurrentPopup();
                }

                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();

                static int counter = 0;

                if (ImGui::Button("Export as txt", ImVec2(170, 0)))
                {
                    std::ofstream output_txt("../reports/general_image_info_" + std::to_string(counter++) + ".txt"); // Opens (or creates) example.txt for writing

                    if (!output_txt)
                    {
                        std::cerr << "Error: Could not open file for writing!" << std::endl;
                    }

                    output_txt << "Image General info\n\n";
                    output_txt << "Filename: " << loader.filename_path.c_str() << "\n";
                    output_txt << "Dimensions: " << loader.width << "x" << loader.height << "\n";
                    output_txt << "File size: " << loader.size << " bytes ( " << (loader.size / 1024) << " KB )\n";
                    output_txt << "Channels: " << loader.channels << "\n";
                    output_txt << "( " << (loader.channels == 1 ? "Gray" : loader.channels == 2 ? "Gray+Alpha"
                                                                : loader.channels == 3   ? "RGB"
                                                                                  : "RGBA")
                               << " )\n";

                    output_txt.close();
                    message_vstate.init = true;
                    message_vstate.message = "Success export to txt file";
                }

                ImGui::SameLine();

                if (ImGui::Button("Cancel", ImVec2(120, 0)))
                {
                    editor_vstate.image_state.general_info = false;
                    editor_vstate.is_processing = false;
                    ImGui::CloseCurrentPopup();
                }

                ImGui::SetWindowFontScale(1.0f);
                ImGui::EndPopup();
            }
        }
    }

} image_info;