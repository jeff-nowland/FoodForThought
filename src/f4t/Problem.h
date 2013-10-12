#ifndef f4t_Problem_h
#define f4t_Problem_h	1

#include "f4t.h"

#include <string>
#include <unordered_map>

namespace f4t
{
	class Element;

	///	@class Problem.	Base implementation of a problem to be solved.
	class Problem
	{
		std::unordered_map<std::string, Element*>	elements_;
	public:
		Problem();
		virtual ~Problem();

		void add(Element* element);
		void remove(Element* element);
	};
}



#endif
