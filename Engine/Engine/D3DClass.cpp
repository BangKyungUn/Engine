#include "D3DClass.h"



D3DClass::D3DClass()
{
	m_swapChain = 0;
	m_device = 0;
	m_deviceContext = 0;
	m_renderTargetView = 0;
	m_depthStencilBuffer = 0;
	m_depthStencilState = 0;
	m_depthStencilView = 0;
	m_rasterState = 0;
}

D3DClass::D3DClass(const D3DClass &other)
{
}


D3DClass::~D3DClass()
{
}

bool D3DClass::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	HRESULT result;
	IDXGIFactory *factory;
	IDXGIAdapter *adapter;
	IDXGIOutput *adapterOutput;
	unsigned int numModes, i, numerator, denomirator, stringLength;
	DXGI_MODE_DESC *displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;

	// 수직동기화 설정을 저장
	m_vsync_enabled = vsync;

	// DirectX 그래픽 인터페이스 팩토리를 만듭니다.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result)) {
		return false;
	}
	// 팩토리 객체를 사용하여 첫번째 그래픽카드 인터페이스에 대한 어댑터를 만든다.
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result)) {
		return false;
	}
	// 출력 [모니터]에 대한 첫번째 어댑터를 나열
}

void D3DClass::Shutdown()
{
}

void D3DClass::BeginScene(float, float, float, float)
{
}

void D3DClass::EndScene()
{
}

ID3D11Device * D3DClass::GetDevice()
{
	return nullptr;
}

ID3D11DeviceContext * D3DClass::GetDeviceContext()
{
	return nullptr;
}

void D3DClass::GetProjectionMatrix(D3DXMATRIX &)
{
}

void D3DClass::GetWorldMatrix(D3DXMATRIX &)
{
}

void D3DClass::GetOrthoMatrix(D3DXMATRIX &)
{
}

void D3DClass::GetVideoCardInfo(char *, int &)
{
}
