#pragma once

namespace mct::data
{
	class field;
	//---------------------------------------------------------------------------------------------------------
	// class microte
	//---------------------------------------------------------------------------------------------------------
	class microte
	{
	protected:
		point2d point_;

	public:
		microte(point2d point)
			: point_{point}
		{
		}
		point2d point() const noexcept { return point_; }

		virtual ~microte(){}
		virtual bool cycle(field& fld) = 0;
		virtual void draw(HDC dc) const = 0;
	};
}