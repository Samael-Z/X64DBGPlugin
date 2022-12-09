#pragma once
#include "pch.h"
#include <Windows.h>
#include "_plugins.h"
#include "resource.h"
#include <time.h>
#include <psapi.h>
#include <string>
#include<iostream>
using namespace std;


//初始化的导出函数
extern "C" __declspec(dllexport) bool pluginit(PLUG_INITSTRUCT * initStruct);
extern "C" __declspec(dllexport) void plugsetup(PLUG_SETUPSTRUCT * setupStruct);
extern "C" __declspec(dllexport) void CBMENUENTRY(CBTYPE cbType, PLUG_CB_MENUENTRY * info);
BOOL CALLBACK X64DialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL  X64Store2File(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


//对话框消息处理函数
BOOL CALLBACK X64DialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case BTN_STORE:
        {
            X64Store2File(hwnd, message, wParam, lParam);
            EndDialog(hwnd, LOWORD(wParam));
            return TRUE;
        }
        }
    }
    break;
    case WM_CLOSE:
    {
        EndDialog(hwnd, LOWORD(wParam));
    }
    break;
    }
    return TRUE;
}
//dumpshellcode到文件
BOOL  X64Store2File(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    BOOL bRet = FALSE;
    char szBegin[25] = { };
    char szLength[25] = { };
    duint duBegin = 0;
    duint duLength = 0;
    char* pBegin;
    char* plength;
    //获取用户输入的dump开始地址和长度
    GetDlgItemText(hwnd, EDT_Begin, szBegin, 25);
    GetDlgItemText(hwnd, EDT_LENGTH, szLength, 25);
    duBegin = strtoul(szBegin, &pBegin, 16);
    duLength = strtoul(szLength, &plength, 16);
    char* pReadbuf = new char[duLength];
    //读取内存中的shellcode
    bRet = DbgMemRead(duBegin, pReadbuf, duLength);
    if (!bRet)
    {
        return FALSE;
    }
    //初始化随机数，拼接保存shellcode文件名

    srand((unsigned)time(NULL));
    duint duRand = rand() % 100;
    HANDLE hDbg = DbgGetProcessHandle();
    char bufDbgName[MAX_PATH] = { };
    //获取被调试进程所在目录
    DWORD  dwRet = GetModuleFileNameExA(hDbg, NULL, bufDbgName, MAX_PATH);

    if (!dwRet)
    {
        return FALSE;
    }
    char bufRand[25] = { 0 };
    wsprintf(bufRand, "%d", duRand);
    string strexepath = bufDbgName;

    strexepath.erase(strexepath.length() - 4);
    strexepath = strexepath + bufRand + ".bin";
    HANDLE hFile = CreateFile(strexepath.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }
    DWORD dwWrite2Byte = 0;
    bRet = WriteFile(hFile, pReadbuf, duLength, &dwWrite2Byte, NULL);

    if (!bRet)
    {
        return FALSE;
    }
    MessageBox(hwnd, strexepath.c_str(), "写入成功", MB_OK);
    delete pReadbuf;
    return TRUE;
}