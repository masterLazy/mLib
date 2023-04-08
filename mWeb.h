#pragma once
/*****************************************************************************
* mWeb.h
* 网络
*
* 基于OpenSSL、WinSock
*****************************************************************************/

#define _WINSOCK_DEPRECATED_NO_WARNINGS
//已关闭SDL检查，因为openssl\applink.c
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <vector>
#include <ctime>
#include <iostream>
#include <openssl\ssl.h>
#include <openssl\err.h>
//#include <openssl\applink.c>
#include "mFunction.h"
#include "mCode.h"

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"libssl.lib")
#pragma comment(lib,"libcrypto.lib")

using namespace std;

namespace mlib
{
#define CLIENT 1
#define SERVER 2

	/*****************************************************************************
	* HttpMsg
	* HTTP报文
	*
	*****************************************************************************/

	//从资源名获取数据类型
	string GetResType(string resourceName)
	{
		if (resourceName.find(".") == string::npos)
		{
			return "text/html";
		}
		else
		{
			string suf = resourceName.substr(resourceName.find("."));
			//text
			if (suf == ".html" || suf == ".htm")return "text/html";
			else if (suf == ".txt")return "text/plain";
			else if (suf == ".css")return "text/css";
			else if (suf == ".js")return "text/javascript";
			else if (suf == ".json")return "application/json";
			//image
			else if (suf == ".jpg" || suf == ".jpeg")return "image/jpeg";
			else if (suf == ".png")return "image/png";
			else if (suf == ".gif")return "image/gif";
			else if (suf == ".ico")return "image/x-ico";
			else if (suf == ".webp")return "image/webp";
			//audio
			else if (suf == ".mp3")return "audio/mp3";
			else if (suf == ".wav")return "audio/wav";
			//video
			else if (suf == ".mp4")return "video/mpeg4";
			else if (suf == ".avi")return "video/avi";
			//压缩包
			else if (suf == ".zip")return "application/x-zip-compressed";
			else if (suf == ".rar")return "application/octet-stream";
			else if (suf == ".7z")return "application/x-7z-compressed";
			else return "application/octet-stream";
		}
	}

	class HttpMsg
	{
	public:
		string fline;							//请求行/状态行
		vector<pair<string, string>> header;	//头部
		vector<pair<string, string>> par;		//参数
		string data;							//数据
		int dataMode = 0;						//0: 正常; 1: 记录文件名

		//清空
		void clear()
		{
			fline.clear();
			header.clear();
			data.clear();
			dataMode = 0;
		}


		//解析报文
		void split(const char _msg[], int len)
		{
			clear();
			string msg(_msg, len);

			//请求行/状态行
			if (msg.find("\r\n") == string::npos)return;
			fline = msg.substr(0, msg.find("\r\n"));

			//数据
			if (msg.find("\r\n\r\n") == string::npos)return;
			data = msg.substr(msg.find("\r\n\r\n") + 4);
			if (data.find("\r\n\r\n") != string::npos)
			{
				data.erase(data.find("\r\n\r\n"), 4);
			}

			//头部
			int i = msg.find("\r\n") + 2;
			header.clear();
			while (true)
			{
				//分解一行
				header.push_back({
					msg.substr(i, msg.find(": ", i) - i),
					msg.substr(msg.find(": ", i) + 2,
					(msg.find("; ", i) == string::npos ? msg.find("\r\n",i) : min(msg.find("\r\n",i),msg.find("; ",i)))
					- (msg.find(": ", i) + 2))
					});

				if (msg.find("\r\n\r\n") == msg.find("\r\n", i))break;
				i = msg.find("\r\n", i) + 2;
			}

			//参数
			string pars;
			//GET方法
			if (fline.find("GET") != string::npos)
			{
				if (fline.find("?") != string::npos)
				{
					pars = fline.substr(fline.find("?") + 1, fline.find(" HTTP") - fline.find("?") - 1);
				}
			}
			//POST方法
			else if (fline.find("POST") != string::npos)
			{
				pars = data;
			}
			if (!pars.empty())
			{
				i = 0;
				while (pars.find("&", i) != string::npos)
				{
					par.push_back({
						pars.substr(i, pars.find("=", i) - i),
						Uri_decode(pars.substr(pars.find("=", i) + 1, pars.find("&", i) - pars.find("=", i) - 1)),
						});
					i = pars.find("&", i) + 1;
				}
				par.push_back({
						pars.substr(i, pars.find("=", i) - i),
						Uri_decode(pars.substr(pars.find("=", i) + 1)),
					});
			}
		}
		void split(string msg)
		{
			clear();
			//请求行/状态行
			if (msg.find("\r\n") == string::npos)return;
			fline = msg.substr(0, msg.find("\r\n"));

			//数据
			if (msg.find("\r\n\r\n") == string::npos)return;
			data = msg.substr(msg.find("\r\n\r\n") + 4);
			if (data.find("\r\n\r\n") != string::npos)
			{
				data.erase(data.find("\r\n\r\n"), 4);
			}

			//头部
			int i = msg.find("\r\n") + 2;
			header.clear();
			while (true)
			{
				//分解一行
				header.push_back({
					msg.substr(i, msg.find(": ", i) - i),
					msg.substr(msg.find(": ", i) + 2,
					(msg.find("; ", i) == string::npos ? msg.find("\r\n",i) : min(msg.find("\r\n",i),msg.find("; ",i)))
					- (msg.find(": ", i) + 2))
					});

				if (msg.find("\r\n\r\n") == msg.find("\r\n", i))break;
				i = msg.find("\r\n", i) + 2;
			}

			//参数
			string pars;
			//GET方法
			if (fline.find("GET") != string::npos)
			{
				if (fline.find("?") != string::npos)
				{
					pars = fline.substr(fline.find("?") + 1, fline.find(" HTTP") - fline.find("?") - 1);
				}
			}
			//POST方法
			else if (fline.find("POST") != string::npos)
			{
				pars = data;
			}
			if (!pars.empty())
			{
				i = 0;
				while (pars.find("&", i) != string::npos)
				{
					par.push_back({
						pars.substr(i, pars.find("=", i) - i),
						Uri_decode(pars.substr(pars.find("=", i) + 1, pars.find("&", i) - pars.find("=", i) - 1)),
						});
					i = pars.find("&", i) + 1;
				}
				par.push_back({
						pars.substr(i, pars.find("=", i) - i),
						Uri_decode(pars.substr(pars.find("=", i) + 1)),
					});
			}
		}

