#include "pch.h"
#include "CppUnitTest.h"
#include "../Task_2/extended_array.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest2
{
	TEST_CLASS(UnitTest2)
	{
	public:
		
		TEST_METHOD(mean_true_arg)
		{
			ExtArray<int> v1{ 4, 2, 7, 3,-5, 0,-3, 1 };
			Assert::AreEqual(v1.mean(0, 1), 3.0);
		}

		TEST_METHOD(mean_false_arg)
		{
			ExtArray<int> v2{ 4, 2, 7, 3,-5, 0,-3, 1 };
			Assert::AreEqual(v2.mean(5, 5), 0.0);
		}
	};
}
