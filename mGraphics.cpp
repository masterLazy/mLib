/*****************************************************************************
* mGraphics.cpp
* 绘图
*
* 基于Direct2D
*****************************************************************************/

#include "mGraphics.h"

using namespace mlib;

D2D1_COLOR_F Graphics::color(COLORREF color, float alpha)
{
	float r = (float)GetRValue(color) / 255;
	float g = (float)GetGValue(color) / 255;
	float b = (float)GetBValue(color) / 255;

	return D2D1::ColorF(r, g, b, alpha);
}
D2D1_COLOR_F Graphics::color(int _r, int _g, int _b, float alpha)
{
	float r = (float)_r / 255;
	float g = (float)_g / 255;
	float b = (float)_b / 255;

	return D2D1::ColorF(r, g, b, alpha);
}

/*****************************************************************************
* Brush_t
* 画笔
*****************************************************************************/

Brush_t* Graphics::brush(int r, int g, int b, float alpha)
{
	ID2D1SolidColorBrush* temp;
	pRenderTarget->CreateSolidColorBrush(color(r, g, b, alpha), &temp);
	return temp;
}
Brush_t* Graphics::brush(COLORREF _color, float alpha)
{
	ID2D1SolidColorBrush* temp;
	pRenderTarget->CreateSolidColorBrush(color(_color, alpha), &temp);
	return temp;
}
Brush_t* Graphics::brush_f(float r, float g, float b, float alpha)
{
	ID2D1SolidColorBrush* temp;
	pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(r, g, b, alpha), &temp);
	return temp;
}

/*****************************************************************************
* Geometry
* 几何图形
*****************************************************************************/

Geometry::Geometry()
{
	pPathGeometry = nullptr;
	pSink = nullptr;
}
Geometry::~Geometry()
{
	if (pPathGeometry != nullptr)pPathGeometry->Release();
	if (pSink != nullptr)pSink->Release();
}

bool Geometry::init(Graphics& gfx)
{
	HRESULT res;
	//创建路径几何图形
	res = gfx.pFactory->CreatePathGeometry(&pPathGeometry);
	if (!SUCCEEDED(res))return false;
	return true;
}

void Geometry::draw(Graphics& gfx, Brush_t* brush, float lineWidth)
{
	gfx.pRenderTarget->DrawGeometry(pPathGeometry, brush, lineWidth);
}
void Geometry::fill(Graphics& gfx, Brush_t* brush)
{
	gfx.pRenderTarget->FillGeometry(pPathGeometry, brush);
}

bool Geometry::begin_def()
{
	HRESULT res;
	//绑定几何图形接收器
	res = pPathGeometry->Open(&pSink);
	if (!SUCCEEDED(res))return false;

	pSink->SetFillMode(D2D1_FILL_MODE::D2D1_FILL_MODE_WINDING);
	return true;
}
void Geometry::end_def()
{
	pSink->Close();
}

void Geometry::begin(float x, float y, bool filled)
{
	//开始一个新图形
	pSink->BeginFigure(D2D1::Point2F(x, y), filled ?
		D2D1_FIGURE_BEGIN::D2D1_FIGURE_BEGIN_FILLED : D2D1_FIGURE_BEGIN::D2D1_FIGURE_BEGIN_HOLLOW);
}
void Geometry::end(bool closed)
{
	pSink->EndFigure(closed ?
		D2D1_FIGURE_END::D2D1_FIGURE_END_CLOSED : D2D1_FIGURE_END::D2D1_FIGURE_END_OPEN);
}