		//组合报文
		string make() const
		{
			string temp;
			temp += fline;
			temp += "\r\n";
			for (size_t i = 0; i < header.size(); i++)
			{
				temp += header[i].first;
				temp += ": ";
				temp += header[i].second;
				temp += "\r\n";
			}
			temp += "\r\n";
			if (!data.empty() && dataMode == 0)
			{
				temp += data;
				temp += "\r\n\r\n";
			}
			return temp;
		}


		//服务器端: 获取资源名
		string get_req()
		{
			if (fline.find(" ") == string::npos ||
				fline.find(" HTTP") == string::npos)
			{
				return "";
			}
			if (fline.find("?") == string::npos)
			{
				return fline.substr(fline.find(" ") + 1, fline.find(" HTTP") - (fline.find(" ") + 1));
			}
			else
			{
				return fline.substr(fline.find(" ") + 1, fline.find("?") - (fline.find(" ") + 1));
			}
		}
		//客户端: 设置请求行
		void set_req(string resourceName, string method = "GET")
		{
			fline = method;
			fline += " ";
			fline += resourceName;
			if (method == "GET" && !par.empty())
			{
				fline += "?";
				for (int i = 0; i < par.size(); i++)
				{
					if (i > 0)fline += "&";
					fline += par[i].first;
					fline += "=";
					fline += Uri_encode(par[i].second);
				}
			}
			fline += " HTTP/1.1";
		}

		//客户端: 获取状态码
		int get_state()
		{
			if (fline.find(" ") == string::npos)return 0;
			return atoi(fline.substr(fline.find(" ") + 1, 3).c_str());
		}
		//服务器端: 设置状态行
		void set_state(int state = 200)
		{
			fline.clear();
			fline += "HTTP/1.1 ";
			switch (state)
			{
				//信息
			case 100:fline += "100 Continue"; break;
				//成功
			case 200:fline += "200 OK"; break;
				//重定向
			case 301:fline += "301 Moved Permanently"; break;
			case 302:fline += "301 Found"; break;
			case 304:fline += "304 Not Modified"; break;
			case 307:fline += "307 Temporary Redirect"; break;
				//客户端错误
			case 400:fline += "400 Bad Request"; break;
			case 401:fline += "401 Unauthorized"; break;
			case 403:fline += "403 Forbidden"; break;
			case 404:fline += "404 Not Found"; break;
				//服务器错误
			case 500:fline += "500 Internal Server Error"; break;
			case 503:fline += "503 Service Unavailable"; break;
			default:fline += "500 Internal Server Error"; break;
			}
		}

		//服务器端: 获取请求方法
		string get_method()
		{
			if (fline.find(" ") == string::npos)return "";
			return fline.substr(0, fline.find(" "));
		}


