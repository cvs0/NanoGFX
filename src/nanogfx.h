#ifndef NANO_GFX_H
#define NANO_GFX_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#include <stddef.h>

/* Define OpenGL types and constants we need */
#ifndef GL_ARRAY_BUFFER
#define GL_ARRAY_BUFFER 0x8892
#endif
#ifndef GL_ELEMENT_ARRAY_BUFFER
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#endif
#ifndef GL_VERTEX_SHADER
#define GL_VERTEX_SHADER 0x8B31
#endif
#ifndef GL_FRAGMENT_SHADER
#define GL_FRAGMENT_SHADER 0x8B30
#endif
#ifndef GL_COMPILE_STATUS
#define GL_COMPILE_STATUS 0x8B81
#endif
#ifndef GL_LINK_STATUS
#define GL_LINK_STATUS 0x8B82
#endif
#ifndef GL_TRIANGLES
#define GL_TRIANGLES 0x0004
#endif
#ifndef GL_MAP_WRITE_BIT
#define GL_MAP_WRITE_BIT 0x0002
#endif
#ifndef GL_MAP_PERSISTENT_BIT
#define GL_MAP_PERSISTENT_BIT 0x0040
#endif
#ifndef GL_MAP_COHERENT_BIT
#define GL_MAP_COHERENT_BIT 0x0080
#endif

/* Define missing OpenGL types */
#ifndef GLsizeiptr
typedef ptrdiff_t GLsizeiptr;
#endif
#ifndef GLintptr
typedef ptrdiff_t GLintptr;
#endif
#ifndef GLbitfield
typedef unsigned int GLbitfield;
#endif
#ifndef GLchar
typedef char GLchar;
#endif
#ifndef GLAPI
#define GLAPI extern
#endif

/* OpenGL function pointer types for shader functions */
typedef GLuint (APIENTRY *PFNGLCREATESHADERPROC)(GLenum type);
typedef void (APIENTRY *PFNGLSHADERSOURCEPROC)(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
typedef void (APIENTRY *PFNGLCOMPILESHADERPROC)(GLuint shader);
typedef GLuint (APIENTRY *PFNGLCREATEPROGRAMPROC)(void);
typedef void (APIENTRY *PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
typedef void (APIENTRY *PFNGLLINKPROGRAMPROC)(GLuint program);
typedef void (APIENTRY *PFNGLDELETESHADERPROC)(GLuint shader);

/* OpenGL function pointers for shader functions */
static PFNGLCREATESHADERPROC glCreateShader;
static PFNGLSHADERSOURCEPROC glShaderSource;
static PFNGLCOMPILESHADERPROC glCompileShader;
static PFNGLCREATEPROGRAMPROC glCreateProgram;
static PFNGLATTACHSHADERPROC glAttachShader;
static PFNGLLINKPROGRAMPROC glLinkProgram;
static PFNGLDELETESHADERPROC glDeleteShader;

/* -------------------------------------------------
   OpenGL loader (tiny subset)
--------------------------------------------------*/
typedef GLuint (APIENTRY *PFNGLCREATEBUFFERSPROC)(GLsizei,GLuint*);
typedef void   (APIENTRY *PFNGLNAMEDBUFFERSTORAGEPROC)(GLuint,GLsizeiptr,const void*,GLbitfield);
typedef void*  (APIENTRY *PFNGLMAPBUFFERRANGEPROC)(GLuint,GLintptr,GLsizeiptr,GLbitfield);
typedef void   (APIENTRY *PFNGLUNMAPBUFFERPROC)(GLuint);
typedef void   (APIENTRY *PFNGLCREATEVERTEXARRAYSPROC)(GLsizei,GLuint*);
typedef void   (APIENTRY *PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint);
typedef void   (APIENTRY *PFNGLVERTEXATTRIBFORMATPROC)(GLuint,GLint,GLenum,GLboolean,GLuint);
typedef void   (APIENTRY *PFNGLVERTEXATTRIBBINDINGPROC)(GLuint,GLuint);
typedef void   (APIENTRY *PFNGLBINDBUFFERRANGEPROC)(GLenum,GLuint,GLuint,GLintptr,GLsizeiptr);
typedef void   (APIENTRY *PFNGLBINDBUFFERPROC)(GLenum, GLuint);
typedef void   (APIENTRY *PFNGLVERTEXATTRIBPOINTERPROC)(GLuint, GLint, GLenum, GLboolean, GLsizei, const void *);
typedef void   (APIENTRY *PFNGLBINDVERTEXARRAYPROC)(GLuint);
typedef void   (APIENTRY *PFNGLUSEPROGRAMPROC)(GLuint);
typedef void   (APIENTRY *PFNGLMULTIDRAWARRAYSINDIRECTPROC)(GLenum,const void*,GLsizei,GLsizei);

static PFNGLCREATEBUFFERSPROC             glCreateBuffers;
static PFNGLNAMEDBUFFERSTORAGEPROC        glNamedBufferStorage;
static PFNGLMAPBUFFERRANGEPROC            glMapNamedBufferRange;
static PFNGLUNMAPBUFFERPROC               glUnmapNamedBuffer;
static PFNGLCREATEVERTEXARRAYSPROC        glCreateVertexArrays;
static PFNGLENABLEVERTEXATTRIBARRAYPROC   glEnableVertexAttribArray;
static PFNGLVERTEXATTRIBFORMATPROC        glVertexAttribFormat;
static PFNGLVERTEXATTRIBBINDINGPROC       glVertexAttribBinding;
static PFNGLBINDBUFFERRANGEPROC           glBindBufferRange;
static PFNGLBINDBUFFERPROC                glBindBuffer;
static PFNGLVERTEXATTRIBPOINTERPROC       glVertexAttribPointer;
static PFNGLBINDVERTEXARRAYPROC           glBindVertexArray;
static PFNGLUSEPROGRAMPROC                glUseProgram;
static PFNGLMULTIDRAWARRAYSINDIRECTPROC   glMultiDrawArraysIndirect;

static void load_gl(void) {
    #define LOAD(x) *(void**)(&x) = (void*)wglGetProcAddress(#x)
    LOAD(glCreateShader); LOAD(glShaderSource); LOAD(glCompileShader);
    LOAD(glCreateProgram); LOAD(glAttachShader); LOAD(glLinkProgram); LOAD(glDeleteShader);
    LOAD(glCreateBuffers); LOAD(glNamedBufferStorage); LOAD(glMapNamedBufferRange);
    LOAD(glUnmapNamedBuffer); LOAD(glCreateVertexArrays); LOAD(glEnableVertexAttribArray);
    LOAD(glVertexAttribFormat); LOAD(glVertexAttribBinding); LOAD(glBindBufferRange);
    LOAD(glBindBuffer); LOAD(glVertexAttribPointer);
    LOAD(glBindVertexArray); LOAD(glUseProgram); LOAD(glMultiDrawArraysIndirect);
}

/* -------------------------------------------------
   Shader helper
--------------------------------------------------*/
static GLuint make_shader(const char *vs_src, const char *fs_src) {
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_src, NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_src, NULL);
    glCompileShader(fs);
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs); glAttachShader(prog, fs);
    glLinkProgram(prog);
    glDeleteShader(vs); glDeleteShader(fs);
    return prog;
}

