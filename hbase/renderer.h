#pragma once
#include <imgui.h>

namespace hbase
{
	struct imgDimensions
	{
		int     x, y;
		imgDimensions() { x = y = 0; }
		imgDimensions(int _x, int _y) { x = _x; y = _y; }
	};
	class renderer
	{
	public:
		explicit renderer();
		~renderer();

		void on_present();
		void pre_reset();
		void post_reset();

		void load_text();

		void wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* w, int* h);
	public:
		ImFont* m_IconFont;
	//	ImFont* m_IconFontTEST;

		ImFont* m_font;
		ImFont* m_monospaceFont;
		ID3D11ShaderResourceView* ImageHeader{};
		imgDimensions pHeaderSize = { 0, 0 };

		ID3D11ShaderResourceView* m_header_texture{};
		std::vector<ID3D11ShaderResourceView*> m_header_gif_texture;

		ID3D11ShaderResourceView* m_option_texture;
		std::vector<ID3D11ShaderResourceView*> m_option_gif_texture;

		ID3D11ShaderResourceView* m_option_slider_texture;
		std::vector<ID3D11ShaderResourceView*> m_option_slider_gif_texture;

		ID3D11ShaderResourceView* m_sidebar_texture;
		std::vector<ID3D11ShaderResourceView*> m_sidebar_gif_texture;

		ID3D11ShaderResourceView* m_sidebar_slider_texture;
		std::vector<ID3D11ShaderResourceView*> m_sidebar_sidebar_gif_texture;

	private:
		comptr<IDXGISwapChain> m_dxgiSwapchain;
		comptr<ID3D11Device> m_d3dDevice;
		comptr<ID3D11DeviceContext> m_d3dDeviceContext;
	};

	inline renderer* g_renderer{};
}