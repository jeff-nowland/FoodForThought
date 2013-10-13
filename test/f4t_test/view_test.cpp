
#include <check/Check_Console.h>
#include <f4t/View.h>

F4T_CHECK_CONSOLE_SUITE(View, 0)

F4T_CHECK_CONSOLE_TEST(Empty, View, 0)
{
	f4t::View<double> view;
	F4T_CHECK(view.empty());
}

F4T_CHECK_CONSOLE_TEST(Empty_Size0, View, 0)
{
	f4t::View<int>	view;
	F4T_CHECK(view.size() == 0)(view.size());
}

F4T_CHECK_CONSOLE_TEST(Empty_Begin_Eq_End, View, 0)
{
	f4t::View<long> view;
	F4T_CHECK(view.begin() == view.end());
}

F4T_CHECK_CONSOLE_TEST(Empty_RBegin_Eq_REnd, View, 0)
{
	f4t::View<char>	view;
	F4T_CHECK(view.rbegin() == view.rend());
}

F4T_CHECK_CONSOLE_TEST(Data_Sanity, View, 1)
{
	int	arr[2];
	f4t::View<int>	view(&arr[0], &arr[2]);
	F4T_CHECK(view.data() == &arr[0]);
}

F4T_CHECK_CONSOLE_TEST(Not_Empty, View, 1)
{
	int arr[2];
	f4t::View<int>	view(&arr[0], &arr[2]);
	F4T_CHECK(!view.empty());
}

F4T_CHECK_CONSOLE_TEST(Correct_Size, View, 1)
{
	int arr[2];
	f4t::View<int>	view(&arr[0], &arr[2]);
	F4T_CHECK(view.size() == 2)(view.size());
}

F4T_CHECK_CONSOLE_TEST(Not_Empty_Begin_Neq_End, View, 1)
{
	int arr[2];
	f4t::View<int>	view(&arr[0], &arr[2]);
	F4T_CHECK(view.begin() != view.end());
}

F4T_CHECK_CONSOLE_TEST(Not_Empty_RBegin_Neq_REnd, View, 1)
{
	int arr[2];
	f4t::View<int>	view(&arr[0], &arr[2]);
	F4T_CHECK(view.rbegin() != view.rend());
}

F4T_CHECK_CONSOLE_TEST(Begin_Eq_Const_Begin, View, 1)
{
	int arr[2];
	f4t::View<int>			view(&arr[0], &arr[2]);
	f4t::View<int>*			p = &view;
	const f4t::View<int>*	cp = &view;

	F4T_CHECK(p->begin() == cp->begin());
}

F4T_CHECK_CONSOLE_TEST(Begin_Eq_CBegin, View, 1)
{
	int arr[2];
	f4t::View<int>			view(&arr[0], &arr[2]);
	f4t::View<int>*			p = &view;
	const f4t::View<int>*	cp = &view;

	F4T_CHECK(p->begin() == cp->cbegin());
}

F4T_CHECK_CONSOLE_TEST(End_Eq_Const_End, View, 1)
{
	int arr[2];
	f4t::View<int>			view(&arr[0], &arr[2]);
	f4t::View<int>*			p = &view;
	const f4t::View<int>*	cp = &view;

	F4T_CHECK(p->end() == cp->end());
}

F4T_CHECK_CONSOLE_TEST(End_Eq_CEnd, View, 1)
{
	int arr[2];
	f4t::View<int>			view(&arr[0], &arr[2]);
	f4t::View<int>*			p = &view;
	const f4t::View<int>*	cp = &view;

	F4T_CHECK(p->end() == cp->cend());
}

F4T_CHECK_CONSOLE_TEST(RBegin_Eq_Const_RBegin, View, 1)
{
	int arr[2];
	f4t::View<int>			view(&arr[0], &arr[2]);
	f4t::View<int>*			p = &view;
	const f4t::View<int>*	cp = &view;

	F4T_CHECK(p->rbegin() == cp->rbegin());
}

F4T_CHECK_CONSOLE_TEST(RBegin_Eq_CRBegin, View, 1)
{
	int arr[2];
	f4t::View<int>			view(&arr[0], &arr[2]);
	f4t::View<int>*			p = &view;
	const f4t::View<int>*	cp = &view;

	F4T_CHECK(p->rbegin() == cp->crbegin());
}

F4T_CHECK_CONSOLE_TEST(REnd_Eq_Const_REnd, View, 1)
{
	int arr[2];
	f4t::View<int>			view(&arr[0], &arr[2]);
	f4t::View<int>*			p = &view;
	const f4t::View<int>*	cp = &view;

	F4T_CHECK(p->rend() == cp->rend());
}

