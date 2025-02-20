#include "Cube.h"

Cube::Cube(float cube_size, int window_width, int window_height, float speed) {
    _depth_scaling_factor = cube_size;
    _window_width = window_width;
    _window_height = window_height;
    _speed = speed;
    _cube_width = 20;
    _z_buffer.resize(window_width * window_height);
    _buffer.resize(window_width * window_height);
}

float Cube::calculate_x_pos(int i, int j, int k) {
    return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) +
           j * cos(A) * sin(C) + k * sin(A) * sin(C) + 
           i * cos(B) * cos(C);
}

float Cube::calculate_y_pos(int i, int j, int k) {
    return j * cos(A) * cos(C) + k * sin(A) * cos(C) -
           j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) -
           i * cos(B) * sin(C);
}

float Cube::calculate_z_pos(int i, int j, int k) {
    return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}

void Cube::calculate_for_surface(float cube_x, float cube_y, float cube_z, char symbol) {
    float x = Cube::calculate_x_pos(cube_x, cube_y, cube_z);
    float y = Cube::calculate_y_pos(cube_x, cube_y, cube_z);
    float z = Cube::calculate_z_pos(cube_x, cube_y, cube_z) + 100; 
    float z_reciprocal = 1 / z; 
    int x_plane = (_window_width / 2 + _horizontal_offset + _depth_scaling_factor * z_reciprocal * x * 2);
    int y_plane = (_window_height / 2 + _depth_scaling_factor * z_reciprocal * y);
    int position_i = x_plane + y_plane * _window_width; 
    if (position_i >= 0 && position_i < _window_width * _window_height) {
      if (z_reciprocal > _z_buffer[position_i]) {
        _z_buffer[position_i] = z_reciprocal; 
        _buffer[position_i] = symbol;
      }
    }
}

void Cube::rotate() {
    while (true) {
        std::cout << "\x1b[2J";  // Clear the screen
        std::cout << "\x1b[H";   // Move cursor to the top-left

        std::fill(_buffer.begin(), _buffer.end(), _background_character);
        std::fill(_z_buffer.begin(), _z_buffer.end(), 0.0f);
        
        _horizontal_offset = -2 * _cube_width;
        for (float cube_x = -_cube_width; cube_x < _cube_width; cube_x += _speed) {
            for (float cube_y = -_cube_width; cube_y < _cube_width; cube_y += _speed) {
                calculate_for_surface(cube_x, cube_y, -_cube_width, '@');
                calculate_for_surface(_cube_width, cube_y, cube_x, '$');
                calculate_for_surface(-_cube_width, cube_y, -cube_x, '~');
                calculate_for_surface(-cube_x, cube_y, _cube_width, '#');
                calculate_for_surface(cube_x, -_cube_width, -cube_y, ';');
                calculate_for_surface(cube_x, _cube_width, cube_y, '+');
            }
        }
        for (int i = 0; i < _window_width * _window_height; ++i) { 
            std::cout << (i % _window_width ? _buffer[i] : '\n');
        }
        A += 0.05;
        B += 0.05;
        C += 0.05;
        usleep(50000);  // Adjust delay if needed (50ms)
    }
}

bool Cube::set_speed(float speed) {
    if (speed < 0) return false;
    _speed = speed;
    return true;
}

bool Cube::set_background(char c) {
    _background_character = c;
    return true;
}

bool Cube::set_size(float factor) {
    _depth_scaling_factor = factor;
    return true;
}

bool Cube::set_window_height(int height) {
    if (height < 0) return false;
    _window_height = height;
    _z_buffer.resize(get_window_width() * height);
    _buffer.resize(get_window_width() * height);
    return true;
}

bool Cube::set_window_width(int width) {
    if (width < 0) return false;
    _window_width = width;
    _z_buffer.resize(width * get_window_height());
    _buffer.resize(width * get_window_height());
    return true;
}
