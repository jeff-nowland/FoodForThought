
#include "Problem.h"
#include "Element.h"
#include "Exception.h"

#include <algorithm>

namespace f4t
{
	Problem::Problem()
		:	elements_(),
			structure_finalized_(false)
	{}

	Problem::~Problem()
	{
		for (auto& v : elements_) {
			v.second->problem_ = nullptr;
		}
	}

	void Problem::finalize_structure()
	{
		structure_finalized_ = true;

		for (auto& v : elements_) {
			v.second->structure_finalized();
		}
	}

	void Problem::add(Element* element)
	{
		if (element == nullptr) {
			throw argument_null_error("element(nullptr)");
		}

		if (element->problem_ != nullptr) {
			if (element->problem_ != this) {
				throw element_owned_error("element(already owned)");
			}
			return;
		}

		if (structure_finalized_) {
			throw structure_finalized_error("problem(structure-finalized)");
		}

		if (!elements_.insert(std::make_pair(element->name(), element)).second) {
			throw duplicate_argument_error("element(duplicate name)");
		}

		element->problem_ = this;
	}

	void Problem::remove(Element* element)
	{
		if (element == nullptr) {
			throw argument_null_error("element(nullptr)");
		}

		if (element->problem_ != this) {
			throw element_owned_error("element(not owned by problem)");
		}

		elements_.erase(element->name());
		element->problem_ = nullptr;
	}
}

