#pragma once
/*****************************************************************************
* image.h
* ͼ��
*
* ����WIC
*****************************************************************************/

#include <wincodec.h>
#include "../mFunction.h"

namespace mlib
{
	class Image
	{
	private:
		IWICImagingFactory2* pFactory;	//WIC�Ľӿ�
	public:
		IWICBitmap* pBitmap;

		Image()
		{
			pFactory = nullptr;
			pBitmap = nullptr;
			//��ʼ��
			CoInitialize(NULL);
			CoCreateInstance(
				CLSID_WICImagingFactory,
				NULL,
				CLSCTX_INPROC_SERVER,
				IID_PPV_ARGS(&pFactory)
			);
		}
		~Image()
		{
			if (pBitmap != nullptr)pBitmap->Release();
			if (pFactory != nullptr)pFactory->Release();
		}

		//�����հ�ͼ��
		bool init(UINT width, UINT height)
		{
			if (pBitmap != nullptr)pBitmap->Release();

			HRESULT res;
			res = pFactory->CreateBitmap(
				width,
				height,
				GUID_WICPixelFormat32bppRGB,
				WICBitmapCreateCacheOption::WICBitmapCacheOnLoad,
				&pBitmap
			);
			if (!SUCCEEDED(res))return false;
			return true;
		}

		//��ȡͼ����
		UINT get_width() const
		{
			if (pBitmap == nullptr)return 0;

			UINT width, height;
			pBitmap->GetSize(&width, &height);
			return width;
		}
		//��ȡͼ��߶�
		UINT get_height() const
		{
			if (pBitmap == nullptr)return 0;

			UINT width, height;
			pBitmap->GetSize(&width, &height);
			return height;
		}

		//��ȡ����
		COLORREF get(UINT x, UINT y)
		{
			if (pBitmap == nullptr)return false;

			HRESULT res;

			//��ȡLock
			WICRect rect = { x,y,1,1 };
			IWICBitmapLock* pLock;
			res = pBitmap->Lock(&rect, WICBitmapLockFlags::WICBitmapLockRead, &pLock);
			if (!SUCCEEDED(res))return 0;

			//��ȡָ��
			UINT size;
			WICInProcPointer data;
			res = pLock->GetDataPointer(&size, &data);
			if (!SUCCEEDED(res))
			{
				pLock->Release();
				return 0;
			}

			pLock->Release();
			return RGB(data[2], data[1], data[0]);
		}
		//��������
		bool set(UINT x, UINT y, COLORREF color)
		{
			if (pBitmap == nullptr)return false;

			HRESULT res;

			//��ȡLock
			WICRect rect = { x,y,1,1 };
			IWICBitmapLock* pLock;
			res = pBitmap->Lock(&rect, WICBitmapLockFlags::WICBitmapLockWrite, &pLock);
			if (!SUCCEEDED(res))return false;

			//��ȡָ��
			UINT size;
			WICInProcPointer data;
			res = pLock->GetDataPointer(&size, &data);
			if (!SUCCEEDED(res))
			{
				pLock->Release();
				return false;
			}

			//д��
			data[2] = GetRValue(color);
			data[1] = GetGValue(color);
			data[0] = GetBValue(color);

			pLock->Release();
			return true;
		}

