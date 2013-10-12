#ifndef f4t_check_Check_Console_h
#define f4t_check_Check_Console_h	1

#include "Check.h"

namespace f4t
{
	namespace check
	{
		class Console :
			public Test_Listener
		{
			Test_Suite*	suite_;
			void*		context_;

			Console();
			~Console();
		public:
			static Console& instance();

			virtual void start() override;
			virtual void stop() override;
			virtual void enter(Test_Suite* suite) override;
			virtual void leave(Test_Suite* suite) override;
			virtual void begin(Test_Case* test) override;
			virtual void end(Test_Case* test) override;
			virtual void notify(Test_Base* test, const Test_Fail& failure) override;
			virtual void error(Test_Base* test, const std::string& best_guess) override;

			bool run(int ac, char**av);

			Test_Suite* suite() const
			{ return suite_; }
		};
	}
}

#define F4T_CHECK_CONSOLE_MAIN					\
	int main(int ac, char** av)					\
	{											\
		using f4t::check::Console;				\
												\
		Console& console = Console::instance();	\
		return console.run(ac, av) ? 0 : 1;		\
	}


#define F4T_CHECK_CONSOLE_SUBSUITE_(NAME, SUPER, ORDER)		\
	class F4T_CHECK_TOK(NAME, _SubSuite)					\
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

#define F4T_CHECK_CONSOLE_SUBSUITE(NAME, SUPER, ORDER)	\
	F4T_CHECK_CONSOLE_SUBSUITE_(NAME, F4T_CHECK_TOK(SUPER, _SubSuite)::instance(), ORDER)

#define F4T_CHECK_CONSOLE_SUITE(NAME, ORDER)	\
	F4T_CHECK_CONSOLE_SUBSUITE_(NAME, f4t::check::Console::instance().suite(), ORDER)

#define F4T_CHECK_CONSOLE_TEST(NAME, SUITE, ORDER)											\
	class F4T_CHECK_TOK(NAME, _Test)														\
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
		virtual void execute(f4t::check::Test_Listener* listener);							\
	};																						\
	F4T_CHECK_TOK(NAME, _Test) F4T_CHECK_TOK(NAME, _Test)::instance_;						\
	void F4T_CHECK_TOK(NAME, _Test)::execute(f4t::check::Test_Listener* listener)

#define F4T_CHECK_CONSOLE_TEST_EXPECT_EXCEPTION(NAME, SUITE, ORDER, CATCH)					\
	class F4T_CHECK_TOK(NAME, _Test)														\
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
										"no exception of type " F4T_CHECK_STR(CATCH))		\
		}																					\
		catch(CATCH&) {																		\
		}																					\
		catch(...) {																		\
			throw;																			\
		}																					\
	}																						\
	void F4T_CHECK_TOK(NAME, _Test)::execute_(f4t::check::Test_Listener* listener)

#endif

