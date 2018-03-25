
// Filename : WinMain.cpp


#include "SystemClass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	SystemClass * System;
	bool result;

	// System 객체를 생성
	System = new SystemClass;
	if (!System) {
		return 0;
	}

	// System 객체를 초기화 , Run 호출
	result = System->Initialize();
	if (result) {
		System->Run();
	}

	// System 객체를 종료하고 메모리 반환.
	System->Shutdown();
	delete System;
	System = 0;

	return 0;

}