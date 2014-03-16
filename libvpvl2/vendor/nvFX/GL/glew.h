#pragma once
#ifndef NVFX_GLEW_H_
#define NVFX_GLEW_H_

#include <stddef.h>

#ifdef _WIN32
#define NVFX_GLEW_OS_WIN32
#endif

#ifdef NVFX_GLEW_OS_WIN32
#include <windows.h>
#endif

/* GLEW mock for nvFX */

#ifdef _MSC_VER
#define NVFX_GLEW_APIENTRY __stdcall *
#else
#define NVFX_GLEW_APIENTRY *
#endif

#define NVFX_GLEW_APIENTRY_EXPORT extern

#ifndef GLAPIENTRY
#define GLAPIENTRY
#endif /* GLAPIENTRY */

#ifndef GLAPI
#define GLAPI extern
#endif /* GLAPI */

#define NVFX_GLEW_DECLARE_TYPES \
    typedef unsigned char GLboolean; \
    typedef int GLint; \
    typedef void GLvoid; \
    typedef char GLchar; \
    typedef char GLcharARB; \
    typedef unsigned char GLubyte; \
    typedef int GLbitfield; \
    typedef int GLsizei; \
    typedef unsigned int GLenum; \
    typedef unsigned int GLuint; \
    typedef unsigned int GLhandleARB; \
    typedef float GLfloat; \
    typedef float GLclampf; \
    typedef ptrdiff_t GLsizeiptr; \
    typedef ptrdiff_t GLintptr

#ifdef _MSC_VER
NVFX_GLEW_DECLARE_TYPES;
#endif /* _MSC_VER */

