#pragma once

namespace mct
{
	//---------------------------------------------------------------------------------------------------------
	class configuration
	{
		const point2d	green_ball_size_{8, 8};
		wstring			green_ball_img_filename_{L"images/ready.png"};
		point2d			field_size_{64, 64};

	public:
		point2d field_size() const noexcept { return field_size_; };
		point2d green_ball_size() const noexcept { return green_ball_size_; };
		const wchar_t* const green_ball_img_filename() const noexcept { return green_ball_img_filename_.c_str(); }
	};

	//---------------------------------------------------------------------------------------------------------
	configuration& config() noexcept;
}