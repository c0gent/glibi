#ifndef __SHADER_HPP_INCLUDED__
#define __SHADER_HPP_INCLUDED__

// Returns new, compiled shader.
GLuint createShader(GLenum type, const GLchar* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    // [INCOMPLETE] If status is equal to GL_TRUE, shader was compiled successfully.
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        // [INCOMPLETE] Retrieve the compile log:
        char buffer[512];
        glGetShaderInfoLog(shader, 512, NULL, buffer);
        fprintf(stdout, "No bueno: %s", buffer);
        throw status;
    }

    return shader;
}

#endif // __SHADER_HPP_INCLUDED__ 
