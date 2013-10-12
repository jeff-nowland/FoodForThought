#ifndef f4t_check_Check_h
#define f4t_check_Check_h	1


#include <cstddef>
#include <string>
#include <vector>
#include <ostream>

#include <boost/lexical_cast.hpp>

#define F4T_CHECK_STR(S)		F4T_CHECK_STR_(S)
#define F4T_CHECK_STR_(S)		#S
#define F4T_CHECK_TOK(A, B)		F4T_CHECK_TOK_(A, B)
#define F4T_CHECK_TOK_(A, B)	A##B

namespace f4t
{

	namespace check
	{
		class Test_Suite;
		class Test_Listener;

		///	@class	Test_Base.  Base class for test objects.
		class Test_Base
		{
			friend class Test_Suite;

			Test_Suite*	suite_;
			int			order_;

			Test_Base(const Test_Base&)				= delete;
			Test_Base& operator =(const Test_Base&)	= delete;

		protected:
			///	Initialize.
			///	@param	suite	Pointer to test suite this test belongs to.
			///	@param	order	Relative order within test suite that this test should be run in.
			Test_Base(Test_Suite* suite, int order);

		public:
			///	Destroy.
			virtual ~Test_Base();

			///	Get the test suite this test belongs to.
			Test_Suite* suite() const
			{ return suite_; }

			///	Get the suite relative order this test should be executed in.
			int order() const
			{ return order_; }

			///	Get the name of the test.
			virtual std::string name() const = 0;

			///	Get the number of test cases run by this test.
			virtual std::size_t count() const = 0;

			///	Run this test.
			///	@param	listener	A place to output test results.
			virtual void run(Test_Listener* listener) = 0;

			///	Get the fully qualified name (= suite's fully qualified name + '.' + name).
			std::string fully_qualified_name() const;
		};

		///	@class Test_Suite.  Container for executing a group of tests.
		class Test_Suite :
			public Test_Base
		{
			std::vector<Test_Base*>	tests_;

		protected:
			///	Initialize.
			///	@param	suite	Test suite this suite belongs to.
			///	@param	order	Relative order within parent suite to execute this suite.
			Test_Suite(Test_Suite* suite, int order);

		public:
			///	Destroy.
			~Test_Suite();

			///	@inherited
			virtual std::size_t count() const override;

			///	@inherited
			virtual void run(Test_Listener* listener) override;

			///	Add a test to this suite.
			///	@param	test	The test to add.
			void add(Test_Base* test);

			///	Remove test from this suite.
			///	@param	test	The test to remove.
			void remove(Test_Base* test);
		};

		///	@class	Test_Case.	Implements a single test instance.
		class Test_Case :
			public Test_Base
		{
		protected:
			///	Initialize.
			///	@param	suite	The owning suite.
			///	@param	order	The suite relative order.
			Test_Case(Test_Suite* suite, int order);

			///	Actually rn the test.
			/// @param	listener	A place to output test results.
			virtual void execute(Test_Listener* listener) = 0;
		public:
			///	@inherited
			virtual std::size_t count() const override;

			///	@inherited
			virtual void run(Test_Listener* listener) override;
		};

		///	@class	Test_Fail.	An instance of a test failure notification.
		class Test_Fail
		{
		public:
			///	@class Param.	A place to put information about the failure.
			class Param
			{
				std::string name_;
				std::string value_;

				template <typename T>
				static std::string cvt_(const T& v)
				{ return boost::lexical_cast<std::string>(v); }

			public:
				///	Initialize.
				///	@typeparam	T		The type of value for the parameter.
				///	@param		name	The name of the parameter.
				///	@param		v		The value of the parameter.
				template <typename T>
				Param(const std::string& name, const T& v)
					:	name_(name),
						value_(cvt_(v))
				{}

				Param(const Param&) = default;
				~Param() = default;
				Param& operator =(const Param&) = default;

				///	Get the name of the parameter.
				const std::string& name() const
				{ return name_; }

				///	Get the value of the parameter.
				const std::string& value() const
				{ return value_; }
			};

		private:
			const char*			file_;
			int					line_;
			std::string			mesg_;
			std::vector<Param>	params_;
		public:
			///	Initialize.
			///	@param	file	The source code file where the failure occured.
			///	@param	line	The source code line where the failure occured.
			///	@param	mesg	The failure message.
			Test_Fail(const char* file, int line, const std::string& mesg);
			Test_Fail(const Test_Fail& o);
			~Test_Fail() = default;
			Test_Fail& operator =(const Test_Fail& o);

