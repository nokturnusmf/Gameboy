#include "emulator.h"

#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void display_callback(unsigned char*);

Emulator::Emulator(const std::string& file_path)
    : cpu({ }), memmap(display, interrupts, input, file_path), display(display_callback), interrupts(cpu, memmap) {
    cpu.a = 0x11;
    cpu.sp = 0xFFFE;
    cpu.pc = 0x100;
}

#include "gl.impl"

static GLFWwindow* window;

void display_callback(unsigned char* pixels) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 160, 144, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glfwSwapBuffers(window);
}

void Emulator::run() {
    glfwInit();
    window = glfwCreateWindow(160, 144, "GameBoy", 0, 0);
    glfwMakeContextCurrent(window);
    setup_gl();

    while (!glfwWindowShouldClose(window)) {
        long cycles = execute();
        interrupts.process();
        display.advance(cycles);
        glfwPollEvents();
    }

    glfwTerminate();
}

#include "opcodes.impl"

#include "opcodes_bits.impl"
