#include "../../../framework.h"
#include <array>
#include "../../field.h"
#include "a001_microte_impl.h"

//---------------------------------------------------------------------------------------------------------
mct::data::microte_impl::a001::microte_impl::~microte_impl(){}
//---------------------------------------------------------------------------------------------------------
void mct::data::microte_impl::a001::microte_impl::_create_neighb(field& fld) const noexcept
{
	static const std::array<std::pair<ptrdiff_t, ptrdiff_t>, 8> neighb_pos{
		std::pair<ptrdiff_t, ptrdiff_t>{ -1, -1 },
		std::pair<ptrdiff_t, ptrdiff_t>{ -1, 0 },
		std::pair<ptrdiff_t, ptrdiff_t>{ -1, 1 },
		std::pair<ptrdiff_t, ptrdiff_t>{ 0, -1 },
		std::pair<ptrdiff_t, ptrdiff_t>{ 0, 1 },
		std::pair<ptrdiff_t, ptrdiff_t>{ 1, -1 },
		std::pair<ptrdiff_t, ptrdiff_t>{ 1, 0 },
		std::pair<ptrdiff_t, ptrdiff_t>{ 1, 1 }
	};
	int pos_left_to_check{8};
	int create_pos_idx{std::rand() % 8};
	while (pos_left_to_check &&
		!fld.can_add_to_next({point().x() + neighb_pos[create_pos_idx].first, point().y() + neighb_pos[create_pos_idx].second}))
	{
		create_pos_idx = ++create_pos_idx % 8;
		--pos_left_to_check;
	}

	if (pos_left_to_check)
	{
		fld.add_to_next(
			std::make_unique<microte_impl>(point2d{point().x() + neighb_pos[create_pos_idx].first, point().y() + neighb_pos[create_pos_idx].second})
		);
	}
}
//---------------------------------------------------------------------------------------------------------
bool mct::data::microte_impl::a001::microte_impl::cycle(field& fld)
{
	const int neighb_count{fld.neighb_count(point())};

	if (neighb_count > 3 || neighb_count == 0)
	{
		return false;
	}
	if (neighb_count > 0)
	{
		_create_neighb(fld);
	}
	return true;
}
//---------------------------------------------------------------------------------------------------------
void mct::data::microte_impl::a001::microte_impl::draw(HDC) const
{
}
//---------------------------------------------------------------------------------------------------------
void mct::data::microte_impl::a001::microte_impl::populate(field& fld)
{
	for (ptrdiff_t x{0}; x != fld.size().x(); ++x)
	{
		fld.add_to_next(std::make_unique<microte_impl>(point2d{x, 0}));
	}
	fld.finish_populate();
}
