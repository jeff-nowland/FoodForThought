#ifndef f4t_View_h
#define f4t_View_h	1

#include "f4t.h"

#include <iterator>
#include <limits>

namespace f4t
{
	///	@class View.	Track a view of a contiguous region of memory.
	///	@typeparam	T	The type of element to track.
	template <typename T>
	class View
	{
	public:
		typedef std::size_t		size_type;
		typedef std::ptrdiff_t	difference_type;
		typedef T				value_type;
		typedef T*				pointer;
		typedef T&				reference;
		typedef const T*		const_pointer;
		typedef const T&		const_reference;
		typedef pointer			iterator;
		typedef const_pointer	const_iterator;

		typedef std::reverse_iterator<iterator>			reverse_iterator;
		typedef std::reverse_iterator<const_iterator>	const_reverse_iterator;

	private:
		pointer	first_ = nullptr,
				last_ = nullptr;
	public:
		///	Initialize.  Start and end are null pointers.
		View()							= default;

		///	Copy.
		View(const View&)				= default;

		///	Destroy.
		~View()							= default;

		///	Assign.
		View& operator =(const View&)	= default;

		///	Initialize.
		///	@param	first	The start of memory.
		///	@param	last	The end of memory.
		View(const_pointer first, const_pointer last)
			:	first_(const_cast<pointer>(first)),
				last_(const_cast<pointer>(last))
		{}

		///	Initialize.
		///	@param	first	The start of memory.
		///	@param	n		The size of memory.
		View(const_pointer first, size_type n)
			:	first_(const_cast<pointer>(first)),
				last_(first_ + n)
		{}

		///	Swap contents.
		///	@param o	Object to swap with.
		View& swap(View& o)
		{
			std::swap(first_, o.first_);
			std::swap(last_, o.last_);
			return *this;
		}

		///	Assign new region.
		///	@param	first	Start of view.
		///	@param	last	End of view.
		View& assign(const_pointer first, const_pointer last)
		{
			first_	= const_cast<pointer>(first);
			last_	= const_cast<pointer>(last);
			return *this;
		}

		///	Assign new region.
		///	@param	first	Start of view.
		///	@param	n		Size of view.
		View& assign_n(const_pointer first, size_type n)
		{
			first_	= const_cast<pointer>(first);
			last_	= first_ + n;
			return *this;
		}

		///	Determine if view is empty.
		bool empty() const
		{ return first_ >= last_; }

		///	Get size of view.
		size_type size() const
		{ return first_ < last_ ? size_type(last_ - first_) : size_type(0); }

		///	Get the maximum possible size of view.
		static size_type max_size() const
		{ return std::numeric_limits<size_type>::max() / sizeof(T); }

		///	Get pointer to data.
		pointer data()
		{ return first_; }

		///	Get pointer to data.
		const_pointer data() const
		{ return first_; }

		///	Get pointer to data.
		const_pointer cdata() const
		{ return first_; }

		///	Reference element in view.
		reference operator()(size_type n)
		{ return first_[n]; }

		///	Reference element in view.
		const_reference operator()(size_type n) const
		{ return first_[n]; }

		///	Start of view iterator.
		iterator begin()
		{ return first_; }

		///	Start of view iterator.
		const_iterator begin() const
		{ return first_; }

		///	Start of view iterator.
		const_iterator cbegin() const
		{ return first_; }

		///	End of view iterator.
		iterator end()
		{ return last_; }

		///	End of view iterator.
		const_iterator end() const
		{ return last_; }

		///	End of view iterator.
		const_iterator cend() const
		{ return last_; }

		///	Start of view iterator in reverse.
		reverse_iterator rbegin()
		{ return reverse_iterator(last_); }

		///	Start of view iterator in reverse.
		const_reverse_iterator rbegin() const
		{ return const_reverse_iterator(last_); }

		///	Start of view iterator in reverse.
		const_reverse_iterator crbegin() const
		{ return const_reverse_iterator(last_); }

		///	End of view iterator in reverse.
		reverse_iterator rend()
		{ return reverse_iterator(first_); }

		///	End of view iterator in reverse.
		const_reverse_iterator rend() const
		{ return const_reverse_iterator(first_); }

		///	End of view iterator in reverse.
		const_reverse_iterator crend() const
		{ return const_reverse_iterator(first_); }

		///	Create a new view.
		///	@param	first	Offset into this view of the start of the new view to create.  If first < 0 then as if from end.
		///	@param	last	Offset into this view of the end of the new view to create. If last < 0 then as if from end.
		///	@returns		A new view offset from the original view by first and last offsets.
		View slice(difference_type first = 0, difference_type last = std::numeric_limits<difference_type>::max()) const
		{
			difference_type n = difference_type(size());
			if (first < 0) {
				first += n;
			}
			first = clamp(first, 0, n);

			if (last < 0) {
				last += n;
			}
			last = clamp(last, first, n);
			return View(first_ + first, first_ + last);
		}

		void copy_to(pointer p, difference_type offset = 0, difference_type n = std::numeric_limits<difference_type>::max()) const
		{
			auto s = slice(offset, n);
			std::copy(s.begin(), s.end(), p);
		}

		void copy(const_pointer p, difference_type offset = 0, difference_type n = std::numeric_limits<difference_type>::max())
		{
			auto s = slice(offset, n);
			std::copy(p, p + s.size(), s.begin());
		}

		void copy_to(View& o, difference_type offset = 0, difference_type n = std::numeric_limits<difference_type>::max(),
					 difference_type o_offset = 0, difference_type o_n = std::numeric_limits<difference_type>::max()) const
		{
			auto s = slice(offset, n),
				d = o.slice(o_offset, o_n);

			auto nc = std::min(s.size(), d.size());
			std::copy(s.begin(), s.begin() + nc, d.begin());
		}

		void copy(const View& o, difference_type offset = 0, difference_type n = std::numeric_limits<difference_type>::max(),
				  difference_type o_offset = 0, difference_type o_n = std::numeric_limits<difference_type>::max())
		{ o.copy_to(*this, o_offset, o_n, offset, n); }

		void fill(const T& value, difference_type offset = 0, difference_type n = std::numeric_limits<difference_type>::max())
		{
			auto s = slice(offset, n);
			std::fill(s.begin(), s.end(), value);
		}

		template <typename FN>
		void assign(const FN& fn)
		{
			auto first	= begin(), last	= end();
			while (first != last) {
				*first++ = fn();
			}
		}

		template <typename U, typename FN>
		void assign(const U& u, const FN& fn)
		{
			auto first = begin(), last	= end();
			auto u_pos = std::begin(u);
			while (first != last) {
				*first++ = fn(*u_pos++);
			}
		}

		template <typename U, typename V, typename FN>
		void assign(const U& u, const V& v, const FN& fn)
		{
			auto first = begin(), last	= end();
			auto u_pos = std::begin(u);
			auto v_pos = std::begin(v);
			while (first != last) {
				*first++ = fn(*u_pos++, *v_pos++);
			}
		}
	};
}



#endif
