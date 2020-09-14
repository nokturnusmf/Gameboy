#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "gl.impl"

#include "emulator.h"
#include "input.h"

class Window {
public:
    Window(const std::string& title, int width, int height, Emulator* emulator) : emulator(emulator) {
        Window::instance = this;

        glfwInit();
        handle = glfwCreateWindow(width, height, title.c_str(), 0, 0);

        glfwMakeContextCurrent(handle);
        setup_gl();

        glfwSetKeyCallback(handle, input_callback);
    }

    ~Window() {
        glfwTerminate();
    }

    static bool display_callback_wrapper(byte* pixels) {
        return instance->display_callback(pixels);
    }

private:
    bool display_callback(byte* pixels) {
        if (!frame_limit) {
            using namespace std::chrono;
            auto now = steady_clock::now();
            if (duration_cast<nanoseconds>(now - prev_frame).count() < 1000000000 / 60) return true;
            prev_frame = now;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 160, 144, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glfwSwapBuffers(handle);

        glfwPollEvents();
        return !glfwWindowShouldClose(handle);
    }

    static void input_callback(GLFWwindow*, int key, int, int action, int) {
        instance->input_callback(key, action);
    }

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

    void input_callback(int key, int action) {
        if (key == GLFW_KEY_TAB) {
            frame_limit = action == GLFW_RELEASE;
            return;
        }

        auto button = get_button(key);
        if (button != Input::Button::None) {
            if (action == GLFW_RELEASE) {
                emulator->release(button);
            } else {
                emulator->press(button);
            }
        }
    }

    static inline Window* instance;

    GLFWwindow* handle;

    bool frame_limit = true;
    std::chrono::time_point<std::chrono::steady_clock> prev_frame = std::chrono::steady_clock::now();

    Emulator* emulator;
};

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <rom>\n";
        return 1;
    }

    Emulator emulator(argv[1], Window::display_callback_wrapper);
    Window window("Gameboy", 640, 576, &emulator);
    emulator.run();
}

void error(const char* msg, int v) {
    std::cerr << msg;
    if (v != -1) {
        std::cerr << std::hex << ' ' << v;
    }
    std::cerr << '\n';
    exit(1);
}