F4T_CHECK_CONSOLE_TEST(REnd_Eq_CREnd, View, 1)
{
	int arr[2];
	f4t::View<int>			view(&arr[0], &arr[2]);
	f4t::View<int>*			p = &view;
	const f4t::View<int>*	cp = &view;

	F4T_CHECK(p->rend() == cp->crend());
}

F4T_CHECK_CONSOLE_TEST(Iterator_Distance, View, 2)
{
	int arr[2];
	f4t::View<int>			view(&arr[0], &arr[2]);

	F4T_CHECK(std::distance(view.begin(), view.end()) == view.size());
}

F4T_CHECK_CONSOLE_TEST(Reverse_Iterator_Distance, View, 2)
{
	int arr[2];
	f4t::View<int>			view(&arr[0], &arr[2]);

	F4T_CHECK(std::distance(view.rbegin(), view.rend()) == view.size());
}

F4T_CHECK_CONSOLE_TEST(Self_Equivalence, View, 3)
{
	int arr[2];
	f4t::View<int>			view(&arr[0], &arr[2]);

	F4T_CHECK(view == view);
}

F4T_CHECK_CONSOLE_TEST(Self_Not_Less, View, 3)
{
	int arr[2];
	f4t::View<int>			view(&arr[0], &arr[2]);

	F4T_CHECK(!(view < view));
}

F4T_CHECK_CONSOLE_TEST(Different_Not_Equivalent, View, 3)
{
	int arr[2], arr2[2];
	f4t::View<int>	view(&arr[0], &arr[2]),
					v2(&arr2[0], &arr2[2]);

	F4T_CHECK(!(view == v2));
}

F4T_CHECK_CONSOLE_TEST(Copy_Ctor, View, 2)
{
	int arr[2];
	f4t::View<int>	view(&arr[0], &arr[2]);
	f4t::View<int>	v2(view);

	F4T_CHECK(view == v2);
}

F4T_CHECK_CONSOLE_TEST(Assignment, View, 3)
{
	int arr[2], arr2[2];
	f4t::View<int>	view(&arr[0], &arr[2]),
					v2(&arr2[0], &arr2[2]);

	v2 = view;
	F4T_CHECK(view == v2);
}

F4T_CHECK_CONSOLE_TEST(Sized_Ctor, View, 2)
{
	int arr[2];
	f4t::View<int>	view(&arr[0], &arr[2]);
	f4t::View<int>	v2(&arr[0], 2);

	F4T_CHECK(view == v2);
}

F4T_CHECK_CONSOLE_TEST(Swap, View, 3)
{
	int arr[2], arr2[2];

	f4t::View<int>	view(&arr[0], &arr[2]);
	f4t::View<int>	v2(&arr2[0], 2);

	v2.swap(view);
	F4T_CHECK(view.begin() == &arr2[0]);
	F4T_CHECK(v2.begin() == &arr[0]);
}

F4T_CHECK_CONSOLE_TEST(Assign_Ptr, View, 3)
{
	int arr[2], arr2[2];

	f4t::View<int>	view(&arr[0], &arr[2]);

	view.assign(&arr2[0], &arr2[2]);
	F4T_CHECK(view.begin() == &arr2[0]);
	F4T_CHECK(view.end() == &arr2[2]);
}

F4T_CHECK_CONSOLE_TEST(Assign_N_Ptr, View, 3)
{
	int arr[2], arr2[2];

	f4t::View<int>	view(&arr[0], &arr[2]);

	view.assign_n(&arr2[0], 2);
	F4T_CHECK(view.begin() == &arr2[0]);
	F4T_CHECK(view.end() == &arr2[2]);
}

F4T_CHECK_CONSOLE_TEST(Copy_To_Ptr, View, 4)
{
	int arr[2] = { 1, 2 },
		arr2[3] = { 3, 4 };

	f4t::View<int>	view(&arr[0], 2);

	view.copy_to(&arr2[0]);
	F4T_CHECK(arr2[0] == 1)(arr2[0]);
	F4T_CHECK(arr2[1] == 2)(arr2[1]);
}

F4T_CHECK_CONSOLE_TEST(Copy_From_Ptr, View, 4)
{
	int arr[2] = { 1, 2 },
		arr2[3] = { 3, 4 };

	f4t::View<int>	view(&arr[0], 2);

	view.copy(&arr2[0]);
	F4T_CHECK(arr[0] == 3)(arr[0]);
	F4T_CHECK(arr[1] == 4)(arr[1]);
}

