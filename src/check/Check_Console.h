#ifndef f4t_check_Check_Console_h
#define f4t_check_Check_Console_h	1

#include "Check.h"

namespace f4t
{
	namespace check
	{
		///	@class Console.	Implements a command line version of a check tester.
		class Console :
			public Test_Listener
		{
			Test_Suite*	suite_;
			void*		context_;

			Console();
			~Console();
		public:
			///	Get global instance.
			static Console& instance();

			///	@inherited
			virtual void start() override;

			///	@inherited
			virtual void stop() override;

			///	@inherited
			virtual void enter(Test_Suite* suite) override;

			///	@inherited
			virtual void leave(Test_Suite* suite) override;

			///	@inherited
			virtual void begin(Test_Case* test) override;

			///	@inherited
			virtual void end(Test_Case* test) override;

			///	@inherited
			virtual void notify(Test_Base* test, const Test_Fail& failure) override;

			///	@inherited
			virtual void error(Test_Base* test, const std::string& best_guess) override;

			///	Run the test suite.
			///	@param	ac	Number of command line parameters.
			///	@param	av	Command line parameters.
			///	@returns	true if all tests pass, false if any test fails or is errant.
			bool run(int ac, char**av);

			///	Get the console test suite.
			Test_Suite* suite() const
			{ return suite_; }
		};
	}
}

///	@macro	F4T_CHECK_CONSOLE_MAIN.	Implements a down and dirty main routine for
///	your console test.
#define F4T_CHECK_CONSOLE_MAIN					\
	int main(int ac, char** av)					\
	{											\
		using f4t::check::Console;				\
												\
		Console& console = Console::instance();	\
		return console.run(ac, av) ? 0 : 1;		\
	}

///	@macro	F4T_CHECK_CONSOLE_SUBSUITE_.	A macro that allows you to make a simple
///	test subsuite linked to the console test.
///	@param	NAME	The name of the suite.
///	@param	SUPER	Pointer to super-suite.
///	@param	ORDER	The suite relative order of this suite.
#define F4T_CHECK_CONSOLE_SUBSUITE_(NAME, SUPER, ORDER)		\
	class F4T_CHECK_TOK(NAME, _SubSuite) :					\
		public f4t::check::Test_Suite						\
	{														\
		static F4T_CHECK_TOK(NAME, _SubSuite)	instance_;	\
	public:													\
		F4T_CHECK_TOK(NAME, _SubSuite)()					\
			:	f4t::check::Test_Suite(SUPER, ORDER)		\
		{}													\
		virtual std::string name() const override			\
		{ return F4T_CHECK_STR(NAME); }						\
		static f4t::check::Test_Suite* instance()			\
		{ return &instance_; }								\
	};														\
	F4T_CHECK_TOK(NAME, _SubSuite) F4T_CHECK_TOK(NAME, _SubSuite)::instance_;

///	@macro	F4T_CHECK_CONSOLE_SUBSUITE_.	A macro that allows you to make a simple
///	test subsuite linked to the console test.
///	@param	NAME	The name of the suite.
///	@param	SUPER	The name of the super-suite.
///	@param	ORDER	The suite relative order of this suite.
#define F4T_CHECK_CONSOLE_SUBSUITE(NAME, SUPER, ORDER)	\
	F4T_CHECK_CONSOLE_SUBSUITE_(NAME, F4T_CHECK_TOK(SUPER, _SubSuite)::instance(), ORDER)

///	@macro	F4T_CHECK_CONSOLE_SUBSUITE_.	A macro that allows you to make a simple
///	test suite linked to the console at the root level
///	@param	NAME	The name of the suite.
///	@param	ORDER	The suite relative order of this suite.
#define F4T_CHECK_CONSOLE_SUITE(NAME, ORDER)	\
	F4T_CHECK_CONSOLE_SUBSUITE_(NAME, f4t::check::Console::instance().suite(), ORDER)

///	@macro	F4T_CHECK_CONSOL_TEST.	Declare a test case.
///	@param	NAME	The name of the test.
///	@param	SUITE	The name of the test suite.
///	@param	ORDER	The suite relative ordering of the test.
#define F4T_CHECK_CONSOLE_TEST(NAME, SUITE, ORDER)											\
	class F4T_CHECK_TOK(NAME, _Test) :														\
		public f4t::check::Test_Case														\
	{																						\
		static F4T_CHECK_TOK(NAME, _Test) instance_;										\
	public:																					\
		F4T_CHECK_TOK(NAME, _Test)()														\
			:	f4t::check::Test_Case(F4T_CHECK_TOK(SUITE, _SubSuite)::instance(), ORDER)	\
		{}																					\
		virtual std::string name() const override											\
		{ return F4T_CHECK_STR(NAME); }														\
	protected:																				\
		virtual void execute(f4t::check::Test_Listener* listener) override;					\
	};																						\
	F4T_CHECK_TOK(NAME, _Test) F4T_CHECK_TOK(NAME, _Test)::instance_;						\
	void F4T_CHECK_TOK(NAME, _Test)::execute(f4t::check::Test_Listener* listener)

///	@macro	F4T_CHECK_CONSOL_TEST.	Declare a test case.
///	@param	NAME	The name of the test.
///	@param	SUITE	The name of the test suite.
///	@param	ORDER	The suite relative ordering of the test.
///	@param	CATCH	The type name of an exception that can be caught indicating that the
///					test has passed.
#define F4T_CHECK_CONSOLE_TEST_EXPECT_EXCEPTION(NAME, SUITE, ORDER, CATCH)					\
	class F4T_CHECK_TOK(NAME, _Test) :														\
		public f4t::check::Test_Case														\
	{																						\
		static F4T_CHECK_TOK(NAME, _Test) instance_;										\
	public:																					\
		F4T_CHECK_TOK(NAME, _Test)()														\
			:	f4t::check::Test_Case(F4T_CHECK_TOK(SUITE, _SubSuite)::instance(), ORDER)	\
		{}																					\
		virtual std::string name() const override											\
		{ return F4T_CHECK_STR(NAME); }														\
	protected:																				\
		virtual void execute(f4t::check::Test_Listener* listener) override;					\
	private:																				\
		void execute_(f4t::check::Test_Listener* listener);									\
	};																						\
	F4T_CHECK_TOK(NAME, _Test) F4T_CHECK_TOK(NAME, _Test)::instance_;						\
	void F4T_CHECK_TOK(NAME, _Test)::execute(f4t::check::Test_Listener* listener)			\
	{																						\
		try {																				\
			execute_(listener);																\
			throw f4t::check::Test_Fail(__FILE__, __LINE__,									\
										"no exception of type " F4T_CHECK_STR(CATCH));		\
		}																					\
		catch(CATCH&) {																		\
		}																					\
		catch(...) {																		\
			throw;																			\
		}																					\
	}																						\
	void F4T_CHECK_TOK(NAME, _Test)::execute_(f4t::check::Test_Listener* listener)

#endif

