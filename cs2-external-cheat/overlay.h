#pragma once
#include <d3d11.h>
#include <Windows.h>
#include "imgui.h"

class Overlay {
public:
	WNDCLASSEXW wc;
	bool done = false;
	ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 0.f);
	float clear_color_with_alpha[4] = { 0.f, 0.f, 0.f, 0.f };
	HWND hwnd;

	ID3D11Device* g_pd3dDevice = nullptr;
	ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
	IDXGISwapChain* g_pSwapChain = nullptr;
	UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
	ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
public:
	bool CreateOverlay();
	bool CreateDeviceD3D();
	void CleanupDeviceD3D();
	void CreateRenderTarget();
	void CleanupRenderTarget();
	void ReleaseOverlay();
	void RenderGui();
	void MakeNewFrame();
	void imguiInit();
};

inline Overlay overlay;