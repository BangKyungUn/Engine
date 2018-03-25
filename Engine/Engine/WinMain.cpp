
// Filename : WinMain.cpp


#include "SystemClass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	SystemClass * System;
	bool result;

	// System ��ü�� ����
	System = new SystemClass;
	if (!System) {
		return 0;
	}

	// System ��ü�� �ʱ�ȭ , Run ȣ��
	result = System->Initialize();
	if (result) {
		System->Run();
	}

	// System ��ü�� �����ϰ� �޸� ��ȯ.
	System->Shutdown();
	delete System;
	System = 0;

	return 0;

}