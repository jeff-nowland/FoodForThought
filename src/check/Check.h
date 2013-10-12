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

		class Test_Case :
			public Test_Base
		{
		protected:
			Test_Case(Test_Suite* suite, int order);

			virtual void execute(Test_Listener* listener) = 0;
		public:
			virtual std::size_t count() const override;
			virtual void run(Test_Listener* listener) override;
		};

		class Test_Fail
		{
		public:
			class Param
			{
				std::string name_;
				std::string value_;

				template <typename T>
				static std::string cvt_(const T& v)
				{ return boost::lexical_cast<std::string>(v); }

			public:
				template <typename T>
				Param(const std::string& name, const T& v)
					:	name_(name),
						value_(cvt_(v))
				{}

				Param(const Param&) = default;
				~Param() = default;
				Param& operator =(const Param&) = default;

				const std::string& name() const
				{ return name_; }

				const std::string& value() const
				{ return value_; }
			};

		private:
			const char*			file_;
			int					line_;
			std::string			mesg_;
			std::vector<Param>	params_;
		public:
			Test_Fail(const char* file, int line, const std::string& mesg);
			Test_Fail(const Test_Fail& o);
			~Test_Fail() = default;
			Test_Fail& operator =(const Test_Fail& o);

			template <typename T>
			Test_Fail& add(const std::string& name, const T& v)
			{
				params_.push_back(Param(name, v));
				return *this;
			}

			void swap(Test_Fail& o);

			const char* file() const
			{ return file_; }

			int line() const
			{ return line_; }

			const std::string& mesg() const
			{ return mesg_; }

			const std::vector<Param>& params() const
			{ return params_; }

			Test_Fail& F4T_CHECK_FAIL_A;
			Test_Fail& F4T_CHECK_FAIL_B;
		};

		std::ostream& operator <<(std::ostream& strm, const Test_Fail& fail);

		inline Test_Fail make_fail(const char* file, int line, const std::string& m)
		{ return Test_Fail(file, line, m); }

		class Test_Listener
		{
			Test_Listener*				parent_;
			std::vector<Test_Listener*>	children_;

		protected:
			Test_Listener(Test_Listener* parent);

		public:
			virtual ~Test_Listener();

			virtual void start();
			virtual void stop();
			virtual void enter(Test_Suite* suite);
			virtual void leave(Test_Suite* suite);
			virtual void begin(Test_Case* test);
			virtual void end(Test_Case* test);
			virtual void notify(Test_Base* test, const Test_Fail& failure);
			virtual void error(Test_Base* test, const std::string& best_guess);

			void add(Test_Listener* child);
			void remove(Test_Listener* child);
		};
	}
}

#define F4T_CHECK_FAIL_A(X)		F4T_CHECK_FAIL_C(X, B)
#define F4T_CHECK_FAIL_B(X)		F4T_CHECK_FAIL_C(X, A)
#define F4T_CHECK_FAIL_C(X, N)	F4T_CHECK_FAIL_A.add(F4T_CHECK_STR(X), (X)).F4T_CHECK_TOK(F4T_CHECK_FAIL_, N)
#define F4T_CHECK_FAILURE(X)	throw f4t::check::make_fail(__FILE__, __LINE__, F4T_CHECK_STR(X)).F4T_CHECK_FAIL_A
#define F4T_CHECK(EXPR)			if (EXPR);else F4T_CHECK_FAILURE(EXPR)

#endif

