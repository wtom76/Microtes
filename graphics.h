#pragma once
#include <memory>

namespace mct
{
	//---------------------------------------------------------------------------------------------------------
	/// class graphics
	//---------------------------------------------------------------------------------------------------------
	class graphics
	{
	// types
	public:
		//---------------------------------------------------------------------------------------------------------
		// class paint_session
		//---------------------------------------------------------------------------------------------------------
		class paint_session
		{
		private:
			int				size_x_{0};				// in items
			int				size_y_{0};				// in items
			int				item_size_x_{0};		// size of one item in pixels
			int				item_size_y_{0};		// size of one item in pixels
			HDC				hdc_{nullptr};
			HDC				memdc_{nullptr};
			HBITMAP			hbitmap_{nullptr};
			HGDIOBJ			hbitmap_old_{nullptr};
			Graphics		graphics_;

		public:
			paint_session(HDC hdc, point2d size_in_items, point2d item_size);
			void finish() const;
			~paint_session();

			Graphics& graphics() noexcept { return graphics_; }
		};

	// data
	private:
		ULONG_PTR						gdi_plus_token_{0};
		Gdiplus::GdiplusStartupInput	gdisi_{};
		const point2d					green_ball_size_;
		unique_ptr<ImageAttributes>		green_ball_attr_;
		unique_ptr<Image>				green_ball_;

	// methods
	public:
		graphics();
		~graphics();

		paint_session session(HDC dc) const;
		void draw_green_ball(paint_session& session, point2d point) const;
	};

	//---------------------------------------------------------------------------------------------------------
	graphics& ginst();
}