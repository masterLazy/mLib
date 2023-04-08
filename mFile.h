#pragma once
/*****************************************************************************
* mCode.h
* 文件操作
*
* 基于 stdio.h、WindowsAPI
 ****************************************************************************/

#include <Windows.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "mFunction.h"
#include "mCode.h"

#undef min
#undef max

namespace mlib
{
	//mode
#define READ	1
#define WRITE	2

//code
#define ANSI		0
#define UTF16_LE	1
#define UTF8		2

	class File
	{
	private:
		std::wstring dir;
		std::wstring name;

	public:
		FILE* file;
		int code;

		File()
		{
			code = ANSI;
			file = NULL;
		}
		~File()
		{
			if (file != NULL)fclose(file);
		}

		//设置根目录
		void set_dir(const std::wstring& _dir = L"")
		{
			dir = _dir;
		}
		void set_dir(const std::string& _dir = "")
		{
			dir = StrToWstr(_dir);
		}


		//设置文件保存编码方式(写入时, 在打开文件前调用)
		void set_code(int _code)
		{
			code = _code;
		}
		//获取文件保存编码方式(读取时，在打开文件后调用)
		int get_code() const
		{
			return code;
		}

		//打开或创建文件
		/*mode
		* r		打开以便读取。如果文件不存在或无法找到fopen调用失败。
		* w		打开用于写入的空文件。如果给定文件存在，则其内容会被销毁。
		* a		在文件末尾打开以进行写入(追加)，在新数据写入到文件之前不移除文件末尾(EOF)标记。 创建文件(如果文件不存在)。
		* r+	打开以便读取和写入。文件必须存在。
		* w+	打开用于读取和写入的空文件。如果文件存在，则其内容会被销毁。
		* a+	打开以进行读取和追加。追加操作包括在新数据写入文件之前移除 EOF 标记。写入完成后，EOF标记不会还原。创建文件(如果文件不存在)。
		*/
		bool open(const std::wstring& filename, const std::wstring& mode)
		{
			if (file != NULL)return false;

			_wfopen_s(&file, filename.c_str(), mode.c_str());

			return file != NULL;
		}
		bool open(const std::string& filename, const std::string& mode)
		{
			if (file != NULL)return false;

			fopen_s(&file, filename.c_str(), mode.c_str());

			return file != NULL;
		}

		//关闭文件
		bool close()
		{
			if (file == NULL)return false;

			bool res = fclose(file);
			file = NULL;
			return res;
		}

		//设置文件读取“光标”位置
		//from: SEEK_...
		bool set_cur(long distance, int from) const
		{
			if (file == NULL)return false;

			return fseek(file, distance, from);
		}
		//获取文件读取“光标”位置
		long get_cur() const
		{
			return ftell(file);
		}

		//获取文件大小
		long get_size()
		{
			if (file == NULL)return 0;

			long cur = ftell(file);
			fseek(file, 0, SEEK_END);
			long size = ftell(file);
			fseek(file, cur, SEEK_SET);
			return size;
		}

		//读取文件
		size_t read(void* buf, size_t bufSize, size_t elementSize, size_t elementCount)
		{
			memset(buf, 0, bufSize);
			return fread_s(buf, bufSize, elementSize, elementCount, file);
		}

		//写入文件
		size_t write(void* buf, size_t elementSize, size_t elementCount)
		{
			return fwrite(buf, elementSize, elementCount, file);
		}
		size_t write(std::string& buf)
		{
			return fwrite(buf.c_str(), sizeof(char), buf.size(), file);
		}
		size_t write(std::wstring& buf)
		{
			return fwrite(buf.c_str(), sizeof(wchar_t), buf.size(), file);
		}

		//删除文件(且先关闭)
		void delete_self()
		{
			close();
			DeleteFile((dir + name).c_str());
		}


		//静态函数

		//创建文件夹
		static bool create_dir(std::wstring dir)
		{
			return CreateDirectory(dir.c_str(), NULL);
		}
		static bool create_dir(std::string dir)
		{
			return CreateDirectoryA(dir.c_str(), NULL);
		}

