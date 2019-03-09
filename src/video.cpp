#include <opencv2/opencv.hpp>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

#include "video.h"
#include "primitives.h"

// Shaders are necessary because we are using the flexible OpenGL 3.0 pipeline
const GLchar *vert_src ="\n" \
"#version 330                                  \n" \
"#extension GL_ARB_explicit_attrib_location: enable  \n" \
"                                              \n" \
"layout(location = 0) in vec2 in_position;     \n" \
"                                              \n" \
"void main()                                   \n" \
"{                                             \n" \
"  gl_Position = vec4(in_position, 0.0, 1.0);  \n" \
"}                                             \n";

const GLchar *frag_src ="\n" \
"void main (void)                              \n" \
"{                                             \n" \
"  gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);    \n" \
"}                                             \n";

VideoDisplay::VideoDisplay(const char* fname) {
    load_video(fname);
}

void VideoDisplay::generate3DView() {
    // First we generate shaders TODO: Error checking on shader creation (definitely needed for user created shaders)
    GLuint frag_shader, vert_shader;
    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    
    glShaderSource(frag_shader, 1, &frag_src, NULL);
    glShaderSource(vert_shader, 1, &vert_src, NULL);
    
    glCompileShader(frag_shader);
    glCompileShader(vert_shader);
    
    program_id = glCreateProgram();
    glAttachShader(program_id, frag_shader);
    glAttachShader(program_id, vert_shader);
    glLinkProgram(program_id); // Final link program step. No error checking at the moment, as video is simply sampled

    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);
    
    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(primitives::plane::vertices), primitives::plane::vertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindVertexArray(0);
    
    glDeleteBuffers(1, &vbo_id); // Delete buffer id as it's now stored in the vao
}

void VideoDisplay::load_video(const char* name) {
    // Load a video, uses C-style conventions
    video = std::make_unique<cv::VideoCapture>(name);
    if (!video->isOpened()) {
        return;
    }
    
    double fRate = video->get(cv::CAP_PROP_FPS);
    
    // Get the first image of the sequence and store it in OpenGL Memory
    cv::Mat imageFrame;
    (*video) >> imageFrame;
    if (imageFrame.empty()) return;
    
    // Flip the image, as OpenGL coordinates start at the bottom
    cv::flip(imageFrame, imageFrame, 0);
    
    // Set the pixel store state so it's compatable with OpenCV's image storing method
    glPixelStorei(GL_UNPACK_ALIGNMENT, (imageFrame.step & 3) ? 1 : 4);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, imageFrame.step/imageFrame.elemSize());
    
    // Store the VideoDisplay texture ID
    glGenTextures( 1, &texture_id);
    glBindTexture( GL_TEXTURE_2D, texture_id );
    
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S , GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    
    // Note that OpenCV uses BGR internally, so we use this in our call to glTexImage2D
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imageFrame.cols, imageFrame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, imageFrame.data);
    
    // Unbind buffer to reset state
    glBindTexture( GL_TEXTURE_2D, 0 );
    
    generate3DView();
    
    this->loaded = true;
}

void VideoDisplay::render() {
    // Render the VideoDisplay. Simply, we will use a quad
    glUseProgram(program_id);

    glBindVertexArray(vao_id);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glBindVertexArray(0);
    glUseProgram(0);
}

int VideoDisplay::getTexId() {
    return this->texture_id;
}

bool VideoDisplay::isLoaded() {
    return this->loaded;
}
