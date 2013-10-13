#ifndef f4t_Virtual_Buffer_h
#define f4t_Virtual_Buffer_h	1

#include "View.h"

namespace f4t
{
	///	@class	Virtual_Buffer. Holds a buffer of slices.
	class Virtual_Buffer
	{
	public:
		typedef View<scalar>	view_type;

		typedef view_type::size_type		size_type;
		typedef view_type::difference_type	difference_type;
		typedef view_type::value_type		value_type;
		typedef view_type::pointer			pointer;
		typedef view_type::reference		reference;
		typedef view_type::const_pointer	const_pointer;
		typedef view_type::const_reference	const_reference;
		typedef view_type::iterator			iterator;
		typedef view_type::const_iterator	const_iterator;

	private:
		size_type	count_ = 0,
					dim_ = 0;
		view_type	view_;

	public:
		///	Initialize.
		Virtual_Buffer() = default;

		///	Copy.
		Virtual_Buffer(const Virtual_Buffer&) = default;

		///	Destroy.
		~Virtual_Buffer() = default;

		///	Initialize.  Actual pointers will be resolved later.
		///	@param	count	Number of slices.
		///	@param	dim		Size of slice.
		Virtual_Buffer (size_type count, size_type dim)
			:	count_(count),
				dim_(dim),
				view_()
		{
		}

		///	Assign.
		Virtual_Buffer& operator =(const Virtual_Buffer&) = default;

		///	Assign pointer.
		///	@param	p	The start of the first slice.
		Virtual_Buffer& operator =(const_pointer p)
		{
			view_.assign_n(p, count_ * dim_);
			return *this;
		}

		///	Swap contents.
		///	@param	o	Buffer to swap.
		Virtual_Buffer& swap(Virtual_Buffer& o)
		{
			std::swap(count_, o.count_);
			std::swap(dim_, o.dim_);
			view_.swap(o.view_);
			return *this;
		}

		///	Reset the buffer data.
		Virtual_Buffer& reset()
		{
			view_.fill(0.0);
			return *this;
		}

		///	Determine if empty buffer.
		bool empty() const
		{
			return count_ == 0 || dim_ == 0;
		}

		///	Get size of buffer slice.
		size_type size() const
		{
			return dim_;
		}

		///	Get total allocation size.
		size_type allocated_size () const
		{
			return count_ * dim_;
		}

		///	Resolve a buffer slice.
		///	@param	offset	The slice offset.
		view_type resolve(size_type offset)
		{
			offset %= count_;
			offset *= dim_;
			return view_.slice(offset, offset + dim_);
		}
	};
}


#endif
