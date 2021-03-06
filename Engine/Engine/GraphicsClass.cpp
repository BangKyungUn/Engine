#include "GraphicsClass.h"



GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass &other)
{
}


GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// Direct3D 객체 생성
	m_D3D = new D3DClass;
	if (!m_D3D) {
		return false;
	}

	// Direct3D 객체 초기화
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}
	return true;
}

void GraphicsClass::Shutdown()
{
	// D3D 객체 반환
	if (m_D3D) {
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}
	return;
}

bool GraphicsClass::Frame()
{
	bool result;
	// 그래픽 렌더링 수행
	result = Render();
	if (!result) {
		return false;
	}
	return true;
}

bool GraphicsClass::Render()
{
	// 버퍼 초기화
	m_D3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	// 버퍼에 그려진 씬을 화면에 출력
	m_D3D->EndScene();
	return true;;
}
