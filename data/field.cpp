#include "../framework.h"
#include "../graphics.h"
#include <cassert>
#include "field.h"
#include "configuration.h"

//---------------------------------------------------------------------------------------------------------
mct::data::field::field()
	: size_{config().field_size()}
	, bounds_{config().field_size().x() - 1, config().field_size().y() - 1}
	, data_on_field_(config().field_size().product(), nullptr)
	, next_data_on_field_(config().field_size().product(), nullptr)
{
	assert(bounds_.x() >= 0);
	assert(bounds_.y() >= 0);
}
//---------------------------------------------------------------------------------------------------------
void mct::data::field::draw(HDC dc)
{
	graphics::paint_session session{ginst().session(dc)};

	{
		std::lock_guard<std::mutex> lock{data_mtx_};
		for (const std::unique_ptr<microte>& mic : data_)
		{
			ginst().draw_green_ball(session, mic->point());
		}
	}

	session.finish();
}
//---------------------------------------------------------------------------------------------------------
void mct::data::field::cycle()
{
	next_data_.clear();
	memset(next_data_on_field_.data(), 0, next_data_on_field_.size() * sizeof(decltype(next_data_on_field_)::value_type));
	{
		const std::lock_guard<std::mutex> lock{data_mtx_};
		for (std::unique_ptr<microte>& mic : data_)
		{
			if (mic->cycle(*this))
			{
				add_to_next(std::move(mic));
			}
		}
		_finish_cycle();
	}
}
//---------------------------------------------------------------------------------------------------------
void mct::data::field::_finish_cycle()
{
	data_.swap(next_data_);
	data_on_field_.swap(next_data_on_field_);
}
//---------------------------------------------------------------------------------------------------------
void mct::data::field::finish_populate()
{
	const std::lock_guard<std::mutex> lock{data_mtx_};
	_finish_cycle();
}
//---------------------------------------------------------------------------------------------------------
mct::data::microte* mct::data::field::at(point2d point) const noexcept
{
	return data_on_field_[size_.idx1d(point)];
}
//---------------------------------------------------------------------------------------------------------
int mct::data::field::neighb_count(point2d point) const noexcept
{
	int neighb_count{0};

	if (point.x() > 0)
	{
		if (point.y() > 0)
		{
			if (at({point.x() - 1, point.y() - 1}))
			{
				++neighb_count;
			}
		}
		if (at({point.x() - 1, point.y()}))
		{
			++neighb_count;
		}
		if (point.y() < bounds_.y())
		{
			if (at({point.x() - 1, point.y() + 1}))
			{
				++neighb_count;
			}
		}
	}
	if (point.y() > 0)
	{
		if (at({point.x(), point.y() - 1}))
		{
			++neighb_count;
		}
	}
	if (point.y() < bounds_.y())
	{
		if (at({point.x(), point.y() + 1}))
		{
			++neighb_count;
		}
	}
	if (point.x() < bounds_.x())
	{
		if (point.y() > 0)
		{
			if (at({point.x() + 1, point.y() - 1}))
			{
				++neighb_count;
			}
		}
		if (at({point.x() + 1, point.y()}))
		{
			++neighb_count;
		}
		if (point.y() < bounds_.y())
		{
			if (at({point.x() + 1, point.y() + 1}))
			{
				++neighb_count;
			}
		}
	}

	return neighb_count;
}
//---------------------------------------------------------------------------------------------------------
bool mct::data::field::can_add_to_next(point2d point) const noexcept
{
	if (point.x() < 0 || point.y() < 0 || point.x() >= size_.x() || point.y() >= size_.y())
	{
		return false;
	}
	const ptrdiff_t idx{size_.idx1d(point)};
	return !data_on_field_[idx] && !next_data_on_field_[idx];
}
//---------------------------------------------------------------------------------------------------------
void mct::data::field::add_to_next(std::unique_ptr<microte>&& mic) noexcept
{
	const ptrdiff_t idx{size_.idx1d(mic->point())};
	assert(!next_data_on_field_[idx]);
	next_data_on_field_[idx] = mic.get();
	next_data_.emplace_back(std::move(mic));
}