void Geometry::add_line(float x, float y)
{
	pSink->AddLine(D2D1::Point2F(x, y));
}
void Geometry::add_bezier(float x1, float y1, float x, float y)
{
	pSink->AddQuadraticBezier(D2D1::QuadraticBezierSegment(
		D2D1::Point2F(x1, y1),
		D2D1::Point2F(x, y)
	));
}
void Geometry::add_bezier(float x1, float y1, float x2, float y2, float x, float y)
{
	pSink->AddBezier(D2D1::BezierSegment(
		D2D1::Point2F(x1, y1),
		D2D1::Point2F(x2, y2),
		D2D1::Point2F(x, y)
	));
}
void Geometry::add_arc(float x, float y, float r, bool neg, bool large)
{
	pSink->AddArc(D2D1::ArcSegment(
		D2D1::Point2F(x, y),
		D2D1::SizeF(r, r),
		0,
		neg ? D2D1_SWEEP_DIRECTION::D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE : D2D1_SWEEP_DIRECTION::D2D1_SWEEP_DIRECTION_CLOCKWISE,
		large ? D2D1_ARC_SIZE::D2D1_ARC_SIZE_LARGE : D2D1_ARC_SIZE::D2D1_ARC_SIZE_SMALL
	));
}

/*****************************************************************************
* 类主体
*****************************************************************************/

Graphics::Graphics()
{
	pFactory = nullptr;
	pRenderTarget = nullptr;
	pWriteFactory = nullptr;
	hWnd = NULL;
	mode = 0;
	trans = D2D1::Matrix3x2F::Translation(0, 0);
}
Graphics::~Graphics()
{
	if (pFactory != nullptr)pFactory->Release();
	if (pRenderTarget != nullptr)pRenderTarget->Release();
	if (pRenderTarget != nullptr)pWriteFactory->Release();
}

bool Graphics::init(HWND _hWnd)
{
	if (pFactory != nullptr)pFactory->Release();
	if (pRenderTarget != nullptr)pRenderTarget->Release();
	if (pRenderTarget != nullptr)pWriteFactory->Release();

	mode = 1;
	hWnd = _hWnd;

	HRESULT res;

	//创建ID2D1Factory
	res = D2D1CreateFactory(D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_SINGLE_THREADED,//设置为单线程
		&pFactory);
	if (!SUCCEEDED(res))return false;

	//创建ID2D1HwndRenderTarget
	RECT rect;
	GetClientRect(hWnd, &rect);

	res = pFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rect.right, rect.bottom)),
		(ID2D1HwndRenderTarget**)&pRenderTarget
	);
	if (!SUCCEEDED(res))return false;

	//创建ID2D1WriteFactory
	res = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		(IUnknown**)&pWriteFactory
	);
	if (!SUCCEEDED(res))return false;

	return true;
}
bool Graphics::init(const Image& _img)
{
	if (pFactory != nullptr)pFactory->Release();
	if (pRenderTarget != nullptr)pRenderTarget->Release();
	if (pRenderTarget != nullptr)pWriteFactory->Release();

	mode = 2;
	img = _img;

	HRESULT res;

	//创建ID2D1Factory
	res = D2D1CreateFactory(D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_SINGLE_THREADED,//设置为单线程
		&pFactory);
	if (!SUCCEEDED(res))return false;

	//创建ID2D1HwndRenderTarget
	res = pFactory->CreateWicBitmapRenderTarget(
		img.pBitmap,
		D2D1::RenderTargetProperties(),
		&pRenderTarget
	);
	if (!SUCCEEDED(res))return false;

	//创建ID2D1WriteFactory
	res = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		(IUnknown**)&pWriteFactory
	);
	if (!SUCCEEDED(res))return false;

	return true;
}

bool Graphics::begin_draw()
{
	if (mode == 0)return false;
	pRenderTarget->BeginDraw();
	return true;
}
bool Graphics::end_draw()
{
	return SUCCEEDED(pRenderTarget->EndDraw());
}

bool Graphics::resize(UINT32 width, UINT32 height)
{
	if (pFactory == nullptr)return false;
	if (pRenderTarget != nullptr)pRenderTarget->Release();

	RECT rect;
	if (width == AUTO || height == AUTO)
	{
		if (mode == 1)
		{
			GetClientRect(hWnd, &rect);
		}
		else if (mode == 2)
		{
			rect.right = img.get_width();
			rect.bottom = img.get_height();
		}
	}
	else
	{
		rect.right = width;
		rect.bottom = height;
	}

	width = rect.right;
	height = rect.bottom;
	HRESULT res = pFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(width, height)),
		(ID2D1HwndRenderTarget**)&pRenderTarget
	);
	if (!SUCCEEDED(res))return false;

	return true;
}

