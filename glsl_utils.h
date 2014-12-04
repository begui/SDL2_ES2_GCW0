#ifndef _GLSDLUTIL_H
#define _GLSDLUTIL_H
#include <SDL2/SDL_opengles2.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cstdio>

void printGLVersion()
{
	printf("Opengl Vender: %s\n",
			(const char*) glGetString(GL_VENDOR));
	printf("Opengl Renderer: %s\n",
			(const char*) glGetString(GL_RENDERER));
	printf("Opengl Version: %s\n", 
			(const char *)glGetString(GL_VERSION));
	printf("GLSL Version: %s\n",
			(const char *)glGetString( GL_SHADING_LANGUAGE_VERSION));
}


static GLuint loadProgram(GLuint vertex, GLuint fragment, std::vector<std::string> attributes)
{
	// Create the program object
	GLuint programObject = glCreateProgram ( );

	if ( programObject == 0 )
		return 0;

	glAttachShader ( programObject, vertex);
	glAttachShader ( programObject, fragment);

	int i = 0;
	std::for_each(std::begin(attributes), std::end(attributes), 
		[&](std::string attribute)
		{
			glBindAttribLocation(programObject, i++, attribute.c_str());
		});

	// Link the program
	glLinkProgram ( programObject );

	GLint linked;
	// Check the link status
	glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

	if ( !linked ) 
	{
		GLint infoLen = 0;

		glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );

		if ( infoLen > 1 )
		{
			char* infoLog = (char *)malloc (sizeof(char) * infoLen );

			glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
			std::cerr << "Error linking program:" << std::endl << infoLog ;            

			free ( infoLog );
		}

		glDeleteProgram ( programObject );
		return GL_FALSE;
	}
	return programObject;
}

static GLuint loadShader ( GLenum type, const char *shaderSrc )
{
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader ( type );

	if ( shader == GL_FALSE )
		return 0;

	// Load the shader source
	glShaderSource ( shader, 1, &shaderSrc, NULL );

	// Compile the shader
	glCompileShader ( shader );

	// Check the compile status
	glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

	if ( !compiled ) 
	{
		GLint infoLen = 0;

		glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );

		if ( infoLen > 1 )
		{
			char* infoLog = (char *)malloc (sizeof(char) * infoLen );

			glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
			std::cerr <<  "Error compiling shader: " << infoLog << std::endl;            

			free ( infoLog );
		}

		glDeleteShader ( shader );
		return 0;
	}
	return shader;
}
#endif
