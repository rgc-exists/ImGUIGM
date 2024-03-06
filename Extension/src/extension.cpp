#include <iostream>
#include <d3d11.h>
#include <Windows.h>
#include <commctrl.h>
#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

#define YYEXPORT extern "C" __declspec(dllexport)

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ImGuiGMSSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam,
	LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {

	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
		return true;
	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

YYEXPORT double imgui_init(char* hwnd, char* device, char* device_context) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;
	style.Colors[ImGuiCol_WindowBg].w = 1.0f;

	ImGui_ImplWin32_Init((void*)hwnd);
	ImGui_ImplDX11_Init((ID3D11Device*)device, (ID3D11DeviceContext*)device_context);

	SetWindowSubclass((HWND)hwnd, ImGuiGMSSubclassProc, 1, 1);

	return 0;
} 

YYEXPORT double imgui_destroy(char* hwnd) {
	RemoveWindowSubclass((HWND)hwnd, ImGuiGMSSubclassProc, 1);

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	return 0;
}

YYEXPORT double imgui_newframe() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	return 0;
}

YYEXPORT double imgui_render() {
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	return 0;
}

YYEXPORT double imgui_begin(char* name) { return (double)ImGui::Begin(name); }
YYEXPORT double imgui_end() { ImGui::End(); return 0; }
YYEXPORT double imgui_text(char* text) { ImGui::Text(text); return 0; }
YYEXPORT double imgui_separator() { ImGui::Separator(); return 0; }
YYEXPORT double imgui_separator_text(char* text) { ImGui::SeparatorText(text); return 0; }

YYEXPORT double imgui_input_real(char* label, double val, char* format) 
{ 
    ImGui::InputDouble(label, &val, 0, 0, format);
    return val;
}

YYEXPORT char* imgui_input_text(char* label, char* text) 
{ 
    char txt[1024];
    snprintf(txt, sizeof(txt), "%s", text);
    ImGui::InputText(label, txt, 1024);
    return txt;
}

YYEXPORT double imgui_button(char* label) { return ImGui::Button(label); }
YYEXPORT double imgui_checkbox(char* label) { bool c; return ImGui::Checkbox(label, &c); }