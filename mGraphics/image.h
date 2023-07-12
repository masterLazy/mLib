#pragma once
/*****************************************************************************
* image.h
* 图像
*
* 基于WIC
*****************************************************************************/

#include <wincodec.h>
#include "../mFunction.h"

namespace mlib
{
	class Image
	{
	private:
		IWICImagingFactory2* pFactory;	//WIC的接口
	public:
		IWICBitmap* pBitmap;

		Image()
		{
			pFactory = nullptr;
			pBitmap = nullptr;
			//初始化
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

		//创建空白图像
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

		//获取图像宽度
		UINT get_width() const
		{
			if (pBitmap == nullptr)return 0;

			UINT width, height;
			pBitmap->GetSize(&width, &height);
			return width;
		}
		//获取图像高度
		UINT get_height() const
		{
			if (pBitmap == nullptr)return 0;

			UINT width, height;
			pBitmap->GetSize(&width, &height);
			return height;
		}

		//获取像素
		COLORREF get(UINT x, UINT y)
		{
			if (pBitmap == nullptr)return false;

			HRESULT res;

			//获取Lock
			WICRect rect = { x,y,1,1 };
			IWICBitmapLock* pLock;
			res = pBitmap->Lock(&rect, WICBitmapLockFlags::WICBitmapLockRead, &pLock);
			if (!SUCCEEDED(res))return 0;

			//获取指针
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
		//设置像素
		bool set(UINT x, UINT y, COLORREF color)
		{
			if (pBitmap == nullptr)return false;

			HRESULT res;

			//获取Lock
			WICRect rect = { x,y,1,1 };
			IWICBitmapLock* pLock;
			res = pBitmap->Lock(&rect, WICBitmapLockFlags::WICBitmapLockWrite, &pLock);
			if (!SUCCEEDED(res))return false;

			//获取指针
			UINT size;
			WICInProcPointer data;
			res = pLock->GetDataPointer(&size, &data);
			if (!SUCCEEDED(res))
			{
				pLock->Release();
				return false;
			}

			//写入
			data[2] = GetRValue(color);
			data[1] = GetGValue(color);
			data[0] = GetBValue(color);

			pLock->Release();
			return true;
		}

		//从文件加载图像
		bool load_from_file(const wchar_t filename[])
		{
			if (pBitmap != nullptr)pBitmap->Release();

			HRESULT res;

			//创建解码器
			IWICBitmapDecoder* pDecoder;
			res = pFactory->CreateDecoderFromFilename(
				filename,
				NULL,
				GENERIC_READ,
				WICDecodeOptions::WICDecodeMetadataCacheOnLoad,
				&pDecoder
			);
			if (!SUCCEEDED(res))return false;

			//检索帧
			IWICBitmapFrameDecode* pFrame;
			res = pDecoder->GetFrame(0, &pFrame);
			if (!SUCCEEDED(res))
			{
				pDecoder->Release();
				return false;
			}

			//转换像素格式
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

			//创建位图
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
		//从内存加载图像(文件流)
		bool load_from_mem(const BYTE buf[], DWORD size)
		{
			if (pBitmap != nullptr)pBitmap->Release();

			HRESULT res;

			//创建流
			IWICStream* pStream;
			res = pFactory->CreateStream(&pStream);
			if (!SUCCEEDED(res))return false;

			//从内存初始化流
			res = pStream->InitializeFromMemory(
				const_cast<BYTE*>(buf),
				size
			);
			if (!SUCCEEDED(res))
			{
				pStream->Release();
				return false;
			}

			//创建解码器
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

			//检索帧
			IWICBitmapFrameDecode* pFrame;
			res = pDecoder->GetFrame(0, &pFrame);
			if (!SUCCEEDED(res))
			{
				pStream->Release();
				pDecoder->Release();
				return false;
			}

			//转换像素格式
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

			//创建位图
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
		//从内存加载图像
		//[x][y]->[x*height+y]
		bool load_from_mem(const COLORREF buf[], UINT width, UINT height)
		{
			if (pBitmap != nullptr)pBitmap->Release();

			HRESULT res;

			//手动转换吧
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

			//创建位图
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
		//从资源加载图像
		bool load_from_res(HINSTANCE hInstance, const wchar_t rcName[], const wchar_t rcType[])
		{
			if (pBitmap != nullptr)pBitmap->Release();

			HRESULT res;

			//定位资源
			HRSRC hRes = FindResource(hInstance, rcName, rcType);
			res = hRes ? S_OK : E_FAIL;
			if (!SUCCEEDED(res))return false;

			//加载资源
			HGLOBAL hData = LoadResource(hInstance, hRes);
			res = hData ? S_OK : E_FAIL;
			if (!SUCCEEDED(res))return false;

			//锁定资源
			void* pFile = LockResource(hData);
			res = pFile ? S_OK : E_FAIL;
			if (!SUCCEEDED(res))return false;

			//计算图像大小
			DWORD resSize;
			resSize = SizeofResource(hInstance, hRes);
			res = resSize ? S_OK : E_FAIL;
			if (!SUCCEEDED(res))return false;

			//加载
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
		//从OpenCV Mat加载图像
#ifdef OPENCV_CORE_MAT_HPP
		bool load_from_mat(const cv::Mat& mat)
		{
			bool res;

			//创建空图像
			res = init(mat.cols, mat.rows);
			if (!res)return false;

			//获取Lock
			WICRect rect = { 0,0,mat.cols, mat.rows };
			IWICBitmapLock* pLock;
			res = pBitmap->Lock(&rect, WICBitmapLockFlags::WICBitmapLockWrite, &pLock);
			if (!SUCCEEDED(res))return false;

			//获取指针
			UINT size;
			WICInProcPointer data;
			res = pLock->GetDataPointer(&size, &data);
			if (!SUCCEEDED(res))
			{
				pLock->Release();
				return false;
			}

			//从Mat读取
			cv::Mat_<cv::Vec3b> mat_ = mat;
			for (size_t x = 0; x < mat.cols; x++)
			{
				for (size_t y = 0; y < mat.rows; y++)
				{
					//input_setting(x, y, RGB(mat_(y, x)[0], mat_(y, x)[1], mat_(y, x)[2], ));
					//写入
					data[y * mat.cols * 4 + x * 4 + 2] = mat_(y, x)[0];
					data[y * mat.cols * 4 + x * 4 + 1] = mat_(y, x)[1];
					data[y * mat.cols * 4 + x * 4 + 0] = mat_(y, x)[2];
				}
			}

			pLock->Release();

			return true;
		}
#endif

		//保存图像到文件
		//支持: jpg, jpeg, tif, tiff, png, bmp, gif
		bool save_to_file(const wchar_t filename[]) const
		{
			if (pBitmap == nullptr)return false;

			HRESULT res;

			//创建流
			IWICStream* pStream;
			res = pFactory->CreateStream(&pStream);
			if (!SUCCEEDED(res))return false;

			//初始化流
			res = pStream->InitializeFromFilename(filename, GENERIC_WRITE);
			if (!SUCCEEDED(res))
			{
				pStream->Release();
				return false;
			}

			//创建编码器
			IWICBitmapEncoder* pEncoder;

			//这里检测一下保存的图片文件类型
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

			//初始化编码器
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

			//创建帧
			IWICBitmapFrameEncode* pFrame;
			res = pEncoder->CreateNewFrame(&pFrame, NULL);
			if (!SUCCEEDED(res))
			{
				pStream->Release();
				pEncoder->Release();
				return false;
			}

			//初始化帧
			pFrame->Initialize(NULL);
			if (!SUCCEEDED(res))
			{
				pStream->Release();
				pEncoder->Release();
				pFrame->Release();
				return false;
			}

			//写入帧
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

			//提交帧(到编码器)
			res = pFrame->Commit();
			if (!SUCCEEDED(res))
			{
				pStream->Release();
				pEncoder->Release();
				pFrame->Release();
				return false;
			}

			//提交编码器(到流)
			res = pEncoder->Commit();

			pStream->Release();
			pEncoder->Release();
			pFrame->Release();

			if (!SUCCEEDED(res))return false;
			return true;
		}

		//转为vector(灰度)
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
		//one: 是否归一化
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