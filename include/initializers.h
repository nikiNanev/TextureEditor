
int init_sdl(sdl_state *state)
{
    // Setup SDL
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "Error: SDL_Init(): " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create window with SDL_Renderer graphics context
    std::string version = std::to_string(VERSION_MAJOR) + "." + std::to_string(VERSION_MINOR) + "." + std::to_string(VERSION_PATCH);
    std::string title = "Texture Editor v" + version;

    state->window = SDL_CreateWindow(title.c_str(),
                                     1280, 720,
                                     SDL_WINDOW_RESIZABLE);
    if (!state->window)
    {
        std::cout << "Error: SDL_CreateWindow(): " << SDL_GetError() << std::endl;
        return 1;
    }

    state->renderer = SDL_CreateRenderer(state->window, NULL);
    if (!state->renderer)
    {
        std::cout << "Error: SDL_CreateRenderer(): " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_GetWindowSize(state->window, &state->width, &state->height);

    return 0;
}

void init_imgui(imgui_state *imgui_vstate, sdl_state *sdl_vstate)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Dear ImGui style appearance
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(sdl_vstate->window, sdl_vstate->renderer);
    ImGui_ImplSDLRenderer3_Init(sdl_vstate->renderer);

    // Load Fonts
    ImGuiIO &io_fonts = ImGui::GetIO();
    io_fonts.Fonts->AddFontDefault();

    imgui_vstate->init_window_flags |= ImGuiWindowFlags_NoMove;
    imgui_vstate->init_window_flags |= ImGuiWindowFlags_NoCollapse;
    imgui_vstate->init_window_flags |= ImGuiWindowFlags_NoResize;

    imgui_vstate->clear_color = GetWarmBackgroundColor(WarmBackgrounds::LatteCream);
}