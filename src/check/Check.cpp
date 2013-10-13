
#include "Check.h"

#include <cassert>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <sstream>

namespace f4t
{
	namespace check
	{
		/* ----- Test_Base ----- */
		Test_Base::Test_Base(Test_Suite* suite, int order)
			:	suite_(nullptr),
				order_(order)
		{
			if (suite) {
				suite->add(this);
			}
		}

		Test_Base::~Test_Base()
		{
			if (suite_) {
				suite_->remove(this);
			}
		}

		std::string Test_Base::fully_qualified_name() const
		{
			std::string result;
			if (suite_) {
				result = suite_->fully_qualified_name();
				result.append(1, '.');
			}
			result.append(name());
			return result;
		}

		/* ----- Test_Suite ----- */
		Test_Suite::Test_Suite(Test_Suite* suite, int order)
			:	Test_Base(suite, order)
		{}

		Test_Suite::~Test_Suite()
		{
			std::for_each(tests_.begin(), tests_.end(),
						  [](Test_Base* p) { p->suite_ = nullptr; });
		}

		std::size_t Test_Suite::count() const
		{
			return std::accumulate(tests_.begin(), tests_.end(), std::size_t(0),
								   [](std::size_t a, Test_Base* p) { return a + p->count(); });
		}

		void Test_Suite::run(Test_Listener* listener)
		{
			assert(listener != nullptr);

			listener->enter(this);
			std::for_each(tests_.begin(), tests_.end(),
						  [=](Test_Base* p) { p->run(listener); });
			listener->leave(this);
		}

		void Test_Suite::add(Test_Base* test)
		{
			assert(test != nullptr && test->suite_ == nullptr);

			tests_.insert(std::upper_bound(tests_.begin(), tests_.end(), test,
										   [](Test_Base* pa, Test_Base* pb) { return pa->order_ < pb->order_; }),
						  test);
			test->suite_ = this;
		}

		void Test_Suite::remove(Test_Base* test)
		{
			assert(test != nullptr && test->suite_ == this);

			tests_.erase(std::find(tests_.begin(), tests_.end(), test));
			test->suite_ = nullptr;
		}

		/* ----- Test_Case ----- */
		Test_Case::Test_Case(Test_Suite* suite, int order)
			:	Test_Base(suite, order)
		{
		}

		std::size_t Test_Case::count() const
		{ return 1; }

		void Test_Case::run(Test_Listener* listener)
		{
			assert(listener != nullptr);

			listener->begin(this);
			try {
				execute(listener);
			}
			catch(Test_Fail& fail) {
				listener->notify(this, fail);
			}
			catch(std::exception& ex) {
				listener->error(this, ex.what());
			}
			catch(...) {
				listener->error(this, "I have no idea");
			}
			listener->end(this);
		}

		/* ----- Test_Fail ----- */
#undef F4T_CHECK_FAIL_A
#undef F4T_CHECK_FAIL_B
		Test_Fail::Test_Fail(const char* file, int line, const std::string& mesg)
			:	file_(file),
				line_(line),
				mesg_(mesg),
				params_(),
				F4T_CHECK_FAIL_A(*this),
				F4T_CHECK_FAIL_B(*this)
		{}

		Test_Fail::Test_Fail(const Test_Fail& o)
			:	file_(o.file_),
				line_(o.line_),
				mesg_(o.mesg_),
				params_(o.params_),
				F4T_CHECK_FAIL_A(*this),
				F4T_CHECK_FAIL_B(*this)
		{}

		Test_Fail& Test_Fail::operator =(const Test_Fail& o)
		{
			if (this != &o) {
				Test_Fail tmp(o);
				swap(tmp);
			}
			return *this;
		}

		void Test_Fail::swap(Test_Fail& o)
		{
			std::swap(file_, o.file_);
			std::swap(line_, o.line_);
			mesg_.swap(o.mesg_);
			params_.swap(o.params_);
		}

		std::ostream& operator <<(std::ostream& strm, const Test_Fail& fail)
		{
			std::stringstream ss;

			auto& mesg		= fail.mesg();
			auto& params	= fail.params();

			ss << fail.file() << '@' << fail.line();

			bool delimited = false;
			if (!mesg.empty()) {
				ss << ": " << fail.mesg();
				delimited = true;
			}

			if (!params.empty()) {
				auto pos = params.begin(),
					 end = params.end();

				if (!delimited) {
					ss << ':';
					delimited = true;
				}

				ss << " ['"<<pos->name() << "'='" << pos->value() << '\'';
				while (++pos != end) {
					ss << ",'" << pos->name() << "'='" << pos->value() << '\'';
				}
				ss << ']';
			}

			return (strm << ss.str());
		}

		/* ----- Test_Listener ----- */
		Test_Listener::Test_Listener(Test_Listener* parent)
			:	parent_(nullptr),
				children_()
		{
			if (parent) {
				parent->add(this);
			}
		}

		Test_Listener::~Test_Listener()
		{
			std::for_each(children_.begin(), children_.end(),
						  [](Test_Listener* p) { p->parent_ = nullptr; });
			if (parent_) {
				parent_->remove(this);
			}
		}

		void Test_Listener::start()
		{
			std::for_each(children_.begin(), children_.end(),
						  [](Test_Listener* p) { p->start(); });
		}

		void Test_Listener::stop()
		{
			std::for_each(children_.begin(), children_.end(),
						  [](Test_Listener* p) { p->stop(); });
		}

		void Test_Listener::enter(Test_Suite* suite)
		{
			std::for_each(children_.begin(), children_.end(),
						  [=](Test_Listener* p) { p->enter(suite); });
		}

		void Test_Listener::leave(Test_Suite* suite)
		{
			std::for_each(children_.begin(), children_.end(),
						  [=](Test_Listener* p) { p->leave(suite); });
		}

		void Test_Listener::begin(Test_Case* test)
		{
			std::for_each(children_.begin(), children_.end(),
						  [=](Test_Listener* p) { p->begin(test); });
		}

		void Test_Listener::end(Test_Case* test)
		{
			std::for_each(children_.begin(), children_.end(),
						  [=](Test_Listener* p) { p->end(test); });
		}

		void Test_Listener::notify(Test_Base* test, const Test_Fail& failure)
		{
			std::for_each(children_.begin(), children_.end(),
						  [&](Test_Listener* p) { p->notify(test, failure); });
		}

		void Test_Listener::error(Test_Base* test, const std::string& best_guess)
		{
			std::for_each(children_.begin(), children_.end(),
						  [&](Test_Listener* p) { p->error(test, best_guess); });
		}

		void Test_Listener::add(Test_Listener* child)
		{
			assert(child != nullptr && child->parent_ = nullptr);
			children_.push_back(child);
			child->parent_ = this;
		}

		void Test_Listener::remove(Test_Listener* child)
		{
			assert(child != nullptr && child->parent_ == this);
			children_.erase(std::find(children_.begin(), children_.end(), child));
			child->parent_ = nullptr;
		}
	}
}