namespace nvFX
{

#ifndef _MSC_VER
NVFX_GLEW_DECLARE_TYPES;
#endif /* _MSC_VER */

static const GLenum GL_ACTIVE_PROGRAM = 0x8259;
static const GLenum GL_ACTIVE_SUBROUTINES = 0x8DE5;
static const GLenum GL_ACTIVE_SUBROUTINE_MAX_LENGTH = 0x8E48;
static const GLenum GL_ACTIVE_SUBROUTINE_UNIFORMS = 0x8DE6;
static const GLenum GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS = 0x8E47;
static const GLenum GL_ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH = 0x8E49;
static const GLenum GL_ACTIVE_UNIFORM_BLOCKS = 0x8A36;
static const GLenum GL_ALPHA = 0x1906;
static const GLenum GL_ALPHA_TEST = 0x0BC0;
static const GLenum GL_ALPHA_TEST_FUNC = 0x0BC1;
static const GLenum GL_ALPHA_TEST_REF = 0x0BC2;
static const GLenum GL_ARB_shader_subroutine = 1;
static const GLenum GL_ARRAY_BUFFER = 0x8892;
static const GLenum GL_BACK = 0x0405;
static const GLenum GL_BGRA = 0x80E1;
static const GLenum GL_BLEND = 0x0BE2;
static const GLenum GL_BLEND_COLOR = 0x8005;
static const GLenum GL_BLEND_DST = 0x0BE0;
static const GLenum GL_BLEND_DST_ALPHA = 0x80CA;
static const GLenum GL_BLEND_DST_RGB = 0x80C8;
static const GLenum GL_BLEND_EQUATION = 0x8009;
static const GLenum GL_BLEND_EQUATION_ALPHA = 0x883D;
static const GLenum GL_BLEND_EQUATION_RGB = GL_BLEND_EQUATION;
// GL_BLEND_EQUATION_RGBA_NV is not found
static const GLenum GL_BLEND_SRC = 0x0BE1;
static const GLenum GL_BLEND_SRC_ALPHA = 0x80CB;
static const GLenum GL_BLEND_SRC_RGB = 0x80C9;
static const GLenum GL_CLAMP = 0x2900;
static const GLenum GL_CLAMP_TO_EDGE = 0x812F;
static const GLenum GL_COLOR_ATTACHMENT0 = 0x8CE0;
static const GLenum GL_COLOR_ATTACHMENT0_EXT = 0x8CE0;
static const GLenum GL_COLOR_ATTACHMENT1 = 0x8CE1;
static const GLenum GL_COLOR_ATTACHMENT2 = 0x8CE2;
static const GLenum GL_COLOR_ATTACHMENT3 = 0x8CE3;
static const GLenum GL_COLOR_ATTACHMENT4 = 0x8CE4;
static const GLenum GL_COLOR_ATTACHMENT5 = 0x8CE5;
static const GLenum GL_COLOR_ATTACHMENT6 = 0x8CE6;
static const GLenum GL_COLOR_BUFFER_BIT = 0x00004000;
static const GLenum GL_COLOR_LOGIC_OP = 0x0BF2;
static const GLenum GL_COLOR_WRITEMASK = 0x0C23;
// GL_COMBINED_LINE_STIPPLE_NV is not found
static const GLenum GL_COMPILE_STATUS = 0x8B81;
static const GLenum GL_COMPUTE_SHADER = 0x91B9;
static const GLenum GL_CULL_FACE = 0x0B44;
static const GLenum GL_CULL_FACE_MODE = 0x0B45;
static const GLenum GL_CURRENT_PROGRAM = 0x8B8D;
static const GLenum GL_DEPTH24_STENCIL8 = 0x88F0;
// GL_DEPTH32F_STENCIL8_ARB is not found
static const GLenum GL_DEPTH_ATTACHMENT = 0x8D00;
static const GLenum GL_DEPTH_BOUNDS_EXT = 0x8891;
static const GLenum GL_DEPTH_BOUNDS_TEST_EXT = 0x8890;
static const GLenum GL_DEPTH_BUFFER_BIT = 0x00000100;
static const GLenum GL_DEPTH_CLAMP = 0x864F;
static const GLenum GL_DEPTH_COMPONENT = 0x1902;
// GL_DEPTH_COMPONENT32F_ARB is not found
static const GLenum GL_DEPTH_COMPONENT32F_NV = 0x8DAB;
static const GLenum GL_DEPTH_FUNC = 0x0B74;
static const GLenum GL_DEPTH_TEST = 0x0B71;
static const GLenum GL_DEPTH_TEXTURE_MODE = 0x884B;
static const GLenum GL_DEPTH_WRITEMASK = 0x0B72;
static const GLenum GL_DITHER = 0x0BD0;
static const GLenum GL_DRAW_FRAMEBUFFER = 0x8CA9;
static const GLenum GL_DYNAMIC_DRAW = 0x88E8;
static const GLenum GL_FALSE = 0;
static const GLenum GL_FLOAT = 0x1406;
static const GLenum GL_FLOAT_RG16_NV = 0x8886;
static const GLenum GL_FLOAT_RG32_NV = 0x8887;
static const GLenum GL_FLOAT_RGBA16_NV = 0x888A;
static const GLenum GL_FRAGMENT_SHADER = 0x8B30;
static const GLenum GL_FRAGMENT_SHADER_ARB = 0x8B30;
static const GLenum GL_FRAMEBUFFER = 0x8D40;
static const GLenum GL_FRAMEBUFFER_COMPLETE = 0x8CD5;
static const GLenum GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT = 0x8CD6;
// GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS is not found
static const GLenum GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT = 0x8CD9;
static const GLenum GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER = 0x8CDB;
static const GLenum GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT = 0x8CDA;
static const GLenum GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT = 0x8CD7;
static const GLenum GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER = 0x8CDC;
static const GLenum GL_FRAMEBUFFER_SRGB = 0x8DB9;
static const GLenum GL_FRAMEBUFFER_UNSUPPORTED = 0x8CDD;
static const GLenum GL_FRONT = 0x0404;
static const GLenum GL_FRONT_FACE = 0x0B46;
static const GLenum GL_GEOMETRY_SHADER_ARB = 0x8DD9;
static const GLenum GL_INT = 0x1404;
static const GLenum GL_INTENSITY16F_ARB = 0x881D;
static const GLenum GL_INTENSITY32F_ARB = 0x8817;
static const GLenum GL_INTENSITY8 = 0x804B;
static const GLenum GL_INVALID_ENUM = 0x0500;
static const GLenum GL_INVALID_OPERATION = 0x0502;
static const GLenum GL_INVALID_VALUE = 0x0501;
static const GLenum GL_LINEAR = 0x2601;
static const GLenum GL_LINEAR_MIPMAP_LINEAR = 0x2703;
static const GLenum GL_LINEAR_MIPMAP_NEAREST = 0x2701;
static const GLenum GL_LINE_SMOOTH = 0x0B20;
static const GLenum GL_LINE_STIPPLE = 0x0B24;
static const GLenum GL_LINE_STIPPLE_PATTERN = 0x0B25;
static const GLenum GL_LINE_STIPPLE_REPEAT = 0x0B26;
static const GLenum GL_LINE_WIDTH = 0x0B21;
static const GLenum GL_LINK_STATUS = 0x8B82;
static const GLenum GL_LOGIC_OP_MODE = 0x0BF0;
static const GLenum GL_LUMINANCE = 0x1909;
static const GLenum GL_LUMINANCE8_ALPHA8 = 0x8045;
static const GLenum GL_LUMINANCE_ALPHA = 0x190A;
static const GLenum GL_LUMINANCE_ALPHA16F_ARB = 0x881F;
static const GLenum GL_LUMINANCE_ALPHA32F_ARB = 0x8819;
static const GLenum GL_MAP_INVALIDATE_RANGE_BIT = 0x0004;
static const GLenum GL_MAP_UNSYNCHRONIZED_BIT = 0x0020;
static const GLenum GL_MAP_WRITE_BIT = 0x0002;
static const GLenum GL_MAX_VERTEX_ATTRIBS = 0x8869;
static const GLenum GL_MULTISAMPLE = 0x809D;
static const GLenum GL_NEAREST = 0x2600;
static const GLenum GL_NEAREST_MIPMAP_LINEAR = 0x2702;
static const GLenum GL_NEAREST_MIPMAP_NEAREST = 0x2700;
static const GLenum GL_NONE = 0;
static const GLenum GL_NO_ERROR = 0;
static const GLenum GL_NV_float_buffer = 1;
static const GLenum GL_OUT_OF_MEMORY = 0x0505;
static const GLenum GL_PATH_CLIENT_LENGTH_NV = 0x907F;
static const GLenum GL_PATH_COVER_DEPTH_FUNC_NV = 0x90BF;
static const GLenum GL_PATH_DASH_OFFSET_NV = 0x907E;
static const GLenum GL_PATH_DASH_OFFSET_RESET_NV = 0x90B4;
static const GLenum GL_PATH_FILL_COVER_MODE_NV = 0x9082;
static const GLenum GL_PATH_FILL_MASK_NV = 0x9081;
static const GLenum GL_PATH_FILL_MODE_NV = 0x9080;
static const GLenum GL_PATH_FOG_GEN_MODE_NV = 0x90AC;
static const GLenum GL_PATH_FORMAT_SVG_NV = 0x9070;
static const GLenum GL_PATH_INITIAL_DASH_CAP_NV = 0x907C;
static const GLenum GL_PATH_INITIAL_END_CAP_NV = 0x9077;
static const GLenum GL_PATH_JOIN_STYLE_NV = 0x9079;
static const GLenum GL_PATH_MITER_LIMIT_NV = 0x907A;
static const GLenum GL_PATH_STENCIL_DEPTH_OFFSET_FACTOR_NV = 0x90BD;
static const GLenum GL_PATH_STENCIL_DEPTH_OFFSET_UNITS_NV = 0x90BE;
static const GLenum GL_PATH_STENCIL_FUNC_NV = 0x90B7;
static const GLenum GL_PATH_STROKE_COVER_MODE_NV = 0x9083;
static const GLenum GL_PATH_STROKE_MASK_NV = 0x9084;
static const GLenum GL_PATH_STROKE_WIDTH_NV = 0x9075;
static const GLenum GL_PATH_TERMINAL_DASH_CAP_NV = 0x907D;
static const GLenum GL_PATH_TERMINAL_END_CAP_NV = 0x9078;
// GL_PER_DRAW_BUFFER_BLEND_ENABLE_NV is not found
// GL_PER_DRAW_BUFFER_BLEND_EQUATION_NV is not found
// GL_PER_DRAW_BUFFER_BLEND_FUNCTION_NV is not found
// GL_PER_DRAW_BUFFER_COLOR_WRITEMASK_NV is not found
static const GLenum GL_POINT_SIZE = 0x0B11;
static const GLenum GL_POINT_SMOOTH = 0x0B10;
static const GLenum GL_POINT_SPRITE = 0x8861;
static const GLenum GL_POINT_SPRITE_COORD_ORIGIN = 0x8CA0;
static const GLenum GL_POLYGON_MODE = 0x0B40;
static const GLenum GL_POLYGON_OFFSET_FACTOR = 0x8038;
static const GLenum GL_POLYGON_OFFSET_FILL = 0x8037;
static const GLenum GL_POLYGON_OFFSET_LINE = 0x2A02;
static const GLenum GL_POLYGON_OFFSET_POINT = 0x2A01;
static const GLenum GL_POLYGON_OFFSET_UNITS = 0x2A00;
static const GLenum GL_POLYGON_SMOOTH = 0x0B41;
static const GLenum GL_POLYGON_STIPPLE = 0x0B42;
static const GLenum GL_PROGRAM_BINARY_LENGTH = 0x8741;
static const GLenum GL_PROGRAM_POINT_SIZE = 0x8642;
static const GLenum GL_PROGRAM_SEPARABLE = 0x8258;
static const GLenum GL_R16 = 0x822A;
static const GLenum GL_R16F = 0x822D;
static const GLenum GL_R16I = 0x8233;
static const GLenum GL_R16UI = 0x8234;
static const GLenum GL_R32F = 0x822E;
static const GLenum GL_R32I = 0x8235;
static const GLenum GL_R32UI = 0x8236;
static const GLenum GL_R8 = 0x8229;
static const GLenum GL_R8I = 0x8231;
static const GLenum GL_R8UI = 0x8232;
static const GLenum GL_RASTERIZER_DISCARD = 0x8C89;
static const GLenum GL_READ_FRAMEBUFFER = 0x8CA8;
static const GLenum GL_READ_ONLY = 0x88B8;
static const GLenum GL_READ_WRITE = 0x88BA;
static const GLenum GL_RED = 0x1903;
static const GLenum GL_RED_INTEGER = 0x8D94;
static const GLenum GL_RENDERBUFFER = 0x8D41;
static const GLenum GL_RENDERBUFFER_COLOR_SAMPLES_NV = 0x8E10;
static const GLenum GL_RENDERBUFFER_COVERAGE_SAMPLES_NV = 0x8CAB;
static const GLenum GL_RENDERBUFFER_SAMPLES = 0x8CAB;
static const GLenum GL_REPEAT = 0x2901;
static const GLenum GL_RG = 0x8227;
static const GLenum GL_RG16 = 0x822C;
static const GLenum GL_RG16F = 0x822F;
static const GLenum GL_RG16I = 0x8239;
static const GLenum GL_RG16UI = 0x823A;
static const GLenum GL_RG32F = 0x8230;
static const GLenum GL_RG32I = 0x823B;
static const GLenum GL_RG32UI = 0x823C;
static const GLenum GL_RG8 = 0x822B;
static const GLenum GL_RG8I = 0x8237;
static const GLenum GL_RG8UI = 0x8238;
static const GLenum GL_RGB = 0x1907;
static const GLenum GL_RGB16F_ARB = 0x881B;
static const GLenum GL_RGB32F_ARB = 0x8815;
static const GLenum GL_RGB8 = 0x8051;
static const GLenum GL_RGBA = 0x1908;
static const GLenum GL_RGBA16F_ARB = 0x881A;
static const GLenum GL_RGBA32F_ARB = 0x8814;
static const GLenum GL_RGBA8 = 0x8058;
static const GLenum GL_RGBA8UI = 0x8D7C;
static const GLenum GL_RGBA_INTEGER = 0x8D99;
static const GLenum GL_RG_INTEGER = 0x8228;
static const GLenum GL_SAMPLE_ALPHA_TO_COVERAGE = 0x809E;
static const GLenum GL_SAMPLE_ALPHA_TO_ONE = 0x809F;
static const GLenum GL_SAMPLE_MASK = 0x8E51;
static const GLenum GL_SAMPLE_MASK_VALUE = 0x8E52;
static const GLenum GL_STACK_OVERFLOW = 0x0503;
static const GLenum GL_STACK_UNDERFLOW = 0x0504;
static const GLenum GL_STENCIL_ATTACHMENT = 0x8D20;
static const GLenum GL_STENCIL_BACK_FAIL = 0x8801;
static const GLenum GL_STENCIL_BACK_FUNC = 0x8800;
static const GLenum GL_STENCIL_BACK_PASS_DEPTH_FAIL = 0x8802;
static const GLenum GL_STENCIL_BACK_PASS_DEPTH_PASS = 0x8803;
static const GLenum GL_STENCIL_BACK_REF = 0x8CA3;
static const GLenum GL_STENCIL_BACK_VALUE_MASK = 0x8CA4;
static const GLenum GL_STENCIL_BACK_WRITEMASK = 0x8CA5;
static const GLenum GL_STENCIL_BUFFER_BIT = 0x00000400;
static const GLenum GL_STENCIL_FAIL = 0x0B94;
// GL_STENCIL_FRONT_FAIL is not found
// GL_STENCIL_FRONT_FUNC is not found
// GL_STENCIL_FRONT_PASS_DEPTH_FAIL is not found
// GL_STENCIL_FRONT_PASS_DEPTH_PASS is not found
// GL_STENCIL_FRONT_REF is not found
// GL_STENCIL_FRONT_VALUE_MASK is not found
// GL_STENCIL_FRONT_WRITEMASK is not found
static const GLenum GL_STENCIL_FUNC = 0x0B92;
static const GLenum GL_STENCIL_PASS_DEPTH_FAIL = 0x0B95;
static const GLenum GL_STENCIL_PASS_DEPTH_PASS = 0x0B96;
static const GLenum GL_STENCIL_REF = 0x0B97;
static const GLenum GL_STENCIL_TEST = 0x0B90;
static const GLenum GL_STENCIL_VALUE_MASK = 0x0B93;
static const GLenum GL_STENCIL_WRITEMASK = 0x0B98;
static const GLenum GL_TESS_CONTROL_SHADER = 0x8E88;
static const GLenum GL_TESS_EVALUATION_SHADER = 0x8E87;
static const GLenum GL_TEXTURE0 = 0x84C0;
static const GLenum GL_TEXTURE_1D = 0x0DE0;
static const GLenum GL_TEXTURE_2D = 0x0DE1;
static const GLenum GL_TEXTURE_2D_MULTISAMPLE = 0x9100;
static const GLenum GL_TEXTURE_3D = 0x806F;
static const GLenum GL_TEXTURE_CUBE_MAP = 0x8513;
static const GLenum GL_TEXTURE_MAG_FILTER = 0x2800;
static const GLenum GL_TEXTURE_MIN_FILTER = 0x2801;
static const GLenum GL_TEXTURE_RECTANGLE = 0x84F5;
static const GLenum GL_TEXTURE_WRAP_R = 0x8072;
static const GLenum GL_TEXTURE_WRAP_S = 0x2802;
static const GLenum GL_TEXTURE_WRAP_T = 0x2803;
static const GLenum GL_TRIANGLE_STRIP = 0x0005;
static const GLenum GL_TRUE = 1;
static const GLenum GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS = 0x8A42;
static const GLenum GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES = 0x8A43;
static const GLenum GL_UNIFORM_BLOCK_BINDING = 0x8A3F;
static const GLenum GL_UNIFORM_BUFFER = 0x8A11;
static const GLenum GL_UNSIGNED_BYTE = 0x1401;
static const GLenum GL_UNSIGNED_INT = 0x1405;
static const GLenum GL_UTF8_NV = 0x909A;
static const GLenum GL_VENDOR = 0x1F00;
static const GLenum GL_VERTEX_SHADER = 0x8B31;
static const GLenum GL_VERTEX_SHADER_ARB = 0x8B31;
static const GLenum GL_VERTEX_SHADER_BIT = 0x00000001;
static const GLenum GL_WRITE_ONLY = 0x88B9;

typedef void (NVFX_GLEW_APIENTRY PFNGLACTIVETEXTUREPROC) (GLenum texture);
typedef void (NVFX_GLEW_APIENTRY PFNGLALPHAFUNCPROC)(GLenum, GLfloat);
typedef void (NVFX_GLEW_APIENTRY PFNGLATTACHOBJECTARBPROC) (GLhandleARB containerObj, GLhandleARB obj);
typedef void (NVFX_GLEW_APIENTRY PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (NVFX_GLEW_APIENTRY PFNGLBINDATTRIBLOCATIONPROC) (GLuint program, GLuint index, const GLchar* name);
typedef void (NVFX_GLEW_APIENTRY PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (NVFX_GLEW_APIENTRY PFNGLBINDBUFFERBASEPROC) (GLenum target, GLuint index, GLuint buffer);
typedef void (NVFX_GLEW_APIENTRY PFNGLBINDBUFFERRANGEPROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (NVFX_GLEW_APIENTRY PFNGLBINDFRAMEBUFFERPROC) (GLenum target, GLuint framebuffer);
typedef void (NVFX_GLEW_APIENTRY PFNGLBINDIMAGETEXTUREPROC) (GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
typedef void (NVFX_GLEW_APIENTRY PFNGLBINDPROGRAMPIPELINEPROC) (GLuint pipeline);
typedef void (NVFX_GLEW_APIENTRY PFNGLBINDRENDERBUFFERPROC) (GLenum target, GLuint renderbuffer);
typedef void (NVFX_GLEW_APIENTRY PFNGLBINDSAMPLERPROC) (GLuint unit, GLuint sampler);
typedef void (NVFX_GLEW_APIENTRY PFNGLBINDTEXTUREPROC)(GLenum, GLuint);
typedef void (NVFX_GLEW_APIENTRY PFNGLBINDVERTEXARRAYPROC) (GLuint array);
typedef void (NVFX_GLEW_APIENTRY PFNGLBLENDCOLORPROC) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void (NVFX_GLEW_APIENTRY PFNGLBLENDEQUATIONSEPARATEPROC) (GLenum, GLenum);
typedef void (NVFX_GLEW_APIENTRY PFNGLBLENDFUNCPROC)(GLenum, GLenum);
typedef void (NVFX_GLEW_APIENTRY PFNGLBLENDFUNCSEPARATEPROC) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
typedef void (NVFX_GLEW_APIENTRY PFNGLBLITFRAMEBUFFERPROC) (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef void (NVFX_GLEW_APIENTRY PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
typedef void (NVFX_GLEW_APIENTRY PFNGLBUFFERSUBDATAPROC) (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data);
typedef GLenum (NVFX_GLEW_APIENTRY PFNGLCHECKFRAMEBUFFERSTATUSPROC) (GLenum target);
typedef void (NVFX_GLEW_APIENTRY PFNGLCLEARPROC)(GLbitfield);
typedef void (NVFX_GLEW_APIENTRY PFNGLCLEARCOLORPROC)(GLfloat, GLfloat, GLfloat, GLfloat);
typedef void (NVFX_GLEW_APIENTRY PFNGLCOLORMASKPROC)(GLboolean, GLboolean, GLboolean, GLboolean);
typedef void (NVFX_GLEW_APIENTRY PFNGLCOMPILESHADERPROC) (GLuint shader);
typedef void (NVFX_GLEW_APIENTRY PFNGLCOVERFILLPATHNVPROC) (GLuint path, GLenum coverMode);
typedef void (NVFX_GLEW_APIENTRY PFNGLCOVERSTROKEPATHNVPROC) (GLuint name, GLenum coverMode);
typedef GLuint (NVFX_GLEW_APIENTRY PFNGLCREATEPROGRAMPROC) (void);
typedef GLuint (NVFX_GLEW_APIENTRY PFNGLCREATESHADERPROC) (GLenum type);
typedef GLhandleARB (NVFX_GLEW_APIENTRY PFNGLCREATESHADEROBJECTARBPROC) (GLenum shaderType);
typedef void (NVFX_GLEW_APIENTRY PFNGLCULLFACEPROC)(GLenum);
typedef void (NVFX_GLEW_APIENTRY PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint* buffers);
typedef void (NVFX_GLEW_APIENTRY PFNGLDELETEFRAMEBUFFERSPROC) (GLsizei n, const GLuint* framebuffers);
typedef void (NVFX_GLEW_APIENTRY PFNGLDELETEOBJECTARBPROC) (GLhandleARB obj);
typedef void (NVFX_GLEW_APIENTRY PFNGLDELETEPATHSNVPROC) (GLuint path, GLsizei range);
typedef void (NVFX_GLEW_APIENTRY PFNGLDELETEPROGRAMPROC) (GLuint program);
typedef void (NVFX_GLEW_APIENTRY PFNGLDELETEPROGRAMPIPELINESPROC) (GLsizei n, const GLuint* pipelines);
typedef void (NVFX_GLEW_APIENTRY PFNGLDELETERENDERBUFFERSPROC) (GLsizei n, const GLuint* renderbuffers);
typedef void (NVFX_GLEW_APIENTRY PFNGLDELETESHADERPROC) (GLuint shader);
typedef void (NVFX_GLEW_APIENTRY PFNGLDELETETEXTURESPROC)(GLsizei, const GLuint *);
typedef void (NVFX_GLEW_APIENTRY PFNGLDEPTHFUNCPROC)(GLenum);
typedef void (NVFX_GLEW_APIENTRY PFNGLDEPTHMASKPROC)(GLboolean);
typedef void (NVFX_GLEW_APIENTRY PFNGLDETACHOBJECTARBPROC) (GLhandleARB containerObj, GLhandleARB attachedObj);
typedef void (NVFX_GLEW_APIENTRY PFNGLDETACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (NVFX_GLEW_APIENTRY PFNGLDISABLEPROC)(GLenum);
typedef void (NVFX_GLEW_APIENTRY PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint);
typedef void (NVFX_GLEW_APIENTRY PFNGLDRAWARRAYSPROC)(GLenum, GLint, GLsizei);
typedef void (NVFX_GLEW_APIENTRY PFNGLDRAWBUFFERPROC)(GLenum);
typedef void (NVFX_GLEW_APIENTRY PFNGLDRAWBUFFERSPROC) (GLsizei n, const GLenum* bufs);
typedef void (NVFX_GLEW_APIENTRY PFNGLENABLEPROC)(GLenum);
typedef void (NVFX_GLEW_APIENTRY PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint);
typedef void (NVFX_GLEW_APIENTRY PFNGLFRAMEBUFFERRENDERBUFFERPROC) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (NVFX_GLEW_APIENTRY PFNGLFRAMEBUFFERTEXTURE2DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (NVFX_GLEW_APIENTRY PFNGLFRONTFACEPROC)(GLenum);
typedef void (NVFX_GLEW_APIENTRY PFNGLGENBUFFERSPROC) (GLsizei n, GLuint* buffers);
typedef void (NVFX_GLEW_APIENTRY PFNGLGENFRAMEBUFFERSPROC) (GLsizei n, GLuint* framebuffers);
typedef GLuint (NVFX_GLEW_APIENTRY PFNGLGENPATHSNVPROC) (GLsizei range);
typedef void (NVFX_GLEW_APIENTRY PFNGLGENPROGRAMPIPELINESPROC) (GLsizei n, GLuint* pipelines);
typedef void (NVFX_GLEW_APIENTRY PFNGLGENRENDERBUFFERSPROC) (GLsizei n, GLuint* renderbuffers);
typedef void (NVFX_GLEW_APIENTRY PFNGLGENTEXTURESPROC)(GLsizei, GLuint *);
typedef void (NVFX_GLEW_APIENTRY PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint* arrays);
typedef void (NVFX_GLEW_APIENTRY PFNGLGETACTIVEUNIFORMPROC) (GLuint program, GLuint index, GLsizei maxLength, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
typedef void (NVFX_GLEW_APIENTRY PFNGLGETACTIVEUNIFORMBLOCKIVPROC) (GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params);
typedef GLint (NVFX_GLEW_APIENTRY PFNGLGETATTRIBLOCATIONPROC) (GLuint program, const GLchar* name);
typedef GLenum (NVFX_GLEW_APIENTRY PFNGLGETERRORPROC)();
typedef void (NVFX_GLEW_APIENTRY PFNGLGETINFOLOGARBPROC) (GLhandleARB obj, GLsizei maxLength, GLsizei* length, GLcharARB *infoLog);
typedef void (NVFX_GLEW_APIENTRY PFNGLGETINTEGERVPROC)(GLenum, GLint *);
typedef void (NVFX_GLEW_APIENTRY PFNGLGETPROGRAMBINARYPROC) (GLuint program, GLsizei bufSize, GLsizei* length, GLenum *binaryFormat, GLvoid *binary);
typedef void (NVFX_GLEW_APIENTRY PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef void (NVFX_GLEW_APIENTRY PFNGLGETPROGRAMPIPELINEIVPROC) (GLuint pipeline, GLenum pname, GLint* params);
typedef void (NVFX_GLEW_APIENTRY PFNGLGETPROGRAMSTAGEIVPROC) (GLuint program, GLenum shadertype, GLenum pname, GLint* values);
typedef void (NVFX_GLEW_APIENTRY PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint* param);
typedef void (NVFX_GLEW_APIENTRY PFNGLGETRENDERBUFFERPARAMETERIVPROC) (GLenum target, GLenum pname, GLint* params);
typedef void (NVFX_GLEW_APIENTRY PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef void (NVFX_GLEW_APIENTRY PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint* param);
typedef const GLubyte * (NVFX_GLEW_APIENTRY PFNGLGETSTRINGPROC) (GLenum name);
typedef GLuint (NVFX_GLEW_APIENTRY PFNGLGETSUBROUTINEINDEXPROC) (GLuint program, GLenum shadertype, const GLchar* name);
typedef GLint (NVFX_GLEW_APIENTRY PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC) (GLuint program, GLenum shadertype, const GLchar* name);
typedef GLuint (NVFX_GLEW_APIENTRY PFNGLGETUNIFORMBLOCKINDEXPROC) (GLuint program, const GLchar* uniformBlockName);
typedef GLint (NVFX_GLEW_APIENTRY PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const GLchar* name);
typedef GLboolean (NVFX_GLEW_APIENTRY PFNGLISPROGRAMPIPELINEPROC) (GLuint pipeline);
typedef void (NVFX_GLEW_APIENTRY PFNGLLINEWIDTHPROC)(GLfloat);
typedef void (NVFX_GLEW_APIENTRY PFNGLLINKPROGRAMPROC) (GLuint program);
typedef void (NVFX_GLEW_APIENTRY PFNGLLOGICOPPROC)(GLenum);
typedef GLvoid* (NVFX_GLEW_APIENTRY PFNGLMAPBUFFERPROC) (GLenum target, GLenum access);
typedef GLvoid* (NVFX_GLEW_APIENTRY PFNGLMAPBUFFERRANGEPROC) (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef void (NVFX_GLEW_APIENTRY PFNGLPATHPARAMETERFNVPROC) (GLuint path, GLenum pname, GLfloat value);
typedef void (NVFX_GLEW_APIENTRY PFNGLPATHPARAMETERINVPROC) (GLuint path, GLenum pname, GLint value);
typedef void (NVFX_GLEW_APIENTRY PFNGLPATHSTENCILDEPTHOFFSETNVPROC) (GLfloat factor, GLfloat units);
typedef void (NVFX_GLEW_APIENTRY PFNGLPATHSTRINGNVPROC) (GLuint path, GLenum format, GLsizei length, const void* pathString);
typedef void (NVFX_GLEW_APIENTRY PFNGLPOINTSIZEPROC)(GLfloat);
typedef void (NVFX_GLEW_APIENTRY PFNGLPOLYGONMODEPROC)(GLenum, GLenum);
typedef void (NVFX_GLEW_APIENTRY PFNGLPOLYGONOFFSETPROC)(GLfloat, GLfloat);
typedef void (NVFX_GLEW_APIENTRY PFNGLPROGRAMPARAMETERIPROC) (GLuint program, GLenum pname, GLint value);
typedef void (NVFX_GLEW_APIENTRY PFNGLRENDERBUFFERSTORAGEPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (NVFX_GLEW_APIENTRY PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (NVFX_GLEW_APIENTRY PFNGLRENDERBUFFERSTORAGEMULTISAMPLECOVERAGENVPROC) (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (NVFX_GLEW_APIENTRY PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const GLchar** strings, const GLint* lengths);
typedef void (NVFX_GLEW_APIENTRY PFNGLSTENCILFILLPATHINSTANCEDNVPROC) (GLsizei numPaths, GLenum pathNameType, const void* paths, GLuint pathBase, GLenum fillMode, GLuint mask, GLenum transformType, const GLfloat *transformValues);
typedef void (NVFX_GLEW_APIENTRY PFNGLSTENCILFILLPATHNVPROC) (GLuint path, GLenum fillMode, GLuint mask);
typedef void (NVFX_GLEW_APIENTRY PFNGLSTENCILFUNCSEPARATEPROC) (GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);
typedef void (NVFX_GLEW_APIENTRY PFNGLSTENCILOPSEPARATEPROC) (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
typedef void (NVFX_GLEW_APIENTRY PFNGLSTENCILSTROKEPATHNVPROC) (GLuint path, GLint reference, GLuint mask);
typedef void (NVFX_GLEW_APIENTRY PFNGLTEXIMAGE1DPROC)(GLenum, GLint, GLint, GLsizei, GLint, GLenum, GLenum, const GLvoid *);
typedef void (NVFX_GLEW_APIENTRY PFNGLTEXIMAGE2DPROC)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *);
typedef void (NVFX_GLEW_APIENTRY PFNGLTEXIMAGE2DMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void (NVFX_GLEW_APIENTRY PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (NVFX_GLEW_APIENTRY PFNGLTEXPARAMETERFPROC)(GLenum, GLenum, GLfloat);
typedef void (NVFX_GLEW_APIENTRY PFNGLTEXPARAMETERIPROC)(GLenum, GLenum, GLint);
typedef void (NVFX_GLEW_APIENTRY PFNGLTEXTUREPARAMETERIEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLint param);
typedef void (NVFX_GLEW_APIENTRY PFNGLUNIFORM1FPROC) (GLint location, GLfloat v0);
typedef void (NVFX_GLEW_APIENTRY PFNGLUNIFORM1FVPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (NVFX_GLEW_APIENTRY PFNGLUNIFORM1IPROC) (GLint location, GLint v0);
typedef void (NVFX_GLEW_APIENTRY PFNGLUNIFORM1IVPROC) (GLint location, GLsizei count, const GLint* value);
typedef void (NVFX_GLEW_APIENTRY PFNGLUNIFORM2FVPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (NVFX_GLEW_APIENTRY PFNGLUNIFORM2IVPROC) (GLint location, GLsizei count, const GLint* value);
typedef void (NVFX_GLEW_APIENTRY PFNGLUNIFORM3FVPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (NVFX_GLEW_APIENTRY PFNGLUNIFORM3IVPROC) (GLint location, GLsizei count, const GLint* value);
typedef void (NVFX_GLEW_APIENTRY PFNGLUNIFORM4FVPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (NVFX_GLEW_APIENTRY PFNGLUNIFORM4IVPROC) (GLint location, GLsizei count, const GLint* value);
typedef void (NVFX_GLEW_APIENTRY PFNGLUNIFORMBLOCKBINDINGPROC) (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
typedef void (NVFX_GLEW_APIENTRY PFNGLUNIFORMSUBROUTINESUIVPROC) (GLenum shadertype, GLsizei count, const GLuint* indices);
typedef GLboolean (NVFX_GLEW_APIENTRY PFNGLUNMAPBUFFERPROC) (GLenum target);
typedef void (NVFX_GLEW_APIENTRY PFNGLUSEPROGRAMPROC) (GLuint program);
typedef void (NVFX_GLEW_APIENTRY PFNGLUSEPROGRAMSTAGESPROC) (GLuint pipeline, GLbitfield stages, GLuint program);
typedef void (NVFX_GLEW_APIENTRY PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
typedef void (NVFX_GLEW_APIENTRY PFNGLVIEWPORTPROC)(GLint, GLint, GLsizei, GLsizei);

NVFX_GLEW_APIENTRY_EXPORT PFNGLACTIVETEXTUREPROC glActiveTexture;
NVFX_GLEW_APIENTRY_EXPORT PFNGLALPHAFUNCPROC glAlphaFunc;
NVFX_GLEW_APIENTRY_EXPORT PFNGLATTACHOBJECTARBPROC glAttachObjectARB;
NVFX_GLEW_APIENTRY_EXPORT PFNGLATTACHSHADERPROC glAttachShader;
NVFX_GLEW_APIENTRY_EXPORT PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
NVFX_GLEW_APIENTRY_EXPORT PFNGLBINDBUFFERPROC glBindBuffer;
NVFX_GLEW_APIENTRY_EXPORT PFNGLBINDBUFFERBASEPROC glBindBufferBase;
NVFX_GLEW_APIENTRY_EXPORT PFNGLBINDBUFFERRANGEPROC glBindBufferRange;
NVFX_GLEW_APIENTRY_EXPORT PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
NVFX_GLEW_APIENTRY_EXPORT PFNGLBINDIMAGETEXTUREPROC glBindImageTexture;
NVFX_GLEW_APIENTRY_EXPORT PFNGLBINDPROGRAMPIPELINEPROC glBindProgramPipeline;
NVFX_GLEW_APIENTRY_EXPORT PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
NVFX_GLEW_APIENTRY_EXPORT PFNGLBINDSAMPLERPROC glBindSampler;
NVFX_GLEW_APIENTRY_EXPORT PFNGLBINDTEXTUREPROC glBindTexture;
NVFX_GLEW_APIENTRY_EXPORT PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
NVFX_GLEW_APIENTRY_EXPORT PFNGLBLENDCOLORPROC glBlendColor;
NVFX_GLEW_APIENTRY_EXPORT PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate;
NVFX_GLEW_APIENTRY_EXPORT PFNGLBLENDFUNCPROC glBlendFunc;
NVFX_GLEW_APIENTRY_EXPORT PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate;
NVFX_GLEW_APIENTRY_EXPORT PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer;
NVFX_GLEW_APIENTRY_EXPORT PFNGLBUFFERDATAPROC glBufferData;
NVFX_GLEW_APIENTRY_EXPORT PFNGLBUFFERSUBDATAPROC glBufferSubData;
NVFX_GLEW_APIENTRY_EXPORT PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
NVFX_GLEW_APIENTRY_EXPORT PFNGLCLEARPROC glClear;
NVFX_GLEW_APIENTRY_EXPORT PFNGLCLEARCOLORPROC glClearColor;
NVFX_GLEW_APIENTRY_EXPORT PFNGLCOLORMASKPROC glColorMask;
NVFX_GLEW_APIENTRY_EXPORT PFNGLCOMPILESHADERPROC glCompileShader;
NVFX_GLEW_APIENTRY_EXPORT PFNGLCOVERFILLPATHNVPROC glCoverFillPathNV;
NVFX_GLEW_APIENTRY_EXPORT PFNGLCOVERSTROKEPATHNVPROC glCoverStrokePathNV;
NVFX_GLEW_APIENTRY_EXPORT PFNGLCREATEPROGRAMPROC glCreateProgram;
NVFX_GLEW_APIENTRY_EXPORT PFNGLCREATESHADERPROC glCreateShader;
NVFX_GLEW_APIENTRY_EXPORT PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB;
NVFX_GLEW_APIENTRY_EXPORT PFNGLCULLFACEPROC glCullFace;
NVFX_GLEW_APIENTRY_EXPORT PFNGLDELETEBUFFERSPROC glDeleteBuffers;
NVFX_GLEW_APIENTRY_EXPORT PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
NVFX_GLEW_APIENTRY_EXPORT PFNGLDELETEOBJECTARBPROC glDeleteObjectARB;
NVFX_GLEW_APIENTRY_EXPORT PFNGLDELETEPATHSNVPROC glDeletePathsNV;
NVFX_GLEW_APIENTRY_EXPORT PFNGLDELETEPROGRAMPROC glDeleteProgram;
NVFX_GLEW_APIENTRY_EXPORT PFNGLDELETEPROGRAMPIPELINESPROC glDeleteProgramPipelines;
NVFX_GLEW_APIENTRY_EXPORT PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
NVFX_GLEW_APIENTRY_EXPORT PFNGLDELETESHADERPROC glDeleteShader;
NVFX_GLEW_APIENTRY_EXPORT PFNGLDELETETEXTURESPROC glDeleteTextures;
NVFX_GLEW_APIENTRY_EXPORT PFNGLDEPTHFUNCPROC glDepthFunc;
NVFX_GLEW_APIENTRY_EXPORT PFNGLDEPTHMASKPROC glDepthMask;
NVFX_GLEW_APIENTRY_EXPORT PFNGLDETACHOBJECTARBPROC glDetachObjectARB;
NVFX_GLEW_APIENTRY_EXPORT PFNGLDETACHSHADERPROC glDetachShader;
NVFX_GLEW_APIENTRY_EXPORT PFNGLDISABLEPROC glDisable;
NVFX_GLEW_APIENTRY_EXPORT PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
NVFX_GLEW_APIENTRY_EXPORT PFNGLDRAWARRAYSPROC glDrawArrays;
NVFX_GLEW_APIENTRY_EXPORT PFNGLDRAWBUFFERPROC glDrawBuffer;
NVFX_GLEW_APIENTRY_EXPORT PFNGLDRAWBUFFERSPROC glDrawBuffers;
NVFX_GLEW_APIENTRY_EXPORT PFNGLENABLEPROC glEnable;
NVFX_GLEW_APIENTRY_EXPORT PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
NVFX_GLEW_APIENTRY_EXPORT PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
NVFX_GLEW_APIENTRY_EXPORT PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
NVFX_GLEW_APIENTRY_EXPORT PFNGLFRONTFACEPROC glFrontFace;
NVFX_GLEW_APIENTRY_EXPORT PFNGLGENBUFFERSPROC glGenBuffers;
NVFX_GLEW_APIENTRY_EXPORT PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
NVFX_GLEW_APIENTRY_EXPORT PFNGLGENPATHSNVPROC glGenPathsNV;
NVFX_GLEW_APIENTRY_EXPORT PFNGLGENPROGRAMPIPELINESPROC glGenProgramPipelines;
NVFX_GLEW_APIENTRY_EXPORT PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
NVFX_GLEW_APIENTRY_EXPORT PFNGLGENTEXTURESPROC glGenTextures;
NVFX_GLEW_APIENTRY_EXPORT PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
NVFX_GLEW_APIENTRY_EXPORT PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform;
NVFX_GLEW_APIENTRY_EXPORT PFNGLGETACTIVEUNIFORMBLOCKIVPROC glGetActiveUniformBlockiv;
NVFX_GLEW_APIENTRY_EXPORT PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
NVFX_GLEW_APIENTRY_EXPORT PFNGLGETERRORPROC glGetError;
NVFX_GLEW_APIENTRY_EXPORT PFNGLGETINFOLOGARBPROC glGetInfoLogARB;
NVFX_GLEW_APIENTRY_EXPORT PFNGLGETINTEGERVPROC glGetIntegerv;
NVFX_GLEW_APIENTRY_EXPORT PFNGLGETPROGRAMBINARYPROC glGetProgramBinary;
NVFX_GLEW_APIENTRY_EXPORT PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
NVFX_GLEW_APIENTRY_EXPORT PFNGLGETPROGRAMPIPELINEIVPROC glGetProgramPipelineiv;
NVFX_GLEW_APIENTRY_EXPORT PFNGLGETPROGRAMSTAGEIVPROC glGetProgramStageiv;
NVFX_GLEW_APIENTRY_EXPORT PFNGLGETPROGRAMIVPROC glGetProgramiv;
NVFX_GLEW_APIENTRY_EXPORT PFNGLGETRENDERBUFFERPARAMETERIVPROC glGetRenderbufferParameteriv;
NVFX_GLEW_APIENTRY_EXPORT PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
NVFX_GLEW_APIENTRY_EXPORT PFNGLGETSHADERIVPROC glGetShaderiv;
NVFX_GLEW_APIENTRY_EXPORT PFNGLGETSTRINGPROC glGetString;
NVFX_GLEW_APIENTRY_EXPORT PFNGLGETSUBROUTINEINDEXPROC glGetSubroutineIndex;
NVFX_GLEW_APIENTRY_EXPORT PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC glGetSubroutineUniformLocation;
NVFX_GLEW_APIENTRY_EXPORT PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex;
NVFX_GLEW_APIENTRY_EXPORT PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
NVFX_GLEW_APIENTRY_EXPORT PFNGLISPROGRAMPIPELINEPROC glIsProgramPipeline;
NVFX_GLEW_APIENTRY_EXPORT PFNGLLINEWIDTHPROC glLineWidth;
NVFX_GLEW_APIENTRY_EXPORT PFNGLLINKPROGRAMPROC glLinkProgram;
NVFX_GLEW_APIENTRY_EXPORT PFNGLLOGICOPPROC glLogicOp;
NVFX_GLEW_APIENTRY_EXPORT PFNGLMAPBUFFERPROC glMapBuffer;
NVFX_GLEW_APIENTRY_EXPORT PFNGLMAPBUFFERRANGEPROC glMapBufferRange;
NVFX_GLEW_APIENTRY_EXPORT PFNGLPATHPARAMETERFNVPROC glPathParameterfNV;
NVFX_GLEW_APIENTRY_EXPORT PFNGLPATHPARAMETERINVPROC glPathParameteriNV;
NVFX_GLEW_APIENTRY_EXPORT PFNGLPATHSTENCILDEPTHOFFSETNVPROC glPathStencilDepthOffsetNV;
NVFX_GLEW_APIENTRY_EXPORT PFNGLPATHSTRINGNVPROC glPathStringNV;
NVFX_GLEW_APIENTRY_EXPORT PFNGLPOINTSIZEPROC glPointSize;
NVFX_GLEW_APIENTRY_EXPORT PFNGLPOLYGONMODEPROC glPolygonMode;
NVFX_GLEW_APIENTRY_EXPORT PFNGLPOLYGONOFFSETPROC glPolygonOffset;
NVFX_GLEW_APIENTRY_EXPORT PFNGLPROGRAMPARAMETERIPROC glProgramParameteri;
NVFX_GLEW_APIENTRY_EXPORT PFNGLPROGRAMPARAMETERIPROC glProgramParameteriARB;
NVFX_GLEW_APIENTRY_EXPORT PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
NVFX_GLEW_APIENTRY_EXPORT PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glRenderbufferStorageMultisample;
NVFX_GLEW_APIENTRY_EXPORT PFNGLRENDERBUFFERSTORAGEMULTISAMPLECOVERAGENVPROC glRenderbufferStorageMultisampleCoverageNV;
NVFX_GLEW_APIENTRY_EXPORT PFNGLSHADERSOURCEPROC glShaderSource;
NVFX_GLEW_APIENTRY_EXPORT PFNGLSTENCILFILLPATHINSTANCEDNVPROC glStencilFillPathInstancedNV;
NVFX_GLEW_APIENTRY_EXPORT PFNGLSTENCILFILLPATHNVPROC glStencilFillPathNV;
NVFX_GLEW_APIENTRY_EXPORT PFNGLSTENCILFUNCSEPARATEPROC glStencilFuncSeparate;
NVFX_GLEW_APIENTRY_EXPORT PFNGLSTENCILOPSEPARATEPROC glStencilOpSeparate;
NVFX_GLEW_APIENTRY_EXPORT PFNGLSTENCILSTROKEPATHNVPROC glStencilStrokePathNV;
NVFX_GLEW_APIENTRY_EXPORT PFNGLTEXIMAGE1DPROC glTexImage1D;
NVFX_GLEW_APIENTRY_EXPORT PFNGLTEXIMAGE2DPROC glTexImage2D;
NVFX_GLEW_APIENTRY_EXPORT PFNGLTEXIMAGE2DMULTISAMPLEPROC glTexImage2DMultisample;
NVFX_GLEW_APIENTRY_EXPORT PFNGLTEXIMAGE3DPROC glTexImage3D;
NVFX_GLEW_APIENTRY_EXPORT PFNGLTEXPARAMETERFPROC glTexParameterf;
NVFX_GLEW_APIENTRY_EXPORT PFNGLTEXPARAMETERIPROC glTexParameteri;
NVFX_GLEW_APIENTRY_EXPORT PFNGLTEXTUREPARAMETERIEXTPROC glTextureParameteriEXT;
NVFX_GLEW_APIENTRY_EXPORT PFNGLUNIFORM1FPROC glUniform1f;
NVFX_GLEW_APIENTRY_EXPORT PFNGLUNIFORM1FVPROC glUniform1fv;
NVFX_GLEW_APIENTRY_EXPORT PFNGLUNIFORM1IPROC glUniform1i;
NVFX_GLEW_APIENTRY_EXPORT PFNGLUNIFORM1IVPROC glUniform1iv;
NVFX_GLEW_APIENTRY_EXPORT PFNGLUNIFORM2FVPROC glUniform2fv;
NVFX_GLEW_APIENTRY_EXPORT PFNGLUNIFORM2IVPROC glUniform2iv;
NVFX_GLEW_APIENTRY_EXPORT PFNGLUNIFORM3FVPROC glUniform3fv;
NVFX_GLEW_APIENTRY_EXPORT PFNGLUNIFORM3IVPROC glUniform3iv;
NVFX_GLEW_APIENTRY_EXPORT PFNGLUNIFORM4FVPROC glUniform4fv;
NVFX_GLEW_APIENTRY_EXPORT PFNGLUNIFORM4IVPROC glUniform4iv;
NVFX_GLEW_APIENTRY_EXPORT PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding;
NVFX_GLEW_APIENTRY_EXPORT PFNGLUNIFORMSUBROUTINESUIVPROC glUniformSubroutinesuiv;
NVFX_GLEW_APIENTRY_EXPORT PFNGLUNMAPBUFFERPROC glUnmapBuffer;
NVFX_GLEW_APIENTRY_EXPORT PFNGLUSEPROGRAMPROC glUseProgram;
NVFX_GLEW_APIENTRY_EXPORT PFNGLUSEPROGRAMSTAGESPROC glUseProgramStages;
NVFX_GLEW_APIENTRY_EXPORT PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
NVFX_GLEW_APIENTRY_EXPORT PFNGLVIEWPORTPROC glViewport;

typedef void (NVFX_GLEW_APIENTRY PFNGLUNIFORMMATRIX4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (NVFX_GLEW_APIENTRY PFNGLPROGRAMUNIFORM1IPROC) (GLuint program, GLint location, GLint v0);
typedef void (NVFX_GLEW_APIENTRY PFNGLPROGRAMUNIFORM1IVPROC) (GLuint program, GLint location, GLsizei count, const GLint *value);
typedef void (NVFX_GLEW_APIENTRY PFNGLPROGRAMUNIFORM1FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
typedef void (NVFX_GLEW_APIENTRY PFNGLPROGRAMUNIFORM2IVPROC) (GLuint program, GLint location, GLsizei count, const GLint *value);
typedef void (NVFX_GLEW_APIENTRY PFNGLPROGRAMUNIFORM2FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
typedef void (NVFX_GLEW_APIENTRY PFNGLPROGRAMUNIFORM3IVPROC) (GLuint program, GLint location, GLsizei count, const GLint *value);
typedef void (NVFX_GLEW_APIENTRY PFNGLPROGRAMUNIFORM3FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
typedef void (NVFX_GLEW_APIENTRY PFNGLPROGRAMUNIFORM4IVPROC) (GLuint program, GLint location, GLsizei count, const GLint *value);
typedef void (NVFX_GLEW_APIENTRY PFNGLPROGRAMUNIFORM4FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
typedef void (NVFX_GLEW_APIENTRY PFNGLPROGRAMUNIFORMMATRIX4FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void* (NVFX_GLEW_APIENTRY PFNGLXGETPROCADDRESSPROC) (const GLubyte *name);

NVFX_GLEW_APIENTRY_EXPORT PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
NVFX_GLEW_APIENTRY_EXPORT PFNGLPROGRAMUNIFORM1IPROC glProgramUniform1i;
NVFX_GLEW_APIENTRY_EXPORT PFNGLPROGRAMUNIFORM1IVPROC glProgramUniform1iv;
NVFX_GLEW_APIENTRY_EXPORT PFNGLPROGRAMUNIFORM1FVPROC glProgramUniform1fv;
NVFX_GLEW_APIENTRY_EXPORT PFNGLPROGRAMUNIFORM2IVPROC glProgramUniform2iv;
NVFX_GLEW_APIENTRY_EXPORT PFNGLPROGRAMUNIFORM2FVPROC glProgramUniform2fv;
NVFX_GLEW_APIENTRY_EXPORT PFNGLPROGRAMUNIFORM3IVPROC glProgramUniform3iv;
NVFX_GLEW_APIENTRY_EXPORT PFNGLPROGRAMUNIFORM3FVPROC glProgramUniform3fv;
NVFX_GLEW_APIENTRY_EXPORT PFNGLPROGRAMUNIFORM4IVPROC glProgramUniform4iv;
NVFX_GLEW_APIENTRY_EXPORT PFNGLPROGRAMUNIFORM4FVPROC glProgramUniform4fv;
NVFX_GLEW_APIENTRY_EXPORT PFNGLPROGRAMUNIFORMMATRIX4FVPROC glProgramUniformMatrix4fv;
NVFX_GLEW_APIENTRY_EXPORT PFNGLXGETPROCADDRESSPROC glXGetProcAddress;

// glDispatchComputeARB is not found
// glGetProcAddress is not found
// glPROGRAMUNIFORM is not found
// glPROGRAMUNIFORMMATRIXV is not found
// glPROGRAMUNIFORMV is not found
// glXGetProcAddress is not found

struct FunctionResolver {
    static inline int makeVersion(int major, int minor) {
        return major * 100 + minor * 10;
    }
    virtual ~FunctionResolver() {}
    virtual bool hasExtension(const char *name) const = 0;
    virtual void *resolve(const char *name) const = 0;
    virtual int queryVersion() const = 0;
};

GLAPI void initializeOpenGLFunctions(const FunctionResolver *resolver);
GLAPI void initialize();
GLAPI void cleanup();

#ifndef NVFX_GLEW_OS_WIN32
static inline void DebugBreak() {}
#endif

} /* namespace nvFX */

#undef NVFX_GLEW_DECLARE_TYPES

#endif /* NVFX_GLEW__H_ */