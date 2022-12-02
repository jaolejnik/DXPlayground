#include "DeviceResources.h"
#include "Renderer.h"
#include "MainClass.h"
#include "ShaderManager.h"

// Main function: Creates window, calls initialization functions, and hosts
// the render loop.
INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    HRESULT hr = S_OK;

    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    // Begin initialization.

    // Instantiate the window manager class.
    std::shared_ptr<MainClass> winMain = std::shared_ptr<MainClass>(new MainClass());
    // Create a window.
    hr = winMain->CreateDesktopWindow();

    if (SUCCEEDED(hr))
    {
        std::vector<ShaderType> shaders = {
            ShaderType::Default,
            ShaderType::SHLight,
            ShaderType::Normal,
            ShaderType::Diffuse,
            ShaderType::Skybox,
        };

        std::vector<std::string> cubemaps = {
            "Tenerife",
            "Fort",
            "Yokohama",
            "Skansen",
            "VRLab",
            "debug1",
        };

        // Instantiate the device manager class.
        std::shared_ptr<DeviceResources> deviceResources = std::shared_ptr<DeviceResources>(
            new DeviceResources());
        deviceResources->CreateDeviceResources(winMain->GetWindowHandle());

        std::shared_ptr<ShaderManager> shaderManager = std::shared_ptr<ShaderManager>(
            new ShaderManager(
                deviceResources,
                shaders));
        std::shared_ptr<CubemapManager> cubemapManager = std::shared_ptr<CubemapManager>(
            new CubemapManager(
                deviceResources->GetDevice(),
                cubemaps));

        // Instantiate the renderer.
        std::shared_ptr<Renderer>
            renderer = std::shared_ptr<Renderer>(
                new Renderer(
                    deviceResources,
                    shaderManager,
                    cubemapManager));

        // We have a window, so initialize window size-dependent resources.
        deviceResources->CreateWindowResources(winMain->GetWindowHandle());
        renderer->SetupSH();

        // Run the program.
        hr = winMain->Run(deviceResources, renderer);
    }

    // Cleanup is handled in destructors.
    return hr;
}
