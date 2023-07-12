#pragma once
/*****************************************************************************
* mFigure.h
* 科学绘图
*
*****************************************************************************/

#include <thread>
#include "../mMath.h"
#include "../mFunction.h"

namespace mlib
{
	static std::vector <void*> _figs;

	/*****************************************************************************
	* 内置的颜色
	*****************************************************************************/
	namespace defcol
	{
		static const COLORREF blue = RGB(0x1f, 0x77, 0xb4);
		static const COLORREF orange = RGB(0xff, 0x7f, 0x0e);
		static const COLORREF green = RGB(0x2c, 0xa0, 0x2c);
		static const COLORREF red = RGB(0xd6, 0x27, 0x28);
		static const COLORREF purple = RGB(0x94, 0x64, 0xbd);
	}

	/*****************************************************************************
	* 绘制的项
	*****************************************************************************/

	class FigObjBase
	{
	public:
		virtual Rect get_rect() = 0;
		virtual void draw(Graphics& gfx, RECT rDraw, Rect rect) = 0;
	};
	namespace FigObj
	{
		/*****************************************************************************
		* 折线图
		* Plot
		*****************************************************************************/
		class Plot :public FigObjBase
		{
		private:
			std::vector<float> x, y;
			COLORREF color;
			float lineWidth;
		public:
			Plot() {}
			/// <param name="_x">所有点的x坐标</param>
			/// <param name="_y">所有点的y坐标</param>
			/// <param name="_color">折线的颜色</param>
			/// <param name="_lineWidth">折线的线宽</param>
			Plot(const std::vector<float>& _x, const std::vector<float>& _y, COLORREF _color, float _lineWidth = 2)
			{
				x = _x;
				y = _y;
				y.resize(x.size());
				color = _color;
				lineWidth = _lineWidth;
			}

			Rect get_rect()
			{
				Rect temp = { 0 };
				if (x.empty() || y.empty())
				{
					return temp;
				}
				temp.left = temp.right = x[0];
				temp.top = temp.bottom = y[0];
				for (size_t i = 1; i < x.size(); i++)
				{
					temp.left = min(temp.left, x[i]);
					temp.right = max(temp.right, x[i]);
					temp.top = max(temp.top, y[i]);
					temp.bottom = min(temp.bottom, y[i]);
				}
				return temp;
			}
			void draw(Graphics& gfx, RECT rDraw, Rect rect)
			{
				if (x.empty())return;
				for (size_t i = 0; i < x.size() - 1; i++)
				{
					float X1 = Map(x[i], rect.left, rect.right, rDraw.left, rDraw.right);
					float Y1 = Map(y[i], rect.top, rect.bottom, rDraw.top, rDraw.bottom);
					float X2 = Map(x[i + 1], rect.left, rect.right, rDraw.left, rDraw.right);
					float Y2 = Map(y[i + 1], rect.top, rect.bottom, rDraw.top, rDraw.bottom);
					if (X1<rDraw.left || X1>rDraw.right ||
						Y1<rDraw.top || Y1>rDraw.bottom ||
						X2<rDraw.left || X2>rDraw.right ||
						Y2<rDraw.top || Y2>rDraw.bottom)
					{
						continue;
					}
					gfx.fill_circle(X1, Y1, lineWidth / 2, gfx.brush(color));
					gfx.fill_circle(X2, Y2, lineWidth / 2, gfx.brush(color));
					gfx.draw_line(X1, Y1, X2, Y2, gfx.brush(color), lineWidth);
				}
			}
		};

		/*****************************************************************************
		* 散点图
		* Scatter
		*****************************************************************************/
		class Scatter :public FigObjBase
		{
		private:
			std::vector<float> x, y;
			COLORREF color;
			float radius;
		public:
			Scatter() {}
			/// <param name="_x">所有点的x坐标</param>
			/// <param name="_y">所有点的y坐标</param>
			/// <param name="_color">点的颜色</param>
			/// <param name="_radius">点的半径</param>
			Scatter(const std::vector<float>& _x, const std::vector<float>& _y, COLORREF _color, float _radius = 2)
			{
				x = _x;
				y = _y;
				y.resize(x.size());
				color = _color;
				radius = _radius;
			}

