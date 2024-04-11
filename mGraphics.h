#pragma once
/*****************************************************************************
* mGraphics.h
* ��ͼ
*
* ����Direct2D
*****************************************************************************/

#include <Windows.h>
#include <windowsx.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <dwrite.h>
#include <vector>

#include "mMath.h"

#include "mGraphics/image.h"	//ͼ��

/*#undef max
#undef min*/

#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")

namespace mlib
{
	const COLORREF CTRL_BG_COLOR = 0xf0f0f0;	//�ؼ��ı�����ɫ

	typedef ID2D1Brush Brush_t;

	/*****************************************************************************
	* Graphics
	* Graphics����
	*****************************************************************************/

#define AUTO	-1
	class Graphics
	{
	private:
		ID2D1Factory* pFactory;				//Direct2D�Ľӿ�
		IDWriteFactory* pWriteFactory;		//DirectWrite�Ľӿ�
		ID2D1RenderTarget* pRenderTarget;	//��ȾĿ��

		int mode;	//1: HWND; 2: Image
		HWND hWnd;
		Image img;
		D2D1::Matrix3x2F trans;

		//תD2D1_COLOR_F
		static D2D1_COLOR_F color(COLORREF color, float flpha = 1);
		static D2D1_COLOR_F color(int _r, int _g, int _b, float alpha = 1);

	public:
		//ע��:��Ҫ�õ�gfx�ĺ���,�������ӦΪ����(Graphics &gfx),��Ϊ�漰gfx�Ĳ������ܻ��޸�����ֵ.

		/*****************************************************************************
		* Brush_t
		* ����
		* ��������ĳ�һ����
		*****************************************************************************/

		//������ɫ����
		Brush_t* brush(int r, int g, int b, float alpha = 1);
		Brush_t* brush(COLORREF color, float alpha = 1);
		Brush_t* brush_f(float r, float g, float b, float alpha = 1);

		/*****************************************************************************
		* Geometry
		* ����ͼ��
		*****************************************************************************/

		class Geometry
		{
		private:
			ID2D1PathGeometry* pPathGeometry;	//Direct2D·������ͼ�εĽӿ�
			ID2D1GeometrySink* pSink;			//����ͼ�ν�����

		public:
			Geometry();
			~Geometry();

			//��ʼ��(���ȵ���Graphics::BeginDraw())
			bool init(Graphics& gfx);

			//����
			void draw(Graphics& gfx, Brush_t* brush, float lineWidth = 1);
			void fill(Graphics& gfx, Brush_t* brush);

			//��ʼ����˼���ͼ��
			bool begin_def();

			//��������˼���ͼ��
			void end_def();

			//��(x,y)��ʼһ����ͼ��,֮�󽫽�����ӵ��˼���ͼ����
			//filled:	�Ƿ�������ͼ��
			//	�������,����Fill()ʱ��ͼ�ν����ᱻ����
			void begin(float x, float y, bool filled = true);

			//����ͼ��
			//closed:	�Ƿ��Ǳպϼ���ͼ��.�����,���ӵ�ǰ�㵽������һ��ֱ��
			//	���filled==true,����Fill()ʱ��Զ�������һ����
			void end(bool closed = false);

			//��ӵ�(x,y)��ֱ��
			void add_line(float x, float y);

			//��ӵ�(x,y)�ı���������,���Ƶ�Ϊ(x1,y1)
			void add_bezier(float x1, float y1, float x, float y);

			//��ӵ�(x,y)�ı���������,���Ƶ�ֱ�Ϊ(x1,y1),(x2,y2)
			void add_bezier(float x1, float y1, float x2, float y2, float x, float y);

			//��ӵ�(x,y)��Բ��,Բ���뾶Ϊr(r>=�������/2)
			//neg:		�Ƿ�Ϊ��ʱ��
			//large:	���ĽǶ��Ƿ�>=180��
			void add_arc(float x, float y, float r, bool neg = false, bool large = false);
		};

		/*****************************************************************************
		* ������
		*****************************************************************************/

		int m_x, m_y;	//���λ��
		int m_x0, m_y0; //��갴�º�ĳ�ʼλ��
		int m_dx, m_dy;	//����԰��º��ƶ��ľ���
		int m_whl;		//�����ֹ���ֵ
		bool m_down;	//�������Ƿ���

		Graphics();
		~Graphics();


		/*��������*/


		//��ʼ������ͼ������
		bool init(HWND hWnd);
		//��ʼ������ͼ��ͼ��
		bool init(const Image& img);

