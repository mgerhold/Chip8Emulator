#include "Chip8Renderer/Chip8Renderer.hpp"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#ifdef _MSC_VER
    // suppress warnings for ImGui headers
    #pragma warning( push , 0 )
    #pragma warning( disable : 26477 26455 26440 26493 26477 26432 26447 26481 26496 26485 26495 )
    #pragma warning( disable : ALL_CODE_ANALYSIS_WARNINGS )
#endif
#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>
#ifdef _MSC_VER
    #pragma warning( pop )
#endif
#include "Chip8Renderer/OpenFileDialog.hpp"


void framebuffer_size_callback(GLFWwindow* window, int width, int height) noexcept;
void processInput(GLFWwindow* window) noexcept;

// settings
constexpr unsigned int SCR_WIDTH = 1440;
constexpr unsigned int SCR_HEIGHT = 810;

Chip8Renderer::Chip8Renderer(Chip8::Chip8& chip8) noexcept
    : mWindow(nullptr), mChip8(chip8), mPixelSize(4.f), mPixelColor{1.0f, 1.0f, 1.0f}
    , mBackgroundColor{}, mRunning(false), mLastInstruction(0x0000)
{
    mLastElapsedTime = mClock.restart();
}

void Chip8Renderer::free() {
    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}

bool Chip8Renderer::createWindow() {
    // glfw: initialize and configure
    // ------------------------------    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    mWindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Chip8 Emulator", nullptr, nullptr);
    if (mWindow == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    // center window on primary monitor
    centerWindow(mWindow, glfwGetPrimaryMonitor());

    glfwMakeContextCurrent(mWindow);
    glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    // init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    const ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
    constexpr char const * glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);

    return true;
}

void Chip8Renderer::startRenderLoop() {
    while (!glfwWindowShouldClose(mWindow)) {
        // input
        glfwPollEvents();
        processInput(mWindow);

        // render
        glClearColor(mBackgroundColor[0], mBackgroundColor[1], mBackgroundColor[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderImGui();

        // swap
        glfwSwapBuffers(mWindow);
    }
}

void Chip8Renderer::renderImGui() {
    // ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Chip 8 Emulator");
    if (ImGui::Button("Load ROM file...")) {
        auto path = OpenFileDialog::open();
        if (path.has_value()) {
            auto filename = path.value();
            if (mChip8.loadROM(filename))
                mMessage = "ROM has been loaded!";
            else
                mMessage = "Could not load ROM!";
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Eject")) {
        mChip8.reset();
        mMessage = "ROM has been ejected!";
        mLastInstruction = Chip8::Instruction(0x0000);
    }

    ImGui::Separator();

    ImGui::Text("Render settings");
    ImGui::PushItemWidth(170);
    ImGui::ColorEdit3("pixel color", mPixelColor);
    ImGui::ColorEdit3("background color", mBackgroundColor);
    ImGui::SliderFloat("pixel size", &mPixelSize, 1.f, 30.f);
    ImGui::PopItemWidth();

    ImGui::Separator();

    ImGui::Text("Program counter: 0x%03X", mChip8.getProgramCounter());
    ImGui::Text("Address pointer: 0x%03X", mChip8.getAddressPointer());
    for (uint8_t i = 0; i <= 0x7; ++i) {
        ImGui::Text("V%01X: 0x%02X  | ", i, mChip8.getRegister(i));
        ImGui::SameLine();
        ImGui::Text("V%01X: 0x%02X", i + 0x8, mChip8.getRegister(i + 0x8));
    }
    if (mLastInstruction.getValue() != 0x0000)
        ImGui::Text("Last instruction: 0x%04X", mLastInstruction.getValue());
    else
        ImGui::Text("Last instruction: <none>");
    auto instruction = mChip8.getNextInstruction();
    if (instruction.getValue() != 0x0000)
        ImGui::Text("Next instruction: 0x%04X", instruction.getValue());
    else {
        ImGui::Text("Next instruction: <none>");
        mMessage = "End of program reached!";
    }

    ImGui::Separator();

    int currentCompatibilityModeIndex = (mChip8.getCompatibilityMode() == Chip8::CompatibilityMode::OriginalChip8 ? 0 : 1);
    ImGui::PushItemWidth(170);
    if (ImGui::Combo("Compatibility Mode", &currentCompatibilityModeIndex, "Chip8\0SuperChip\0\0")) {
        if (currentCompatibilityModeIndex == 0) {
            mChip8.setCompatibilityMode(Chip8::CompatibilityMode::OriginalChip8);
            mMessage = "Set compatibility mode to Chip8!";
        } else if (currentCompatibilityModeIndex == 1) {
            mChip8.setCompatibilityMode(Chip8::CompatibilityMode::SuperChip);
            mMessage = "Set compatibility mode to SuperChip!";
        }
    }
    ImGui::PopItemWidth();

    if (ImGui::Button("Step") && !mRunning) {
        if (instruction.getValue() != 0x0000) {
            mChip8.step();
            mLastInstruction = instruction;
        }
    }
    ImGui::SameLine();


    if (mRunning) {
        auto now = mClock.getElapsedTime();
        float deltaTime = now - mLastElapsedTime;
        while (deltaTime >= 1.f / 60.f) {
            mLastInstruction = mChip8.getNextInstruction();
            mChip8.step();
            deltaTime -= (1.f / 60.f);
            mLastElapsedTime = now;
        }
        if (ImGui::Button("Pause")) {
            mRunning = false;
        }
    } else {
        if (ImGui::Button("Run")) {
            mRunning = true;
            mLastElapsedTime = 0.f;
            mClock.restart();
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Restart")) {
        mChip8.reset(false);
        mMessage = "Program restarted!";
        mLastInstruction = Chip8::Instruction(0x0000);
    }

    ImGui::Separator();
    ImGui::Text(mMessage.c_str());

    ImGui::End();


    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Chip8Renderer::centerWindow(GLFWwindow* window, GLFWmonitor* monitor) {
    // taken from: https://vallentin.dev/2014/02/07/glfw-center-window
    if (!monitor)
        return;

    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if (!mode)
        return;

    int monitorX, monitorY;
    glfwGetMonitorPos(monitor, &monitorX, &monitorY);

    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    glfwSetWindowPos(window,
        monitorX + (mode->width - windowWidth) / 2,
        monitorY + (mode->height - windowHeight) / 2);
}

void processInput(GLFWwindow* window) noexcept {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow*, int width, int height) noexcept {
    glViewport(0, 0, width, height);
}
