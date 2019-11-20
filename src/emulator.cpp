#include "emulator.h"

#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void display_callback(byte*);

Emulator::Emulator(const std::string& file_path)
    : cpu({ }), memmap(display, interrupts, timer, input, file_path), interrupts(cpu, memmap), display(memmap, interrupts, display_callback), timer(interrupts) {
}

#include "gl.impl"

static GLFWwindow* window;
static bool running = true;

void display_callback(byte* pixels) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 160, 144, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glfwSwapBuffers(window);

    glfwPollEvents();
    running = !glfwWindowShouldClose(window);
}

static Input* input;

Input::Button get_button(int key) {
    switch (key) {
    case GLFW_KEY_SPACE:
        return Input::Button::A;
    case GLFW_KEY_LEFT_SHIFT:
        return Input::Button::B;
    case GLFW_KEY_W:
        return Input::Button::Up;
    case GLFW_KEY_A:
        return Input::Button::Left;
    case GLFW_KEY_S:
        return Input::Button::Down;
    case GLFW_KEY_D:
        return Input::Button::Right;
    case GLFW_KEY_ENTER:
        return Input::Button::Start;
    case GLFW_KEY_BACKSPACE:
        return Input::Button::Select;
    default:
        return Input::Button::None;
    }
}

void input_callback(GLFWwindow*, int key, int, int action, int) {
    auto button = get_button(key);
    if (button != Input::Button::None) {
        if (action == GLFW_RELEASE) {
            input->release(button);
        } else {
            input->press(button);
        }
    }
}

void Emulator::run() {
    glfwInit();
    window = glfwCreateWindow(640, 576, "GameBoy", 0, 0);
    glfwMakeContextCurrent(window);
    setup_gl();
    ::input = &input;
    glfwSetKeyCallback(window, input_callback);

    long cycles, total_cycles = 0;
    while (running) {
        total_cycles += cycles = execute();
        display.advance(cycles);
        timer.update(cycles);
        interrupts.process();
    }

    glfwTerminate();
}

#include "opcodes.impl"

#include "opcodes_bits.impl"