F4T_CHECK_CONSOLE_TEST(Copy_To_View, View, 4)
{
	int arr[2] = { 1, 2 },
		arr2[3] = { 3, 4 };

	f4t::View<int>	view(&arr[0], 2),
					v2(&arr2[0], 2);

	view.copy_to(v2);
	F4T_CHECK(arr2[0] == 1)(arr2[0]);
	F4T_CHECK(arr2[1] == 2)(arr2[1]);
}

F4T_CHECK_CONSOLE_TEST(Copy_From_View, View, 4)
{
	int arr[2] = { 1, 2 },
		arr2[3] = { 3, 4 };

	f4t::View<int>	view(&arr[0], 2),
					v2(&arr2[0], 2);

	view.copy(v2);
	F4T_CHECK(arr[0] == 3)(arr[0]);
	F4T_CHECK(arr[1] == 4)(arr[1]);
}

F4T_CHECK_CONSOLE_TEST(Fill, View, 4)
{
	int arr[3] = { 1, 2, 3 };

	f4t::View<int>	view(&arr[0], 2);

	view.fill(42);
	F4T_CHECK(arr[0] == 42)(arr[0]);
	F4T_CHECK(arr[1] == 42)(arr[1]);
	F4T_CHECK(arr[2] == 3)(arr[2]);
}

F4T_CHECK_CONSOLE_TEST(Apply_0, View, 5)
{
	int arr[3] = { 1, 2, 3 };

	f4t::View<int>	view(&arr[0], 2);

	view.apply([]() { return 4; });
	F4T_CHECK(arr[0] == 4)(arr[0]);
	F4T_CHECK(arr[1] == 4)(arr[1]);
	F4T_CHECK(arr[2] == 3)(arr[2]);
}

F4T_CHECK_CONSOLE_TEST(Apply_1, View, 5)
{
	int arr[3] = { 1, 2, 3 },
		arr2[3] = { 4, 5, 6 };

	f4t::View<int>	view(&arr[0], 2),
					u(&arr2[0], 2);

	view.apply(u, [](int u) { return u * u; });
	F4T_CHECK(arr[0] == 16)(arr[0]);
	F4T_CHECK(arr[1] == 25)(arr[1]);
	F4T_CHECK(arr[2] == 3)(arr[2]);
}

F4T_CHECK_CONSOLE_TEST(Apply_2, View, 5)
{
	int arr[3] = { 1, 2, 3 },
		arr2[3] = { 4, 5, 6 },
		arr3[3] = { 5, 6, 7 };

	f4t::View<int>	view(&arr[0], 2),
					u(&arr2[0], 2),
					v(&arr3[0], 2);

	view.apply(u, v, [](int u, int v) { return u * v; });
	F4T_CHECK(arr[0] == 20)(arr[0]);
	F4T_CHECK(arr[1] == 30)(arr[1]);
	F4T_CHECK(arr[2] == 3)(arr[2]);
}

F4T_CHECK_CONSOLE_TEST(Apply_3, View, 5)
{
	int arr[3] = { 1, 2, 3 },
		arr2[] = { 4, 5 },
		arr3[] = { 5, 6 },
		arr4[] = { 6, 7 };

	f4t::View<int>	view(&arr[0], 2),
					u(&arr2[0], 2),
					v(&arr3[0], 2),
					w(&arr4[0], 2);

	view.apply(u, v, w, [](int u, int v, int w) {
		return u * v + w;
	});
	F4T_CHECK(arr[0] == 26)(arr[0]);
	F4T_CHECK(arr[1] == 37)(arr[1]);
	F4T_CHECK(arr[2] == 3)(arr[2]);
}

F4T_CHECK_CONSOLE_TEST(Apply_4, View, 5)
{
	int arr[3] = { 1, 2, 3 },
		arr2[] = { 4, 5 },
		arr3[] = { 5, 6 },
		arr4[] = { 6, 7 },
		arr5[] = { 7, 8 };

	f4t::View<int>	view(&arr[0], 2),
					u(&arr2[0], 2),
					v(&arr3[0], 2),
					w(&arr4[0], 2),
					x(&arr5[0], 2);

	view.apply(u, v, w, x, [](int u, int v, int w, int x) {
		return u * v + w * x;
	});
	F4T_CHECK(arr[0] == 62)(arr[0]);
	F4T_CHECK(arr[1] == 86)(arr[1]);
	F4T_CHECK(arr[2] == 3)(arr[2]);
}