		//获取头部中项的值
		string get_header(string item) const
		{
			for (size_t i = 0; i < header.size(); i++)
			{
				if (header[i].first == item)return header[i].second;
			}
			return "";
		}
		//设置头部中项的值(如没有则添加)
		void set_header(string item, string value)
		{
			for (size_t i = 0; i < header.size(); i++)
			{
				if (header[i].first == item)
				{
					header[i].second = value;
					return;
				}
			}
			header.push_back({ item,value });
		}
		//自动设置头部中项的值
		int auto_header(string item)
		{
			if (item == "Content-Length")
			{
				char temp[20] = { 0 };
				if (dataMode == 0)
				{
					_itoa_s(data.size(), temp, 20);
				}
				else
				{
					//获取文件大小
					FILE* f;
					fopen_s(&f, data.c_str(), "rb");
					if (f == NULL)return -1;

					fseek(f, 0, SEEK_END);
					size_t fileSize = ftell(f);
					fseek(f, 0, SEEK_SET);
					fclose(f);

					_ltoa_s(fileSize, temp, 20);
				}
				set_header("Content-Length", temp);
			}
			else if (item == "Date")
			{
				time_t t;
				time(&t);
				tm info;
				gmtime_s(&info, &t);

				//格式例: Sat, 02 Jul 2011 20:45:57 GMT
				char date[50];
				strftime(date, 50 * sizeof(char), "%a, %d %b %Y %H:%M:%S GMT", &info);

				set_header("Date", date);
			}
			return 0;
		}


		//客户端: 生成请求报文
		void make_req_msg(string hostName, string resourceName, string method = "GET")
		{
			clear();
			set_req(resourceName, method);
			set_header("connection", "close");
			set_header("Host", hostName);
			if (method == "POST" && !par.empty())
			{
				data.clear();
				for (int i = 0; i < par.size(); i++)
				{
					if (i > 0)data += "&";
					data += par[i].first;
					data += "=";
					data += Uri_encode(par[i].second);
				}
			}
		}

		//服务器端: 生成状态页
		void make_state_page()
		{
			data = "<!DOCTYPE HTML><html><head><title>";
			data += fline.substr(fline.find(" ") + 1);
			data += "</title></head><body><center><h1>";
			data += fline.substr(fline.find(" ") + 1);
			data += "</h1><hr /><p>mLazy</p></center></body></html>";
		}

		//设置数据为文件
		int load_data(const char filename[], const char root[] = "")
		{
			FILE* f;
			string full = root;
			full += filename;
			fopen_s(&f, full.c_str(), "rb");
			if (f == NULL)return -1;
			else
			{
				fclose(f);
				data = full;
				dataMode = 1;
				return 0;
			}
		}
	};

	/*****************************************************************************
	* Socket
	* 自动套接字
	*
	* 2023.1.29：SSL部分仅完成了客户端
	*****************************************************************************/

#define HTTP_PORT 80
#define HTTPS_PORT 443

#define _USING_SOCK (mode == 1 ? sock : cli_sock)
#define send_BUF_SIZE 1024
#define recv_BUF_SIZE 2048
	class Socket
	{
	private:
		WSADATA wd;
		SOCKET cli_sock;
		sockaddr_in addr;
		int mode = 0;//0: 未初始化
		SSL* ssl = nullptr;
		bool recved = false;//已接收到SSL缓冲区

	public:
		SOCKET sock;

		//关闭套接字
		void close()
		{
			closesocket(sock);
			WSACleanup();
			mode = 0;
		}

		//获取错误码
		static int get_error()
		{
			return WSAGetLastError();
		}

