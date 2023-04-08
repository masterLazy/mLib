#pragma once
/*****************************************************************************
* mCode.h
* �ļ�����
*
* ���� stdio.h��WindowsAPI
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

		//���ø�Ŀ¼
		void set_dir(const std::wstring& _dir = L"")
		{
			dir = _dir;
		}
		void set_dir(const std::string& _dir = "")
		{
			dir = StrToWstr(_dir);
		}


		//�����ļ�������뷽ʽ(д��ʱ, �ڴ��ļ�ǰ����)
		void set_code(int _code)
		{
			code = _code;
		}
		//��ȡ�ļ�������뷽ʽ(��ȡʱ���ڴ��ļ������)
		int get_code() const
		{
			return code;
		}

		//�򿪻򴴽��ļ�
		/*mode
		* r		���Ա��ȡ������ļ������ڻ��޷��ҵ�fopen����ʧ�ܡ�
		* w		������д��Ŀ��ļ�����������ļ����ڣ��������ݻᱻ���١�
		* a		���ļ�ĩβ���Խ���д��(׷��)����������д�뵽�ļ�֮ǰ���Ƴ��ļ�ĩβ(EOF)��ǡ� �����ļ�(����ļ�������)��
		* r+	���Ա��ȡ��д�롣�ļ�������ڡ�
		* w+	�����ڶ�ȡ��д��Ŀ��ļ�������ļ����ڣ��������ݻᱻ���١�
		* a+	���Խ��ж�ȡ��׷�ӡ�׷�Ӳ���������������д���ļ�֮ǰ�Ƴ� EOF ��ǡ�д����ɺ�EOF��ǲ��ỹԭ�������ļ�(����ļ�������)��
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

		//�ر��ļ�
		bool close()
		{
			if (file == NULL)return false;

			bool res = fclose(file);
			file = NULL;
			return res;
		}

		//�����ļ���ȡ����ꡱλ��
		//from: SEEK_...
		bool set_cur(long distance, int from) const
		{
			if (file == NULL)return false;

			return fseek(file, distance, from);
		}
		//��ȡ�ļ���ȡ����ꡱλ��
		long get_cur() const
		{
			return ftell(file);
		}

		//��ȡ�ļ���С
		long get_size()
		{
			if (file == NULL)return 0;

			long cur = ftell(file);
			fseek(file, 0, SEEK_END);
			long size = ftell(file);
			fseek(file, cur, SEEK_SET);
			return size;
		}

		//��ȡ�ļ�
		size_t read(void* buf, size_t bufSize, size_t elementSize, size_t elementCount)
		{
			memset(buf, 0, bufSize);
			return fread_s(buf, bufSize, elementSize, elementCount, file);
		}

		//д���ļ�
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

		//ɾ���ļ�(���ȹر�)
		void delete_self()
		{
			close();
			DeleteFile((dir + name).c_str());
		}


		//��̬����

		//�����ļ���
		static bool create_dir(std::wstring dir)
		{
			return CreateDirectory(dir.c_str(), NULL);
		}
		static bool create_dir(std::string dir)
		{
			return CreateDirectoryA(dir.c_str(), NULL);
		}

		//ɾ���ļ�/�ļ���
		static bool delete_file(std::wstring file)
		{
			return DeleteFile(file.c_str());
		}
		static bool delete_file(std::string file)
		{
			return DeleteFileA(file.c_str());
		}

		//��ȡ�������ڵ��ļ���(����; �������ļ���)
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

		//Ѱ��Ŀ¼�µ�ָ���ļ�/�ļ���(����ģ������; ������ΪNULL)
		static void find_file(std::vector<std::wstring>* files, std::vector<std::wstring>* dirs, std::wstring name, std::wstring path = L"\\")
		{
			if (files == NULL && dirs == NULL)return;
			if (path == L"\\")path = get_dir();

			WIN32_FIND_DATA data;//�ļ���Ϣ

			//���ҵ�һ��
			HANDLE hFindFile = FindFirstFile((path + name).c_str(), &data);
			if (hFindFile == INVALID_HANDLE_VALUE)return;

			bool res;
			do
			{
				//���ļ���
				if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (dirs != NULL)
					{
						dirs->push_back(data.cFileName);
						if (dirs->back() == L"." || dirs->back() == L"..")dirs->pop_back();
					}
				}
				//���ļ�
				else
				{
					if (files != NULL)files->push_back(data.cFileName);
				}
				//����
				res = FindNextFile(hFindFile, &data);
			} while (res);
			FindClose(hFindFile);
		}
		static void find_file(std::vector<std::string>* files, std::vector<std::string>* dirs, std::string name, std::string path = "\\")
		{
			if (files == NULL && dirs == NULL)return;
			if (path == "\\")path = get_dirA();

			WIN32_FIND_DATAA data;//�ļ���Ϣ

			//���ҵ�һ��
			HANDLE hFindFile = FindFirstFileA((path + name).c_str(), &data);
			if (hFindFile == INVALID_HANDLE_VALUE)return;

			bool res;
			do
			{
				//���ļ���
				if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (dirs != NULL)
					{
						dirs->push_back(data.cFileName);
						if (dirs->back() == "." || dirs->back() == "..")dirs->pop_back();
					}
				}
				//���ļ�
				else
				{
					if (files != NULL)files->push_back(data.cFileName);
				}
				//����
				res = FindNextFileA(hFindFile, &data);
			} while (res);
			FindClose(hFindFile);
		}

		//Ѱ��Ŀ¼�µ������ļ�/�ļ���(������ΪNULL)
		static void find_all_file(std::vector<std::wstring>* files, std::vector<std::wstring>* dirs, std::wstring path = L"\\")
		{
			find_file(files, dirs, L"*", path);
		}
		static void find_all_file(std::vector<std::string>* files, std::vector<std::string>* dirs, std::string path = "\\")
		{
			find_file(files, dirs, "*", path);
		}

		//��ȡ�ļ���Ϣ
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