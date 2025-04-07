#include "pch.h"
#include "CppUnitTest.h"
#include "../Task_1/extended_array.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(mean)
		{
			ExtArray<int> v1{};
			Assert::AreEqual(v1.mean(), 0.0);
		}

		TEST_METHOD(median)
		{
			ExtArray<int> v2{};
			Assert::AreEqual(v2.median(), 0.0);
		}

		TEST_METHOD(mode)
		{
			ExtArray<int> v3{};
			Assert::AreEqual(v3.mode().first, 0);
		}
	};
}