			///	Add a parameter to the failure.
			///	@typename	T		The type of the parameter's value.
			///	@param		name	The name of the parameter.
			///	@param		v		The value of the parameter.
			template <typename T>
			Test_Fail& add(const std::string& name, const T& v)
			{
				params_.push_back(Param(name, v));
				return *this;
			}

			///	Swap contents of failure.
			///	@param	o	The failure to swap with.
			void swap(Test_Fail& o);

			///	Get the source code file.
			const char* file() const
			{ return file_; }

			///	Get the source code line.
			int line() const
			{ return line_; }

			///	Get the failure message.
			const std::string& mesg() const
			{ return mesg_; }

			///	Get the parameter list.
			const std::vector<Param>& params() const
			{ return params_; }

			Test_Fail& F4T_CHECK_FAIL_A;
			Test_Fail& F4T_CHECK_FAIL_B;
		};

		std::ostream& operator <<(std::ostream& strm, const Test_Fail& fail);

		///	Make a failure.
		///	@param	file	The source code file.
		///	@param	line	The source code line.
		///	@param	m		The failure message.
		inline Test_Fail make_fail(const char* file, int line, const std::string& m)
		{ return Test_Fail(file, line, m); }

		///	@class	Test_Listener.	A place to manage test events.
		class Test_Listener
		{
			Test_Listener*				parent_;
			std::vector<Test_Listener*>	children_;

		protected:
			///	Initialize.
			///	@param	parent	An owning listener or nullptr.
			Test_Listener(Test_Listener* parent);

		public:
			///	Destroy.
			virtual ~Test_Listener();

			///	Called prior to starting a test run.
			virtual void start();

			///	Called on completion of a test run.
			virtual void stop();

			///	Called when starting a test suite.
			///	@param	suite	The test suite that is about to run.
			virtual void enter(Test_Suite* suite);

			///	Called after completion of a test suite.
			///	@param	suite	The test suite that just completed.
			virtual void leave(Test_Suite* suite);

			///	Called when starting a test.
			///	@param	test	The test about to run.
			virtual void begin(Test_Case* test);

			///	Called after completion of a test.
			///	@param	test	The test that just completed.
			virtual void end(Test_Case* test);

			///	Notify of a test failure.
			///	@param	test	The failing test.
			///	@param	failure	The failure information.
			virtual void notify(Test_Base* test, const Test_Fail& failure);

			///	Notify of a test error (exception thrown that was not properly checked for).
			///	@param	test		The errant test.
			///	@param	best_guess	Our best guess as to what went wrong.  For exceptions in the std::exception
			///						class hierarchy this will be the output of the what() method, otherwise
			///						I just say I have no idea.
			virtual void error(Test_Base* test, const std::string& best_guess);

			///	Add a child test listener.
			///	@param	child	The child to add.
			void add(Test_Listener* child);

			///	Remove a child test listner.
			///	@param	child	The child to remove.
			void remove(Test_Listener* child);
		};
	}
}

///	@macro F4T_CHECK_FAIL_A.	Implements the start of a macro chaining event that helps
///	one to add parameters to a test failure.
///	@param	X	A parameter that will be both the stringized name and value of the parameter.
#define F4T_CHECK_FAIL_A(X)		F4T_CHECK_FAIL_C(X, B)

///	@macro F4T_CHECK_FAIL_B.	The second half of the macro chaing facility.
///	@param	X	A parameter that will be both the stringized name and value of the parameter.
#define F4T_CHECK_FAIL_B(X)		F4T_CHECK_FAIL_C(X, A)

///	@macro F4T_CHECK_FAIL_C.	Glue to put @see F4T_CHECK_FAIL_A and @see F4T_CHECK_FAIL_B together.
///	@param	X	A parameter that will be both the stringized name and value of the parameter.
///	@param	N	The next in chain.
#define F4T_CHECK_FAIL_C(X, N)	F4T_CHECK_FAIL_A.add(F4T_CHECK_STR(X), (X)).F4T_CHECK_TOK(F4T_CHECK_FAIL_, N)

///	@macro	F4T_CHECK_FAILURE.	Packages a failure and starts the notification process.
///	@param	X					The failure, becomes mesg parameter of a @see Test_Fail
#define F4T_CHECK_FAILURE(X)	throw f4t::check::make_fail(__FILE__, __LINE__, F4T_CHECK_STR(X)).F4T_CHECK_FAIL_A

///	@macro	F4T_CHECK.	Test an expression, fail if the expression is false.
///	@param	EXPR		The expression.
#define F4T_CHECK(EXPR)			if (EXPR);else F4T_CHECK_FAILURE(EXPR)

#endif

