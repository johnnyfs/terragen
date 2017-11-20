#ifndef SHADERS_H
#define SHADERS_H

#include <GL/glew.h>
#include <GL/gl.h>

#define ATTRIB(p, a) GLuint a = glGetAttribLocation(p, #a)
#define UNIFORM(p, u) GLuint u = glGetUniformLocation(p, #u)

typedef void (*gl_get_f)(GLuint, GLenum, GLint*);
typedef void (*gl_set_f)(GLuint, GLsizei, GLsizei*, GLchar*);

static char*
file(const char *path)
{
    FILE *f = fopen(path, "rt");
    if (f == NULL) return NULL;
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *src = malloc((size_t)len + 1);
    if (src == NULL) return NULL;
    fread(src, 1, (size_t)len, f);
    src[len - 1] = '\0';
    return src;
}

GLuint
validated(GLuint obj, GLenum check, gl_get_f get, gl_set_f set, char **log)
{
    GLint passed;
    GLint len;

    get(obj, check, &passed);

    if (passed != GL_FALSE) {
        return obj;
    }

    get(obj, GL_INFO_LOG_LENGTH, &len);
    *log = malloc((size_t)len);

    if (*log != NULL) {
        set(obj, len, NULL, *log);
    }

    return 0;
}

GLuint
shader(GLenum type, const char *src_path, char **log)
{
    char *src = file(src_path);
    if (src == NULL) return 0;
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, (const char**)&src, NULL);
    glCompileShader(s);

    return validated(s, GL_COMPILE_STATUS, (gl_get_f)glGetShaderiv, (gl_set_f)glGetShaderInfoLog, log);
}

#endif //SHADERS_H
