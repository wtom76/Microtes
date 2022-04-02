#pragma once
#include <vector>
#include <list>
#include <mutex>
#include <memory>
#include "microte.h"

namespace mct::data
{
	//---------------------------------------------------------------------------------------------------------
	// class field
	//---------------------------------------------------------------------------------------------------------
	class field
	{
	// types
	// data
	private:
		const point2d						size_;
		const point2d						bounds_;
		std::mutex							data_mtx_;
		std::list<std::unique_ptr<microte>>	data_;
		std::list<std::unique_ptr<microte>>	next_data_;
		std::vector<microte*>				data_on_field_;
		std::vector<microte*>				next_data_on_field_;
	// methods
	private:
		void _finish_cycle();
	public:
		field();
		void draw(HDC dc);
		void cycle();
		void finish_populate();

		point2d size() const noexcept { return size_; }
		point2d bounds() const noexcept { return bounds_; }
		microte* at(point2d point) const noexcept;
		int neighb_count(point2d point) const noexcept;
		bool can_add_to_next(point2d point) const noexcept;
		void add_to_next(std::unique_ptr<microte>&& mic) noexcept;
	};
}
