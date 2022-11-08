#include "GUI.h"

using namespace GUI;

void GUI::Initialize(void *hWnd, ID3D11Device *device, ID3D11DeviceContext *deviceContext)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(device, deviceContext);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // io.Fonts->AddFontDefault();
    // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    // ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    // IM_ASSERT(font != NULL);
}

void GUI::StartFrame()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void GUI::Render()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void GUI::MainWindow(float *clearColor, int *currentShader)
{
    static int counter = 0;

    ImGui::Begin("General"); // Create a window called "Hello, world!" and append into it.

    ImGui::Combo("Shader", currentShader, ShaderTypeToCstring, IM_ARRAYSIZE(ShaderTypeToCstring), ShaderType::COUNT);
    ImGui::Separator();

    ImGui::ColorEdit3("clear color", clearColor); // Edit 3 floats representing a color

    ImGui::Separator();

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::End();
}

void GUI::ModelWindow(Model *model)
{
    Transform &tf = model->GetTransform();
    DirectX::XMFLOAT3 angles = {
        DirectX::XMConvertToDegrees(tf.m_rotation.x),
        DirectX::XMConvertToDegrees(tf.m_rotation.y),
        DirectX::XMConvertToDegrees(tf.m_rotation.z),
    };

    ImGui::Begin(model->GetName());

    ImGui::Text("TRANSLATION");
    ImGui::SliderFloat("T X", &(tf.m_translation.x), -10.0f, 10.0f);
    ImGui::SliderFloat("T Y", &(tf.m_translation.y), -10.0f, 10.0f);
    ImGui::SliderFloat("T Z", &(tf.m_translation.z), -10.0f, 10.0f);

    ImGui::Separator();

    ImGui::Text("ROTATION");
    ImGui::SliderFloat("R X", &(angles.x), 0.0f, 360.0f);
    ImGui::SliderFloat("R Y", &(angles.y), 0.0f, 360.0f);
    ImGui::SliderFloat("R Z", &(angles.z), 0.0f, 360.0f);
    tf.m_rotation = {
        DirectX::XMConvertToRadians(angles.x),
        DirectX::XMConvertToRadians(angles.y),
        DirectX::XMConvertToRadians(angles.z),
    };

    ImGui::Separator();

    ImGui::Text("SCALE");
    ImGui::Checkbox("Uniform scale", &tf.m_useUniformScale);
    if (tf.m_useUniformScale)
    {
        ImGui::SliderFloat("S U", &(tf.m_scale.x), 0.0f, 10.0f);
        tf.m_scale.y = tf.m_scale.x;
        tf.m_scale.z = tf.m_scale.x;
    }
    else
    {
        ImGui::SliderFloat("S X", &(tf.m_scale.x), 0.0f, 10.0f);
        ImGui::SliderFloat("S Y", &(tf.m_scale.y), 0.0f, 10.0f);
        ImGui::SliderFloat("S Z", &(tf.m_scale.z), 0.0f, 10.0f);
    }

    ImGui::End();
}

LRESULT GUI::ProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
}

void GUI::Cleanup()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
