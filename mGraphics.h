#pragma once
/*****************************************************************************
* mGraphics.h
* 绘图
*
* 基于Direct2D
*****************************************************************************/

#include <Windows.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <dwrite.h>

#include "mGraphics/image.h"	//图像

#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
#pragma comment(lib,"windowscodecs.lib")

namespace mlib
{
#define CTRL_BG_COLOR ((COLORREF)0xf0f0f0)	//控件的背景颜色

	/*****************************************************************************
	* Font
	* 字体
	*
	*****************************************************************************/

	//字体粗细
	enum class FontWeight
	{
		THIN = 100,
		EXTRA_LIGHT = 200,
		ULTRA_LIGHT = 200,
		LIGHT = 300,
		SEMI_LIGHT = 350,
		NORMAL = 400,
		REGULAR = 400,
		MEDIUM = 500,
		DEMI_BOLD = 600,
		SEMI_BOLD = 600,
		BOLD = 700,
		EXTRA_BOLD = 800,
		ULTRA_BOLD = 800,
		BLACK = 900,
		HEAVY = 900,
		EXTRA_BLACK = 950,
		ULTRA_BLACK = 950
	};
	//字体样式
	enum class FontStyle
	{
		NORMAL,	//正常
		OBLIQUE,	//斜体
		ITALIC	//意大利体
	};
	//字体
	struct Font
	{
		const wchar_t* szFontName = L"微软雅黑";		//字体名称
		FontWeight weight = FontWeight::REGULAR;	//字体粗细
		FontStyle style = FontStyle::NORMAL;		//字体样式
		float size = 20;							//字体大小
	};

	typedef ID2D1Brush Brush_t;

	/*****************************************************************************
	* Graphics
	* Graphics主类
	*
	*****************************************************************************/

#define AUTO	-1
	class Graphics
	{
	private:
		ID2D1Factory* pFactory;				//Direct2D的接口
		IDWriteFactory* pWriteFactory;		//DirectWrite的接口
		ID2D1RenderTarget* pRenderTarget;	//渲染目标

		int mode;	//1: HWND; 2: Image
		HWND hWnd;
		Image img;
		D2D1::Matrix3x2F trans;

		//转D2D1_COLOR_F
		static D2D1_COLOR_F color(COLORREF color, float flpha = 1);
		static D2D1_COLOR_F color(int _r, int _g, int _b, float alpha = 1);

	public:
		//注意:需要用到gfx的函数,该项参数应为引用(Graphics &gfx),因为涉及gfx的操作可能会修改它的值.

		/*****************************************************************************
		* 画笔
		*
		*****************************************************************************/

		//创建纯色画笔
		Brush_t* brush(int r, int g, int b, float alpha = 1);
		Brush_t* brush(COLORREF color, float alpha = 1);
		Brush_t* brush_f(float r, float g, float b, float alpha = 1);

		/*****************************************************************************
		* Geometry
		* 几何图形
		*
		*****************************************************************************/

		class Geometry
		{
		private:
			ID2D1PathGeometry* pPathGeometry;	//Direct2D路径几何图形的接口
			ID2D1GeometrySink* pSink;			//几何图形接收器

		public:
			Geometry();
			~Geometry();

			//初始化(需先调用Graphics::BeginDraw())
			bool init(Graphics& gfx);

			//绘制
			void draw(Graphics& gfx, Brush_t* brush, float lineWidth = 1);
			void fill(Graphics& gfx, Brush_t* brush);

			//开始定义此几何图形
			bool begin_def();

			//结束定义此几何图形
			void end_def();

			//从(x,y)开始一个新图形,之后将将其添加到此几何图形中
			//filled:	是否是填充该图形
			//	如果不是,调用Fill()时该图形将不会被绘制
			void begin(float x, float y, bool filled = true);

			//结束图形
			//closed:	是否是闭合几何图形.如果是,则会从当前点到起点绘制一条直线
			//	如果filled==true,调用Fill()时永远会绘制这一条线
			void end(bool closed = false);

			//添加到(x,y)的直线
			void add_line(float x, float y);

			//添加到(x,y)的贝塞尔曲线,控制点为(x1,y1)
			void add_bezier(float x1, float y1, float x, float y);

			//添加到(x,y)的贝塞尔曲线,控制点分别为(x1,y1),(x2,y2)
			void add_bezier(float x1, float y1, float x2, float y2, float x, float y);

			//添加到(x,y)的圆弧,圆弧半径为r(r>=两点距离/2)
			//neg:	是否为逆时针
			//large:	弧的角度是否>=180°
			void add_arc(float x, float y, float r, bool neg = false, bool large = false);
		};

		/*****************************************************************************
		* 类主体
		*
		*****************************************************************************/

		Graphics();
		~Graphics();

		//初始化：绘图到窗口
		bool init(HWND hWnd);
		//初始化：绘图到图像
		bool init(const Image& img);

		//开始绘图
		bool begin_draw();
		//结束绘图
		bool end_draw();

		//重设渲染区大小
		bool resize(UINT32 width = AUTO, UINT32 height = AUTO);

		//清空屏幕
		void clear(COLORREF color = RGB(0, 0, 0));


		//旋转绘图空间
		void trans_rotate(float x, float y, float angle);
		//缩放绘图空间
		void trans_scale(float x, float y, float rateX, float rateY = AUTO);
		//偏移绘图空间
		void trans_shift(float x, float y);
		//清空变换
		void trans_clear();


		//绘制文本
		bool draw_text(float x, float y, const wchar_t str[], Brush_t* brush, Font font = { L"微软雅黑",FontWeight::REGULAR,FontStyle::NORMAL,20 });
		bool draw_text_c(float midX, float y, const wchar_t str[], Brush_t* brush, Font font = { L"微软雅黑",FontWeight::REGULAR,FontStyle::NORMAL,20 });

		//绘制直线
		void draw_line(float x1, float y1, float x2, float y2, Brush_t* brush, float lineWidth = 1);

		//绘制矩形
		void draw_rectangle(float left, float top, float right, float bottom, Brush_t* brush, float lineWidth = 1);
		void fill_rectangle(float left, float top, float right, float bottom, Brush_t* brush);

		//绘制圆角矩形
		void draw_r_rectangle(float left, float top, float right, float bottom, float r, Brush_t* brush, float lineWidth = 1);
		void fill_r_rectangle(float left, float top, float right, float bottom, float r, Brush_t* brush);

		//绘制椭圆
		void draw_ellipse(float x, float y, float width, float height, Brush_t* brush, float lineWidth = 1);
		void fill_ellipse(float x, float y, float width, float height, Brush_t* brush);

		//绘制正圆
		void draw_circle(float x, float y, float r, Brush_t* brush, float lineWidth = 1);
		void fill_circle(float x, float y, float r, Brush_t* brush);

		//绘制图像
		bool draw_image(const Image& img, float x, float y, float width = AUTO, float height = AUTO, float angle = 0, float alpha = 1);
		bool draw_image_s(const Image& img, float x, float y, float scale = 1, float angle = 0, float alpha = 1);
	};

	typedef Graphics::Geometry Geometry;
}