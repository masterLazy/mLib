#pragma once
/******************************************************
* mGraphics.h
* ��ͼ
*
* ����Direct2D
******************************************************/

#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>

#pragma comment(lib,"d2d1.lib");
#pragma comment(lib,"dwrite.lib")

#define CTRL_BG_COLOR ((COLORREF)0xf0f0f0)	//�ؼ��ı�����ɫ

/******************************************************
* Font
* ����
* 
******************************************************/

//�����ϸ
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

typedef ID2D1Brush Brush_t;

class Graphics
{
private:
	ID2D1Factory* pFactory;					//Direct2D�Ľӿ�
	ID2D1HwndRenderTarget* pRenderTarget;	//��ȾĿ��
	IDWriteFactory* pWriteFactory;			//DirectWrite�Ľӿ�

	//תD2D1_COLOR_F
	static D2D1_COLOR_F color(COLORREF color, float flpha = 1);
	static D2D1_COLOR_F color(int _r, int _g, int _b, float alpha = 1);

public:
	//ע��:��Ҫ�õ�gfx�ĺ���,�������ӦΪ����(Graphics &gfx),��Ϊ�漰gfx�Ĳ������ܻ��޸�����ֵ.

	/******************************************************
	* ����
	*
	******************************************************/

	//������ɫ����
	Brush_t* brush(int r, int g, int b, float alpha = 1);
	Brush_t* brush(float r, float g, float b, float alpha = 1);

	/******************************************************
	* Geometry
	* ����ͼ��
	*
	******************************************************/

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

		//��(x,y)��ʼһ����ͼ��,֮�󽫽������ӵ��˼���ͼ����
		//filled:	�Ƿ�������ͼ��
		//	�������,����Fill()ʱ��ͼ�ν����ᱻ����
		void begin(float x, float y, bool filled = true);

		//����ͼ��
		//closed:	�Ƿ��Ǳպϼ���ͼ��.�����,���ӵ�ǰ�㵽������һ��ֱ��
		//	���filled==true,����Fill()ʱ��Զ�������һ����
		void end(bool closed = false);

		//���ӵ�(x,y)��ֱ��
		void add_line(float x, float y);

		//���ӵ�(x,y)�ı���������,���Ƶ�Ϊ(x1,y1)
		void add_bezier(float x1, float y1, float x, float y);

		//���ӵ�(x,y)�ı���������,���Ƶ�ֱ�Ϊ(x1,y1),(x2,y2)
		void add_bezier(float x1, float y1, float x2, float y2, float x, float y);

		//���ӵ�(x,y)��Բ��,Բ���뾶Ϊr(r>=�������/2)
		//neg:	�Ƿ�Ϊ��ʱ��
		//large:	���ĽǶ��Ƿ�>=180��
		void add_arc(float x, float y, float r, bool neg = false, bool large = false);
	};

	/******************************************************
	* Bitmap
	* λͼ
	*
	******************************************************/

	class Bitmap
	{
	private:
		ID2D1Bitmap* pBitmap;				//Direct2Dλͼ�Ľӿ�
		D2D1_BITMAP_PROPERTIES properties;	//λͼ��ʽ

	public:
		Bitmap();
		~Bitmap();

		//��ʼ��(���ȵ���Graphics::BeginDraw())
		bool init(Graphics& gfx, const COLORREF arr[], int width, int height);

		//����
		void draw(Graphics& gfx, float x, float y, float width, float height, float alpha = 1);
	};

	/******************************************************
	* ����
	*
	******************************************************/

	Graphics();
	~Graphics();

	//��ʼ��
	bool init(HWND hWnd);

	//��ʼ��ͼ
	void begin_draw();

	//������ͼ
	void end_draw();

	//��������ȾĿ�ꡱ��С
	bool resize(HWND hWnd);

	//�����Ļ
	void clear(COLORREF color = RGB(0, 0, 0));

	//�����ı�
	bool draw_text(float x, float y, const wchar_t str[], Brush_t* brush, Font font = { L"΢���ź�",FontWeight::REGULAR,FontStyle::NORMAL,20 });
	bool draw_c_text(float midX, float y, const wchar_t str[], Brush_t* brush, Font font = { L"΢���ź�",FontWeight::REGULAR,FontStyle::NORMAL,20 });

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
};

typedef Graphics::Geometry Geometry;
typedef Graphics::Bitmap Bitmap;