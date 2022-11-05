#include "MainClass.h"
#include "GUI.h"

MainClass::MainClass()
{
    m_windowClassName = L"Direct3DWindowClass";
    m_hInstance = NULL;
}

HRESULT MainClass::CreateDesktopWindow()
{
    if (m_hInstance == NULL)
        m_hInstance = (HINSTANCE)GetModuleHandle(NULL);

    HICON hIcon = NULL;
    WCHAR szExePath[MAX_PATH];
    GetModuleFileName(NULL, szExePath, MAX_PATH);

    // If the icon is NULL, then use the first one found in the exe
    if (hIcon == NULL)
        hIcon = ExtractIcon(m_hInstance, szExePath, 0);

    // Register the windows class
    WNDCLASS wndClass;
    wndClass.style = CS_DBLCLKS;
    wndClass.lpfnWndProc = MainClass::StaticWindowProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = m_hInstance;
    wndClass.hIcon = hIcon;
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = m_windowClassName.c_str();

    if (!RegisterClass(&wndClass))
    {
        DWORD dwError = GetLastError();
        if (dwError != ERROR_CLASS_ALREADY_EXISTS)
            return HRESULT_FROM_WIN32(dwError);
    }

    m_rc;
    int x = CW_USEDEFAULT;
    int y = CW_USEDEFAULT;

    // No menu in this example.
    m_hMenu = NULL;

    SetRect(&m_rc, 0, 0, DEF_WIN_W, DEF_WIN_H);
    AdjustWindowRect(&m_rc, WS_OVERLAPPEDWINDOW, (m_hMenu != NULL) ? true : false);

    // Create the window for our viewport.
    m_hWnd = CreateWindow(m_windowClassName.c_str(),
                          L"DXPlayground",
                          WS_OVERLAPPEDWINDOW,
                          x, y,
                          (m_rc.right - m_rc.left), (m_rc.bottom - m_rc.top),
                          0,
                          m_hMenu,
                          m_hInstance,
                          0);

    if (m_hWnd == NULL)
    {
        DWORD dwError = GetLastError();
        return HRESULT_FROM_WIN32(dwError);
    }

    return S_OK;
}

HRESULT MainClass::Run(std::shared_ptr<DeviceResources> deviceResources,
                       std::shared_ptr<Renderer> renderer)
{
    HRESULT hr = S_OK;

    if (!IsWindowVisible(m_hWnd))
        ShowWindow(m_hWnd, SW_SHOW);

    ID3D11Device *device = deviceResources->GetDevice();
    ID3D11DeviceContext *context = deviceResources->GetDeviceContext();
    ID3D11RenderTargetView *renderTarget = deviceResources->GetRenderTarget();
    ID3D11DepthStencilView *depthStencil = deviceResources->GetDepthStencil();

    GUI::Initialize(m_hWnd, device, context);

    // The render loop is controlled here.
    bool bGotMsg;
    MSG msg;
    msg.message = WM_NULL;
    PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

    float clearColor[4] = {0.0f, 0.0f, 0.f, 1.0f};

    while (WM_QUIT != msg.message)
    {
        // Process window events.
        // Use PeekMessage() so we can use idle time to render the scene.
        bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);

        if (bGotMsg)
        {
            // Translate and dispatch the message
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            GUI::StartFrame();
            GUI::Window(clearColor);

            // Clear the render target and the z-buffer.
            context->ClearRenderTargetView(
                renderTarget,
                clearColor);

            context->ClearDepthStencilView(
                depthStencil,
                D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
                1.0f,
                0);

            // Set the render target.
            context->OMSetRenderTargets(
                1,
                &renderTarget,
                depthStencil);

            // Update the scene.
            renderer->Tick();

            // Render frames during idle time (when no messages are waiting).
            renderer->Render();

            GUI::Render();
            // Present the frame to the screen.
            deviceResources->Present();
        }
    }

    GUI::Cleanup();

    return hr;
}

// Process windows messages. This looks for window close events, letting us
// exit out of the sample.
LRESULT CALLBACK MainClass::StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (GUI::ProcHandler(hWnd, uMsg, wParam, lParam))
        return true;

    switch (uMsg)
    {
    case WM_CLOSE:
    {
        HMENU hMenu;
        hMenu = GetMenu(hWnd);
        if (hMenu != NULL)
        {
            DestroyMenu(hMenu);
        }
        DestroyWindow(hWnd);
        UnregisterClass(
            m_windowClassName.c_str(),
            m_hInstance);
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