F4T_CHECK_CONSOLE_TEST(Apply_5, View, 5)
{
	int arr[3] = { 1, 2, 3 },
		arr2[] = { 4, 5 },
		arr3[] = { 5, 6 },
		arr4[] = { 6, 7 },
		arr5[] = { 7, 8 },
		arr6[] = { 9, 10 };

	f4t::View<int>	view(&arr[0], 2),
					u(&arr2[0], 2),
					v(&arr3[0], 2),
					w(&arr4[0], 2),
					x(&arr5[0], 2),
					y(&arr6[0], 2);

	view.apply(u, v, w, x, y, [](int u, int v, int w, int x, int y) {
		return u * v + w * x + y;
	});

	F4T_CHECK(arr[0] == 71)(arr[0]);
	F4T_CHECK(arr[1] == 96)(arr[1]);
	F4T_CHECK(arr[2] == 3)(arr[2]);
}

F4T_CHECK_CONSOLE_TEST(Apply_6, View, 5)
{
	int arr[3] = { 1, 2, 3 },
		arr2[] = { 4, 5 },
		arr3[] = { 5, 6 },
		arr4[] = { 6, 7 },
		arr5[] = { 7, 8 },
		arr6[] = { 9, 10 },
		arr7[] = { 11, 12 };

	f4t::View<int>	view(&arr[0], 2),
					u(&arr2[0], 2),
					v(&arr3[0], 2),
					w(&arr4[0], 2),
					x(&arr5[0], 2),
					y(&arr6[0], 2),
					z(&arr7[0], 2);

	view.apply(u, v, w, x, y, z,
			   [](int u, int v, int w, int x, int y, int z) {
		return u * v + w * x + y * z;
	});

	F4T_CHECK(arr[0] == 161)(arr[0]);
	F4T_CHECK(arr[1] == 206)(arr[1]);
	F4T_CHECK(arr[2] == 3)(arr[2]);
}

F4T_CHECK_CONSOLE_TEST(Mutate_0, View, 5)
{
	int arr[3] = { 1, 2, 3 };

	f4t::View<int>	view(&arr[0], 2);

	view.mutate([]() { return 4; });
	F4T_CHECK(arr[0] == 5)(arr[0]);
	F4T_CHECK(arr[1] == 6)(arr[1]);
	F4T_CHECK(arr[2] == 3)(arr[2]);
}

F4T_CHECK_CONSOLE_TEST(Mutate_1, View, 5)
{
	int arr[3] = { 1, 2, 3 },
		arr2[3] = { 4, 5, 6 };

	f4t::View<int>	view(&arr[0], 2),
					u(&arr2[0], 2);

	view.mutate(u, [](int u) { return u * u; });
	F4T_CHECK(arr[0] == 17)(arr[0]);
	F4T_CHECK(arr[1] == 27)(arr[1]);
	F4T_CHECK(arr[2] == 3)(arr[2]);
}

F4T_CHECK_CONSOLE_TEST(Mutate_2, View, 5)
{
	int arr[3] = { 1, 2, 3 },
		arr2[3] = { 4, 5, 6 },
		arr3[3] = { 5, 6, 7 };

	f4t::View<int>	view(&arr[0], 2),
					u(&arr2[0], 2),
					v(&arr3[0], 2);

	view.mutate(u, v, [](int u, int v) { return u * v; });
	F4T_CHECK(arr[0] == 21)(arr[0]);
	F4T_CHECK(arr[1] == 32)(arr[1]);
	F4T_CHECK(arr[2] == 3)(arr[2]);
}

F4T_CHECK_CONSOLE_TEST(Mutate_3, View, 5)
{
	int arr[3] = { 1, 2, 3 },
		arr2[] = { 4, 5 },
		arr3[] = { 5, 6 },
		arr4[] = { 6, 7 };

	f4t::View<int>	view(&arr[0], 2),
					u(&arr2[0], 2),
					v(&arr3[0], 2),
					w(&arr4[0], 2);

	view.mutate(u, v, w, [](int u, int v, int w) {
		return u * v + w;
	});
	F4T_CHECK(arr[0] == 27)(arr[0]);
	F4T_CHECK(arr[1] == 39)(arr[1]);
	F4T_CHECK(arr[2] == 3)(arr[2]);
}

