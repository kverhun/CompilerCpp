#include "stdafx.h"
#include "CppUnitTest.h"

#include <LexicalAnalyzer\_LexicalAnalyzerLib.h>

#include <LexicalAnalyzer\DFA.h>
#include <LexicalAnalyzer\DFACreationHelpers.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace LexicalAnalyzer;

namespace LexicalAnalyzerUnitTests
{
	TEST_CLASS(DFAUnitTest)
	{
	public:

		TEST_METHOD(ShouldCreateOK)
		{
			// TODO: Your test code here
			DFA dfa;
		}

		TEST_METHOD(ShouldAddStatesOK)
		{
			DFA dfa;
			dfa.AddState(0, DFA::ST_TERMINAL_ACCEPTED);
			dfa.AddState(1, DFA::ST_TERMINAL_REJECTED);
			dfa.AddState(2, DFA::ST_INTERMEDIATE);

			dfa.AddState(4, DFA::ST_TERMINAL_ACCEPTED);
			dfa.AddState(5, DFA::ST_TERMINAL_REJECTED);
			dfa.AddState(6, DFA::ST_INTERMEDIATE);

			dfa.AddState(7, DFA::ST_TERMINAL_ACCEPTED);
			dfa.AddState(8, DFA::ST_TERMINAL_REJECTED);
			dfa.AddState(9, DFA::ST_INTERMEDIATE);

			dfa.AddState(10, DFA::ST_TERMINAL_ACCEPTED);
			dfa.AddState(11, DFA::ST_TERMINAL_REJECTED);
			dfa.AddState(12, DFA::ST_INTERMEDIATE);

			dfa.AddState(253, DFA::ST_TERMINAL_ACCEPTED);
			dfa.AddState(123, DFA::ST_TERMINAL_REJECTED);
			dfa.AddState(456346, DFA::ST_INTERMEDIATE);
		}

		TEST_METHOD(ShouldThrowExceptionWhenDuplicateState)
		{
			DFA dfa;
			dfa.AddState(0, DFA::ST_TERMINAL_ACCEPTED);
			dfa.AddState(1, DFA::ST_TERMINAL_REJECTED);
			try
			{
				dfa.AddState(1, DFA::ST_INTERMEDIATE);
			}
			catch (std::logic_error&)
			{
				Assert::IsTrue(true);
				return;
			}
			Assert::IsTrue(false);
		}

		TEST_METHOD(ShouldAddDuplicateStateSameType)
		{
			DFA dfa;
			dfa.AddState(0, DFA::ST_TERMINAL_ACCEPTED);
			dfa.AddState(1, DFA::ST_TERMINAL_REJECTED);
			dfa.AddState(1, DFA::ST_TERMINAL_REJECTED);
		}

		TEST_METHOD(ShouldRemainInStartState)
		{
			{
				DFA dfa;
				dfa.AddState(1, DFA::ST_TERMINAL_ACCEPTED);
				dfa.SetStartState(1);
				dfa.Reset();
				auto state = dfa.GetCurrentState();
				Assert::IsTrue(DFA::ST_TERMINAL_ACCEPTED == state);
			}

			{
				DFA dfa;
				dfa.AddState(4, DFA::ST_TERMINAL_REJECTED);
				dfa.SetStartState(4);
				dfa.Reset();
				auto state = dfa.GetCurrentState();
				Assert::IsTrue(DFA::ST_TERMINAL_REJECTED == state);
			}

			{
				DFA dfa;
				dfa.AddState(123, DFA::ST_INTERMEDIATE);
				dfa.SetStartState(123);
				dfa.Reset();
				auto state = dfa.GetCurrentState();
				Assert::IsTrue(DFA::ST_INTERMEDIATE == state);
			}

		}

		//------------------------------------------------------------------------------
		// aaaa...a 
		TEST_METHOD(ShouldAcceptCase1)
		{
			DFA dfa;
			dfa.AddState(1, DFA::ST_TERMINAL_ACCEPTED);
			dfa.AddState(2, DFA::ST_TERMINAL_REJECTED);
			dfa.AddTransition(1, 'a', 1);
			dfa.AddTransition(1, 'b', 2);
			dfa.AddTransition(1, 'c', 2);
			dfa.SetStartState(1);
			dfa.Reset();
			
			for (int i = 0; i < 50; ++i)
				Assert::IsTrue(DFA::ST_TERMINAL_ACCEPTED == dfa.AddInput('a'));

			dfa.Reset();

			for (int i = 0; i < 10; ++i)
				Assert::IsTrue(DFA::ST_TERMINAL_ACCEPTED == dfa.AddInput('a'));
			auto state1 = dfa.AddInput('b');
			Assert::IsTrue(DFA::ST_TERMINAL_REJECTED == state1);
			Assert::IsTrue(DFA::ST_TERMINAL_REJECTED == dfa.AddInput('b'));
			Assert::IsTrue(DFA::ST_TERMINAL_REJECTED == dfa.AddInput('c'));
			Assert::IsTrue(DFA::ST_TERMINAL_REJECTED == dfa.AddInput('a'));

			dfa.Reset();
			Assert::IsTrue(DFA::ST_TERMINAL_ACCEPTED == dfa.GetCurrentState());

			Assert::IsTrue(DFA::ST_TERMINAL_ACCEPTED == dfa.Process("aaaaaaa"));
			Assert::IsTrue(DFA::ST_TERMINAL_REJECTED == dfa.Process("aaabaaa"));
		}

		TEST_METHOD(ShouldConstructDfaForStringCase1)
		{
			auto dfa = DFACreationHelpers::CreateForString("hello world");

			Assert::IsTrue(DFA::ST_TERMINAL_ACCEPTED == dfa.Process("hello world"));
			Assert::IsTrue(DFA::ST_TERMINAL_ACCEPTED != dfa.Process("hello"));
			Assert::IsTrue(DFA::ST_TERMINAL_ACCEPTED != dfa.Process("hello "));
			Assert::IsTrue(DFA::ST_TERMINAL_ACCEPTED != dfa.Process("world"));
			Assert::IsTrue(DFA::ST_TERMINAL_ACCEPTED != dfa.Process("123 c21dxd"));
		}

		TEST_METHOD(ShouldConstructDfaForStringCase2)
		{
			auto dfa = DFACreationHelpers::CreateForString("int");

			Assert::IsTrue(DFA::ST_TERMINAL_ACCEPTED == dfa.Process("int"));
			Assert::IsTrue(DFA::ST_TERMINAL_ACCEPTED != dfa.Process("intint"));
			Assert::IsTrue(DFA::ST_TERMINAL_ACCEPTED != dfa.Process(" int"));
			Assert::IsTrue(DFA::ST_TERMINAL_ACCEPTED != dfa.Process("int "));
			Assert::IsTrue(DFA::ST_TERMINAL_ACCEPTED != dfa.Process("123int1243"));
			Assert::IsTrue(DFA::ST_TERMINAL_ACCEPTED != dfa.Process(" int \n"));
		}
	};
}