			Rect get_rect()
			{
				Rect temp = { 0 };
				if (x.empty() || y.empty())
				{
					return temp;
				}
				temp.left = temp.right = x[0];
				temp.top = temp.bottom = y[0];
				for (size_t i = 1; i < x.size(); i++)
				{
					temp.left = min(temp.left, x[i]);
					temp.right = max(temp.right, x[i]);
					temp.top = max(temp.top, y[i]);
					temp.bottom = min(temp.bottom, y[i]);
				}
				return temp;
			}
			void draw(Graphics& gfx, RECT rDraw, Rect rect)
			{
				if (x.empty())return;
				for (size_t i = 0; i < x.size(); i++)
				{
					float X = Map(x[i], rect.left, rect.right, rDraw.left, rDraw.right);
					float Y = Map(y[i], rect.top, rect.bottom, rDraw.top, rDraw.bottom);
					if (X<rDraw.left || X>rDraw.right ||
						Y<rDraw.top || Y>rDraw.bottom)
					{
						continue;
					}
					gfx.fill_circle(X, Y, radius, gfx.brush(color));
				}
			}
		};

		/*****************************************************************************
		* 单通道图像
		* Simage
		*****************************************************************************/
		class Simage :public FigObjBase
		{
		private:
			std::vector<float> img;
			unsigned w, h;
			Cmap cmap;
		public:
			Simage() {}
			/// <param name="_img">图像</param>
			/// <param name="width">图像宽度</param>
			/// <param name="height">图像高度</param>
			/// <param name="_cmap">颜色映射方案</param>
			/// <param name="revY">是否反转 Y 轴</param>
			Simage(const std::vector<float>& _img, unsigned width, unsigned height,
				Cmap _cmap = Cmap::viridis, bool revY = true)
			{
				w = width;
				h = height;
				cmap = _cmap;
				//别忘了图片本来就是反转的
				if (revY)
				{
					img = _img;
				}
				else
				{
					img.resize(w * h);
					for (size_t x = 0; x < w; x++)
					{
						for (size_t y = 0; y < h; y++)
						{
							img[x * h + y] = _img[x * h + (h - y - 1)];
						}
					}
				}
			}

			Rect get_rect()
			{
				return { 0,(Math_F)h,(Math_F)w,0 };
			}
			void draw(Graphics& gfx, RECT rDraw, Rect rect)
			{
				if (img.empty())return;
				//找最大最小值
				float max, min;
				max = min = img[0];
				for (size_t i = 0; i < w * h; i++)
				{
					if (img[i] > max)max = img[i];
					if (img[i] < min)min = img[i];
				}
				//渲染颜色(暂且用CPU)
				COLORREF* color = new COLORREF[w * h];
				for (size_t i = 0; i < w * h; i++)
				{
					color[i] = ColorMap(img[i], cmap, min, max);
				}
				//绘制(好草率啊！！！！！！！！！！！！！！！！！)
				Image img;
				img.load_from_mem(color, w, h);
				float X1 = Map(0, rect.left, rect.right, rDraw.left, rDraw.right);
				float X2 = Map(w, rect.left, rect.right, rDraw.left, rDraw.right);
				float Y1 = Map(0, rect.top, rect.bottom, rDraw.bottom, rDraw.top);
				float Y2 = Map(h, rect.top, rect.bottom, rDraw.bottom, rDraw.top);
				gfx.draw_image(img, X1, Y1, X2 - X1, Y2 - Y1, 0, 1, false);
				delete[] color;
			}
		};
	}

