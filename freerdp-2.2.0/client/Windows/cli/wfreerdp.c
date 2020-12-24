/**
 * FreeRDP: A Remote Desktop Protocol Implementation
 * Windows Client
 *
 * Copyright 2009-2011 Jay Sorg
 * Copyright 2010-2011 Vic Lee
 * Copyright 2010-2011 Marc-Andre Moreau <marcandre.moreau@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <winpr/windows.h>

#include <winpr/crt.h>
#include <winpr/credui.h>

#include <freerdp/freerdp.h>
#include <freerdp/constants.h>

#include <freerdp/client/file.h>
#include <freerdp/client/cmdline.h>
#include <freerdp/client/channels.h>
#include <freerdp/channels/channels.h>

#include "resource.h"

#include "wf_client.h"

#include <shellapi.h>
void Topsec(char* s, char c);
DWORD RdpMessageBox(LPWSTR Xcmd, char RpszMultiByte[], char* Xstr_Name_Ip[2], char* XPass_word,char* szUserInfo);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{	
	int status;
	HANDLE thread;
	wfContext* wfc;
	DWORD dwExitCode;
	rdpContext* context;
	rdpSettings* settings;
	LPWSTR cmd;
	char** argv = NULL;
	RDP_CLIENT_ENTRY_POINTS clientEntryPoints = { 0 };
	int ret = 1;
	int argc = 0, i;
	LPWSTR* args = NULL; //存贮164组-功能项参数

	WINPR_UNUSED(hInstance);
	WINPR_UNUSED(hPrevInstance);
	WINPR_UNUSED(lpCmdLine);
	WINPR_UNUSED(nCmdShow);

	RdpClientEntry(&clientEntryPoints);
	context = freerdp_client_context_new(&clientEntryPoints);

	if (!context)
		return -1;

	cmd = GetCommandLineW();
	if (!cmd)
		goto out;
	args = CommandLineToArgvW(cmd, &argc);

	//添加可以自定义输入参数的控件，并返回数据到cmd进行解析
	if (!args || (argc <= 1))
	{
		argv = calloc((size_t)argc, sizeof(char*));

		if (!argv)
			goto out;

		int _size = WideCharToMultiByte(CP_UTF8, 0, args[0], -1, NULL, 0, NULL, NULL);
		if (_size <= 0)
			goto out;
		argv[0] = calloc((size_t)_size, sizeof(char));

		if (!argv[0])
			goto out;

		if (WideCharToMultiByte(CP_UTF8, 0, args[0], -1, argv[0], _size, NULL, NULL) != _size)
			goto out;

		/*
		    MessageBoxA(0, "第一个C窗体程序", "消息", 3);
		    //参数1：系统弹出
		    //	参数2：消息内容
		    //	参数3：标题
		    //	参数4：对话框的类型
		    //	0  只有 确定 按钮；
		    //	1  确定 取消 按钮；
		    //	2  终止 重试 忽略 按钮
		    //	3  是 否 取消 按钮
		*/
		int WRdpRet;
		WRdpRet = MessageBoxA(0, (LPCSTR)("参数过少，请检查当前参数后重新输入!"), (LPCSTR)("信息提示"),
		                      MB_RETRYCANCEL);//4-2
		                      //MB_YESNO | MB_ICONQUESTION);//6-2
		while (1)
		{		
			if (WRdpRet == 4)
			{ 	
				//避免关掉当前程序继续连接，没有把clientEntryPoints与context初始化的错误
				RdpClientEntry(&clientEntryPoints);
				context = freerdp_client_context_new(&clientEntryPoints);

				if (!context)
					return -1;
				cmd = GetCommandLineW();
				if (!cmd)
					goto out;

				char RpszMultiByte[1024];
				memset(RpszMultiByte, 0, sizeof(RpszMultiByte));

				char Xstr_Name[CREDUI_MAX_USERNAME_LENGTH + 1] = { 0 };
				char Xstr_Ip[128] = { 0 };
				char Pass_word[CREDUI_MAX_PASSWORD_LENGTH + 1] = { 0 }; //格式=(虚拟机)服务器主机密码
				char* Xstr_Name_Ip[2];
				Xstr_Name_Ip[0] = (char*)malloc(sizeof(char) * 1024);
				Xstr_Name_Ip[1] = (char*)malloc(sizeof(char) * 1024);
				sprintf(Xstr_Name_Ip[0], "");
				sprintf(Xstr_Name_Ip[1], "");


				char* szUserInfo;
				szUserInfo = (char*)malloc(sizeof(char) * 1024);	
				sprintf(szUserInfo, "");

				DWORD Rret = RdpMessageBox(cmd, RpszMultiByte, Xstr_Name_Ip, Pass_word, szUserInfo);
				//cmd需要清空处理
				//char* CmdNull;
				//CmdNull = (char*)malloc(sizeof(char) * 1024);
				//sprintf(CmdNull, "");
				//WCHAR CmdTemp[1024] = { 0 };
				//MultiByteToWideChar(CP_ACP, 0, CmdNull, -1, CmdTemp, 1024);
				//cmd = CmdTemp;//cmd清空处理,避免重复登录参数问题

				if (Rret != 0)
				{
					goto out;
					//break;
				}
				//拼接默认参数
				//提示：\r\n您连接的用户账号：tophc\r\n您连接的用户密码：123456\r\n您连接的Ip地址：10.30.10.51
				if ((Xstr_Name_Ip[0] != '\0') && (Pass_word[0] != '\0') && (Xstr_Name_Ip[1][0] != '\0'))
				{
					sprintf(szUserInfo, "%s /u:%s /p:%s /v:%s /sound +clipboard ", RpszMultiByte, Xstr_Name_Ip[0], Pass_word, Xstr_Name_Ip[1]);//+clipboard 
					// char* ----> LPWSTR/wchar_t*
					WCHAR Temp[1024] = { 0 };
					//mbstowcs(Temp, RpszMultiByte,strlen(RpszMultiByte));//不可行
					MultiByteToWideChar(CP_ACP, 0, szUserInfo, -1, Temp, 1024); //可行
					cmd = Temp;

					if (!cmd)
						goto out;

					args = CommandLineToArgvW(cmd, &argc);
					//***********************
					//后处理流程
					argv = calloc((size_t)argc, sizeof(char*));

					if (!argv)
						goto out;

					for (i = 0; i < argc; i++)
					{
						int size = WideCharToMultiByte(CP_UTF8, 0, args[i], -1, NULL, 0, NULL, NULL);
						if (size <= 0)
							goto out;
						argv[i] = calloc((size_t)size, sizeof(char));

						if (!argv[i])
							goto out;

						if (WideCharToMultiByte(CP_UTF8, 0, args[i], -1, argv[i], size, NULL, NULL) !=
							size)
							goto out;
					}

					settings = context->settings;
					wfc = (wfContext*)context;

					if (!settings || !wfc)
						goto out;

					status = freerdp_client_settings_parse_command_line(settings, argc, argv,
																		FALSE); //命令行参数解析函数入口

					if (status)
					{
						//参数不全导致连接不成功，帮助日志信息提示入口
						freerdp_client_settings_command_line_status_print(settings, status, argc, argv);
						//手动退出:0或者输入的服务端用户名/密码/ip等参数不匹配:0
						WRdpRet = MessageBoxA(
						    0,
						    (LPCSTR)("请检查是否导致出现以下情况:\r\n1.客户端用户手动关闭服务端远程桌面\r\n2.客户端自定义窗口输入的服务端用户名/密码/ip等参数不匹配\r\n3.检查服务端是否正常开机运行状态"),
						    (LPCSTR)("信息提示"), MB_RETRYCANCEL);
						//goto out;
					}

					if (freerdp_client_start(context) != 0)
						goto out;

					thread = freerdp_client_get_thread(context);

					if (thread)
					{
						//int TOY = WaitForSingleObject(thread, INFINITE);
						if (WaitForSingleObject(thread, INFINITE) == WAIT_OBJECT_0) //启动远程桌面入口
						{
							//手动退出:0或者输入的服务端用户名/密码/ip等参数不匹配:0
							WRdpRet = MessageBoxA(0, (LPCSTR)("请检查是否导致出现以下情况:\r\n1.客户端用户手动关闭服务端远程桌面\r\n2.客户端自定义窗口输入的服务端用户名/密码/ip等参数不匹配\r\n3.检查服务端是否正常开机运行状态"),
							    (LPCSTR)("信息提示"), MB_RETRYCANCEL);
							GetExitCodeThread(thread, &dwExitCode);
							ret = (int)dwExitCode;
						}
					}

					if (freerdp_client_stop(context) != 0)
						goto out;
				}
				else
				{
					//拼接字符串
					strcat(szUserInfo, "请检查是否导致出现以下情况:\r\n1.客户端用户手动关闭服务端远程桌面\r\n2.客户端自定义窗口输入的服务端用户名/密码/ip等参数不匹配\r\n3.检查服务端是否正常开机运行状态");
					int WRet = MessageBoxA(0, (LPCSTR)szUserInfo, (LPCSTR)("警告"), MB_RETRYCANCEL);
					if (WRet == 2)
						break;
				}
			}
			else
				goto out;
		}			
	}
	else
	{
		//>1
		argv = calloc((size_t)argc, sizeof(char*));

		if (!argv)
			goto out;

		for (i = 0; i < argc; i++)
		{
			int size = WideCharToMultiByte(CP_UTF8, 0, args[i], -1, NULL, 0, NULL, NULL);
			if (size <= 0)
				goto out;
			argv[i] = calloc((size_t)size, sizeof(char));

			if (!argv[i])
				goto out;

			if (WideCharToMultiByte(CP_UTF8, 0, args[i], -1, argv[i], size, NULL, NULL) != size)
				goto out;
		}

		settings = context->settings;
		wfc = (wfContext*)context;

		if (!settings || !wfc)
			goto out;

		status = freerdp_client_settings_parse_command_line(settings, argc, argv,
		                                                    FALSE); //命令行参数解析函数入口

		if (status)
		{
			//参数不全导致连接不成功，帮助日志信息提示入口
			freerdp_client_settings_command_line_status_print(settings, status, argc, argv);
			goto out;
		}

		if (freerdp_client_start(context) != 0)
			goto out;

		thread = freerdp_client_get_thread(context);

		if (thread)
		{
			if (WaitForSingleObject(thread, INFINITE) == WAIT_OBJECT_0) //启动远程桌面入口
			{
				//手动退出:0或者输入的服务端用户名/密码/ip等参数不匹配:0
				int RdpRet =MessageBoxA(0,
				                (LPCSTR)("请检查是否导致出现以下情况:\r\n1.客户端用户手动关闭服务端远程桌面\r\n2.客户端自定义窗口输入的服务端用户名/密码/ip等参数不匹配\r\n3.检查服务端是否正常开机运行状态\r\n4.检查是否中止了当前连接登录"),
				    (LPCSTR)("信息提示"), MB_OK); // MB_YESNO | MB_ICONQUESTION
				GetExitCodeThread(thread, &dwExitCode);
				ret = (int)dwExitCode;
			}
		}

		if (freerdp_client_stop(context) != 0)
			goto out;
	}

