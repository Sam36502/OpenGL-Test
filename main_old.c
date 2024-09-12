#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>

#include <SDL2/SDL_opengl.h>
#include <GL/GLU.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512

void err_msg(const char *msg);
void check_gl_err();
void printProgramLog( GLuint program );

SDL_Window *g_window = NULL;
TTF_Font *g_font = NULL;
SDL_GLContext g_glcontext;

int main(int argc, char* args[]) {

	// Set OpenGL Settings
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Initialisation
	if (SDL_Init(SDL_INIT_VIDEO) < 0) err_msg("Failed to initialise SDL");

	g_window = SDL_CreateWindow(
		"OpenGL 2.1 Render Test",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_WINDOW_OPENGL
		| SDL_WINDOW_SHOWN
	);
	if (g_window == NULL) err_msg("Failed to create Window");

	// JRAPHICS
	puts("[INFO] Creating OpenGL Context..."); fflush(stdout);
	g_glcontext = SDL_GL_CreateContext(g_window);
	if (g_glcontext == NULL) err_msg("Failed to create GL Context");

	// Set VSync on
	puts("[INFO] Enabling Vertical Synch..."); fflush(stdout);
	if (SDL_GL_SetSwapInterval(1) < 0) err_msg("Failed to set VSync");

	// Set colour buffers' default values
	glClearColor( 0.0, 0.0, 0.0, 1.0 );
	check_gl_err();

	puts("[INFO] Setting render matricies..."); fflush(stdout);
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	check_gl_err();
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	check_gl_err();

	puts("[INFO] Init Complete."); fflush(stdout);

	printf("[INFO] #### OpenGL Information ####\n");
	printf("[INFO]       Vendor: %s\n", (const char *) glGetString(GL_VENDOR));
	printf("[INFO]     Renderer: %s\n", (const char *) glGetString(GL_RENDERER));
	printf("[INFO]      Version: %s\n", (const char *) glGetString(GL_VERSION));
	printf("[INFO]     SL Vers.: %s\n", (const char *) glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("[INFO] Is current OpenGL version supported? %s\n", GL_VERSION_2_1 ? "Yes!" : "NO!?");

	// Main Loop
	bool isRunning = true;
	bool redraw = true;
	SDL_Event curr_event;
	while (isRunning) {

		// Handle events
		int scode = SDL_WaitEventTimeout(&curr_event, 10);

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
			puts("[INFO] Drawing..."); fflush(stdout);

			// MANLY GPU RENDERING!!!!!
			puts("[INFO] | Clearing Color Buffer..."); fflush(stdout);
			glClear(GL_COLOR_BUFFER_BIT);
			check_gl_err();

			puts("[INFO] | Sending quad vertices..."); fflush(stdout);
			glBegin( GL_QUADS );
			glVertex2f( -	0.5f, -	0.5f );
			glVertex2f( 	0.5f, -	0.5f );
			glVertex2f( 	0.5f, 	0.5f );
			glVertex2f( -	0.5f, 	0.5f );
			glEnd();

			SDL_GL_SwapWindow(g_window);

			redraw = false;
			puts("[INFO] Done..."); fflush(stdout);
		}

	}

	// Termination
	//SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);
	TTF_CloseFont(g_font);
	TTF_Quit();
	SDL_Quit();
	return 0;
}

void err_msg(const char *msg) {
	printf("[ERROR] %s: %s\n", msg, SDL_GetError());
	exit(1);
}

void check_gl_err() {
	GLenum err = glGetError();
	switch (err) {
		case GL_NO_ERROR: return;
		case GL_INVALID_ENUM: err_msg("An unacceptable value is specified for an enumerated argument"); break;
		case GL_INVALID_VALUE: err_msg("A numeric argument is out of range"); break;
		case GL_INVALID_OPERATION: err_msg("The specified operation is not allowed in the current state"); break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: err_msg("The framebuffer object is not complete"); break;
		case GL_OUT_OF_MEMORY: err_msg("There is not enough memory left to execute the command"); break;
		case GL_STACK_UNDERFLOW: err_msg("An attempt has been made to perform an operation that would cause an internal stack to underflow"); break;
		case GL_STACK_OVERFLOW: err_msg("An attempt has been made to perform an operation that would cause an internal stack to overflow"); break;
	}
}
