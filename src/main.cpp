#include "libs/imgui/imgui.h"
#include "libs/imgui/imgui_impl_sdl2.h"
#include "libs/imgui/imgui_impl_opengl3.h"

#include "libs/Coloforma/Println.h"

#include <SDL.h>
#include <SDL_opengl.h>

int main()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		alx::println(alx::Colour::Red, "Error: {}", SDL_GetError());
		return EXIT_FAILURE;
	}

	const char* glslVersion = "#version 130";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	auto windowFlags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_Window* window =
		SDL_CreateWindow("Real Engine 0.0.1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, windowFlags);
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, glContext);
	SDL_GL_SetSwapInterval(1); // Enable vsync


	// Imgui setup
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	[[maybe_unused]] ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init(glslVersion);

	// State
	ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


	// Main loop
	bool done;

	while (!done) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT)
				done = true;
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE
				&& event.window.windowID == SDL_GetWindowID(window))
				done = true;
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		ImGui::Render();
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		glClearColor(
			clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}