		//客户端: 初始化(返回错误码)
		int init(SSL* _ssl = nullptr)
		{
			ssl = _ssl;

			int res;
			//初始化网络库
			res = WSAStartup(MAKEWORD(2, 2), &wd);
			if (res != 0)return res;

			//创建套接字
			sock = socket(AF_INET, SOCK_STREAM, NULL);
			if (sock == -1)
			{
				res = WSAGetLastError();
				close();
				return res;
			}

			//设置为非阻塞套接字
			u_long ul = 1;
			ioctlsocket(sock, FIONBIO, &ul);

			mode = CLIENT;
			return 0;
		}
		//服务器端: 初始化(返回错误码)
		int init(const char ip[], int port, SSL* _ssl = nullptr)
		{
			ssl = _ssl;

			int res;
			//初始化网络库
			res = WSAStartup(MAKEWORD(2, 2), &wd);
			if (res != 0)return -1;

			//创建套接字
			sock = socket(AF_INET, SOCK_STREAM, NULL);
			if (sock == -1)
			{
				close();
				return -1;
			}

			//绑定地址(命名套接字)
			memset(&addr, 0, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
			inet_pton(AF_INET, ip, &addr.sin_addr);
			if (bind(sock, (sockaddr*)&addr, sizeof(addr)) == -1)
			{
				close();
				return -1;
			}

			//设置为非阻塞套接字
			u_long ul = 1;
			ioctlsocket(sock, FIONBIO, &ul);

			mode = SERVER;
			return 0;
		}

		//客户端: 连接服务器端(IP地址/主机名)(返回-2表示没有可用连接)
		int connect(const char host[], int port, float waitSec = 3)
		{
			if (mode != CLIENT)return -1;

			memset(&addr, 0, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);

			bool hostName = false;
			for (int i = 0; i < strlen(host); i++)
			{
				if (host[i] >= 'A' && host[i] <= 'Z' ||
					host[i] >= 'a' && host[i] <= 'z')
				{
					hostName = true;
					break;
				}
			}
			if (hostName)
			{
				if (gethostbyname(host) == nullptr)
				{
					return -1;
				}
				else
				{
					memcpy(&addr.sin_addr, gethostbyname(host)->h_addr, 4);
				}
			}
			else
			{
				inet_pton(AF_INET, host, &addr.sin_addr);
			}

			//连接
			int res, err = 0;
			clock_t timer = clock();
			while (true)
			{
				res = ::connect(sock, (sockaddr*)&addr, sizeof(addr));
				if (res == 0)break;
				err = WSAGetLastError();
				if (clock() - timer > waitSec * 1000 ||
					err != WSAEWOULDBLOCK && err != WSAEALREADY)
				{
					break;
				}
			}
			if (ssl != nullptr)
			{
				SSL_set_fd(ssl, sock);
				SSL_set_connect_state(ssl);//放在SSL_connect()之前
				while (true)
				{
					res = SSL_connect(ssl);
					if (res == 0)break;
					err = SSL_get_error(ssl, res);
					if (err != SSL_ERROR_WANT_READ && err != SSL_ERROR_WANT_WRITE)
					{
						break;
					}
				}

				/*cout << "==========SSL_connect()==========" << endl;
				cout << "res = " << res << endl;
				cout << "err = " << err << endl;
				cout << "errno = " << errno << endl;
				cout << "WSAGetLastError() = " << WSAGetLastError() << endl;
				cout << SSL_get_cipher(ssl) << endl;*/

				if (err != SSL_ERROR_NONE)
				{
					return -1;
				}
			}
			return 0;
		}

		//服务器端: 监听客户端连接
		int listen(int maxLen = 5)
		{
			if (mode != SERVER)return -1;

			//监听
			if (::listen(sock, maxLen) == -1)
			{
				return -1;
			}
			return 0;
		}
		//服务器端: 接收(等待)客户端连接(返回-2表示没有可用连接)
		int accept(float waitSec = 3)
		{
			if (mode != SERVER)return -1;

			//检查是否有可用连接
			fd_set fds;
			FD_ZERO(&fds);
			FD_SET(sock, &fds);
			timeval tv = { (int)waitSec,(waitSec - (int)waitSec) * 1000000 };
			select(NULL, &fds, NULL, NULL, &tv);
			if (!FD_ISSET(sock, &fds))return -2;

			//接受连接
			cli_sock = ::accept(sock, nullptr, nullptr);
			return 0;
		}

		//服务器端: 关闭与客户端的连接
		void end()
		{
			if (mode != SERVER)return;
			closesocket(cli_sock);
		}

		//发送信息(返回-2表示发送/接收不可用)
		int send(const char msg[], int len, float waitSec = 3)
		{
			if (mode == 0)return -1;

			//检查发送是否可用
			fd_set fds;
			FD_ZERO(&fds);
			FD_SET(_USING_SOCK, &fds);
			timeval tv = { (int)waitSec,(waitSec - (int)waitSec) * 1000000 };
			select(NULL, NULL, &fds, NULL, &tv);
			if (!FD_ISSET(_USING_SOCK, &fds))return -2;

			int res, err;
			if (ssl == nullptr)
			{
				res = ::send(_USING_SOCK, msg, len * sizeof(char), NULL);
			}
			else
			{
			write:
				res = SSL_write(ssl, msg, len * sizeof(char));
				err = SSL_get_error(ssl, res);

				/*cout << "==========SSL_write()==========" << endl;
				cout << "msg = " << msg << endl;
				cout << "res = " << res << endl;
				cout << "err = " << err << endl;
				cout << "errno = " << errno << endl;
				cout << "WSAGetLastError() = " << WSAGetLastError() << endl;
				cout << SSL_get_cipher(ssl) << endl;*/

				int i = WSAGetLastError();
				if (err == SSL_ERROR_WANT_READ && i != 0)goto write;
			}
			if (res < 0)
			{
				return -1;
			}

			return 0;
		}
		int send(const string msg, float waitSec = 3)
		{
			return send(msg.c_str(), msg.size(), waitSec);
		}
		//接收信息(返回实际接收的字节数, 返回-2表示发送/接收不可用)
		int recv(char _buf[], int len, float waitSec = 3)
		{
			if (mode == 0)return -1;

			if (ssl == nullptr)
			{
				//检查接收是否可用
				fd_set fds;
				FD_ZERO(&fds);
				FD_SET(_USING_SOCK, &fds);
				timeval tv = { (int)waitSec,(waitSec - (int)waitSec) * 1000000 };
				select(NULL, &fds, NULL, NULL, &tv);
				if (!FD_ISSET(_USING_SOCK, &fds))return -2;
			}

			memset(_buf, 0, len * sizeof(char));
			int res, err;
			if (ssl == nullptr)
			{
				res = ::recv(_USING_SOCK, _buf, (len > 1 ? (len - 1) : len) * sizeof(char), NULL);

				if (res < 0)
				{
					return -1;
				}
			}
			else
			{
				time_t timer = clock();
			read:
				res = SSL_read(ssl, _buf, (len > 1 ? (len - 1) : len) * sizeof(char));
				err = SSL_get_error(ssl, res);

				/*cout << "==========SSL_read()==========" << endl;
				cout << "buf = " << *buf << endl;
				cout << "res = " << res << endl;
				cout << "err = " << err << endl;
				cout << "errno = " << errno << endl;
				cout << "WSAGetLastError() = " << WSAGetLastError() << endl;
				cout << SSL_get_cipher(ssl) << endl;*/

				if (err == SSL_ERROR_WANT_READ)
				{
					if (clock() - timer <= waitSec * 1000)goto read;
					return -2;
				}
				if (err != SSL_ERROR_NONE)
				{
					return -1;
				}
				recved = true;
			}
			return res;
		}
		int recv(string* _buf, float waitSec = 3)
		{
			if (mode == 0)return -1;

			if (ssl != nullptr && !recved || ssl == nullptr)
			{
				//检查接收是否可用
				fd_set fds;
				FD_ZERO(&fds);
				FD_SET(_USING_SOCK, &fds);
				timeval tv = { (int)waitSec,(waitSec - (int)waitSec) * 1000000 };
				select(NULL, &fds, NULL, NULL, &tv);
				if (!FD_ISSET(_USING_SOCK, &fds))return -2;
			}

			_buf->clear();
			char* buf = new char[recv_BUF_SIZE];
			int res, err;
			if (ssl == nullptr)
			{
				while (true)
				{
					memset(buf, 0, recv_BUF_SIZE);
					res = ::recv(_USING_SOCK, buf, recv_BUF_SIZE - 1, NULL);
					if (res <= 0)
					{
						if (WSAGetLastError() == WSAEWOULDBLOCK)
						{
							break;
						}
						return -1;
					}
					*_buf += buf;
				}
			}
			else
			{
				time_t timer = clock();
				while (clock() - timer <= waitSec * 1000)
				{
					memset(buf, 0, recv_BUF_SIZE);
					res = SSL_read(ssl, buf, recv_BUF_SIZE - 1);
					err = SSL_get_error(ssl, res);

					if (err == SSL_ERROR_WANT_READ)
					{
						continue;
					}
					timer = clock();
					if (err != SSL_ERROR_NONE)return -1;

					*_buf += buf;
				}
				recved = false;
			}
			return _buf->size();
		}

		//发送/接收文件
		int send_file(const char filename[], float waitSec = 3)
		{
			int res;

			FILE* f;
			fopen_s(&f, filename, "rb");
			if (f == NULL)return -1;
			//获取文件大小
			fseek(f, 0, SEEK_END);
			size_t fileSize = ftell(f);
			fseek(f, 0, SEEK_SET);

			//循环发送
			char* _buf = new char[send_BUF_SIZE];
			for (size_t i = 0; i < fileSize; i += send_BUF_SIZE)
			{
				//读取文件
				memset(_buf, 0, send_BUF_SIZE);
				fread(_buf, sizeof(char), send_BUF_SIZE, f);
				//发送
				if (i + send_BUF_SIZE < fileSize)
				{
					res = send(_buf, send_BUF_SIZE, waitSec);
					if (res != 0)
					{
						delete[] _buf;
						return -1;
					}
				}
				else
				{
					res = send(_buf, fileSize - i, waitSec);
					if (res != 0)
					{
						delete[] _buf;
						return -1;
					}
				}
			}
			delete[] _buf;
			fclose(f);
			return 0;
		}
		int recv_file(const char _filename[], const char root[] = "recive\\", float waitSec = 3)
		{
			int res;

			wchar_t wroot[100];
			mbstowcs_s(NULL, wroot, root, 100);
			CreateDirectory(wroot, NULL);

			string filename;
			FILE* f;
			int j;
			char temp[10];
			filename = (string)root + _filename;
			fopen_s(&f, filename.c_str(), "rb");
			if (f != NULL)
			{
				fclose(f);
				for (int i = 2; i < 100; i++)
				{
					filename = (string)root + _filename;

					_itoa_s(i, temp, 10);
					j = filename.find(".");
					if (j == string::npos)
					{
						filename += '(';
						filename += temp;
						filename += ')';
					}
					else
					{
						filename.insert(j, "(");
						filename.insert(j + 1, ")");
						filename.insert(j + 1, temp);
					}

					fopen_s(&f, filename.c_str(), "rb");
					if (f == NULL)
					{
						break;
					}
					fclose(f);
				}
			}

			fopen_s(&f, filename.c_str(), "wb");
			if (f == NULL)
			{
				return -1;
			}

			//循环接收
			char* buf = new char[recv_BUF_SIZE];
			while (true)
			{
				memset(buf, 0, recv_BUF_SIZE);
				res = recv(buf, recv_BUF_SIZE, waitSec);
				if (res <= 0)
				{
					break;
				}
				fwrite(buf, sizeof(char), res, f);
			}
			delete[] buf;
			fclose(f);
			return 0;
		}

		//发送HTTP报文
		int send(const HttpMsg msg, float waitSec = 3)
		{
			if (mode == 0)return -1;
			int res;

			res = send(msg.make(), waitSec);
			if (res != 0)return -1;

			if (msg.dataMode == 1)
			{
				res = send_file(msg.data.c_str(), waitSec);
				if (res < 0)return res;
				send("\r\n\r\n", 2, waitSec);
			}

			return 0;
		}
		//接收HTTP报文(超过限定的接收到文件, -1表示无限制)
		int recv(HttpMsg* msg, size_t max = 1024, string resourceName = "/", const char root[] = "recive\\", float waitSec = 3)
		{
			if (mode == 0)return -1;

			msg->clear();
			string _msg;
			_msg.clear();
			char temp;
			int res;

			//接收报头
			while (true)
			{
				res = recv(&temp, 1, waitSec);
				if (res <= 0)
				{
					return res;
				}

				_msg += temp;
				if (_msg.find("\r\n\r\n") != string::npos)break;
			}
			msg->split(_msg);
			if (msg->get_state() != 200)return 0;

			//接收数据
			if (msg->get_header("Content-Length") != "")
			{
				size_t size = atoi(msg->get_header("Content-Length").c_str());
				//multipart/form-data
				if (msg->get_header("Content-Type") == "multipart/form-data")
				{
					//分隔符
					string bdy;
					int i = _msg.find("boundary=") + 9;
					bdy = "--";
					bdy += _msg.substr(i, _msg.find("\r\n", i) - i);

					//循环接收
					char* ctemp = new char[recv_BUF_SIZE];
					vector<char> _buf;
					string temp, name, filename;
					FILE* f = NULL;
					while (true)
					{
						memset(ctemp, 0, recv_BUF_SIZE);
						res = recv(ctemp, recv_BUF_SIZE, waitSec);
						if (res == 0)
						{
							if (f != NULL)
							{
								if (!_buf.empty())
								{
									for (int j = 0; j < max(0, _buf.size() - bdy.size() - 2); j++)
									{
										fwrite(&_buf[j], sizeof(char), 1, f);
									}
								}
								fclose(f);
							}
							delete[] ctemp;
							break;
						}
						else if (res < 0)
						{
							if (f != NULL)
							{
								fclose(f);
								remove(filename.c_str());
							}
							delete[] ctemp;
							break;
						}

						for (int i = 0; i < res; i++)_buf.push_back(ctemp[i]);
						while (find(_buf, bdy) != string::npos)
						{
							i = find(_buf, bdy);
							//读取之前的信息
							if (f != NULL && i > 0)
							{
								for (int j = 0; j < max(0, i - 2); j++)
								{
									fwrite(&_buf[j], sizeof(char), 1, f);
								}
								long i = ftell(f);
								fclose(f);
								f = NULL;
								if (i == 0)
								{
									remove(filename.c_str());
								}
								_buf.erase(_buf.begin(), _buf.begin() + i);
							}
							//如果是结束符
							if (find(_buf, bdy + "--") == i)break;
							//取出数据信息并删除
							temp = substr(_buf, 0, find(_buf, "\r\n\r\n"));
							_buf.erase(_buf.begin(), _buf.begin() + temp.size() + 4);//包括\r\n\r\n
							//一般数据
							if (temp.find("filename") == string::npos)
							{
								if (temp.find("name") == string::npos)
								{
									delete[] ctemp;
									return -1;
								}
								i = temp.find("name=\"") + 6;
								name = temp.substr(i, temp.find("\"", i) - i);

								//取出数据并删除
								temp = substr(_buf, 0, find(_buf, "\r\n"));
								_buf.erase(_buf.begin(), _buf.begin() + temp.size() + 2);

								msg->par.push_back({ name,temp });
							}
							//文件
							else
							{
								if (temp.find("filename") == string::npos)return -1;
								i = temp.find("filename=\"") + 10;
								name = temp.substr(i, temp.find("\"", i) - i);

								//生成文件名
								int j;
								filename = (string)root + name;
								if (fopen_s(&f, filename.c_str(), "rb") != NULL)
								{
									for (int i = 2; i < 100; i++)
									{
										filename = (string)root + name;
										j = filename.find(".");
										if (j == string::npos)
										{
											filename += '(';
											filename += temp;
											filename += ')';
										}
										else
										{
											filename.insert(j, "(");
											filename.insert(j + 1, ")");
											filename.insert(j + 1, temp);
										}
										if (fopen_s(&f, filename.c_str(), "rb") == NULL)
										{
											fclose(f);
											break;
										}
									}
								}
								else
								{
									fclose(f);
								}

								//打开文件
								wchar_t wroot[100];
								mbstowcs_s(NULL, wroot, root, 100);
								CreateDirectory(wroot, NULL);

								fopen_s(&f, filename.c_str(), "wb");
								if (f == NULL)return -1;
							}
						}
						if (f != NULL)
						{
							for (int j = 0; j < max(0, _buf.size() - bdy.size() - 2); j++)
							{
								fwrite(&_buf[j], sizeof(char), 1, f);
							}
							_buf.erase(_buf.begin(), _buf.begin() + max(0, _buf.size() - bdy.size() - 2));
						}
					}
				}
				//限制以内报文
				else if (max == -1 || size < max)
				{
					string temp;
					res = recv(&temp, waitSec);
					_msg += temp;
					msg->split(_msg);
				}
				//超过限制报文
				else
				{
					//生成文件名
					if (resourceName == "/")resourceName = "index.html";
					//只取最后的文件名
					if (resourceName.find("/") != string::npos)
					{
						if (resourceName.find("?") != string::npos)
						{
							resourceName = resourceName.substr(resourceName.find_last_of("/") + 1,
								resourceName.find("?") - resourceName.find_last_of("/") - 1);
						}
						else
						{
							resourceName = resourceName.substr(resourceName.find_last_of("/") + 1);
						}
					}
					//加上后缀
					if (resourceName.find(".") == string::npos)
					{
						resourceName += ".html";
					}
					//接收文件
					return recv_file(resourceName.c_str(), root, waitSec);
				}
			}
			//可恶，没给长度，硬着头皮收吧
			else
			{
				if (max == 0)
				{
					//生成文件名
					if (resourceName == "/")resourceName = "index.html";
					//只取最后的文件名
					if (resourceName.find("/") != string::npos)
					{
						if (resourceName.find("?") != string::npos)
						{
							resourceName = resourceName.substr(resourceName.find_last_of("/") + 1,
								resourceName.find("?") - resourceName.find_last_of("/") - 1);
						}
						else
						{
							resourceName = resourceName.substr(resourceName.find_last_of("/") + 1);
						}
					}
					//加上后缀
					if (resourceName.find(".") == string::npos)
					{
						resourceName += ".html";
					}
					//接收文件
					return recv_file(resourceName.c_str(), root, waitSec);
				}
				string temp;
				recv(&temp, waitSec);
				_msg += temp;
				msg->split(_msg);
			}
			return 0;
		}
	};

