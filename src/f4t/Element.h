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
		///	Initialize.
		///	@param	problem	The problem this element belongs to.
		///	@param	name	The name of the problem, must be unique to the problem.
		Element(Problem* problem, const std::string& name);

	public:
		///	Destory.
		virtual ~Element();

		///	Receive notification that the problem's structure is finalized.
		virtual void structure_finalized();

		///	Get the problem.
		Problem* problem() const
		{ return problem_; }

		///	Get the element name.
		const std::string& name() const
		{ return name_; }
	};
}



#endif
