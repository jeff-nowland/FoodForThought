
#include "Problem.h"
#include "Element.h"

#include <algorithm>

namespace f4t
{
	Problem::Problem()
		:	elements_()
	{}

	Problem::~Problem()
	{
		for (auto& v : elements_) {
			v.second->problem_ = nullptr;
		}
	}

	void Problem::add(Element* element)
	{
		assert(element != nullptr
			   && element->problem_ == nullptr
			   && elements_.find(element->name() == elements_.end()));

		elements_.insert(std::make_pair(element->name(), element));
		element->problem_ = this;
	}

	void Problem::remove(Element* element)
	{
		assert(element != nullptr
			   && element->problem_ == this);
		elements_.erase(element->name());
		element->problem_ = nullptr;
	}
}