		//��ʼ��ͼ
		bool begin_draw();
		//������ͼ
		bool end_draw();

		//������Ⱦ����С
		bool resize(UINT32 width = AUTO, UINT32 height = AUTO);

		//�����Ļ
		void clear(COLORREF color = RGB(0, 0, 0));


		/*��ͼ�ռ�*/


		//��ת��ͼ�ռ�
		void trans_rotate(float x, float y, float angle);
		//���Ż�ͼ�ռ�
		void trans_scale(float x, float y, float rateX, float rateY = AUTO);
		//ƫ�ƻ�ͼ�ռ�
		void trans_shift(float x, float y);
		//��ձ任
		void trans_clear();


		/*�ı�*/

		//�����ϸ
#undef BLACK
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
		//������ʽ
		enum class FontStyle
		{
			NORMAL,	//����
			OBLIQUE,	//б��
			ITALIC	//�������
		};
		//����
		struct Font
		{
			const wchar_t* szFontName = L"΢���ź�";		//��������
			FontWeight weight = FontWeight::REGULAR;	//�����ϸ
			FontStyle style = FontStyle::NORMAL;		//������ʽ
			float size = 20;							//�����С
		};
		inline static const Font def_font;

		//�ı����뷽ʽ(��ָ������)
		enum class FontAlign {
			left_top, top, right_top,
			left, middle, right,
			left_bottom, bottom, right_bottom
		};
		//�����ı�
		bool draw_text(float x, float y, const std::wstring& str, Brush_t* brush, FontAlign align = FontAlign::left_top, Font font = def_font);
		//�����ı�(�޶�������)
		bool draw_text(float left, float top, float right, float bottom, const std::wstring& str, Brush_t* brush, Font font = def_font);

		//��ȡ�ı���ռ����(����ڻ���ԭ��)
		Rect get_text_rect(const std::wstring& str, Font font);


		/*����ͼ��*/


		//����ֱ��
		void draw_line(float x1, float y1, float x2, float y2, Brush_t* brush, float lineWidth = 1);

		//���ƾ���
		void draw_rectangle(float left, float top, float right, float bottom, Brush_t* brush, float lineWidth = 1);
		void fill_rectangle(float left, float top, float right, float bottom, Brush_t* brush);

		//����Բ�Ǿ���
		void draw_r_rectangle(float left, float top, float right, float bottom, float r, Brush_t* brush, float lineWidth = 1);
		void fill_r_rectangle(float left, float top, float right, float bottom, float r, Brush_t* brush);

		//������Բ
		void draw_ellipse(float x, float y, float width, float height, Brush_t* brush, float lineWidth = 1);
		void fill_ellipse(float x, float y, float width, float height, Brush_t* brush);

		//������Բ
		void draw_circle(float x, float y, float r, Brush_t* brush, float lineWidth = 1);
		void fill_circle(float x, float y, float r, Brush_t* brush);

		//���ƶ����
		void draw_polygon(std::vector<Point> pts, Brush_t* brush, float lineWidth = 1);
		void fill_polygon(std::vector<Point> pts, Brush_t* brush);

		//����ͼ��
		//interpolation: �Ƿ��ֵ
		bool draw_image(const Image& img, float x, float y, float width = AUTO, float height = AUTO,
			float angle = 0, float alpha = 1, bool interpolation = true);
		bool draw_image_s(const Image& img, float x, float y, float scale = 1, float angle = 0,
			float alpha = 1, bool interpolation = true);


		/*����*/


		//�� HWND ģʽ���Զ�����
		//ʹ�÷����� WndProc() �м���: if(gfx.proc(hWnd, message, wParam, lParam))return 0;
		bool proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, bool no_erase = true);
		/* ���еĲ�������:
		* WM_CREATE			init()
		* WM_SIZE			resize()
		* WM_ERASEBKGND		return 0;
		* WM_MOUSEMOVE
		* WM_LBUTTONDOWN
		* WM_LBUTTONUP
		* WM_MOUSEWHEEL		���úͼ�������йصı���
		*/
	};

	typedef Graphics::Geometry Geometry;
	typedef Graphics::FontWeight FontWeight;
	typedef Graphics::FontStyle FontStyle;
	typedef Graphics::FontAlign FontAlign;
	typedef Graphics::Font Font;
}

#include "mGraphics/figure.h"	//��ѧ��ͼ

#include "mGraphics.hpp"