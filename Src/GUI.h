#pragma once

#include "../pch.h"
#include "Model.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace GUI
{
    void Initialize(void *hWnd, // To be called in the setup stage
                    ID3D11Device *device,
                    ID3D11DeviceContext *deviceContext);
    void StartFrame();
    void MainWindow(float *clearColor, int *currentShader);
    void ModelWindow(Model *model);
    void Render();
    LRESULT ProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void Cleanup();
}
