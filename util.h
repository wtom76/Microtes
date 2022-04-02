#pragma once

namespace mct
{
	//---------------------------------------------------------------------------------------------------------
	// class point2d
	//---------------------------------------------------------------------------------------------------------
	class point2d
	{
		ptrdiff_t x_{0};
		ptrdiff_t y_{0};

	public:
		constexpr point2d(ptrdiff_t x, ptrdiff_t y) noexcept
			: x_{x}
			, y_{y}
		{}
		constexpr ptrdiff_t x() const noexcept { return x_; }
		constexpr ptrdiff_t y() const noexcept { return y_; }
		constexpr ptrdiff_t product() const noexcept { return x_ * y_; }

		// corresponding index in one-dimensional container. *this is assumed to be size
		constexpr ptrdiff_t idx1d(point2d coord) const noexcept { return coord.y() * x_ + coord.x(); }
	};

	//---------------------------------------------------------------------------------------------------------
	// corresponding index in one-dimensional container
	static constexpr ptrdiff_t idx1d(point2d size, point2d coord) noexcept { return size.idx1d(coord); }
	//---------------------------------------------------------------------------------------------------------
	double rand(double rand_min, double rand_max);
}