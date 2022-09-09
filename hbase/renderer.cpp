#include"pch.h"
#include "logger.h"
#include "pointers.h"
#include "renderer.h"
#include <imgui.h>
#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_win32.h>
#include <imgui_internal.h>
#include"main_gui.h"
#include"ui/ui_manager.h"
#include "keyboard.h"
#include"ui_settings.h"
#include "./ui/icnon_font.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace hbase
{
	renderer::renderer() :
		m_dxgiSwapchain(*g_pointers->m_swapchain)
	{
		void* d3d_device{};
		if (SUCCEEDED(m_dxgiSwapchain->GetDevice(__uuidof(ID3D11Device), &d3d_device)))
		{
			m_d3dDevice.Attach(static_cast<ID3D11Device*>(d3d_device));
		}
		else
		{
			throw std::runtime_error("Failed to get D3D device.");
		}

		m_d3dDevice->GetImmediateContext(m_d3dDeviceContext.GetAddressOf());
		auto file_path = std::filesystem::path(MENU_PATH "\\");
		if (!std::filesystem::exists(file_path))
		{
			std::filesystem::create_directory(file_path);
		}
		else if (!std::filesystem::is_directory(file_path))
		{
			std::filesystem::remove(file_path);
			std::filesystem::create_directory(file_path);
		}
		file_path /= "imgui.ini";

		ImGuiContext* ctx = ImGui::CreateContext();

		static std::string path = file_path.make_preferred().string();
		ctx->IO.IniFilename = path.c_str();

		ImGui_ImplDX11_Init(m_d3dDevice.Get(), m_d3dDeviceContext.Get());
		ImGui_ImplWin32_Init(g_pointers->m_hwnd);

		std::string str = MENU_PATH"\\ui\\fonts\\" + g_ui_settings.options["font"].get<std::string>();
		std::string fontstr = MENU_PATH"\\ui\\fonts\\icons.ttf";

		m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(str.c_str(), 20.f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesChineseFull());

		ImFontConfig font_icons_cfg{};
		font_icons_cfg.FontDataOwnedByAtlas = false;
		std::strcpy(font_icons_cfg.Name, "Icons");


		m_IconFont = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(icon_font_data), sizeof(icon_font_data), 15.f ,&font_icons_cfg);
	//	m_IconFontTEST = ImGui::GetIO().Fonts->AddFontFromFileTTF(fontstr.c_str(), 20.f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesChineseFull());
		load_text();
		g_renderer = this;
	}

	renderer::~renderer()
	{
		ImGui_ImplWin32_Shutdown();
		ImGui_ImplDX11_Shutdown();
		ImGui::DestroyContext();

		g_renderer = nullptr;
	}

	void renderer::load_text()
	{
		if (g_ui_settings.options["header"]["count"].get<int>() > 1)
		{
			for (int i = 1; i < g_ui_settings.options["header"]["count"].get<int>() + 1; i++)
			{
				std::string filePath = MENU_PATH"\\ui\\" + g_ui_settings.options["header"]["directory"].get<std::string>() + "\\" + g_ui_settings.options["header"]["header_before"].get<std::string>() + std::to_string(i) + g_ui_settings.options["header"]["header_after"].get<std::string>();
				ID3D11ShaderResourceView* idle{};
				int image_width = 0;
				int image_height = 0;
				if (LoadTextureFromFile(filePath.c_str(), &idle, &image_width, &image_height))
				{
					m_header_gif_texture.push_back(idle);
				}
			}
		}
		else
		{
			std::string filePath = MENU_PATH"\\ui\\" + g_ui_settings.options["header"]["directory"].get<std::string>() + "\\" + g_ui_settings.options["header"]["header_before"].get<std::string>() + g_ui_settings.options["header"]["header_after"].get<std::string>();
			int image_width = 0;
			int image_height = 0;
			LoadTextureFromFile(filePath.c_str(), &m_header_texture, &image_width, &image_height);
		}

		//load option
		if (g_ui_settings.options["option"]["count"].get<int>() > 1 && g_ui_settings.options["option"]["is_texture_mode"].get<bool>())
		{
			for (int i = 1; i < g_ui_settings.options["option"]["count"].get<int>() + 1; i++)
			{
				std::string filePath = MENU_PATH"\\ui\\" + g_ui_settings.options["option"]["directory"].get<std::string>() + "\\" + g_ui_settings.options["option"]["option_before"].get<std::string>() + std::to_string(i) + g_ui_settings.options["option"]["option_after"].get<std::string>();
				ID3D11ShaderResourceView* idle{};
				int image_width = 0;
				int image_height = 0;
				if (LoadTextureFromFile(filePath.c_str(), &idle, &image_width, &image_height))
				{
					m_option_gif_texture.push_back(idle);
				}
			}
		}
		else
		{
			std::string filePath = MENU_PATH"\\ui\\" + g_ui_settings.options["option"]["directory"].get<std::string>() + "\\" + g_ui_settings.options["option"]["option_before"].get<std::string>() + g_ui_settings.options["option"]["option_after"].get<std::string>();
			int image_width = 0;
			int image_height = 0;
			LoadTextureFromFile(filePath.c_str(), &m_option_texture, &image_width, &image_height);
		}

		//load option slider
		if (g_ui_settings.options["option"]["slider"]["count"].get<int>() > 1 && g_ui_settings.options["option"]["slider"]["is_texture_mode"].get<bool>())
		{
			for (int i = 1; i < g_ui_settings.options["option"]["slider"]["count"].get<int>() + 1; i++)
			{
				std::string filePath = MENU_PATH"\\ui\\" + g_ui_settings.options["option"]["slider"]["directory"].get<std::string>() + "\\" + g_ui_settings.options["option"]["slider"]["slider_before"].get<std::string>() + std::to_string(i) + g_ui_settings.options["option"]["slider"]["slider_after"].get<std::string>();
				ID3D11ShaderResourceView* idle{};
				int image_width = 0;
				int image_height = 0;
				if (LoadTextureFromFile(filePath.c_str(), &idle, &image_width, &image_height))
				{
					m_option_slider_gif_texture.push_back(idle);
				}
			}
		}
		else
		{
			std::string filePath = MENU_PATH"\\ui\\" + g_ui_settings.options["option"]["slider"]["directory"].get<std::string>() + "\\" + g_ui_settings.options["option"]["slider"]["slider_before"].get<std::string>() + g_ui_settings.options["option"]["slider"]["slider_after"].get<std::string>();
			int image_width = 0;
			int image_height = 0;
			LoadTextureFromFile(filePath.c_str(), &m_option_slider_texture, &image_width, &image_height);
		}

		//load sidebar
		if (g_ui_settings.options["sidebar"]["count"].get<int>() > 1 && g_ui_settings.options["sidebar"]["is_texture_mode"].get<bool>())
		{
			for (int i = 1; i < g_ui_settings.options["sidebar"]["count"].get<int>() + 1; i++)
			{
				std::string filePath = MENU_PATH"\\ui\\" + g_ui_settings.options["sidebar"]["directory"].get<std::string>() + "\\" + g_ui_settings.options["sidebar"]["sidebar_before"].get<std::string>() + std::to_string(i) + g_ui_settings.options["sidebar"]["sidebar_after"].get<std::string>();
				ID3D11ShaderResourceView* idle{};
				int image_width = 0;
				int image_height = 0;
				if (LoadTextureFromFile(filePath.c_str(), &idle, &image_width, &image_height))
				{
					m_sidebar_gif_texture.push_back(idle);
				}
			}
		}
		else
		{
			std::string filePath = MENU_PATH"\\ui\\" + g_ui_settings.options["sidebar"]["directory"].get<std::string>() + "\\" + g_ui_settings.options["sidebar"]["sidebar_before"].get<std::string>() + g_ui_settings.options["sidebar"]["sidebar_after"].get<std::string>();
			int image_width = 0;
			int image_height = 0;
			LoadTextureFromFile(filePath.c_str(), &m_sidebar_texture, &image_width, &image_height);
		}

		//load sidebar slider
		if (g_ui_settings.options["sidebar"]["slider"]["count"].get<int>() > 1 && g_ui_settings.options["sidebar"]["slider"]["is_texture_mode"].get<bool>())
		{
			for (int i = 1; i < g_ui_settings.options["sidebar"]["slider"]["count"].get<int>() + 1; i++)
			{
				std::string filePath = MENU_PATH"\\ui\\" + g_ui_settings.options["sidebar"]["slider"]["directory"].get<std::string>() + "\\" + g_ui_settings.options["sidebar"]["slider"]["slider_before"].get<std::string>() + std::to_string(i) + g_ui_settings.options["sidebar"]["slider"]["slider_after"].get<std::string>();
				ID3D11ShaderResourceView* idle{};
				int image_width = 0;
				int image_height = 0;
				if (LoadTextureFromFile(filePath.c_str(), &idle, &image_width, &image_height))
				{
					m_sidebar_sidebar_gif_texture.push_back(idle);
				}
			}
		}
		else
		{
			std::string filePath = MENU_PATH"\\ui\\" + g_ui_settings.options["sidebar"]["slider"]["directory"].get<std::string>() + "\\" + g_ui_settings.options["option"]["slider"]["slider_before"].get<std::string>() + g_ui_settings.options["sidebar"]["slider"]["slider_after"].get<std::string>();
			int image_width = 0;
			int image_height = 0;
			LoadTextureFromFile(filePath.c_str(), &m_sidebar_slider_texture, &image_width, &image_height);
		}
	}

	void renderer::on_present()
	{
		//if (g_ui_mgr->m_opened)
		//{
		//	ImGui::GetIO().MouseDrawCursor = true;
		//	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
		//}
		//else
		//{
		//	ImGui::GetIO().MouseDrawCursor = false;
		//	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
		//}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		g_main_gui.tick();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	void renderer::pre_reset()
	{
		ImGui_ImplDX11_InvalidateDeviceObjects();
	}

	void renderer::post_reset()
	{
		ImGui_ImplDX11_CreateDeviceObjects();
	}

	void renderer::wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
		case WM_SIZE:
			break;
		case WM_LBUTTONDOWN:
			break;
		case WM_LBUTTONUP:
			break;
		case WM_RBUTTONDOWN:
			break;
		case WM_RBUTTONUP:
			break;
		case WM_MBUTTONDOWN:
			break;
		case WM_MBUTTONUP:
			break;
		case WM_MOUSEWHEEL:
			break;
		case WM_MOUSEMOVE:
			break;
		case WM_KEYDOWN: case WM_KEYUP: case WM_SYSKEYDOWN: case WM_SYSKEYUP:
		{
			auto functions = g_keyBoard->g_keyboardFunctions;
			for (auto& function : functions)
			{
				function((DWORD)wparam, lparam & 0xFFFF, (lparam >> 16) & 0xFF, (lparam >> 24) & 1, (msg == WM_SYSKEYDOWN || msg == WM_SYSKEYUP), (lparam >> 30) & 1, (msg == WM_SYSKEYUP || msg == WM_KEYUP));
			}
		}
		break;
		case WM_CHAR:
			break;
		}

		if (g_ui_mgr->m_opened)
		{
			ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
		}
	}

	// Simple helper function to load an image into a DX11 texture with common settings
	bool renderer::LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* w, int* h)
	{
		// Load from disk into a raw RGBA buffer
		int image_width = 0;
		int image_height = 0;
		unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
		if (image_data == NULL)
			return false;

		// Create texture
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = image_width;
		desc.Height = image_height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;

		ID3D11Texture2D* pTexture = NULL;
		D3D11_SUBRESOURCE_DATA subResource;
		subResource.pSysMem = image_data;
		subResource.SysMemPitch = desc.Width * 4;
		subResource.SysMemSlicePitch = 0;
		m_d3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

		// Create texture view
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		m_d3dDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
		pTexture->Release();

		*w = desc.Width;

		*h = desc.Height;

		stbi_image_free(image_data);

		return true;
	}
}