#ifndef MY_GL_H
#define MY_GL_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <math.h>

#define NULL_PROGRAM (GLuint) 0
#define MAX_SHADER_LINES 1024	// Maximum number of lines of shader source
#define MAX_SHADER_COLS 1024	// Maximum number of chars per line of shader source
#define VERTEX_BUFFER_SIZE (64 * sizeof(gl_vertex))
#define INDEX_BUFFER_SIZE (256 * sizeof(GLuint))

// Vertex Attribute locations
#define VAL_POSITION 0
#define VAL_COLOUR 1
#define VAL_VERT_UV 2

// Uniform Locations
#define UNI_ANGLE 0


typedef struct {
	GLfloat r;
	GLfloat g;
	GLfloat b;
} gl_rgb;

typedef struct {
	GLfloat x;
	GLfloat y;
} gl_point_2d;

typedef struct {
	GLfloat x;
	GLfloat y;
	GLfloat z;
} gl_point_3d;

typedef struct {
	gl_point_3d pos;
	gl_point_2d uv;
	gl_rgb colour;
} gl_vertex;

//	Initialises everything we need for OpenGL
//	
//	This includes GLEW and the SDL OpenGL context
//	as well as loading in and compiling shader files.
//	
//	Takes in the major and minor version numbers of OpenGL to request.
void gl_init(int major, int minor, SDL_Window *window);

//	Terminates all the OpenGL stuff
//	
void gl_term();

//	Loads and compiles a shader from a source file
//	
//	Returns ID of the newly loaded and compiled shader,
//	or 0 if an error occurs during loading/compiling.
//	Messages are logged.
GLuint gl_load_shader(GLenum type, const char *source_filename);

//	Links a program and handles errors
//	
void gl_link_program(GLuint program);

//	Uploads a bunch of vertex data to the GPU
//	
//	If the model was successfully uploaded, the function returns 0,
//	otherwise, if there was not enough room, it returns 1
int gl_upload_vertices(gl_vertex *vertex_data, GLuint vertex_count, GLuint *index_data, GLuint index_count);

//	Removes all models from the GPU, freeing it up for new models
//	
void gl_clear_vertices();

//	Renders all the models uploaded to the GPU
//	
void gl_draw_vertices();

//	Rotates a vertex/series of vertices around the X-axis by some angle
//	
void gl_vert_rot_x(gl_vertex *vert, size_t count, GLfloat angle);

//	Rotates a vertex/series of vertices around the Z-axis by some angle
//	
void gl_vert_rot_z(gl_vertex *vert, size_t count, GLfloat angle);

//	Load a texture from a .bmp file
//	
//	Returns the GLuint texture name
GLuint gl_load_texture(const char *image_filename);

//	Loads a debug texture from a pixel data literal
//	
//	Returns the GLuint texture name.
//	The debug texture is a 2x4 square with values:
//	
//		Red, green, blue, black
//		cyan, magenta, yellow, white
//	
GLuint gl_load_debug_texture();

//	Checks if any GL errors occurred and exits if an error was found
//	
//	Takes a string message explaining the context of where it was called.
//	Logs any gl errors that were found and exits if a failure was detected,
//	otherwise, if all is good, it returns without doing anything.
void gl_check_err(char *msg);

//	Takes the ID of a program and prints out its info log
//	
void gl_print_prog_log(GLuint program);

//	Takes the ID of a shader and prints out its info log
//	
void gl_print_shader_log(GLuint shader);

#endif