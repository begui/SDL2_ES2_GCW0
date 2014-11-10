#ifndef _ROTATETRIANGLE_H
#define _ROTATETRIANGLE_H
#include "glsl_utils.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const GLfloat DEG_TO_RAD = 0.0174532925;

class RotateTriangle
{
	public:
		RotateTriangle()
		{
			GLuint _vertextShader = loadShader(GL_VERTEX_SHADER, vert);
			GLuint _fragmentShader = loadShader(GL_FRAGMENT_SHADER, frag);
			_program = loadProgram(_vertextShader, _fragmentShader, {"vPosition","mRotation"});
			glDeleteShader(_vertextShader);
			glDeleteShader(_fragmentShader);
		}

		~RotateTriangle(){glDeleteProgram(_program);}

		void render()
		{
			GLfloat vVertices[] = {  
				0.0f,  0.5f, 0.0f, 
				-0.5f, -0.5f, 0.0f,
				0.5f, -0.5f, 0.0f };

			glUseProgram(_program);	

			static GLfloat angle = 0.0f;
			angle += 0.8f;
			if(angle > 360.0f) angle = 0.0f;
			glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), DEG_TO_RAD * angle, glm::vec3(0.0f,1.0f,0.0f));

			GLint location = glGetUniformLocation(_program, "mRotation"); 
			if( location >= 0 )
			{
				glUniformMatrix4fv(location, 1, GL_FALSE, &rotationMatrix[0][0]);
			}

			glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
			glEnableVertexAttribArray(0);
			glDrawArrays(GL_TRIANGLES, 0, 3 );

			glUseProgram(0);	
		}

	private:
		GLuint _program;

		const char *vert =  
		"attribute vec4 vPosition;						\n"
		"uniform mat4 mRotation;						\n"
		"void main()									\n"
		"{												\n"
		"   gl_Position = mRotation * vPosition;		\n"
		"}\0											\n";

		const char *frag =  
		"precision mediump float;\n"\
		"void main()									\n"
		"{												\n"
		"  gl_FragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );	\n"
		"}\0											\n";
};
#endif
