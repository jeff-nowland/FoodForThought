
#include "Problem.h"
#include "Element.h"
#include "Exception.h"

#include <algorithm>

namespace f4t
{
	Problem::Problem()
		:	elements_(),
			buffers_(),
			total_dim_(0),
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

		auto& name = element->name();
		if (name.empty()) {
			throw invalid_name_error("element(empty name)");
		}

		if (structure_finalized_) {
			throw structure_finalized_error("problem(structure-finalized)");
		}

		if (!elements_.insert(std::make_pair(name, element)).second) {
			throw invalid_name_error("element(duplicate name)");
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

	Virtual_Buffer* Problem::make_buffer(const std::string& name, std::size_t count, std::size_t dim)
	{
		if (name.empty()) {
			throw invalid_name_error("name(empty)");
		}

		if (structure_finalized_) {
			throw structure_finalized_error("problem(structure-finalized)");
		}

		auto r = buffers_.insert(std::make_pair(name, Virtual_Buffer(count, dim)));
		if (!r.second) {
			throw invalid_name_error("buffer(duplicate name)");
		}
		total_dim_ += (count * dim);
		return &r.first->second;
	}
}