	/*****************************************************************************
	* Figure
	* Figure 主类
	* 注：窗口被关闭后只是隐藏了，可以调用 show() 显示。
	*****************************************************************************/
	class Figure
	{
	private:
		HWND hWnd = NULL;
		Graphics gfx;
		bool inited = false;
		std::vector<FigObjBase*> objs;
		//协调两个线程
		bool drawing = false;
		bool editing = false;

		//绘制
		void draw(RECT rWindow)
		{
			drawing = true;
			int iEdge = max(60, min(rWindow.right, rWindow.bottom) / 10);//边距
			//计算绘图区范围
			if (bAutoRect && !objs.empty())
			{
				rect = objs[0]->get_rect();
				Rect temp;
				for (auto i : objs)
				{
					temp = i->get_rect();
					rect.left = min(rect.left, temp.left);
					rect.right = max(rect.right, temp.right);
					rect.top = max(rect.top, temp.top);
					rect.bottom = min(rect.bottom, temp.bottom);
				}
			}
			RECT rDraw = { iEdge + 10,iEdge + 10, rWindow.right - iEdge - 10,rWindow.bottom - iEdge - 10 };
			//绘制方框
			gfx.draw_rectangle(iEdge, iEdge, rWindow.right - iEdge, rWindow.bottom - iEdge, gfx.brush(0, 0, 0));
			//绘制标题
			gfx.draw_text_c(rWindow.right / 2.0, iEdge / 2.0, title.c_str(), gfx.brush(0, 0, 0));
			//绘制刻度、网格
			//x轴
			float tick = 1;
			if (rect.right - rect.left != 0)
			{
				while (tick < (rect.right - rect.left) / 5)
				{
					tick += ::pow(10, int(log10(tick)));
				}
				while (tick > (rect.right - rect.left) / 5)
				{
					tick -= ::pow(10, int(log10(tick)) - 2);
				}
			}
			float min = int(rect.left / tick) * tick;
			for (float x = min; x <= rect.right; x += tick)
			{
				float X = Map(x, min, rect.right, rDraw.left, rDraw.right);
				gfx.draw_line(X, rWindow.bottom - iEdge, X, rWindow.bottom - iEdge + 10, gfx.brush(0, 0, 0));
				char temp[16];
				sprintf_s(temp, "%.3g", x);
				gfx.draw_text_c(X, rWindow.bottom - iEdge + 10,
					StrToWstr(temp).c_str(),
					gfx.brush(0, 0, 0));
				if (bGrid)
				{
					gfx.draw_line(X, iEdge, X, rWindow.bottom - iEdge, gfx.brush(0, 0, 0, 0.2));
				}
			}
			//y轴(注意top比bottom大)
			tick = 1;
			if (rect.top - rect.bottom != 0)
			{
				while (tick < (rect.top - rect.bottom) / 5)
				{
					tick += ::pow(10, int(log10(tick)));
				}
				while (tick > (rect.top - rect.bottom) / 5)
				{
					tick -= ::pow(10, int(log10(tick)) - 2);
				}
			}
			min = int(rect.bottom / tick) * tick;
			for (float y = min; y <= rect.top; y += tick)
			{
				float Y = Map(y, min, rect.top, rDraw.bottom, rDraw.top);
				gfx.draw_line(iEdge, Y, iEdge - 10, Y, gfx.brush(0, 0, 0));
				char temp[16];
				sprintf_s(temp, "%.3g", y);
				gfx.draw_text(iEdge - 15 - 7.5 * strlen(temp), Y - 10,
					StrToWstr(temp).c_str(),
					gfx.brush(0, 0, 0));
				if (bGrid)
				{
					gfx.draw_line(iEdge, Y, rWindow.right - iEdge, Y, gfx.brush(0, 0, 0, 0.2));
				}
			}
			//绘制项
			for (auto i : objs)
			{
				i->draw(gfx, rDraw, rect);
			}
			drawing = false;
		}

