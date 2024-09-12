#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "gl.h"


#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 1024
#define TICK_MS 32
#define ROT_SPEED 0.025f

void err_msg(const char *msg);

static SDL_Window *g_window = NULL;


int main(int argc, char* args[]) {

	// Initialisation
	if (SDL_Init(SDL_INIT_VIDEO) < 0) err_msg("Failed to initialise SDL");
	g_window = SDL_CreateWindow(
		"OpenGL Render Test",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_WINDOW_OPENGL
		| SDL_WINDOW_SHOWN
	);
	if (g_window == NULL) err_msg("Failed to create Window");

	// Initialise OpenGL version 4.5
	gl_init(4, 5, g_window);

	// Create Program
	GLuint program = glCreateProgram();
	GLuint vert_shader = gl_load_shader(GL_VERTEX_SHADER, "shaders/3D_rot.vert");
	glAttachShader(program, vert_shader);
	GLuint frag_shader = gl_load_shader(GL_FRAGMENT_SHADER, "shaders/uv_blend.frag");
	glAttachShader(program, frag_shader);
	gl_link_program(program);

	// Create model data
	GLfloat model_angle = 0.0f;
	bool is_spinning = true;
	gl_vertex vertex_data[] = {
		{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
		{ {  0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
		{ {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 0.0f } },
		{ { -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } },
		{ { -0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f } },
		{ { -0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
		{ {  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f }, { 1.0f, 0.0f, 1.0f } },
		{ {  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f } },
	};
	GLuint index_data[] = {
		0, 1, 2,
		0, 2, 3,
		0, 5, 6,
		0, 6, 1,
		0, 3, 4,
		0, 4, 5,
		7, 2, 1,
		7, 1, 6,
		7, 4, 3,
		7, 3, 2,
		7, 5, 4,
		7, 6, 5
	};

	// Upload model to GPU
	gl_vert_rot_x(vertex_data, 8, 0.05f);
	gl_vert_rot_z(vertex_data, 8, 0.15f);
	gl_check_err("Failed to set rotation angle");
	if (gl_upload_vertices(vertex_data, 8, index_data, 36) != 0) {
		puts("No room on the broom!");
		return 1;
	}

	// Create Texture
	GLuint test_tex = gl_load_texture("textures/uv_test.bmp");

	// Main Loop
	bool isRunning = true;
	bool redraw = true;
	Uint64 last_tick = 0;
	SDL_Event curr_event;
	while (isRunning) {

		// Handle events
		int scode = SDL_WaitEventTimeout(&curr_event, 10);

		// Rotate vertices
		Uint64 now = SDL_GetTicks64();
		if (last_tick != 0) {
			Uint64 elapsed = now - last_tick;
			if (elapsed >= TICK_MS) {
				last_tick = now;

				if (is_spinning) {
					model_angle += ROT_SPEED;
					if (model_angle >= 2*M_PI) model_angle -= 2*M_PI;
					redraw = true;
				}

				fflush(stdout);
			}
		} else {
			last_tick = now;
		}

		if (scode != 0) {
			switch (curr_event.type) {
				case SDL_QUIT:
					isRunning = false;
				continue;

				case SDL_KEYDOWN: {
				} break;

				case SDL_KEYUP: {
					SDL_KeyCode kc = curr_event.key.keysym.sym;
					switch (kc) {
						case SDLK_SPACE: is_spinning = !is_spinning; break;
						default: break;
					};
					redraw = true;
				} break;

				case SDL_MOUSEBUTTONDOWN: 
				case SDL_MOUSEBUTTONUP:
				break;

				case SDL_MOUSEMOTION:
				break;

				case SDL_MOUSEWHEEL: {
				} break;
			}
		}

		if (redraw) {
			// Clear Screen
			glClear(GL_COLOR_BUFFER_BIT);
			gl_check_err("Failed to clear colour buffer");

			// Load program, draw models, unload program
			glUseProgram(program);
			glUniform1f(UNI_ANGLE, model_angle);
			glBindTexture(GL_TEXTURE_2D, test_tex);
			gl_draw_vertices();
			glUseProgram(NULL_PROGRAM);

			// Swap buffers
			SDL_GL_SwapWindow(g_window);

			redraw = false;
		}

	}

	// Termination
	gl_term();
	SDL_DestroyWindow(g_window);
	SDL_Quit();
	return 0;
}

void err_msg(const char *msg) {
	printf("[ERROR] %s: %s\n", msg, SDL_GetError());
	exit(1);
}
