#pragma once
/*****************************************************************************
* mWeb.h
* ����
*
* ����OpenSSL��WinSock
*****************************************************************************/

#define _WINSOCK_DEPRECATED_NO_WARNINGS
//�ѹر�SDL��飬��Ϊopenssl\applink.c
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
	* HTTP����
	*
	*****************************************************************************/

	//����Դ����ȡ��������
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
			//ѹ����
			else if (suf == ".zip")return "application/x-zip-compressed";
			else if (suf == ".rar")return "application/octet-stream";
			else if (suf == ".7z")return "application/x-7z-compressed";
			else return "application/octet-stream";
		}
	}

	class HttpMsg
	{
	public:
		string fline;							//������/״̬��
		vector<pair<string, string>> header;	//ͷ��
		vector<pair<string, string>> par;		//����
		string data;							//����
		int dataMode = 0;						//0: ����; 1: ��¼�ļ���

		//���
		void clear()
		{
			fline.clear();
			header.clear();
			data.clear();
			dataMode = 0;
		}


		//��������
		void split(const char _msg[], int len)
		{
			clear();
			string msg(_msg, len);

			//������/״̬��
			if (msg.find("\r\n") == string::npos)return;
			fline = msg.substr(0, msg.find("\r\n"));

			//����
			if (msg.find("\r\n\r\n") == string::npos)return;
			data = msg.substr(msg.find("\r\n\r\n") + 4);
			if (data.find("\r\n\r\n") != string::npos)
			{
				data.erase(data.find("\r\n\r\n"), 4);
			}

			//ͷ��
			int i = msg.find("\r\n") + 2;
			header.clear();
			while (true)
			{
				//�ֽ�һ��
				header.push_back({
					msg.substr(i, msg.find(": ", i) - i),
					msg.substr(msg.find(": ", i) + 2,
					(msg.find("; ", i) == string::npos ? msg.find("\r\n",i) : min(msg.find("\r\n",i),msg.find("; ",i)))
					- (msg.find(": ", i) + 2))
					});

				if (msg.find("\r\n\r\n") == msg.find("\r\n", i))break;
				i = msg.find("\r\n", i) + 2;
			}

			//����
			string pars;
			//GET����
			if (fline.find("GET") != string::npos)
			{
				if (fline.find("?") != string::npos)
				{
					pars = fline.substr(fline.find("?") + 1, fline.find(" HTTP") - fline.find("?") - 1);
				}
			}
			//POST����
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
			//������/״̬��
			if (msg.find("\r\n") == string::npos)return;
			fline = msg.substr(0, msg.find("\r\n"));

			//����
			if (msg.find("\r\n\r\n") == string::npos)return;
			data = msg.substr(msg.find("\r\n\r\n") + 4);
			if (data.find("\r\n\r\n") != string::npos)
			{
				data.erase(data.find("\r\n\r\n"), 4);
			}

			//ͷ��
			int i = msg.find("\r\n") + 2;
			header.clear();
			while (true)
			{
				//�ֽ�һ��
				header.push_back({
					msg.substr(i, msg.find(": ", i) - i),
					msg.substr(msg.find(": ", i) + 2,
					(msg.find("; ", i) == string::npos ? msg.find("\r\n",i) : min(msg.find("\r\n",i),msg.find("; ",i)))
					- (msg.find(": ", i) + 2))
					});

				if (msg.find("\r\n\r\n") == msg.find("\r\n", i))break;
				i = msg.find("\r\n", i) + 2;
			}

			//����
			string pars;
			//GET����
			if (fline.find("GET") != string::npos)
			{
				if (fline.find("?") != string::npos)
				{
					pars = fline.substr(fline.find("?") + 1, fline.find(" HTTP") - fline.find("?") - 1);
				}
			}
			//POST����
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

		//��ϱ���
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


		//��������: ��ȡ��Դ��
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
		//�ͻ���: ����������
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

		//�ͻ���: ��ȡ״̬��
		int get_state()
		{
			if (fline.find(" ") == string::npos)return 0;
			return atoi(fline.substr(fline.find(" ") + 1, 3).c_str());
		}
		//��������: ����״̬��
		void set_state(int state = 200)
		{
			fline.clear();
			fline += "HTTP/1.1 ";
			switch (state)
			{
				//��Ϣ
			case 100:fline += "100 Continue"; break;
				//�ɹ�
			case 200:fline += "200 OK"; break;
				//�ض���
			case 301:fline += "301 Moved Permanently"; break;
			case 302:fline += "301 Found"; break;
			case 304:fline += "304 Not Modified"; break;
			case 307:fline += "307 Temporary Redirect"; break;
				//�ͻ��˴���
			case 400:fline += "400 Bad Request"; break;
			case 401:fline += "401 Unauthorized"; break;
			case 403:fline += "403 Forbidden"; break;
			case 404:fline += "404 Not Found"; break;
				//����������
			case 500:fline += "500 Internal Server Error"; break;
			case 503:fline += "503 Service Unavailable"; break;
			default:fline += "500 Internal Server Error"; break;
			}
		}

		//��������: ��ȡ���󷽷�
		string get_method()
		{
			if (fline.find(" ") == string::npos)return "";
			return fline.substr(0, fline.find(" "));
		}


		//��ȡͷ�������ֵ
		string get_header(string item) const
		{
			for (size_t i = 0; i < header.size(); i++)
			{
				if (header[i].first == item)return header[i].second;
			}
			return "";
		}
		//����ͷ�������ֵ(��û�������)
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
		//�Զ�����ͷ�������ֵ
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
					//��ȡ�ļ���С
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

				//��ʽ��: Sat, 02 Jul 2011 20:45:57 GMT
				char date[50];
				strftime(date, 50 * sizeof(char), "%a, %d %b %Y %H:%M:%S GMT", &info);

				set_header("Date", date);
			}
			return 0;
		}


		//�ͻ���: ����������
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

		//��������: ����״̬ҳ
		void make_state_page()
		{
			data = "<!DOCTYPE HTML><html><head><title>";
			data += fline.substr(fline.find(" ") + 1);
			data += "</title></head><body><center><h1>";
			data += fline.substr(fline.find(" ") + 1);
			data += "</h1><hr /><p>mLazy</p></center></body></html>";
		}

		//��������Ϊ�ļ�
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
	* �Զ��׽���
	*
	* 2023.1.29��SSL���ֽ�����˿ͻ���
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
		int mode = 0;//0: δ��ʼ��
		SSL* ssl = nullptr;
		bool recved = false;//�ѽ��յ�SSL������

	public:
		SOCKET sock;

		//�ر��׽���
		void close()
		{
			closesocket(sock);
			WSACleanup();
			mode = 0;
		}

		//��ȡ������
		static int get_error()
		{
			return WSAGetLastError();
		}

		//�ͻ���: ��ʼ��(���ش�����)
		int init(SSL* _ssl = nullptr)
		{
			ssl = _ssl;

			int res;
			//��ʼ�������
			res = WSAStartup(MAKEWORD(2, 2), &wd);
			if (res != 0)return res;

			//�����׽���
			sock = socket(AF_INET, SOCK_STREAM, NULL);
			if (sock == -1)
			{
				res = WSAGetLastError();
				close();
				return res;
			}

			//����Ϊ�������׽���
			u_long ul = 1;
			ioctlsocket(sock, FIONBIO, &ul);

			mode = CLIENT;
			return 0;
		}
		//��������: ��ʼ��(���ش�����)
		int init(const char ip[], int port, SSL* _ssl = nullptr)
		{
			ssl = _ssl;

			int res;
			//��ʼ�������
			res = WSAStartup(MAKEWORD(2, 2), &wd);
			if (res != 0)return -1;

			//�����׽���
			sock = socket(AF_INET, SOCK_STREAM, NULL);
			if (sock == -1)
			{
				close();
				return -1;
			}

			//�󶨵�ַ(�����׽���)
			memset(&addr, 0, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
			inet_pton(AF_INET, ip, &addr.sin_addr);
			if (bind(sock, (sockaddr*)&addr, sizeof(addr)) == -1)
			{
				close();
				return -1;
			}

			//����Ϊ�������׽���
			u_long ul = 1;
			ioctlsocket(sock, FIONBIO, &ul);

			mode = SERVER;
			return 0;
		}

		//�ͻ���: ���ӷ�������(IP��ַ/������)(����-2��ʾû�п�������)
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

			//����
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
				SSL_set_connect_state(ssl);//����SSL_connect()֮ǰ
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

		//��������: �����ͻ�������
		int listen(int maxLen = 5)
		{
			if (mode != SERVER)return -1;

			//����
			if (::listen(sock, maxLen) == -1)
			{
				return -1;
			}
			return 0;
		}
		//��������: ����(�ȴ�)�ͻ�������(����-2��ʾû�п�������)
		int accept(float waitSec = 3)
		{
			if (mode != SERVER)return -1;

			//����Ƿ��п�������
			fd_set fds;
			FD_ZERO(&fds);
			FD_SET(sock, &fds);
			timeval tv = { (int)waitSec,(waitSec - (int)waitSec) * 1000000 };
			select(NULL, &fds, NULL, NULL, &tv);
			if (!FD_ISSET(sock, &fds))return -2;

			//��������
			cli_sock = ::accept(sock, nullptr, nullptr);
			return 0;
		}

		//��������: �ر���ͻ��˵�����
		void end()
		{
			if (mode != SERVER)return;
			closesocket(cli_sock);
		}

		//������Ϣ(����-2��ʾ����/���ղ�����)
		int send(const char msg[], int len, float waitSec = 3)
		{
			if (mode == 0)return -1;

			//��鷢���Ƿ����
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
		//������Ϣ(����ʵ�ʽ��յ��ֽ���, ����-2��ʾ����/���ղ�����)
		int recv(char _buf[], int len, float waitSec = 3)
		{
			if (mode == 0)return -1;

			if (ssl == nullptr)
			{
				//�������Ƿ����
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
				//�������Ƿ����
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

		//����/�����ļ�
		int send_file(const char filename[], float waitSec = 3)
		{
			int res;

			FILE* f;
			fopen_s(&f, filename, "rb");
			if (f == NULL)return -1;
			//��ȡ�ļ���С
			fseek(f, 0, SEEK_END);
			size_t fileSize = ftell(f);
			fseek(f, 0, SEEK_SET);

			//ѭ������
			char* _buf = new char[send_BUF_SIZE];
			for (size_t i = 0; i < fileSize; i += send_BUF_SIZE)
			{
				//��ȡ�ļ�
				memset(_buf, 0, send_BUF_SIZE);
				fread(_buf, sizeof(char), send_BUF_SIZE, f);
				//����
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

			//ѭ������
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

		//����HTTP����
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
		//����HTTP����(�����޶��Ľ��յ��ļ�, -1��ʾ������)
		int recv(HttpMsg* msg, size_t max = 1024, string resourceName = "/", const char root[] = "recive\\", float waitSec = 3)
		{
			if (mode == 0)return -1;

			msg->clear();
			string _msg;
			_msg.clear();
			char temp;
			int res;

			//���ձ�ͷ
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

			//��������
			if (msg->get_header("Content-Length") != "")
			{
				size_t size = atoi(msg->get_header("Content-Length").c_str());
				//multipart/form-data
				if (msg->get_header("Content-Type") == "multipart/form-data")
				{
					//�ָ���
					string bdy;
					int i = _msg.find("boundary=") + 9;
					bdy = "--";
					bdy += _msg.substr(i, _msg.find("\r\n", i) - i);

					//ѭ������
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
							//��ȡ֮ǰ����Ϣ
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
							//����ǽ�����
							if (find(_buf, bdy + "--") == i)break;
							//ȡ��������Ϣ��ɾ��
							temp = substr(_buf, 0, find(_buf, "\r\n\r\n"));
							_buf.erase(_buf.begin(), _buf.begin() + temp.size() + 4);//����\r\n\r\n
							//һ������
							if (temp.find("filename") == string::npos)
							{
								if (temp.find("name") == string::npos)
								{
									delete[] ctemp;
									return -1;
								}
								i = temp.find("name=\"") + 6;
								name = temp.substr(i, temp.find("\"", i) - i);

								//ȡ�����ݲ�ɾ��
								temp = substr(_buf, 0, find(_buf, "\r\n"));
								_buf.erase(_buf.begin(), _buf.begin() + temp.size() + 2);

								msg->par.push_back({ name,temp });
							}
							//�ļ�
							else
							{
								if (temp.find("filename") == string::npos)return -1;
								i = temp.find("filename=\"") + 10;
								name = temp.substr(i, temp.find("\"", i) - i);

								//�����ļ���
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

								//���ļ�
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
				//�������ڱ���
				else if (max == -1 || size < max)
				{
					string temp;
					res = recv(&temp, waitSec);
					_msg += temp;
					msg->split(_msg);
				}
				//�������Ʊ���
				else
				{
					//�����ļ���
					if (resourceName == "/")resourceName = "index.html";
					//ֻȡ�����ļ���
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
					//���Ϻ�׺
					if (resourceName.find(".") == string::npos)
					{
						resourceName += ".html";
					}
					//�����ļ�
					return recv_file(resourceName.c_str(), root, waitSec);
				}
			}
			//�ɶ�û�����ȣ�Ӳ��ͷƤ�հ�
			else
			{
				if (max == 0)
				{
					//�����ļ���
					if (resourceName == "/")resourceName = "index.html";
					//ֻȡ�����ļ���
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
					//���Ϻ�׺
					if (resourceName.find(".") == string::npos)
					{
						resourceName += ".html";
					}
					//�����ļ�
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
	* �Զ�HTTP/HTTPS
	*
	* 2023.1.29��������˿ͻ���
	*****************************************************************************/

	//��URL�н���������
	string GetHost(string url)
	{
		int res = url.find("//");
		if (res == string::npos) res = 0;
		else res += 2;
		if (url.find("/", res) == string::npos) return url.substr(res);
		return url.substr(res, url.find("/", res) - res);
	}
	//��URL�н�����Դ��
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
	//��URL�н����˿ڣ����󷵻�-1��
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

		//�ͻ��ˣ���ʼ��(���ش�����)
		int init(SSL* ssl = nullptr)
		{
			if (ssl != nullptr)bHttps = true;
			return s.init(ssl);
		}
		//�������ˣ���ʼ��(���ش�����)
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

		//�ر�
		void close()
		{
			s.close();
		}

		//��ȡ������
		static int GetError()
		{
			return WSAGetLastError();
		}

		//�ͻ��ˣ����ӷ�����(����-2��ʾû�п�������)
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
		//�������ˣ��ȴ�����������(����-2��ʾû�п�������)
		int connect(int maxLen = 5, float waitSec = 3)
		{
			if (!listening)
			{
				int res = s.listen(maxLen);
				if (res != 0)return res;
			}
			return s.accept(waitSec);
		}

		//�ͻ��ˣ�������ҳ��string
		string GetHtml(const string resourceName = "/", float waitSec = 3)
		{
			//����URI
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
			//��Edge��UA
			smsg.set_header("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/109.0.0.0 Safari/537.36 Edg/109.0.1518.70");
			s.send(smsg, waitSec);

			s.recv(&rmsg, -1, "", "", waitSec);
			return rmsg.data;
		}
		//�ͻ��ˣ������ļ�(����-3��ʾ302Found)
		int GetFile(const string resourceName = "/", const string savePath = "recive\\", float waitSec = 3)
		{
			//����URI
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
			//��Edge��UA
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

		//�������ˣ��ر���ͻ��˵�����
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

		//��ʼ��
		void init(string savePath, SSL* _ssl = nullptr)
		{
			root = savePath;
			ssl = _ssl;
		}

		//��ȡ��Դ������text���;ͷ������ݣ����򱣴浽�ļ���
		string Get(string url)
		{

		}
	};
}