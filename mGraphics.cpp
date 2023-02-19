/******************************************************
* mGraphics.cpp
* 绘图
* 
* 基于Direct2D
******************************************************/

#include "mGraphics.h"

/******************************************************
* 画笔
*
******************************************************/

Brush_t* Graphics::brush(int r, int g, int b, float alpha)
{
	ID2D1SolidColorBrush* temp;
	pRenderTarget->CreateSolidColorBrush(Color(r, g, b, alpha), &temp);
	return temp;
}
Brush_t* Graphics::brush(float r, float g, float b, float alpha)
{
	ID2D1SolidColorBrush* temp;
	pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(r, g, b, alpha), &temp);
	return temp;
}

/******************************************************
* Geometry
* 几何图形
*
******************************************************/

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
	if (res != S_OK)return false;
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
	if (res != S_OK)return false;

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

/******************************************************
* Bitmap
* 位图
*
******************************************************/

Bitmap::Bitmap()
{
	pBitmap = nullptr;
}
Bitmap::~Bitmap()
{
	if (pBitmap != nullptr)pBitmap->Release();
}

bool Bitmap::init(Graphics& gfx, const COLORREF arr[], int width, int height)
{
	//设置位图属性(32bit,无alpha)
	gfx.pRenderTarget->GetDpi(&properties.dpiX, &properties.dpiY);
	properties.pixelFormat.format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	properties.pixelFormat.alphaMode = D2D1_ALPHA_MODE::D2D1_ALPHA_MODE_IGNORE;

	//创建位图
	HRESULT res = gfx.pRenderTarget->CreateBitmap(D2D1::SizeU(width, height), arr, (UINT32)width * sizeof(COLORREF), &properties, &pBitmap);
	if (res != S_OK)return false;

	return true;
}

void Bitmap::draw(Graphics& gfx, float x, float y, float width, float height, float alpha)
{
	gfx.pRenderTarget->DrawBitmap(pBitmap, D2D1::RectF(x, y, width, height), alpha);
}

/******************************************************
* 主类
*
******************************************************/

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

Graphics::Graphics()
{
	pFactory = nullptr;
	pRenderTarget = nullptr;
	pWriteFactory = nullptr;
}
Graphics::~Graphics()
{
	if (pFactory != nullptr)pFactory->Release();
	if (pRenderTarget != nullptr)pRenderTarget->Release();
	if (pRenderTarget != nullptr)pWriteFactory->Release();
}

bool Graphics::init(HWND hWnd)
{
	HRESULT res;

	//创建ID2D1Factory
	res = D2D1CreateFactory(D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_SINGLE_THREADED,//设置为单线程
		&pFactory);
	if (res != S_OK)return false;

	//创建ID2D1HwndRenderTarget
	RECT rect;
	GetClientRect(hWnd, &rect);

	res = pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rect.right, rect.bottom)),
		&pRenderTarget);
	if (res != S_OK)return false;

	//创建ID2D1WriteFactory
	res = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		(IUnknown**)&pWriteFactory
	);
	if (res != S_OK)return false;

	return true;
}

void Graphics::begin_draw()
{
	pRenderTarget->BeginDraw();
}
void Graphics::end_draw()
{
	pRenderTarget->EndDraw();
}

bool Graphics::resize(HWND hWnd)
{
	if (pFactory == nullptr)return false;
	if (pRenderTarget != nullptr)pRenderTarget->Release();

	RECT rect;
	GetClientRect(hWnd, &rect);

	HRESULT res = pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rect.right, rect.bottom)),
		&pRenderTarget);
	if (res != S_OK)return false;

	return true;
}

void Graphics::clear(COLORREF color)
{
	pRenderTarget->Clear(Color(color));
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
	if (res != S_OK)return false;

	D2D1_SIZE_F renderTargetSize = pRenderTarget->GetSize();
	pRenderTarget->DrawTextW(str, wcslen(str), textFormat, D2D1::RectF(x, y, renderTargetSize.width, renderTargetSize.height), brush);

	return true;
}
bool Graphics::draw_c_text(float midX, float y, const wchar_t str[], Brush_t* brush, Font font)
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