#include "InputClass.h"



InputClass::InputClass()
{
}

InputClass::InputClass(const InputClass &other)
{
}



InputClass::~InputClass()
{
}

void InputClass::Initialize()
{
	int i;
	// 모든 키들을 눌리지 않은 상태로 초기화
	for (i = 0; i < 256; i++) {
		m_Keys[i] = false;
	}
	return;
}

void InputClass::KeyDown(unsigned int input)
{
	// 키가 눌렸다면 그 상태를 배열에 저장
	m_Keys[input] = true;
	return;
}

void InputClass::KeyUp(unsigned int input)
{
	// 키가 떼어졌다면 그 상태를 배열에 저장
	m_Keys[input] = false;
	return;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	// 현재 키가 눌림/뗌 상태인지 반환
	return m_Keys[key];
}
