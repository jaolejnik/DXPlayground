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
            Diffuse,
            Normal,
        };

        // Instantiate the device manager class.
        std::shared_ptr<DeviceResources> deviceResources = std::shared_ptr<DeviceResources>(
            new DeviceResources());
        deviceResources->CreateDeviceResources(winMain->GetWindowHandle());

        std::shared_ptr<ShaderManager> shaderManager = std::shared_ptr<ShaderManager>(
            new ShaderManager(
                deviceResources,
                shaders));

        // Instantiate the renderer.
        std::shared_ptr<Renderer> renderer = std::shared_ptr<Renderer>(
            new Renderer(
                deviceResources,
                shaderManager));

        // We have a window, so initialize window size-dependent resources.
        deviceResources->CreateWindowResources(winMain->GetWindowHandle());
        renderer->CreateWindowSizeDependentResources();

        // Go full-screen.
        // deviceResources->GoFullScreen();

        // Whoops! We resized the "window" when we went full-screen. Better
        // tell the renderer.
        // renderer->CreateWindowSizeDependentResources();

        // Run the program.
        hr = winMain->Run(deviceResources, renderer);
    }

    // Cleanup is handled in destructors.
    return hr;
}