void Graphics::clear(COLORREF _color)
{
	pRenderTarget->Clear(color(_color));
}

void Graphics::trans_rotate(float x, float y, float angle)
{
	trans = trans * D2D1::Matrix3x2F::Rotation(
		angle,
		D2D1::Point2F(x, y));
	pRenderTarget->SetTransform(trans);
}
void Graphics::trans_scale(float x, float y, float rateX, float rateY)
{
	if (rateY == AUTO)rateY = rateX;

	trans = trans * D2D1::Matrix3x2F::Scale(
		D2D1::Size(rateX, rateY),
		D2D1::Point2F(x, y));
	pRenderTarget->SetTransform(trans);
}
void Graphics::trans_shift(float x, float y)
{
	trans = trans * D2D1::Matrix3x2F::Translation(x, y);
	pRenderTarget->SetTransform(trans);
}
void Graphics::trans_clear()
{
	trans = D2D1::Matrix3x2F::Translation(0, 0);
	pRenderTarget->SetTransform(trans);
}

bool Graphics::draw_text(float x, float y, const wchar_t str[], Brush_t* brush, Font font)
{
	IDWriteTextFormat* textFormat;
	HRESULT res = pWriteFactory->CreateTextFormat(
		font.szFontName,
		NULL,
		(DWRITE_FONT_WEIGHT)font.weight,
		(DWRITE_FONT_STYLE)font.style,
		DWRITE_FONT_STRETCH_NORMAL,
		font.size,
		L"zh-cn",
		&textFormat
	);
	if (!SUCCEEDED(res))return false;

	D2D1_SIZE_F renderTargetSize = pRenderTarget->GetSize();
	pRenderTarget->DrawText(str, wcslen(str), textFormat, D2D1::RectF(x, y, 1e10, 1e10), brush);

	return true;
}
bool Graphics::draw_text_c(float midX, float y, const wchar_t str[], Brush_t* brush, Font font)
{
	float width = 0;
	for (int i = 0; i < wcslen(str); i++)
	{
		if (str[i] < 255)
		{
			width += 0.5;
		}
		else
		{
			width += 1;
		}
	}
	return draw_text(midX - width * font.size / 2, y, str, brush, font);
}
bool Graphics::draw_text(float left, float top, float right, float bottom, const wchar_t str[], Brush_t* brush, Font font)
{
	IDWriteTextFormat* textFormat;
	HRESULT res = pWriteFactory->CreateTextFormat(
		font.szFontName,
		NULL,
		(DWRITE_FONT_WEIGHT)font.weight,
		(DWRITE_FONT_STYLE)font.style,
		DWRITE_FONT_STRETCH_NORMAL,
		font.size,
		L"zh-cn",
		&textFormat
	);
	if (!SUCCEEDED(res))return false;

	D2D1_SIZE_F renderTargetSize = pRenderTarget->GetSize();
	pRenderTarget->DrawText(str, wcslen(str), textFormat, D2D1::RectF(left, top, right, bottom), brush);

	return true;
}

void Graphics::draw_line(float x1, float y1, float x2, float y2, Brush_t* brush, float lineWidth)
{
	pRenderTarget->DrawLine(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), brush, lineWidth);
}

void Graphics::draw_rectangle(float left, float top, float right, float bottom, Brush_t* brush, float lineWidth)
{
	pRenderTarget->DrawRectangle(D2D1::Rect(left, top, right, bottom), brush, lineWidth);
}
void Graphics::fill_rectangle(float left, float top, float right, float bottom, Brush_t* brush)
{
	pRenderTarget->FillRectangle(D2D1::Rect(left, top, right, bottom), brush);
}

void Graphics::draw_r_rectangle(float left, float top, float right, float bottom, float r, Brush_t* brush, float lineWidth)
{
	pRenderTarget->DrawRoundedRectangle(D2D1::RoundedRect(D2D1::Rect(left, top, right, bottom), r, r), brush, lineWidth);
}
void Graphics::fill_r_rectangle(float left, float top, float right, float bottom, float r, Brush_t* brush)
{
	pRenderTarget->FillRoundedRectangle(D2D1::RoundedRect(D2D1::Rect(left, top, right, bottom), r, r), brush);
}

