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
	LPWSTR* args = NULL; //����164��-���������

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

	//��ӿ����Զ�����������Ŀؼ������������ݵ�cmd���н���
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
		    MessageBoxA(0, "��һ��C�������", "��Ϣ", 3);
		    //����1��ϵͳ����
		    //	����2����Ϣ����
		    //	����3������
		    //	����4���Ի��������
		    //	0  ֻ�� ȷ�� ��ť��
		    //	1  ȷ�� ȡ�� ��ť��
		    //	2  ��ֹ ���� ���� ��ť
		    //	3  �� �� ȡ�� ��ť
		*/
		int WRdpRet;
		WRdpRet = MessageBoxA(0, (LPCSTR)("�������٣����鵱ǰ��������������!"), (LPCSTR)("��Ϣ��ʾ"),
		                      MB_RETRYCANCEL);//4-2
		                      //MB_YESNO | MB_ICONQUESTION);//6-2
		while (1)
		{		
			if (WRdpRet == 4)
			{ 	
				//����ص���ǰ����������ӣ�û�а�clientEntryPoints��context��ʼ���Ĵ���
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
				char Pass_word[CREDUI_MAX_PASSWORD_LENGTH + 1] = { 0 }; //��ʽ=(�����)��������������
				char* Xstr_Name_Ip[2];
				Xstr_Name_Ip[0] = (char*)malloc(sizeof(char) * 1024);
				Xstr_Name_Ip[1] = (char*)malloc(sizeof(char) * 1024);
				sprintf(Xstr_Name_Ip[0], "");
				sprintf(Xstr_Name_Ip[1], "");


				char* szUserInfo;
				szUserInfo = (char*)malloc(sizeof(char) * 1024);	
				sprintf(szUserInfo, "");

				DWORD Rret = RdpMessageBox(cmd, RpszMultiByte, Xstr_Name_Ip, Pass_word, szUserInfo);
				//cmd��Ҫ��մ���
				//char* CmdNull;
				//CmdNull = (char*)malloc(sizeof(char) * 1024);
				//sprintf(CmdNull, "");
				//WCHAR CmdTemp[1024] = { 0 };
				//MultiByteToWideChar(CP_ACP, 0, CmdNull, -1, CmdTemp, 1024);
				//cmd = CmdTemp;//cmd��մ���,�����ظ���¼��������

				if (Rret != 0)
				{
					goto out;
					//break;
				}
				//ƴ��Ĭ�ϲ���
				//��ʾ��\r\n�����ӵ��û��˺ţ�tophc\r\n�����ӵ��û����룺123456\r\n�����ӵ�Ip��ַ��10.30.10.51
				if ((Xstr_Name_Ip[0] != '\0') && (Pass_word[0] != '\0') && (Xstr_Name_Ip[1][0] != '\0'))
				{
					sprintf(szUserInfo, "%s /u:%s /p:%s /v:%s /sound +clipboard ", RpszMultiByte, Xstr_Name_Ip[0], Pass_word, Xstr_Name_Ip[1]);//+clipboard 
					// char* ----> LPWSTR/wchar_t*
					WCHAR Temp[1024] = { 0 };
					//mbstowcs(Temp, RpszMultiByte,strlen(RpszMultiByte));//������
					MultiByteToWideChar(CP_ACP, 0, szUserInfo, -1, Temp, 1024); //����
					cmd = Temp;

					if (!cmd)
						goto out;

					args = CommandLineToArgvW(cmd, &argc);
					//***********************
					//��������
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
																		FALSE); //�����в��������������

					if (status)
					{
						//������ȫ�������Ӳ��ɹ���������־��Ϣ��ʾ���
						freerdp_client_settings_command_line_status_print(settings, status, argc, argv);
						//�ֶ��˳�:0��������ķ�����û���/����/ip�Ȳ�����ƥ��:0
						WRdpRet = MessageBoxA(
						    0,
						    (LPCSTR)("�����Ƿ��³����������:\r\n1.�ͻ����û��ֶ��رշ����Զ������\r\n2.�ͻ����Զ��崰������ķ�����û���/����/ip�Ȳ�����ƥ��\r\n3.��������Ƿ�������������״̬"),
						    (LPCSTR)("��Ϣ��ʾ"), MB_RETRYCANCEL);
						//goto out;
					}

					if (freerdp_client_start(context) != 0)
						goto out;

					thread = freerdp_client_get_thread(context);

					if (thread)
					{
						//int TOY = WaitForSingleObject(thread, INFINITE);
						if (WaitForSingleObject(thread, INFINITE) == WAIT_OBJECT_0) //����Զ���������
						{
							//�ֶ��˳�:0��������ķ�����û���/����/ip�Ȳ�����ƥ��:0
							WRdpRet = MessageBoxA(0, (LPCSTR)("�����Ƿ��³����������:\r\n1.�ͻ����û��ֶ��رշ����Զ������\r\n2.�ͻ����Զ��崰������ķ�����û���/����/ip�Ȳ�����ƥ��\r\n3.��������Ƿ�������������״̬"),
							    (LPCSTR)("��Ϣ��ʾ"), MB_RETRYCANCEL);
							GetExitCodeThread(thread, &dwExitCode);
							ret = (int)dwExitCode;
						}
					}

					if (freerdp_client_stop(context) != 0)
						goto out;
				}
				else
				{
					//ƴ���ַ���
					strcat(szUserInfo, "�����Ƿ��³����������:\r\n1.�ͻ����û��ֶ��رշ����Զ������\r\n2.�ͻ����Զ��崰������ķ�����û���/����/ip�Ȳ�����ƥ��\r\n3.��������Ƿ�������������״̬");
					int WRet = MessageBoxA(0, (LPCSTR)szUserInfo, (LPCSTR)("����"), MB_RETRYCANCEL);
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
		                                                    FALSE); //�����в��������������

		if (status)
		{
			//������ȫ�������Ӳ��ɹ���������־��Ϣ��ʾ���
			freerdp_client_settings_command_line_status_print(settings, status, argc, argv);
			goto out;
		}

		if (freerdp_client_start(context) != 0)
			goto out;

		thread = freerdp_client_get_thread(context);

		if (thread)
		{
			if (WaitForSingleObject(thread, INFINITE) == WAIT_OBJECT_0) //����Զ���������
			{
				//�ֶ��˳�:0��������ķ�����û���/����/ip�Ȳ�����ƥ��:0
				int RdpRet =MessageBoxA(0,
				                (LPCSTR)("�����Ƿ��³����������:\r\n1.�ͻ����û��ֶ��رշ����Զ������\r\n2.�ͻ����Զ��崰������ķ�����û���/����/ip�Ȳ�����ƥ��\r\n3.��������Ƿ�������������״̬\r\n4.����Ƿ���ֹ�˵�ǰ���ӵ�¼"),
				    (LPCSTR)("��Ϣ��ʾ"), MB_OK); // MB_YESNO | MB_ICONQUESTION
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
	//���Ͱ�ȫ���

	//���������������ӽ��棬��ʼ����ر����ı��ֶ�
	static CREDUI_INFOA wfUiInfo = {
		sizeof(CREDUI_INFOA), NULL,
		"Please Input Your Data",
		"Xtopsec@FreeRdp", NULL
	};
	//                                                      "
	//"#�û������������Ǵ�дU��ͷ,��������������@����������IP#                         "
	//"����:Utophc@10.30.10.51

	//������¼����
	BOOL fSave;
	DWORD Xstatus;
	DWORD dwFlags;
	char User_Name_Ip[CREDUI_MAX_USERNAME_LENGTH + 1] = { 0 }; //��ʽ=(�����)��������������@(�����)����������IP
	char* XUser_Name_Ip;                                    //����U������ֶ�
	XUser_Name_Ip = (char*)malloc(sizeof(char) * CREDUI_MAX_USERNAME_LENGTH);
	char* Name_Ip; //����U������ֶ�
	Name_Ip = (char*)malloc(sizeof(char) * CREDUI_MAX_USERNAME_LENGTH);
	char* RName_Ip; //����U������ֶ�
	RName_Ip = (char*)malloc(sizeof(char) * CREDUI_MAX_USERNAME_LENGTH);

	fSave = FALSE; //Ĭ�Ϲرմ�д
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

	//�жϺ��û���@ip����ip\�û���
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
		//�ָ�(�����)��������������@(�����)����������IP
		/* ��ȡ��һ�����ַ��� */
		Xstr_Name_Ip[0] = strtok(Name_Ip, "@");
		/* ������ȡ���������ַ��� */
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
		//�ָ�(�����)����������IP\(�����)��������������
		/* ��ȡ��һ�����ַ��� */
		Xstr_Name_Ip[1] = strtok(Name_Ip, "\\");
		/* ������ȡ���������ַ��� */
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

	//���⴦��*.com
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
	
	//�����������
	sprintf(szUserInfo,
	        "������Ϣ��ʾ��\r\n�����ӵ��û��˺�(Xtopsec)��%s\r\n�����ӵ��û�����(123456)��%"
	        "s\r\n�����ӵ�Ip��ַ(port:3389)��%s\r\n\n\n",
	        Xstr_Name_Ip[0], XPass_word, Xstr_Name_Ip[1]);

	//�ֶ����������в���
	int Psize;
	char pszMultiByte[1024];
	memset(pszMultiByte, 0, sizeof(pszMultiByte));

	Psize = WideCharToMultiByte(CP_ACP, 0, Xcmd, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, Xcmd, -1, pszMultiByte, Psize, NULL,
	                    NULL); // LPWSTR/wchar_t* ----> char*
	Psize = strlen(pszMultiByte);
	memset(RpszMultiByte, 0, sizeof(RpszMultiByte));
	strncpy(RpszMultiByte, pszMultiByte, (Psize - 2));
	//ȥ����ͷ��"
	Topsec(RpszMultiByte, '"');	

	return Xstatus;
}