out:
	freerdp_client_context_free(context);

	if (argv)
	{
		for (i = 0; i < argc; i++)
			free(argv[i]);

		free(argv);
	}

	LocalFree(args);
	return ret;
}

void Topsec(char* s, char c)
{
	if (s == NULL)
		return;
		//s = "!";
		
	char* t = s;
	while (*s == c)
	{
		s++;
	}
	if (*s)
	{
		char* t1 = s;
		while (*s)
		{
			s++;
		}
		s--;
		while (*s == c)
		{
			s--;
		}
		while (t1 <= s)
		{
			*(t++) = *(t1++);
		}
	}
	*t = 0;
	return;
}

DWORD RdpMessageBox(LPWSTR Xcmd, char RpszMultiByte[], char* Xstr_Name_Ip[], char XPass_word[],
                    char* szUserInfo)
{
	//类型安全检查

	//命令行启动的连接界面，初始化相关标题文本字段
	static CREDUI_INFOA wfUiInfo = {
		sizeof(CREDUI_INFOA), NULL,
		"Please Input Your Data",
		"Xtopsec@FreeRdp", NULL
	};
	//                                                      "
	//"#用户名参数必须是大写U开头,服务器主机名称@服务器主机IP#                         "
	//"例如:Utophc@10.30.10.51

	//弹出登录窗口
	BOOL fSave;
	DWORD Xstatus;
	DWORD dwFlags;
	char User_Name_Ip[CREDUI_MAX_USERNAME_LENGTH + 1] = { 0 }; //格式=(虚拟机)服务器主机名称@(虚拟机)服务器主机IP
	char* XUser_Name_Ip;                                    //保存U后面的字段
	XUser_Name_Ip = (char*)malloc(sizeof(char) * CREDUI_MAX_USERNAME_LENGTH);
	char* Name_Ip; //保存U后面的字段
	Name_Ip = (char*)malloc(sizeof(char) * CREDUI_MAX_USERNAME_LENGTH);
	char* RName_Ip; //保存U后面的字段
	RName_Ip = (char*)malloc(sizeof(char) * CREDUI_MAX_USERNAME_LENGTH);

	fSave = FALSE; //默认关闭大写
	dwFlags = CREDUI_FLAGS_DO_NOT_PERSIST | CREDUI_FLAGS_EXCLUDE_CERTIFICATES;
	Xstatus = CredUIPromptForCredentialsA(&wfUiInfo, "", NULL, 0, User_Name_Ip,
	                                      CREDUI_MAX_USERNAME_LENGTH + 1, XPass_word,
	                                      CREDUI_MAX_PASSWORD_LENGTH + 1, &fSave, dwFlags);

	//yes:0 cancel:1223
	strcpy(XUser_Name_Ip, User_Name_Ip);
	strcpy(Name_Ip, User_Name_Ip);
	strcpy(RName_Ip, User_Name_Ip);
	

	if (XUser_Name_Ip[0] == 'U')
	{
		Topsec(XUser_Name_Ip, 'U');
		Topsec(Name_Ip, 'U');
		Topsec(RName_Ip, 'U');
		
	}
	else
	{
		Topsec(XUser_Name_Ip, '\\');
		Topsec(Name_Ip, '\\');
		Topsec(RName_Ip, '\\');
	}

	//判断含用户名@ip还是ip\用户名
	char RFlag;
	RFlag = 0x80;
	
	while (*XUser_Name_Ip)
	{
		if (*XUser_Name_Ip == '@')
		{
			RFlag = '@';
			break;
		}
		else if (*XUser_Name_Ip == '\\')
		{
			RFlag = '\\';
			break;
		}
		XUser_Name_Ip++;
	}

	if (RFlag == '@')
	{
		//分割(虚拟机)服务器主机名称@(虚拟机)服务器主机IP
		/* 获取第一个子字符串 */
		Xstr_Name_Ip[0] = strtok(Name_Ip, "@");
		/* 继续获取其他的子字符串 */
		int iFlag = 0;
		while (Xstr_Name_Ip[0] != NULL)
		{
			Xstr_Name_Ip[1] = strtok(NULL, "@");
			if (Xstr_Name_Ip[1] == NULL)
			{
				Xstr_Name_Ip[1] = "";
			}
			if (iFlag == 0)
				break;
		}
	}
	else if (RFlag == '\\')
	{
		//分割(虚拟机)服务器主机IP\(虚拟机)服务器主机名称
		/* 获取第一个子字符串 */
		Xstr_Name_Ip[1] = strtok(Name_Ip, "\\");
		/* 继续获取其他的子字符串 */
		int iFlag = 0;
		while (Xstr_Name_Ip[1] != NULL)
		{
			Xstr_Name_Ip[0] = strtok(NULL, "\\");
			if (Xstr_Name_Ip[0] == NULL)
			{
				Xstr_Name_Ip[0] = "";
			}
			if (iFlag == 0)
				break;
		}
	}
	else
	{
		Xstr_Name_Ip[0] = Name_Ip;
		Xstr_Name_Ip[1] = ""; 
	}

	//特殊处理*.com
	char *Rp;
	Rp = (char*)malloc(sizeof(char) * 1024);
	Rp = strchr(RName_Ip, '.'); // com
	if (Rp != NULL)
	{
		Topsec(Rp, '.');
		if (strcmp(Rp, "com") == 0)
		{
			Xstr_Name_Ip[0] = User_Name_Ip;
			Xstr_Name_Ip[1] = "";
		}
	}
	
	//具体参数解析
	sprintf(szUserInfo,
	        "帮助信息提示：\r\n您连接的用户账号(Xtopsec)：%s\r\n您连接的用户密码(123456)：%"
	        "s\r\n您连接的Ip地址(port:3389)：%s\r\n\n\n",
	        Xstr_Name_Ip[0], XPass_word, Xstr_Name_Ip[1]);

	//手动测试命令行参数
	int Psize;
	char pszMultiByte[1024];
	memset(pszMultiByte, 0, sizeof(pszMultiByte));

	Psize = WideCharToMultiByte(CP_ACP, 0, Xcmd, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, Xcmd, -1, pszMultiByte, Psize, NULL,
	                    NULL); // LPWSTR/wchar_t* ----> char*
	Psize = strlen(pszMultiByte);
	memset(RpszMultiByte, 0, sizeof(RpszMultiByte));
	strncpy(RpszMultiByte, pszMultiByte, (Psize - 2));
	//去除两头的"
	Topsec(RpszMultiByte, '"');	

	return Xstatus;
}