		static LRESULT CALLBACK wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			auto i = GetWindowLong(hWnd, sizeof(long));
			Figure* _this = nullptr;
			if (_figs.size() > i)
			{
				_this = (Figure*)_figs[i];
				if (!_this->inited)
				{
					_this->gfx.init(hWnd);
					_this->inited = true;
				}
			}
			switch (message)
			{
			case WM_ERASEBKGND:
				return 0;
			case WM_SIZE:
			{
				RECT rect;
				GetClientRect(hWnd, &rect);
				if (_this != nullptr)
				{
					_this->gfx.resize(rect.right, rect.bottom);
				}
				return 0;
			}
			case WM_PAINT:
				if (_this != nullptr)
				{
					RECT rect;
					GetClientRect(hWnd, &rect);
					//绘图
					if (!_this->editing)
					{
						_this->gfx.begin_draw();
						_this->gfx.clear(RGB(255, 255, 255));
						_this->draw(rect);
						_this->gfx.end_draw();
					}
				}
				return 0;
			case WM_CLOSE:
				ShowWindow(hWnd, SW_HIDE);
				return 0;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		static void create(Figure* _this)
		{
			//注册窗口
			MSG msg;
			WNDCLASS wndclass;
			std::wstring name = L"Figure " + std::to_wstring(_figs.size() + 1);

			wndclass.style = CS_HREDRAW | CS_VREDRAW;
			wndclass.lpfnWndProc = wnd_proc;
			wndclass.cbClsExtra = 0;
			wndclass.cbWndExtra = sizeof(Figure*);
			wndclass.hInstance = NULL;
			wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
			wndclass.hbrBackground = WHITE_BRUSH;
			wndclass.lpszMenuName = NULL;
			wndclass.lpszClassName = name.c_str();

			if (!RegisterClass(&wndclass))
			{
				MessageBox(NULL, L"窗口注册失败！", name.c_str(), MB_ICONERROR);
				return;
			}

			_this->hWnd = CreateWindow(name.c_str(),
				name.c_str(),//窗口的标题
				WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_SIZEBOX,
				CW_USEDEFAULT,//窗口的x坐标
				CW_USEDEFAULT,//窗口的y坐标
				600,  //窗口的宽
				500, //窗口的高
				NULL,
				NULL,
				NULL,
				NULL);
			SetWindowLong(_this->hWnd, sizeof(long), _figs.size());
			_figs.push_back(_this);

			UpdateWindow(_this->hWnd);

			while (true)
			{
				if (PeekMessage(&msg, _this->hWnd, 0, 0, PM_REMOVE))
				{
					if (msg.message == WM_QUIT)
					{
						break;
					}
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}

	public:
		bool bAutoRect = true;			//是否自动调整绘图区范围
		mlib::Rect rect = { 0,1,1,0 };	//绘图区范围

		bool bGrid = false;				//绘制网格

		std::wstring title;				//绘图区的标题

		~Figure()
		{
			clear();
		}

		//初始化
		HWND init(const wchar_t szTitle[] = L"")
		{
			title = szTitle;
			//开另一线程
			std::thread th(create, this);
			th.detach();

			while (hWnd == NULL);
			Sleep(5);//安全缓冲时间，不然会有玄学 Bug
			return hWnd;
		}
		bool if_init()
		{
			return inited;
		}

		//显示
		void show()
		{
			ShowWindow(hWnd, SW_SHOW);
		}
		//隐藏
		void hide()
		{
			ShowWindow(hWnd, SW_HIDE);
		}

		//开始编辑 (编辑指clear()和add())
		void begin_edit()
		{
			editing = true;
			while (drawing);
			Sleep(5);//安全缓冲时间，不然会有玄学 Bug
		}
		//结束编辑 (编辑指clear()和add())
		void end_edit()
		{
			editing = false;
		}
		//清空
		void clear()
		{
			for (auto i : objs)delete i;
			objs.clear();
		}
		//添加要画的东西
		template<typename Ty> void add(const Ty& obj)
		{
			//新建一层
			Ty* pObj = new Ty;
			*pObj = obj;

			objs.push_back(pObj);
		}
	};
}