	/*****************************************************************************
	* Http
	* 自动HTTP/HTTPS
	*
	* 2023.1.29：仅完成了客户端
	*****************************************************************************/

	//从URL中解析主机名
	string GetHost(string url)
	{
		int res = url.find("//");
		if (res == string::npos) res = 0;
		else res += 2;
		if (url.find("/", res) == string::npos) return url.substr(res);
		return url.substr(res, url.find("/", res) - res);
	}
	//从URL中解析资源名
	string GetRes(string url)
	{
		int res = url.find("//");
		if (res == string::npos) res = 0;
		else res += 2;
		if (url.find("/", res) == string::npos) return "/";
		else
		{
			if (url.find(":", res) == string::npos)
			{
				return url.substr(url.find("/", res));
			}
			else
			{
				return url.substr(url.find("/", res), url.find(":") - url.find("/", res));
			}
		}
	}
	//从URL中解析端口（错误返回-1）
	int GetPort(string url)
	{
		int res = url.find("//");
		if (res == string::npos) res = 0;
		else res += 2;
		if (url.find(":", res) != string::npos)
		{
			if (url.find("/", res) != string::npos)
			{
				return atoi(url.substr(url.find(":", res) + 1, url.find("/", res) - url.find(":", res)).c_str());
			}
			else
			{
				return atoi(url.substr(url.find(":", res) + 1).c_str());
			}
		}
		if (url.find("http://") != string::npos)
		{
			return HTTP_PORT;
		}
		else if (url.find("https://") != string::npos)
		{
			return HTTPS_PORT;
		}
		return -1;
	}

