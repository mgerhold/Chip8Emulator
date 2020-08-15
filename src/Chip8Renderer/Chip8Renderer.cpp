#include "Chip8Renderer/Chip8Renderer.hpp"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gsl/gsl>

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
    : mWindow(nullptr), mChip8(chip8), mScaleFactor(0.03f), mPixelColor{1.0f, 1.0f, 1.0f}
    , mBackgroundColor{0.26f, 0.26f, 0.26f}, mClearColor{}, mRunning(false), mStepping(false)
    , mLastInstruction(0x0000), mUpdatesPerSecond(480)
{
    mLastTimerClockTime = mTimerClock.restart();
    mLastUpdateClockTime = mUpdateClock.restart();
}

void Chip8Renderer::free() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(mWindow);
    glfwTerminate();
}

bool Chip8Renderer::createWindow() {
    glfwInit();
    mWindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Chip8 Emulator", nullptr, nullptr);
    if (mWindow == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    centerWindow(mWindow, glfwGetPrimaryMonitor());

    glfwMakeContextCurrent(mWindow);
    glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);

    // glad
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
    constexpr float frameInterval = 1.f / 60.f;
    while (!glfwWindowShouldClose(mWindow)) {
        glfwPollEvents();
        processInput(mWindow);

        if (mStepping) {
            mLastInstruction = mChip8.getNextInstruction();
            mChip8.step();
            mChip8.clockTimers();
            mStepping = false;
        }

        while (mRunning && (mUpdateClock.getElapsedTime() - mLastUpdateClockTime >= 1.f / mUpdatesPerSecond)) {
            mLastInstruction = mChip8.getNextInstruction();
            mChip8.step();
            mLastUpdateClockTime += 1.f / mUpdatesPerSecond;
        }

        if (mTimerClock.getElapsedTime() - mLastTimerClockTime >= frameInterval) {
            // clock timers
            if (mRunning) {
                mChip8.clockTimers();
            }
            // render
            renderDisplay();
            renderImGui();
            glfwSwapBuffers(mWindow);
            mLastTimerClockTime = mTimerClock.getElapsedTime();
        }
    }
}

void Chip8Renderer::renderDisplay() const {
    float ratio;
    int width, height;
    glfwGetFramebufferSize(mWindow, &width, &height);
    ratio = width / (float)height;

    glClearColor(mClearColor[0], mClearColor[1], mClearColor[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(mScaleFactor, mScaleFactor, mScaleFactor);
    glTranslatef(-static_cast<float>(mChip8.DisplayWidth / 2) + 0.5f, static_cast<float>(mChip8.DisplayHeight / 2) - 0.5f, 0.f);
    for (size_t y = 0; y < mChip8.DisplayHeight; ++y) {
        for (size_t x = 0; x < mChip8.DisplayWidth; ++x) {
            if (mChip8.getPixel(x ,y))
                glColor3f(mPixelColor[0], mPixelColor[1], mPixelColor[2]);
            else
                glColor3f(mBackgroundColor[0], mBackgroundColor[1], mBackgroundColor[2]);
            glPushMatrix();
            glTranslatef(static_cast<float>(x), -static_cast<float>(y), 0.0f);
            drawUnitQuad();
            glPopMatrix();
        }
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
    ImGui::ColorEdit3("clear color", mClearColor);
    ImGui::SliderFloat("scale factor", &mScaleFactor, 0.01f, 0.1f);
    ImGui::PopItemWidth();

    ImGui::Separator();

    ImGui::Text("Program counter: 0x%03X", mChip8.getProgramCounter());
    ImGui::Text("Address pointer: 0x%03X", mChip8.getAddressPointer());
    for (uint8_t i = 0; i <= 0x7; ++i) {
        ImGui::Text("V%01X: 0x%02X  | ", i, mChip8.getRegister(i));
        ImGui::SameLine();
        ImGui::Text("V%01X: 0x%02X", i + 0x8, mChip8.getRegister(i + 0x8));
    }
    ImGui::Text("Delay timer: %d", mChip8.getDelayTimer());
    ImGui::Text("Sound timer: %d", mChip8.getSoundTimer());
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

    ImGui::PushItemWidth(170);
    ImGui::SliderInt("updates per second", &mUpdatesPerSecond, 10, 10'000);
    int currentCompatibilityModeIndex = (mChip8.getCompatibilityMode() == Chip8::CompatibilityMode::OriginalChip8 ? 0 : 1);
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
        if (mChip8.getNextInstruction().getValue() != 0x0000) {
            mStepping = true;
        }
    }
    ImGui::SameLine();

    if (mRunning) {
        if (ImGui::Button("Pause")) {
            mRunning = false;
        }
    } else {
        if (ImGui::Button("Run")) {
            mRunning = true;
            mLastTimerClockTime = 0.f;
            mTimerClock.restart();
            mLastUpdateClockTime = 0.f;
            mUpdateClock.restart();
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Restart")) {
        mChip8.reset(false);
        mMessage = "Program restarted!";
        mLastInstruction = Chip8::Instruction(0x0000);
    }

    ImGui::Separator();

    ImGui::Text("%s", mMessage.c_str());

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

void Chip8Renderer::drawUnitQuad() const {
    glBegin(GL_TRIANGLES);
    glVertex3f(-0.5f,  0.5f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.0f);
    glVertex3f( 0.5f, -0.5f, 0.0f);
    glVertex3f( 0.5f, -0.5f, 0.0f);
    glVertex3f( 0.5f,  0.5f, 0.0f);
    glVertex3f(-0.5f,  0.5f, 0.0f);
    glEnd();
}

void processInput(GLFWwindow* window) noexcept {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow*, int width, int height) noexcept {
    glViewport(0, 0, width, height);
}
