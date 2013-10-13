#ifndef f4t_Problem_h
#define f4t_Problem_h	1

#include "f4t.h"
#include "Exception.h"
#include "Virtual_Buffer.h"

#include <string>
#include <unordered_map>

namespace f4t
{
	class Element;
	class Virtual_Buffer;


	///	@class Problem.	Base implementation of a problem to be solved.
	class Problem
	{
		std::unordered_map<std::string, Element*>		elements_;
		std::unordered_map<std::string, Virtual_Buffer>	buffers_;
		std::size_t										total_dim_;
		bool											structure_finalized_;

		Problem(const Problem&)	= delete;
		Problem& operator =(const Problem&) = delete;
	public:
		F4T_DECLARE_SIMPLE_EXCEPTION(problem_error, std::runtime_error);
		F4T_DECLARE_SIMPLE_EXCEPTION(element_owned_error, problem_error);
		F4T_DECLARE_SIMPLE_EXCEPTION(structure_finalized_error, problem_error);
		F4T_DECLARE_SIMPLE_EXCEPTION(invalid_name_error, problem_error);

		///	Initialize.
		Problem();

		///	Destroy.
		virtual ~Problem();

		///	Finalize the structure, allocate all memory required, inform all
		/// elements that structure is finalized.
		virtual void finalize_structure();

		///	Add an element to the problem.
		///	@param	element	The element to add.
		void add(Element* element);

		///	Remove an element from the problem.
		///	@param	element	The element to remove.
		void remove(Element* element);

		///	Determine if a given elements is in the problem set.
		///	@param	name	The name of the element.
		///	@returns	true if element in set, false if not.
		bool has_element(const std::string& name) const
		{ return elements_.find(name) == elements_.end(); }

		///	Get an element from the problem set.
		///	@param	name	The name of the element.
		///	@returns		An element or nullptr if not in set.
		Element* get_element(const std::string& name) const
		{
			auto pos = elements_.find(name);
			return pos != elements_.end() ? const_cast<Element*>(pos->second) : nullptr;
		}

		Virtual_Buffer* make_buffer(const std::string& name, std::size_t count, std::size_t dim);

		Virtual_Buffer* get_buffer(const std::string& name) const
		{
			auto pos = buffers_.find(name);
			return pos != buffers_.end() ? const_cast<Virtual_Buffer*>(&pos->second) : nullptr;
		}

		bool has_buffer(const std::string& name) const
		{
			return buffers_.find(name) != buffers_.end();
		}

		///	Get structure finalized flag.
		bool structure_finalized() const
		{ return structure_finalized_; }
	};
}



#endif
