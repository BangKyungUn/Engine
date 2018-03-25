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
	// ��� Ű���� ������ ���� ���·� �ʱ�ȭ
	for (i = 0; i < 256; i++) {
		m_Keys[i] = false;
	}
	return;
}

void InputClass::KeyDown(unsigned int input)
{
	// Ű�� ���ȴٸ� �� ���¸� �迭�� ����
	m_Keys[input] = true;
	return;
}

void InputClass::KeyUp(unsigned int input)
{
	// Ű�� �������ٸ� �� ���¸� �迭�� ����
	m_Keys[input] = false;
	return;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	// ���� Ű�� ����/�� �������� ��ȯ
	return m_Keys[key];
}
