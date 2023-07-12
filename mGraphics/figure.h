#pragma once
/*****************************************************************************
* mFigure.h
* ��ѧ��ͼ
*
*****************************************************************************/

#include <thread>
#include "../mMath.h"
#include "../mFunction.h"

namespace mlib
{
	static std::vector <void*> _figs;

	/*****************************************************************************
	* ���õ���ɫ
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
	* ���Ƶ���
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
		* ����ͼ
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
			/// <param name="_x">���е��x����</param>
			/// <param name="_y">���е��y����</param>
			/// <param name="_color">���ߵ���ɫ</param>
			/// <param name="_lineWidth">���ߵ��߿�</param>
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
		* ɢ��ͼ
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
			/// <param name="_x">���е��x����</param>
			/// <param name="_y">���е��y����</param>
			/// <param name="_color">�����ɫ</param>
			/// <param name="_radius">��İ뾶</param>
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
		* ��ͨ��ͼ��
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
			/// <param name="_img">ͼ��</param>
			/// <param name="width">ͼ����</param>
			/// <param name="height">ͼ��߶�</param>
			/// <param name="_cmap">��ɫӳ�䷽��</param>
			/// <param name="revY">�Ƿ�ת Y ��</param>
			Simage(const std::vector<float>& _img, unsigned width, unsigned height,
				Cmap _cmap = Cmap::viridis, bool revY = true)
			{
				w = width;
				h = height;
				cmap = _cmap;
				//������ͼƬ�������Ƿ�ת��
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
				//�������Сֵ
				float max, min;
				max = min = img[0];
				for (size_t i = 0; i < w * h; i++)
				{
					if (img[i] > max)max = img[i];
					if (img[i] < min)min = img[i];
				}
				//��Ⱦ��ɫ(������CPU)
				COLORREF* color = new COLORREF[w * h];
				for (size_t i = 0; i < w * h; i++)
				{
					color[i] = ColorMap(img[i], cmap, min, max);
				}
				//����(�ò��ʰ�����������������������������������)
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
	* Figure ����
	* ע�����ڱ��رպ�ֻ�������ˣ����Ե��� show() ��ʾ��
	*****************************************************************************/
	class Figure
	{
	private:
		HWND hWnd = NULL;
		Graphics gfx;
		bool inited = false;
		std::vector<FigObjBase*> objs;
		//Э�������߳�
		bool drawing = false;
		bool editing = false;

		//����
		void draw(RECT rWindow)
		{
			drawing = true;
			int iEdge = max(60, min(rWindow.right, rWindow.bottom) / 10);//�߾�
			//�����ͼ����Χ
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
			//���Ʒ���
			gfx.draw_rectangle(iEdge, iEdge, rWindow.right - iEdge, rWindow.bottom - iEdge, gfx.brush(0, 0, 0));
			//���Ʊ���
			gfx.draw_text_c(rWindow.right / 2.0, iEdge / 2.0, title.c_str(), gfx.brush(0, 0, 0));
			//���ƿ̶ȡ�����
			//x��
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
			//y��(ע��top��bottom��)
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
			//������
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
					//��ͼ
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
			//ע�ᴰ��
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
				MessageBox(NULL, L"����ע��ʧ�ܣ�", name.c_str(), MB_ICONERROR);
				return;
			}

			_this->hWnd = CreateWindow(name.c_str(),
				name.c_str(),//���ڵı���
				WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_SIZEBOX,
				CW_USEDEFAULT,//���ڵ�x����
				CW_USEDEFAULT,//���ڵ�y����
				600,  //���ڵĿ�
				500, //���ڵĸ�
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
		bool bAutoRect = true;			//�Ƿ��Զ�������ͼ����Χ
		mlib::Rect rect = { 0,1,1,0 };	//��ͼ����Χ

		bool bGrid = false;				//��������

		std::wstring title;				//��ͼ���ı���

		~Figure()
		{
			clear();
		}

		//��ʼ��
		HWND init(const wchar_t szTitle[] = L"")
		{
			title = szTitle;
			//����һ�߳�
			std::thread th(create, this);
			th.detach();

			while (hWnd == NULL);
			Sleep(5);//��ȫ����ʱ�䣬��Ȼ������ѧ Bug
			return hWnd;
		}
		bool if_init()
		{
			return inited;
		}

		//��ʾ
		void show()
		{
			ShowWindow(hWnd, SW_SHOW);
		}
		//����
		void hide()
		{
			ShowWindow(hWnd, SW_HIDE);
		}

		//��ʼ�༭ (�༭ָclear()��add())
		void begin_edit()
		{
			editing = true;
			while (drawing);
			Sleep(5);//��ȫ����ʱ�䣬��Ȼ������ѧ Bug
		}
		//�����༭ (�༭ָclear()��add())
		void end_edit()
		{
			editing = false;
		}
		//���
		void clear()
		{
			for (auto i : objs)delete i;
			objs.clear();
		}
		//���Ҫ���Ķ���
		template<typename Ty> void add(const Ty& obj)
		{
			//�½�һ��
			Ty* pObj = new Ty;
			*pObj = obj;

			objs.push_back(pObj);
		}
	};
}