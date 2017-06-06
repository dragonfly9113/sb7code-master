/*
 * Copyright © 2012-2015 Graham Sellers
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <sb7.h>

class movingtri_my_app : public sb7::application
{
    void init()
    {
        static const char title[] = "OpenGL SuperBible - My Moving Triangle";

        sb7::application::init();

        memcpy(info.title, title, sizeof(title));
    }

    virtual void startup()
    {
		static const char * vs_source[] =
		{
			"#version 420 core                                                 \n"
			"                                                                  \n"
			"// 'offset' and 'color' are input vertex attributes			   \n"
			"layout (location = 0) in vec4 offset;							   \n"
			"layout (location = 1) in vec4 color;							   \n"
			"                                                                  \n"
			"// 'vs_color' is an output and will be sent to the next shader stage \n"
			"out vec4 vs_color;												   \n"
			"																   \n"
			"void main(void)                                                   \n"
			"{                                                                 \n"
			"    const vec4 vertices[] = vec4[](vec4( 0.25, -0.25, 0.5, 1.0),  \n"
			"                                   vec4(-0.25, -0.25, 0.5, 1.0),  \n"
			"                                   vec4( 0.25,  0.25, 0.5, 1.0)); \n"
			"                                                                  \n"
			"    //Add 'offset' to our hard-coded vertex position			   \n"
			"    gl_Position = vertices[gl_VertexID] + offset;                 \n"
			"																   \n"
			"	 // Output a fixed value for vs_color						   \n"
			"	 vs_color = color;											   \n"
            "}                                                                 \n"
        };

		static const char * fs_source[] =
		{
			"#version 420 core                                                 \n"
			"                                                                  \n"
			"// Input from the vertex shader								   \n"
			"in vec4 vs_color;												   \n"
			"																   \n"
			"// Output to the framebuffer									   \n"
			"out vec4 color;                                                   \n"
			"                                                                  \n"
			"void main(void)                                                   \n"
			"{                                                                 \n"
			"    //color = vec4(0.0, 0.8, 1.0, 1.0);                           \n"
			"	 color = vs_color;											   \n"
            "}                                                                 \n"
        };

        program = glCreateProgram();
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, fs_source, NULL);
        glCompileShader(fs);

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, vs_source, NULL);
        glCompileShader(vs);

        glAttachShader(program, vs);
        glAttachShader(program, fs);

        glLinkProgram(program);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
    }

    virtual void render(double currentTime)
    {
        const GLfloat color[] = { (float)sin(currentTime) * 0.5f + 0.5f,
  								  (float)cos(currentTime) * 0.5f + 0.5f,
								  0.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, color);

        glUseProgram(program);

		GLfloat attrib[] = { (float)sin(currentTime) * 0.5f,
							 (float)cos(currentTime) * 0.6f,
							 0.0f, 0.0f };
		// Update the value of input attribute 0
		glVertexAttrib4fv(0, attrib);

		// Update the color of the triangle
		const GLfloat tri_color[] = { 0.0f,
									  (float)sin(currentTime) * 0.5f + 0.5f,
									  (float)cos(currentTime) * 0.5f + 0.5f,
									  1.0f };
		glVertexAttrib4fv(1, tri_color);

		// Draw one triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    virtual void shutdown()
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteProgram(program);
    }

private:
    GLuint          program;
    GLuint          vao;
};

DECLARE_MAIN(movingtri_my_app)
