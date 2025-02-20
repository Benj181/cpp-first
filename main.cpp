#include "Cube.h"

int main() {
    printf("\x1b[?25l"); // Hide the cursor
    Cube my_cube = Cube();
    my_cube.set_size(30);
    my_cube.set_speed(1);
    my_cube.set_window_width(176);
    my_cube.set_window_height(44);
    my_cube.rotate();
  
    return 0;
  }