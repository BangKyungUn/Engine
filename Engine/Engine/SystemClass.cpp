#include "SystemClass.h"



SystemClass::SystemClass()
{
	m_Input = 0;
	m_Graphics = 0;
}

SystemClass::SystemClass(const SystemClass &other)
{

}


SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	// initalize
	screenWidth = 0;
	screenHeight = 0;

	// 윈도우즈 api를 사용하여 초기화
	InitializeWindows(screenWidth, screenHeight);

	// input 객체 생성. 키보드 입력을 처리하기위해 사용

	m_Input = new InputClass;
	if (!m_Input) {
		return false;
	}

	// Input 객체 초기화
	m_Input->Initialize();

	// graphics 객체 생성 . 이 객체는 모든 그래픽 요소를 그리는 일을 한다.
	m_Graphics = new GraphicsClass;
	if (!m_Graphics) {
		return false;
	}

	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result) {
		return false;
	}
	return true;
}

void SystemClass::Shutdown()
{
	if (m_Graphics) {
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	if (m_Input) {
		delete m_Input;
		m_Input = 0;
	}

	ShutdownWindows();
	return;
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	// 초기화
	ZeroMemory(&msg, sizeof(MSG));

	done = false;
	while (!done) {
		// 윈도우 메시지 처리
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// 윈도우에서 어플리케이션의 종료를 요청하는 경우 빠져나갑니다.
		if (msg.message == WM_QUIT) {
			done = true;
		}
		else {
			// 그외에는 frame 함수를 처리
			result = Frame();
			if (!result) {
				done = true;
			}
		}
	}

	return;
}

bool SystemClass::Frame() { // 모든 작업 처리 
	bool result;

	// 유저가 Esc를 눌러 어플리케이션을 종료하기를 원하는지 확인
	if (m_Input->IsKeyDown(VK_ESCAPE)) {
		return false;
	}

	// Graphics 객체 작업 처리
	result = m_Graphics->Frame();
	if (!result) {
		return false;
	}
	return true;
}


LRESULT SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// 키가 눌렸는지 체크
	case WM_KEYDOWN:
	{
		// 키가 눌렸다면 인풋 객체에 이 사실을 전달하여 기록
		m_Input->KeyDown((unsigned int)wparam);
		return 0;
	}
	// 눌린 키가 떼어졌는지 확인
	case WM_KEYUP:
	{
		m_Input->KeyUp((unsigned int)wparam);
		return 0;
	}
	// 다른 메세지들은 사용하지 않으므로 기본 메시지 처리기에 전달
	default:
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
}


void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// 외부 포인터를 이 객체로 지정
	ApplicationHandle = this;
	// 앱 인스턴스 가져옴
	m_hinstance = GetModuleHandle(NULL);

	// 앱 이름
	m_applicationName = L"Engine";

	// 윈도우클래스 기본설정
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// 윈도우클래스 등록
	RegisterClassEx(&wc);

	// 모니터 화면의 해상도를 알아온다.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// 풀스크린 모드 변수 값에 따라 화면설정으로 따로한다.
	if (FULL_SCREEN) {  // FULL_SCREEN 변수는 GraphicsClass.h에 있음
		// 만약 풀 스크린모드라면 화면 크기를 데스크톱 크기에 맞추고 색상을 32bit로 전환합니다.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// 풀스크린에 맞는 디스플레이 설정
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// 윈도우의 위치를 화면 왼쪽 위로 설정
		posX = posY = 0;
	}
	else {
		// 윈도우 모드라면 800x600 설정
		screenWidth = 800;
		screenHeight = 600;

		// 윈도우의 위치를 중앙에 설정
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// 설정한 것을 값을 가지고 윈도우를 만들고 해당 핸들을 가져온다.

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// 윈도우를 화면에 표시하고 포커스를 줌.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// 마우스 커서를 표시하지 않는다.
	ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows()
{
	// 마우스 커서를 표시한다.
	ShowCursor(true);

	// 풀스크린 모드를 빠져나올때 디스플레이 설정을 바꾼다.
	if (FULL_SCREEN) {
		ChangeDisplaySettings(NULL, 0);
	}

	// 창을 제거합니다.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// 어플리케이션 인스턴스를 제거합니다.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// 이 클래스에 대한 외부 포인터 참조를 제거합니다.
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// 윈도우가 제거 되었는지 확인.
	case WM_DESTROY: 
	{
		PostQuitMessage(0);
		return 0;
	}
	// 윈도우가 닫히는지 확인
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	// 다른 모든 메세지들은 SystemClass 내의 메시지 처리기에 전달
	default:
		return ApplicationHandle->MessageHandler(hwnd, umsg, wparam, lparam);
	}
}