	class Http
	{
	private:
		bool listening = false;
		string host;
		bool bHttps = false;
	public:
		HttpMsg rmsg;
		HttpMsg smsg;
		Socket s;

		//客户端：初始化(返回错误码)
		int init(SSL* ssl = nullptr)
		{
			if (ssl != nullptr)bHttps = true;
			return s.init(ssl);
		}
		//服务器端：初始化(返回错误码)
		int init(const char ip[], int port = -1, SSL* ssl = nullptr)
		{
			if (ssl != nullptr)bHttps = true;
			if (port == -1)
			{
				return s.init(ip, bHttps ? HTTPS_PORT : HTTP_PORT);
			}
			else
			{
				return s.init(ip, port);
			}
		}

		//关闭
		void close()
		{
			s.close();
		}

		//获取错误码
		static int GetError()
		{
			return WSAGetLastError();
		}

		//客户端：连接服务器(返回-2表示没有可用连接)
		int connect(const char _host[], int port = -1, float waitSec = 3)
		{
			host = _host;
			if (port == -1)
			{
				return s.connect(_host, bHttps ? HTTPS_PORT : HTTP_PORT, waitSec);
			}
			else
			{
				return s.connect(_host, port, waitSec);
			}
		}
		//服务器端：等待并接受连接(返回-2表示没有可用连接)
		int connect(int maxLen = 5, float waitSec = 3)
		{
			if (!listening)
			{
				int res = s.listen(maxLen);
				if (res != 0)return res;
			}
			return s.accept(waitSec);
		}

