/* gui/texture.vsh */
#if defined(GL_ES) || __VERSION__ >= 150
precision highp float;
#endif
#if __VERSION__ < 130
#define in attribute
#define out varying
#endif
in vec2 inPosition;
in vec2 inTexCoord;
out vec2 outTexCoord;

void main() {
    outTexCoord = inTexCoord;
    gl_Position = vec4(inPosition, 0.0, 1.0);
}