F4T_CHECK_CONSOLE_TEST(Mutate_4, View, 5)
{
	int arr[3] = { 1, 2, 3 },
		arr2[] = { 4, 5 },
		arr3[] = { 5, 6 },
		arr4[] = { 6, 7 },
		arr5[] = { 7, 8 };

	f4t::View<int>	view(&arr[0], 2),
					u(&arr2[0], 2),
					v(&arr3[0], 2),
					w(&arr4[0], 2),
					x(&arr5[0], 2);

	view.mutate(u, v, w, x, [](int u, int v, int w, int x) {
		return u * v + w * x;
	});
	F4T_CHECK(arr[0] == 63)(arr[0]);
	F4T_CHECK(arr[1] == 88)(arr[1]);
	F4T_CHECK(arr[2] == 3)(arr[2]);
}

F4T_CHECK_CONSOLE_TEST(Mutate_5, View, 5)
{
	int arr[3] = { 1, 2, 3 },
		arr2[] = { 4, 5 },
		arr3[] = { 5, 6 },
		arr4[] = { 6, 7 },
		arr5[] = { 7, 8 },
		arr6[] = { 9, 10 };

	f4t::View<int>	view(&arr[0], 2),
					u(&arr2[0], 2),
					v(&arr3[0], 2),
					w(&arr4[0], 2),
					x(&arr5[0], 2),
					y(&arr6[0], 2);

	view.mutate(u, v, w, x, y, [](int u, int v, int w, int x, int y) {
		return u * v + w * x + y;
	});

	F4T_CHECK(arr[0] == 72)(arr[0]);
	F4T_CHECK(arr[1] == 98)(arr[1]);
	F4T_CHECK(arr[2] == 3)(arr[2]);
}

F4T_CHECK_CONSOLE_TEST(Mutate_6, View, 5)
{
	int arr[3] = { 1, 2, 3 },
		arr2[] = { 4, 5 },
		arr3[] = { 5, 6 },
		arr4[] = { 6, 7 },
		arr5[] = { 7, 8 },
		arr6[] = { 9, 10 },
		arr7[] = { 11, 12 };

	f4t::View<int>	view(&arr[0], 2),
					u(&arr2[0], 2),
					v(&arr3[0], 2),
					w(&arr4[0], 2),
					x(&arr5[0], 2),
					y(&arr6[0], 2),
					z(&arr7[0], 2);

	view.mutate(u, v, w, x, y, z,
			   [](int u, int v, int w, int x, int y, int z) {
		return u * v + w * x + y * z;
	});

	F4T_CHECK(arr[0] == 162)(arr[0]);
	F4T_CHECK(arr[1] == 208)(arr[1]);
	F4T_CHECK(arr[2] == 3)(arr[2]);
}

F4T_CHECK_CONSOLE_TEST(Slice_1, View, 3)
{
	int arr[] = { 1, 2, 3, 4, 5, 6 };
	f4t::View<int>	view(&arr[0], 6),
					u = view.slice();
	F4T_CHECK(u.begin() == &arr[0]);
	F4T_CHECK(u.end() == &arr[6]);
}

F4T_CHECK_CONSOLE_TEST(Slice_2, View, 3)
{
	int arr[] = { 1, 2, 3, 4, 5, 6 };
	f4t::View<int>	view(&arr[0], 6),
					u = view.slice(2);
	F4T_CHECK(u.begin() == &arr[2]);
	F4T_CHECK(u.end() == &arr[6]);
}

F4T_CHECK_CONSOLE_TEST(Slice_3, View, 3)
{
	int arr[] = { 1, 2, 3, 4, 5, 6 };
	f4t::View<int>	view(&arr[0], 6),
					u = view.slice(2, 4);
	F4T_CHECK(u.begin() == &arr[2]);
	F4T_CHECK(u.end() == &arr[4]);
}

F4T_CHECK_CONSOLE_TEST(Slice_4, View, 3)
{
	int arr[] = { 1, 2, 3, 4, 5, 6 };
	f4t::View<int>	view(&arr[0], 6),
					u = view.slice(2, -2);
	F4T_CHECK(u.begin() == &arr[2]);
	F4T_CHECK(u.end() == &arr[4]);
}

F4T_CHECK_CONSOLE_TEST(Slice_5, View, 3)
{
	int arr[] = { 1, 2, 3, 4, 5, 6 };
	f4t::View<int>	view(&arr[0], 6),
					u = view.slice(-4);
	F4T_CHECK(u.begin() == &arr[2]);
	F4T_CHECK(u.end() == &arr[6]);
}

