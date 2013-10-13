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
		static size_type max_size()
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

		///	Check same region.
		///	@param	o	View to check.
		///	@return		true if other view is same memory as this view.
		bool operator ==(const View& o) const
		{ return first_ == o.first_ && last_ == o.last_; }

		///	Check lower region.
		///	@param	o	View to check.
		///	@return		true if this region lower in memory.
		bool operator <(const View& o) const
		{ return first_ < o.first_ || (first_ == o.first_ && last_ < o.last_); }

		///	Get the overlap between this view and another view.
		///	@param	o	The other view.
		///	@return		The overlap between this memory and the other memory.
		View overlap(const View& o) const
		{
			auto o_first = std::max(first_, o.first_),
				 o_last	= std::min(last_, o.last_);

			if (o_first > o_last) {
				return View();
			}
			return View(o_first, o_last);
		}

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
			first = clamp(first, difference_type(0), n);

			if (last < 0) {
				last += n;
			}
			last = clamp(last, first, n);
			return View(first_ + first, first_ + last);
		}

		///	Copy data to external memory.
		///	@param	p	Destination pointer.
		void copy_to(pointer p) const
		{ std::copy(begin(), end(), p); }

		///	Copy data from external memory.
		///	@param	p	Source pointer.
		void copy(const_pointer p)
		{ std::copy(p, p + size(), begin()); }

		///	Copy data to another view.
		///	@param	o	The other view.
		void copy_to(View& o) const
		{
			auto s = std::min(size(), o.size());
			std::copy(begin(), begin() + s, o.begin());
		}

		///	Copy data from another view.
		///	@param	o	The other view.
		void copy(const View& o)
		{ o.copy_to(*this); }

		///	Fill the view memory with a value.
		///	@param	value	The value.
		void fill(const T& value)
		{ std::fill(begin(), end(), value); }

		///	Apply a function to the data.
		///	@param	fn	The function to apply.
		template <typename FN>
		void apply(const FN& fn)
		{
			auto first	= begin(), last	= end();
			while (first != last) {
				*first++ = fn();
			}
		}

		///	Apply a function to the data.
		///	@param	u	Source data for function.
		///	@param	fn	Function to apply.
		template <typename U, typename FN>
		void apply(const U& u, const FN& fn)
		{
			auto first = begin(), last	= end();
			auto u_pos = std::begin(u);
			while (first != last) {
				*first++ = fn(*u_pos++);
			}
		}

		///	Apply a function to the data.
		///	@param	u	Source data for function.
		///	@param	v	Second source data for function.
		///	@param	fn	Function to apply.
		template <typename U, typename V, typename FN>
		void apply(const U& u, const V& v, const FN& fn)
		{
			auto first = begin(), last	= end();
			auto u_pos = std::begin(u);
			auto v_pos = std::begin(v);
			while (first != last) {
				*first++ = fn(*u_pos++, *v_pos++);
			}
		}

		///	Apply a function to the data.
		///	@param	u	Source data for function.
		///	@param	v	Second source data for function.
		///	@param	w	Third source data for function.
		///	@param	fn	Function to apply.
		template <typename U, typename V, typename W, typename FN>
		void apply(const U& u, const V& v, const W& w, const FN& fn)
		{
			auto first = begin(), last	= end();
			auto u_pos = std::begin(u);
			auto v_pos = std::begin(v);
			auto w_pos = std::begin(w);
			while (first != last) {
				*first++ = fn(*u_pos++, *v_pos++, *w_pos++);
			}
		}

		///	Apply a function to the data.
		///	@param	u	Source data for function.
		///	@param	v	Second source data for function.
		///	@param	w	Third source data for function.
		///	@param	x	Fourth source data for function.
		///	@param	fn	Function to apply.
		template <typename U, typename V, typename W, typename X, typename FN>
		void apply(const U& u, const V& v, const W& w, const X& x, const FN& fn)
		{
			auto first = begin(), last	= end();
			auto u_pos = std::begin(u);
			auto v_pos = std::begin(v);
			auto w_pos = std::begin(w);
			auto x_pos = std::begin(x);
			while (first != last) {
				*first++ = fn(*u_pos++, *v_pos++, *w_pos++, *x_pos++);
			}
		}

		///	Apply a function to the data.
		///	@param	u	Source data for function.
		///	@param	v	Second source data for function.
		///	@param	w	Third source data for function.
		///	@param	x	Fourth source data for function.
		///	@param	y	Fifth source data for function.
		///	@param	fn	Function to apply.
		template <typename U, typename V, typename W, typename X, typename Y, typename FN>
		void apply(const U& u, const V& v, const W& w, const X& x, const Y& y, const FN& fn)
		{
			auto first = begin(), last	= end();
			auto u_pos = std::begin(u);
			auto v_pos = std::begin(v);
			auto w_pos = std::begin(w);
			auto x_pos = std::begin(x);
			auto y_pos = std::begin(y);
			while (first != last) {
				*first++ = fn(*u_pos++, *v_pos++, *w_pos++, *x_pos++, *y_pos++);
			}
		}

		///	Apply a function to the data.
		///	@param	u	Source data for function.
		///	@param	v	Second source data for function.
		///	@param	w	Third source data for function.
		///	@param	x	Fourth source data for function.
		///	@param	y	Fifth source data for function.
		///	@param	z	Sixth source data for function.
		///	@param	fn	Function to apply.
		template <typename U, typename V, typename W, typename X, typename Y, typename Z, typename FN>
		void apply(const U& u, const V& v, const W& w, const X& x, const Y& y, const Z& z, const FN& fn)
		{
			auto first = begin(), last	= end();
			auto u_pos = std::begin(u);
			auto v_pos = std::begin(v);
			auto w_pos = std::begin(w);
			auto x_pos = std::begin(x);
			auto y_pos = std::begin(y);
			auto z_pos = std::begin(z);
			while (first != last) {
				*first++ = fn(*u_pos++, *v_pos++, *w_pos++, *x_pos++, *y_pos++, *z_pos++);
			}
		}

		///	Add a function to the data.
		///	@param	fn	The function to apply.
		template <typename FN>
		void mutate(const FN& fn)
		{
			auto first	= begin(), last	= end();
			while (first != last) {
				*first++ += fn();
			}
		}

		///	Add a function to the data.
		///	@param	u	Source data for function.
		///	@param	fn	Function to mutate.
		template <typename U, typename FN>
		void mutate(const U& u, const FN& fn)
		{
			auto first = begin(), last	= end();
			auto u_pos = std::begin(u);
			while (first != last) {
				*first++ += fn(*u_pos++);
			}
		}

		///	Add a function to the data.
		///	@param	u	Source data for function.
		///	@param	v	Second source data for function.
		///	@param	fn	Function to mutate.
		template <typename U, typename V, typename FN>
		void mutate(const U& u, const V& v, const FN& fn)
		{
			auto first = begin(), last	= end();
			auto u_pos = std::begin(u);
			auto v_pos = std::begin(v);
			while (first != last) {
				*first++ += fn(*u_pos++, *v_pos++);
			}
		}

		///	Add a function to the data.
		///	@param	u	Source data for function.
		///	@param	v	Second source data for function.
		///	@param	w	Third source data for function.
		///	@param	fn	Function to mutate.
		template <typename U, typename V, typename W, typename FN>
		void mutate(const U& u, const V& v, const W& w, const FN& fn)
		{
			auto first = begin(), last	= end();
			auto u_pos = std::begin(u);
			auto v_pos = std::begin(v);
			auto w_pos = std::begin(w);
			while (first != last) {
				*first++ += fn(*u_pos++, *v_pos++, *w_pos++);
			}
		}

		///	Add a function to the data.
		///	@param	u	Source data for function.
		///	@param	v	Second source data for function.
		///	@param	w	Third source data for function.
		///	@param	x	Fourth source data for function.
		///	@param	fn	Function to mutate.
		template <typename U, typename V, typename W, typename X, typename FN>
		void mutate(const U& u, const V& v, const W& w, const X& x, const FN& fn)
		{
			auto first = begin(), last	= end();
			auto u_pos = std::begin(u);
			auto v_pos = std::begin(v);
			auto w_pos = std::begin(w);
			auto x_pos = std::begin(x);
			while (first != last) {
				*first++ += fn(*u_pos++, *v_pos++, *w_pos++, *x_pos++);
			}
		}

		///	Add a function to the data.
		///	@param	u	Source data for function.
		///	@param	v	Second source data for function.
		///	@param	w	Third source data for function.
		///	@param	x	Fourth source data for function.
		///	@param	y	Fifth source data for function.
		///	@param	fn	Function to mutate.
		template <typename U, typename V, typename W, typename X, typename Y, typename FN>
		void mutate(const U& u, const V& v, const W& w, const X& x, const Y& y, const FN& fn)
		{
			auto first = begin(), last	= end();
			auto u_pos = std::begin(u);
			auto v_pos = std::begin(v);
			auto w_pos = std::begin(w);
			auto x_pos = std::begin(x);
			auto y_pos = std::begin(y);
			while (first != last) {
				*first++ += fn(*u_pos++, *v_pos++, *w_pos++, *x_pos++, *y_pos++);
			}
		}

		///	Add a function to the data.
		///	@param	u	Source data for function.
		///	@param	v	Second source data for function.
		///	@param	w	Third source data for function.
		///	@param	x	Fourth source data for function.
		///	@param	y	Fifth source data for function.
		///	@param	z	Sixth source data for function.
		///	@param	fn	Function to mutate.
		template <typename U, typename V, typename W, typename X, typename Y, typename Z, typename FN>
		void mutate(const U& u, const V& v, const W& w, const X& x, const Y& y, const Z& z, const FN& fn)
		{
			auto first = begin(), last	= end();
			auto u_pos = std::begin(u);
			auto v_pos = std::begin(v);
			auto w_pos = std::begin(w);
			auto x_pos = std::begin(x);
			auto y_pos = std::begin(y);
			auto z_pos = std::begin(z);
			while (first != last) {
				*first++ += fn(*u_pos++, *v_pos++, *w_pos++, *x_pos++, *y_pos++, *z_pos++);
			}
		}

	};
}



#endif
