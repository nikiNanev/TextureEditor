#include "common.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#endif

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"

#endif
typedef struct _image_info
{

    void display(editor_state &editor_vstate, loader &loader, Caretaker *caretaker, Originator *originator, message_state &message_vstate, sdl_state &sdl_vstate)
    {

        if (editor_vstate.image_state.general_info && loader.is_texture)
        {

            int width, height, channels, actual_channels, desired_channels = 4;

            unsigned char *loaded_data = stbi_load(
                loader.filename_path.c_str(),
                &width,
                &height,
                &channels,
                desired_channels);

            if (!loaded_data)
            {
                std::cout << "Failed to load image: " + loader.filename_path + " - " + stbi_failure_reason() << std::endl;
            }

            // desired_channels is 0 if we kept original, otherwise it's the forced value

            actual_channels = desired_channels != 0 ? desired_channels : channels;

            size_t size = static_cast<size_t>(width) * height * actual_channels;

            // Free the memory allocated by stb_image
            stbi_image_free(loaded_data);

            // Update channels to reflect what we actually have
            channels = actual_channels;

            ImGui::OpenPopup("Image Info");

            if (ImGui::BeginPopupModal("Image Info", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::SetWindowFontScale(1.5f);
                ImGui::Text("General Image Info");
                ImGui::Separator();

                ImGui::Text("Filename: %s", loader.filename_path.c_str());
                ImGui::Text("Dimensions: %dx%d", width, height);
                ImGui::Text("Size: %lu bytes ( %lu Kb)", size, (size / 1024));
                ImGui::Text("Channels: %d", channels);
                ImGui::SameLine();
                ImGui::Text("( %s )", (channels == 1 ? "Gray" : channels == 2 ? "Gray+Alpha"
                                                            : channels == 3   ? "RGB"
                                                                              : "RGBA"));

                if (ImGui::Button("Ok", ImVec2(60, 0)))
                {
                    editor_vstate.image_state.general_info = false;
                    editor_vstate.is_processing = false;

                    caretaker->backup();
                    originator->save_snapshot(loader.texture, loader.filename_path);

                    message_vstate.init = true;
                    message_vstate.message = " General Info ";

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

                ImGui::SetWindowFontScale(1.0f);
                ImGui::EndPopup();
            }
        }
    }

} image_info;