#include "stdafx.h"
#include "CppUnitTest.h"
#include "CompletionPortListener.h"
#include "TestListener1.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace pinefarm_test
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			TestListener1 listener;
			listener.Start();
		}


		TEST_METHOD(TestMethod2)
		{
			CompletionPortListener listener;
			listener.Start();
		}

	};
}