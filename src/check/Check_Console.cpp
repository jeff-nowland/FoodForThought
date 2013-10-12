
#include "Check_Console.h"

#include <stack>
#include <iostream>

namespace f4t
{
	namespace check
	{
		Console& Console::instance()
		{
			static Console console_;
			return console_;
		}

		namespace
		{
			class Root_Suite :
				public Test_Suite
			{
			public:
				Root_Suite()
					: Test_Suite(nullptr, 0)
				{}

				virtual std::string name() const override
				{ return "root"; }
			};

			struct Stat
			{
				std::size_t	count,
							fail = 0,
							error = 0;

				Stat(std::size_t n = 0)
					: count(n)
				{}

				Stat(const Stat&)				= default;
				~Stat()							= default;
				Stat& operator =(const Stat&)	= default;

				Stat& operator += (const Stat& o)
				{
					count += o.count;
					fail += o.fail;
					error += o.error;
				}
			};

			struct Context
			{
				std::stack<Stat>	stat;
				bool				succeeded = true;

				void clear()
				{
					while (!stat.empty()) {
						stat.pop();
					}
					succeeded = true;
				}
			};

#define C	reinterpret_cast<Context*>(context_)
		}

		Console::Console()
			:	Test_Listener(nullptr),
				suite_(new Root_Suite),
				context_(new Context)
		{
		}

		Console::~Console()
		{
			delete C;
			delete suite_;
		}

		void Console::start()
		{
			std::clog << "START " << suite_->count() << " tests\n";
			C->clear();
		}

		void Console::stop()
		{
			std::clog << "STOP\n";
		}

		void Console::enter(Test_Suite* suite)
		{
			if (suite != suite_) {
				std::clog << "[SUITE ]" << suite->fully_qualified_name() << ' ' << suite->count() << " tests\n";
			}
			C->stat.push(Stat());
		}

		void Console::leave(Test_Suite* suite)
		{
			Stat result = C->stat.top();

			C->stat.pop();

			if (suite != suite_) {
				std::clog << "[------] ";
			}
			else {
				std::clog << "[======] ";
			}

			std::clog << suite->fully_qualified_name() << ' ';
			if (!result.count) {
				std::clog << "NO TESTS RUN";
			}
			else {
				auto pass = result.count - result.fail - result.error;
				auto d	= double(result.count),
					 p	= 100.0 * pass / d,
					 f	= 100.0 * result.fail / d,
					 e	= 100.0 * result.error / d;

				std::clog << result.count << " tests, "
					<< pass << " passed(" << p << "%), "
					<< result.fail << " failed(" << f << "%), "
					<< result.error << " tets errors(" << e << "%)";
			}
			std::clog << std::endl;
		}

		void Console::begin(Test_Case* test)
		{
			std::clog << "[  TEST] " << test->fully_qualified_name();
			C->stat.push(Stat(1));
		}

		void Console::end(Test_Case* test)
		{
			Stat result = C->stat.top();

			C->stat.pop();
			C->stat.top() += result;

			if (result.error > 0) {
				std::clog << " ERROR";
			}
			else if (result.fail > 0) {
				std::clog << " FAIL";
			}
			else {
				std::clog << " pass";
			}
			std::clog << std::endl;
		}

		void Console::notify(Test_Base* test, const Test_Fail& failure)
		{
			std::cerr << ", " << failure;
			++C->stat.top().fail;
			C->succeeded = false;
		}

		void Console::error(Test_Base* test, const std::string& best_guess)
		{
			std::cerr << ", ERROR: " << best_guess;
			++C->stat.top().error;
			C->succeeded = false;
		}


		bool Console::run(int ac, char** av)
		{
			start();
			suite_->run(this);
			stop();
			return C->succeeded;
		}
#undef C
	}
}
