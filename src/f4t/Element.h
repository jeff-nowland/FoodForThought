#ifndef f4t_Element_h
#define f4t_Element_h	1

#include "f4t.h"

#include <string>

namespace f4t
{
	class Problem;

	///	@class Element. A part of a problem's solution.
	class Element
	{
		friend class Problem;

		Problem*	problem_;
		std::string	name_;

	protected:
		Element(Problem* problem, const std::string& name);

	public:
		virtual ~Element();

		Problem* problem() const
		{ return problem_; }

		const std::string& name() const
		{ return name_; }
	};
}



#endif