		//删除文件/文件夹
		static bool delete_file(std::wstring file)
		{
			return DeleteFile(file.c_str());
		}
		static bool delete_file(std::string file)
		{
			return DeleteFileA(file.c_str());
		}

		//获取程序所在的文件夹(绝对; 不包括文件名)
		static std::wstring get_dir()
		{
			std::wstring path;
			wchar_t szPath[MAX_PATH] = { L'\0' };
			GetModuleFileName(NULL, szPath, MAX_PATH);
			path = szPath;
			path = path.substr(0, path.find_last_of(L"\\") + 1);
			return path;
		}
		static std::string get_dirA()
		{
			std::string path;
			char szPath[MAX_PATH] = { '\0' };
			GetModuleFileNameA(NULL, szPath, MAX_PATH);
			path = szPath;
			path = path.substr(0, path.find_last_of("\\") + 1);
			return path;
		}

		//寻找目录下的指定文件/文件夹(可以模糊搜索; 都可以为NULL)
		static void find_file(std::vector<std::wstring>* files, std::vector<std::wstring>* dirs, std::wstring name, std::wstring path = L"\\")
		{
			if (files == NULL && dirs == NULL)return;
			if (path == L"\\")path = get_dir();

			WIN32_FIND_DATA data;//文件信息

			//查找第一次
			HANDLE hFindFile = FindFirstFile((path + name).c_str(), &data);
			if (hFindFile == INVALID_HANDLE_VALUE)return;

			bool res;
			do
			{
				//是文件夹
				if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (dirs != NULL)
					{
						dirs->push_back(data.cFileName);
						if (dirs->back() == L"." || dirs->back() == L"..")dirs->pop_back();
					}
				}
				//是文件
				else
				{
					if (files != NULL)files->push_back(data.cFileName);
				}
				//再找
				res = FindNextFile(hFindFile, &data);
			} while (res);
			FindClose(hFindFile);
		}
		static void find_file(std::vector<std::string>* files, std::vector<std::string>* dirs, std::string name, std::string path = "\\")
		{
			if (files == NULL && dirs == NULL)return;
			if (path == "\\")path = get_dirA();

			WIN32_FIND_DATAA data;//文件信息

			//查找第一次
			HANDLE hFindFile = FindFirstFileA((path + name).c_str(), &data);
			if (hFindFile == INVALID_HANDLE_VALUE)return;

			bool res;
			do
			{
				//是文件夹
				if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (dirs != NULL)
					{
						dirs->push_back(data.cFileName);
						if (dirs->back() == "." || dirs->back() == "..")dirs->pop_back();
					}
				}
				//是文件
				else
				{
					if (files != NULL)files->push_back(data.cFileName);
				}
				//再找
				res = FindNextFileA(hFindFile, &data);
			} while (res);
			FindClose(hFindFile);
		}

		//寻找目录下的所有文件/文件夹(都可以为NULL)
		static void find_all_file(std::vector<std::wstring>* files, std::vector<std::wstring>* dirs, std::wstring path = L"\\")
		{
			find_file(files, dirs, L"*", path);
		}
		static void find_all_file(std::vector<std::string>* files, std::vector<std::string>* dirs, std::string path = "\\")
		{
			find_file(files, dirs, "*", path);
		}

		//获取文件信息
		static WIN32_FIND_DATA get_data(std::wstring name)
		{
			if (name == L"\\")name = get_dir();

			WIN32_FIND_DATA data;
			HANDLE hFindFile = FindFirstFile(name.c_str(), &data);
			FindClose(hFindFile);
			return data;
		}
		static WIN32_FIND_DATAA get_data(std::string name)
		{
			if (name == "\\")name = get_dirA();

			WIN32_FIND_DATAA data;
			HANDLE hFindFile = FindFirstFileA(name.c_str(), &data);
			FindClose(hFindFile);
			return data;
		}
	};
}