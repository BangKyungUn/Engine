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

	// �������� api�� ����Ͽ� �ʱ�ȭ
	InitializeWindows(screenWidth, screenHeight);

	// input ��ü ����. Ű���� �Է��� ó���ϱ����� ���

	m_Input = new InputClass;
	if (!m_Input) {
		return false;
	}

	// Input ��ü �ʱ�ȭ
	m_Input->Initialize();

	// graphics ��ü ���� . �� ��ü�� ��� �׷��� ��Ҹ� �׸��� ���� �Ѵ�.
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

	// �ʱ�ȭ
	ZeroMemory(&msg, sizeof(MSG));

	done = false;
	while (!done) {
		// ������ �޽��� ó��
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// �����쿡�� ���ø����̼��� ���Ḧ ��û�ϴ� ��� ���������ϴ�.
		if (msg.message == WM_QUIT) {
			done = true;
		}
		else {
			// �׿ܿ��� frame �Լ��� ó��
			result = Frame();
			if (!result) {
				done = true;
			}
		}
	}

	return;
}

bool SystemClass::Frame() { // ��� �۾� ó�� 
	bool result;

	// ������ Esc�� ���� ���ø����̼��� �����ϱ⸦ ���ϴ��� Ȯ��
	if (m_Input->IsKeyDown(VK_ESCAPE)) {
		return false;
	}

	// Graphics ��ü �۾� ó��
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
		// Ű�� ���ȴ��� üũ
	case WM_KEYDOWN:
	{
		// Ű�� ���ȴٸ� ��ǲ ��ü�� �� ����� �����Ͽ� ���
		m_Input->KeyDown((unsigned int)wparam);
		return 0;
	}
	// ���� Ű�� ���������� Ȯ��
	case WM_KEYUP:
	{
		m_Input->KeyUp((unsigned int)wparam);
		return 0;
	}
	// �ٸ� �޼������� ������� �����Ƿ� �⺻ �޽��� ó���⿡ ����
	default:
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
}


void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// �ܺ� �����͸� �� ��ü�� ����
	ApplicationHandle = this;
	// �� �ν��Ͻ� ������
	m_hinstance = GetModuleHandle(NULL);

	// �� �̸�
	m_applicationName = L"Engine";

	// ������Ŭ���� �⺻����
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

	// ������Ŭ���� ���
	RegisterClassEx(&wc);

	// ����� ȭ���� �ػ󵵸� �˾ƿ´�.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Ǯ��ũ�� ��� ���� ���� ���� ȭ�鼳������ �����Ѵ�.
	if (FULL_SCREEN) {  // FULL_SCREEN ������ GraphicsClass.h�� ����
		// ���� Ǯ ��ũ������� ȭ�� ũ�⸦ ����ũ�� ũ�⿡ ���߰� ������ 32bit�� ��ȯ�մϴ�.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Ǯ��ũ���� �´� ���÷��� ����
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// �������� ��ġ�� ȭ�� ���� ���� ����
		posX = posY = 0;
	}
	else {
		// ������ ����� 800x600 ����
		screenWidth = 800;
		screenHeight = 600;

		// �������� ��ġ�� �߾ӿ� ����
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// ������ ���� ���� ������ �����츦 ����� �ش� �ڵ��� �����´�.

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// �����츦 ȭ�鿡 ǥ���ϰ� ��Ŀ���� ��.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// ���콺 Ŀ���� ǥ������ �ʴ´�.
	ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows()
{
	// ���콺 Ŀ���� ǥ���Ѵ�.
	ShowCursor(true);

	// Ǯ��ũ�� ��带 �������ö� ���÷��� ������ �ٲ۴�.
	if (FULL_SCREEN) {
		ChangeDisplaySettings(NULL, 0);
	}

	// â�� �����մϴ�.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// ���ø����̼� �ν��Ͻ��� �����մϴ�.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// �� Ŭ������ ���� �ܺ� ������ ������ �����մϴ�.
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// �����찡 ���� �Ǿ����� Ȯ��.
	case WM_DESTROY: 
	{
		PostQuitMessage(0);
		return 0;
	}
	// �����찡 �������� Ȯ��
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	// �ٸ� ��� �޼������� SystemClass ���� �޽��� ó���⿡ ����
	default:
		return ApplicationHandle->MessageHandler(hwnd, umsg, wparam, lparam);
	}
}