void Graphics::draw_ellipse(float x, float y, float width, float height, Brush_t* brush, float lineWidth)
{
	pRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), width, height), brush, lineWidth);
}
void Graphics::fill_ellipse(float x, float y, float width, float height, Brush_t* brush)
{
	pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), width, height), brush);
}

void Graphics::draw_circle(float x, float y, float r, Brush_t* brush, float lineWidth)
{
	return draw_ellipse(x, y, r, r, brush, lineWidth);
}
void Graphics::fill_circle(float x, float y, float r, Brush_t* brush)
{
	return fill_ellipse(x, y, r, r, brush);
}

void Graphics::draw_polygon(std::vector<Point> pts, Brush_t* brush, float lineWidth)
{
	if (pts.size() < 2)return;
	Geometry gmt;
	gmt.init(*this);
	gmt.begin_def();
	gmt.begin(pts[0].x, pts[0].y);
	for (size_t i = 1; i < pts.size(); i++)
	{
		gmt.add_line(pts[i].x, pts[i].y);
	}
	gmt.end();
	gmt.end_def();
	gmt.draw(*this, brush, lineWidth);
}
void Graphics::fill_polygon(std::vector<Point> pts, Brush_t* brush)
{
	if (pts.size() < 2)return;
	Geometry gmt;
	gmt.init(*this);
	gmt.begin_def();
	gmt.begin(pts[0].x, pts[0].y);
	for (size_t i = 1; i < pts.size(); i++)
	{
		gmt.add_line(pts[i].x, pts[i].y);
	}
	gmt.end();
	gmt.end_def();
	gmt.fill(*this, brush);
}

bool Graphics::draw_image(const Image& img, float x, float y, float width, float height, float angle, float alpha, bool interpolation)
{
	if (img.pBitmap == nullptr)return false;

	if (width == AUTO || height == AUTO)
	{
		width = img.get_width();
		height = img.get_height();
	}

	D2D1_POINT_2F pt = trans.TransformPoint(D2D1::Point2F(x + width / 2, y + height / 2));
	trans_rotate(pt.x, pt.y, angle);

	ID2D1Bitmap* pBitmap;
	HRESULT res = pRenderTarget->CreateBitmapFromWicBitmap(
		img.pBitmap,
		NULL,
		&pBitmap
	);
	if (!SUCCEEDED(res))return false;

	pRenderTarget->DrawBitmap(pBitmap, D2D1::RectF(x, y, x + width, y + height), alpha,
		(D2D1_BITMAP_INTERPOLATION_MODE)interpolation);

	pBitmap->Release();
	trans_rotate(pt.x, pt.y, -angle);
	return true;
}
bool Graphics::draw_image_s(const Image& img, float x, float y, float scale, float angle, float alpha, bool interpolation)
{
	return draw_image(img, x, y, scale * img.get_width(), scale * img.get_height(), angle, alpha, interpolation);
}

bool Graphics::proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, bool no_erase)
{
	//return true; 表示需要退出
	m_whl = 0;
	switch (message)
	{
	case WM_CREATE:
		init(hWnd);
		return false;

	case WM_SIZE:
		resize();
		return false;

	case WM_ERASEBKGND:
		if (no_erase)return true;

	case WM_MOUSEMOVE:
		m_x = GET_X_LPARAM(lParam);
		m_y = GET_Y_LPARAM(lParam);
		if (m_down)
		{
			m_dx = m_x - m_x0;
			m_dy = m_y - m_y0;
		}
		return false;

	case WM_LBUTTONDOWN:
		m_down = true;
		m_x0 = m_x;
		m_y0 = m_y;
		return false;

	case WM_LBUTTONUP:
		m_down = false;
		return false;

	case WM_MOUSEWHEEL:
		m_whl = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
		return false;

	default: return false;
	}
}