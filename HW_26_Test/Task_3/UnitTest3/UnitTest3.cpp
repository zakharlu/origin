#include "pch.h"
#include "CppUnitTest.h"
#include "../Task_3/extended_array.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest3
{
	TEST_CLASS(UnitTest3)
	{
	public:
		
		TEST_METHOD(checkSum_wrong_type) // ���� �������������, ���� ������ �������� �� int ��� �� bool
		{
			ExtArray<int> v1{ 1, 1, 0, 0, 1, 1, 0, 1 };;
			Assert::AreEqual(v1.checkSum(), -1);
		}
		
		TEST_METHOD(checkSum_wrong_val) // ���� �������������, ���� � ������� ���� ��������, �� ������ 0 ��� 1
		{
			ExtArray<int> v2{ 2, 1, 0, 0, 1, 1, 0, 1 };
			Assert::AreEqual(v2.checkSum(), -2);
		} 

		TEST_METHOD(checkSum_Sum) // ���� �������������, ���� ��������� ���������� ���������� ��������� ��������, ������ 1
		{
			ExtArray<int> v3{ 1, 1, 0, 0, 1, 1, 0, 1 };
			Assert::AreEqual(v3.checkSum(), 5);
		}
	};
}
