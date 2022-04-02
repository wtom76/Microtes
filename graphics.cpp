#include "framework.h"
#include "graphics.h"
#include "configuration.h"

//---------------------------------------------------------------------------------------------------------
mct::graphics& mct::ginst()
{
	static graphics instance;
	return instance;
}

//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
mct::graphics::paint_session::paint_session(HDC hdc, point2d size_in_items, point2d item_size)
	: hdc_{hdc}
	, size_x_{static_cast<int>(size_in_items.x())}
	, size_y_{static_cast<int>(size_in_items.y())}
	, item_size_x_{static_cast<int>(item_size.x())}
	, item_size_y_{static_cast<int>(item_size.y())}
	, memdc_{CreateCompatibleDC(hdc)}
	, hbitmap_{CreateCompatibleBitmap(hdc, size_x_ * item_size_x_, size_y_ * item_size_y_)}
	, hbitmap_old_{SelectObject(memdc_, hbitmap_)}
	, graphics_{memdc_}
{}
//---------------------------------------------------------------------------------------------------------
void mct::graphics::paint_session::finish() const
{
	BitBlt(hdc_, 0, 0, size_x_ * item_size_x_, size_y_ * item_size_y_, memdc_, 0, 0, SRCCOPY);
}
//---------------------------------------------------------------------------------------------------------
mct::graphics::paint_session::~paint_session()
{
	SelectObject(memdc_, hbitmap_old_);
	DeleteDC(memdc_);
	DeleteObject(hbitmap_);
}
//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
mct::graphics::graphics()
	: green_ball_size_{config().green_ball_size()}
{
	Gdiplus::GdiplusStartup(&gdi_plus_token_, &gdisi_, nullptr);
	green_ball_ = make_unique<Image>(config().green_ball_img_filename());
	assert(green_ball_->GetLastStatus() == Status::Ok);
	green_ball_attr_ = make_unique<ImageAttributes>();
	green_ball_attr_->SetColorKey(Color{180, 180, 180}, Color{255, 255, 255}, ColorAdjustTypeBitmap);
}
//---------------------------------------------------------------------------------------------------------
mct::graphics::~graphics()
{
	green_ball_.reset();
	green_ball_attr_.reset();
	Gdiplus::GdiplusShutdown(gdi_plus_token_);
}
//---------------------------------------------------------------------------------------------------------
mct::graphics::paint_session mct::graphics::session(HDC dc) const
{
	return {dc, config().field_size(), green_ball_size_};
}
//---------------------------------------------------------------------------------------------------------
void mct::graphics::draw_green_ball(paint_session& session, point2d point) const
{
	session.graphics().DrawImage(
		green_ball_.get(),
		Rect{
			static_cast<int>(point.x() * green_ball_size_.x()), static_cast<int>(point.y() * green_ball_size_.y()),
			static_cast<int>(green_ball_size_.x()), static_cast<int>(green_ball_size_.y())
		},
		0, 0, static_cast<int>(green_ball_size_.x()), static_cast<int>(green_ball_size_.y()),
		UnitPixel, green_ball_attr_.get());
}