/* -------------------------------------------------
   Engine context
--------------------------------------------------*/
#define MAX_VERTS  (1<<22)  // 4M verts
#define MAX_DRAWS  2048

typedef struct {
    HDC   hdc;
    HGLRC hglrc;
    GLuint vbo, vao, cmd_buf;
    GLuint shader;
    void *vert_ptr;           // Persistent map
    void *cmd_ptr;            // Persistent map
    size_t vert_offset;
    size_t cmd_count;
} NanoGfx;

static const char *vs =
    "#version 330\n"
    "layout(location=0) in vec3 pos;\n"
    "layout(location=1) in vec4 col;\n"
    "out vec4 vcol;\n"
    "void main(){ gl_Position=vec4(pos,1.0); vcol=col; }\n";

static const char *fs =
    "#version 330\n"
    "in  vec4 vcol;\n"
    "out vec4 frag;\n"
    "void main(){ frag=vcol; }\n";

/* -------------------------------------------------
   Init / shutdown
--------------------------------------------------*/
static void ng_init(NanoGfx *g, HWND hwnd) {
    PIXELFORMATDESCRIPTOR pfd = { sizeof(pfd),1,PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,32,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,0,0,0,0 };
    g->hdc = GetDC(hwnd);
    SetPixelFormat(g->hdc, ChoosePixelFormat(g->hdc,&pfd), &pfd);
    g->hglrc = wglCreateContext(g->hdc);
    wglMakeCurrent(g->hdc, g->hglrc);
    load_gl();

    g->shader = make_shader(vs,fs);

    glCreateBuffers(1,&g->vbo);
    glNamedBufferStorage(g->vbo, MAX_VERTS*sizeof(float)*7, 0,
                         GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    g->vert_ptr = glMapNamedBufferRange(g->vbo, 0, MAX_VERTS*sizeof(float)*7,
                                        GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    glCreateBuffers(1,&g->cmd_buf);
    glNamedBufferStorage(g->cmd_buf, MAX_DRAWS*sizeof(GLuint)*4, 0,
                         GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    g->cmd_ptr = glMapNamedBufferRange(g->cmd_buf, 0, MAX_DRAWS*sizeof(GLuint)*4,
                                       GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    glCreateVertexArrays(1, &g->vao);
    glBindVertexArray(g->vao);
    glBindBuffer(GL_ARRAY_BUFFER, g->vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));

    g->vert_offset = 0;
    g->cmd_count = 0;
}

static void ng_shutdown(NanoGfx *g) {
    wglMakeCurrent(NULL,NULL);
    wglDeleteContext(g->hglrc);
    ReleaseDC(WindowFromDC(g->hdc), g->hdc);
}

/* -------------------------------------------------
   Immediate push
--------------------------------------------------*/
static void ng_push_tri(NanoGfx *g,
                        float x0,float y0,float z0,
                        float x1,float y1,float z1,
                        float x2,float y2,float z2,
                        float r,float g_col,float b,float a) {
    float *v = (float*)g->vert_ptr + g->vert_offset*7;
    #define V(x,y,z,r,g,b,a) *v++=x;*v++=y;*v++=z;*v++=r;*v++=g;*v++=b;*v++=a;
    V(x0,y0,z0,r,g_col,b,a); V(x1,y1,z1,r,g_col,b,a); V(x2,y2,z2,r,g_col,b,a);
    GLuint *c = (GLuint*)g->cmd_ptr + g->cmd_count*4;
    c[0]=g->vert_offset; c[1]=3; c[2]=1; c[3]=0;
    g->vert_offset += 3;
    g->cmd_count++;
}

/* -------------------------------------------------
   Render
--------------------------------------------------*/
static void ng_render(NanoGfx *g) {
    glUseProgram(g->shader);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(g->vao);
   
    if (g->vert_offset > 0) {
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)g->vert_offset);
    }
    SwapBuffers(g->hdc);
    g->vert_offset = 0;
    g->cmd_count = 0;
}

#endif // NANO_GFX_H