		//客户端：接收网页到string
		string GetHtml(const string resourceName = "/", float waitSec = 3)
		{
			//处理URI
			string a, b, c;
			a = resourceName.substr(0, resourceName.find_last_of("/") + 1);
			if (resourceName.find("?") != string::npos)
			{
				b = resourceName.substr(resourceName.find_last_of("/") + 1,
					resourceName.find("?") - resourceName.find_last_of("/") - 1);
				c = resourceName.substr(resourceName.find("?"));
			}
			else
			{
				b = resourceName.substr(resourceName.find_last_of("/") + 1);
			}
			b = Uri_encode(b);

			smsg.set_req(a + b + c);
			smsg.set_header("connection", "keep-alive");
			smsg.set_header("Host", host);
			//用Edge的UA
			smsg.set_header("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/109.0.0.0 Safari/537.36 Edg/109.0.1518.70");
			s.send(smsg, waitSec);

			s.recv(&rmsg, -1, "", "", waitSec);
			return rmsg.data;
		}
		//客户端：接收文件(返回-3表示302Found)
		int GetFile(const string resourceName = "/", const string savePath = "recive\\", float waitSec = 3)
		{
			//处理URI
			string a, b, c;
			a = resourceName.substr(0, resourceName.find_last_of("/") + 1);
			if (resourceName.find("?") != string::npos)
			{
				b = resourceName.substr(resourceName.find_last_of("/") + 1,
					resourceName.find("?") - resourceName.find_last_of("/") - 1);
				c = resourceName.substr(resourceName.find("?"));
			}
			else
			{
				b = resourceName.substr(resourceName.find_last_of("/") + 1);
			}
			b = Uri_encode(b);

			smsg.set_req(a + b + c);
			smsg.set_header("connection", "keep-alive");
			smsg.set_header("Host", host);
			//用Edge的UA
			smsg.set_header("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/109.0.0.0 Safari/537.36 Edg/109.0.1518.70");

			int res;
			res = s.send(smsg, waitSec);
			if (res != 0)return res;

			res = s.recv(&rmsg, 0, resourceName, savePath.c_str(), waitSec);
			//302Found
			if (rmsg.get_state() == 302)
			{
				return -3;
			}
			return res;
		}

		//服务器端：关闭与客户端的连接
		void end()
		{
			s.end();
		}
	};

	typedef Http Https;

	class HttpClient
	{
	private:
		SSL* ssl;
		Socket s;
		string root;
		string url;
	public:
		HttpMsg smsg, rmsg;

		~HttpClient()
		{
			s.close();
		}

		//初始化
		void init(string savePath, SSL* _ssl = nullptr)
		{
			root = savePath;
			ssl = _ssl;
		}

		//获取资源（如是text类型就返回数据，否则保存到文件）
		string Get(string url)
		{

		}
	};
}