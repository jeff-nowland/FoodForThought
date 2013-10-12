
#include <check/Check_Console.h>

#include <stdexcept>

F4T_CHECK_CONSOLE_MAIN

F4T_CHECK_CONSOLE_SUITE(Main, 0)

F4T_CHECK_CONSOLE_TEST(Tube, Main, 0)
{
}

F4T_CHECK_CONSOLE_TEST_EXPECT_EXCEPTION(Tube2, Main, 0, std::runtime_error)
{
	throw std::runtime_error("oops");
}
