#include "emulator.h"

#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void display_callback(byte*);

Emulator::Emulator(const std::string& file_path)
    : cpu({ }), memmap(display, interrupts, timer, input, file_path), interrupts(cpu, memmap), display(memmap, interrupts, display_callback), timer(interrupts) {
    cpu.a = 0x11;
    cpu.sp = 0xFFFE;
    cpu.pc = 0x100;
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

void Emulator::run() {
    glfwInit();
    window = glfwCreateWindow(640, 576, "GameBoy", 0, 0);
    glfwMakeContextCurrent(window);
    setup_gl();

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
