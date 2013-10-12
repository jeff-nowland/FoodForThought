
#include "Element.h"
#include "Problem.h"

namespace f4t
{
	Element::Element(Problem* problem, const std::string& name)
		:	problem_(nullptr),
			name_(name)
	{
		assert(problem != nullptr);
		problem_->add(this);
	}

	Element::~Element()
	{
		if (problem_) {
			problem_->remove(this);
		}
	}

	void Element::structure_finalized()
	{
	}

}

