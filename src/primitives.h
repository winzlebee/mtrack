
typedef float GLfloat;

namespace primitives {
    namespace plane {
        GLfloat vertices[] = {
             -1, -1, 0, // bottom left corner
             -1,  1, 0, // top left corner
              1,  1, 0, // top right corner
             -1, -1, 0, // bottom left corner
              1,  1, 0, // top right corner
              1, -1, 0  // Bottom right corner
             };
             
        int num = 6;
    }
}