		//���ļ�����ͼ��
		bool load_from_file(const wchar_t filename[])
		{
			if (pBitmap != nullptr)pBitmap->Release();

			HRESULT res;

			//����������
			IWICBitmapDecoder* pDecoder;
			res = pFactory->CreateDecoderFromFilename(
				filename,
				NULL,
				GENERIC_READ,
				WICDecodeOptions::WICDecodeMetadataCacheOnLoad,
				&pDecoder
			);
			if (!SUCCEEDED(res))return false;

			//����֡
			IWICBitmapFrameDecode* pFrame;
			res = pDecoder->GetFrame(0, &pFrame);
			if (!SUCCEEDED(res))
			{
				pDecoder->Release();
				return false;
			}

			//ת�����ظ�ʽ
			IWICFormatConverter* pConverter;
			res = pFactory->CreateFormatConverter(&pConverter);
			if (!SUCCEEDED(res))
			{
				pDecoder->Release();
				pFrame->Release();
				return false;
			}
			res = pConverter->Initialize(
				pFrame,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherType::WICBitmapDitherTypeNone,
				NULL,
				0,
				WICBitmapPaletteType::WICBitmapPaletteTypeMedianCut
			);
			if (!SUCCEEDED(res))
			{
				pDecoder->Release();
				pFrame->Release();
				return false;
			}

			//����λͼ
			res = pFactory->CreateBitmapFromSource(
				pConverter,
				WICBitmapCreateCacheOption::WICBitmapCacheOnLoad,
				&pBitmap
			);

			pDecoder->Release();
			pFrame->Release();
			pConverter->Release();

			if (!SUCCEEDED(res))return false;
			return true;
		}
		//���ڴ����ͼ��(�ļ���)
		bool load_from_mem(const BYTE buf[], DWORD size)
		{
			if (pBitmap != nullptr)pBitmap->Release();

			HRESULT res;

			//������
			IWICStream* pStream;
			res = pFactory->CreateStream(&pStream);
			if (!SUCCEEDED(res))return false;

			//���ڴ��ʼ����
			res = pStream->InitializeFromMemory(
				const_cast<BYTE*>(buf),
				size
			);
			if (!SUCCEEDED(res))
			{
				pStream->Release();
				return false;
			}

			//����������
			IWICBitmapDecoder* pDecoder;
			res = pFactory->CreateDecoderFromStream(
				pStream,
				NULL,
				WICDecodeOptions::WICDecodeMetadataCacheOnLoad,
				&pDecoder
			);
			if (!SUCCEEDED(res))
			{
				pStream->Release();
				return false;
			}

			//����֡
			IWICBitmapFrameDecode* pFrame;
			res = pDecoder->GetFrame(0, &pFrame);
			if (!SUCCEEDED(res))
			{
				pStream->Release();
				pDecoder->Release();
				return false;
			}

			//ת�����ظ�ʽ
			IWICFormatConverter* pConverter;
			res = pFactory->CreateFormatConverter(&pConverter);
			if (!SUCCEEDED(res))
			{
				pStream->Release();
				pDecoder->Release();
				pFrame->Release();
				return false;
			}
			res = pConverter->Initialize(
				pFrame,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherType::WICBitmapDitherTypeNone,
				NULL,
				0,
				WICBitmapPaletteType::WICBitmapPaletteTypeMedianCut
			);
			if (!SUCCEEDED(res))
			{
				pStream->Release();
				pDecoder->Release();
				pFrame->Release();
				return false;
			}

			//����λͼ
			res = pFactory->CreateBitmapFromSource(
				pConverter,
				WICBitmapCreateCacheOption::WICBitmapCacheOnLoad,
				&pBitmap
			);

			pStream->Release();
			pDecoder->Release();
			pFrame->Release();
			pConverter->Release();

			if (!SUCCEEDED(res))return false;
			return true;
		}
		//���ڴ����ͼ��
		//[x][y]->[x*height+y]
		bool load_from_mem(const COLORREF buf[], UINT width, UINT height)
		{
			if (pBitmap != nullptr)pBitmap->Release();

			HRESULT res;

			//�ֶ�ת����
			BYTE* temp = new BYTE[width * height * 4];
			memset(temp, 0, width * height * 4);
			for (int x = 0; x < width; x++)
			{
				for (int y = 0; y < height; y++)
				{
					temp[(y * width + x) * 4 + 0] = GetRValue(buf[x * height + y]);
					temp[(y * width + x) * 4 + 1] = GetGValue(buf[x * height + y]);
					temp[(y * width + x) * 4 + 2] = GetBValue(buf[x * height + y]);
				}
			}

			//����λͼ
			res = pFactory->CreateBitmapFromMemory(
				width,
				height,
				GUID_WICPixelFormat32bppRGB,
				width * 4 * sizeof(BYTE),
				width * height * 4 * sizeof(BYTE),
				temp,
				&pBitmap
			);

			delete[] temp;
			if (!SUCCEEDED(res))return false;
			return true;
		}
		//����Դ����ͼ��
		bool load_from_res(HINSTANCE hInstance, const wchar_t rcName[], const wchar_t rcType[])
		{
			if (pBitmap != nullptr)pBitmap->Release();

			HRESULT res;

			//��λ��Դ
			HRSRC hRes = FindResource(hInstance, rcName, rcType);
			res = hRes ? S_OK : E_FAIL;
			if (!SUCCEEDED(res))return false;

			//������Դ
			HGLOBAL hData = LoadResource(hInstance, hRes);
			res = hData ? S_OK : E_FAIL;
			if (!SUCCEEDED(res))return false;

			//������Դ
			void* pFile = LockResource(hData);
			res = pFile ? S_OK : E_FAIL;
			if (!SUCCEEDED(res))return false;

			//����ͼ���С
			DWORD resSize;
			resSize = SizeofResource(hInstance, hRes);
			res = resSize ? S_OK : E_FAIL;
			if (!SUCCEEDED(res))return false;

			//����
			if (!load_from_mem((BYTE*)pFile, resSize))
			{
				return false;
			}

			return true;
		}
		bool load_from_res(HINSTANCE hInstance, WORD index, const wchar_t rcType[])
		{
			return load_from_res(hInstance, MAKEINTRESOURCE(index), rcType);
		}
		//��OpenCV Mat����ͼ��
#ifdef OPENCV_CORE_MAT_HPP
		bool load_from_mat(const cv::Mat& mat)
		{
			bool res;

			//������ͼ��
			res = init(mat.cols, mat.rows);
			if (!res)return false;

			//��ȡLock
			WICRect rect = { 0,0,mat.cols, mat.rows };
			IWICBitmapLock* pLock;
			res = pBitmap->Lock(&rect, WICBitmapLockFlags::WICBitmapLockWrite, &pLock);
			if (!SUCCEEDED(res))return false;

			//��ȡָ��
			UINT size;
			WICInProcPointer data;
			res = pLock->GetDataPointer(&size, &data);
			if (!SUCCEEDED(res))
			{
				pLock->Release();
				return false;
			}

			//��Mat��ȡ
			cv::Mat_<cv::Vec3b> mat_ = mat;
			for (size_t x = 0; x < mat.cols; x++)
			{
				for (size_t y = 0; y < mat.rows; y++)
				{
					//input_setting(x, y, RGB(mat_(y, x)[0], mat_(y, x)[1], mat_(y, x)[2], ));
					//д��
					data[y * mat.cols * 4 + x * 4 + 2] = mat_(y, x)[0];
					data[y * mat.cols * 4 + x * 4 + 1] = mat_(y, x)[1];
					data[y * mat.cols * 4 + x * 4 + 0] = mat_(y, x)[2];
				}
			}

			pLock->Release();

			return true;
		}
#endif

		//����ͼ���ļ�
		//֧��: jpg, jpeg, tif, tiff, png, bmp, gif
		bool save_to_file(const wchar_t filename[]) const
		{
			if (pBitmap == nullptr)return false;

			HRESULT res;

			//������
			IWICStream* pStream;
			res = pFactory->CreateStream(&pStream);
			if (!SUCCEEDED(res))return false;

			//��ʼ����
			res = pStream->InitializeFromFilename(filename, GENERIC_WRITE);
			if (!SUCCEEDED(res))
			{
				pStream->Release();
				return false;
			}

			//����������
			IWICBitmapEncoder* pEncoder;

			//������һ�±����ͼƬ�ļ�����
			if (wcsstr(filename, L".jpg") != NULL ||
				wcsstr(filename, L".jpeg") != NULL)		res = pFactory->CreateEncoder(GUID_ContainerFormatJpeg, NULL, &pEncoder);
			else if (wcsstr(filename, L".tif") != NULL ||
				wcsstr(filename, L".tiff") != NULL)		res = pFactory->CreateEncoder(GUID_ContainerFormatTiff, NULL, &pEncoder);
			else if (wcsstr(filename, L".png") != NULL)	res = pFactory->CreateEncoder(GUID_ContainerFormatPng, NULL, &pEncoder);
			else if (wcsstr(filename, L".bmp") != NULL)	res = pFactory->CreateEncoder(GUID_ContainerFormatBmp, NULL, &pEncoder);
			else if (wcsstr(filename, L".gif") != NULL)	res = pFactory->CreateEncoder(GUID_ContainerFormatGif, NULL, &pEncoder);
			else
			{
				pStream->Release();
				return false;
			}

			if (!SUCCEEDED(res))
			{
				pStream->Release();
				return false;
			}

			//��ʼ��������
			res = pEncoder->Initialize(
				pStream,
				WICBitmapEncoderCacheOption::WICBitmapEncoderNoCache
			);
			if (!SUCCEEDED(res))
			{
				pStream->Release();
				pEncoder->Release();
				return false;
			}

			//����֡
			IWICBitmapFrameEncode* pFrame;
			res = pEncoder->CreateNewFrame(&pFrame, NULL);
			if (!SUCCEEDED(res))
			{
				pStream->Release();
				pEncoder->Release();
				return false;
			}

			//��ʼ��֡
			pFrame->Initialize(NULL);
			if (!SUCCEEDED(res))
			{
				pStream->Release();
				pEncoder->Release();
				pFrame->Release();
				return false;
			}

			//д��֡
			UINT width, height;
			pBitmap->GetSize(&width, &height);
			WICRect rect = { 0,0,width,height };
			res = pFrame->WriteSource(pBitmap, &rect);
			if (!SUCCEEDED(res))
			{
				pStream->Release();
				pEncoder->Release();
				pFrame->Release();
				return false;
			}

			//�ύ֡(��������)
			res = pFrame->Commit();
			if (!SUCCEEDED(res))
			{
				pStream->Release();
				pEncoder->Release();
				pFrame->Release();
				return false;
			}

			//�ύ������(����)
			res = pEncoder->Commit();

			pStream->Release();
			pEncoder->Release();
			pFrame->Release();

			if (!SUCCEEDED(res))return false;
			return true;
		}

		//תΪvector(�Ҷ�)
		std::vector<int> to_vec()
		{
			std::vector<float> res(get_height() * get_width());
			COLORREF color;
			int r, g, b;
			for (size_t x = 0; x < get_width(); x++)
			{
				for (size_t y = 0; y < get_height(); y++)
				{
					color = get(x, y);
					SplitRgb(color, &r, &g, &b);
					res[x * get_height() + y] = round((r + g + b) / 3.0);
				}
			}
		}
		//one: �Ƿ��һ��
		std::vector<float> to_vec_f(bool one = false)
		{
			std::vector<float> res(get_height() * get_width());
			COLORREF color;
			float r, g, b;
			for (size_t x = 0; x < get_width(); x++)
			{
				for (size_t y = 0; y < get_height(); y++)
				{
					color = get(x, y);
					SplitRgb(color, &r, &g, &b);
					res[x * get_height() + y] = (r + g + b) / 3;
					if (one)res[x * get_height() + y] /= 255;
				}
			}
			return res;
		}
	};
}