// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pkg.h"

int g_ispurple = 0;
HMODULE g_dllMod;
HWND  g_Hwnd;
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_dllMod = hModule;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

//初始化插件
bool pluginit(PLUG_INITSTRUCT* initStruct) {
    initStruct->sdkVersion = PLUG_SDKVERSION;
    initStruct->pluginVersion = 1;
    const char* name = "SuperX64";             //你的插件名称
    memset(initStruct->pluginName, 0, 128);
    memcpy(initStruct->pluginName, name, strlen(name));
    return true;
}
//初始化菜单
void plugsetup(PLUG_SETUPSTRUCT* setupStruct) {
    //“相对基址”，这里直接采用utf8编码
    char szPluginName[] = { "Dump"};
    //注意添加的是二级菜单，一级的名称默认跟插件名称一致
    _plugin_menuaddentry(setupStruct->hMenu, 1, szPluginName);
    g_Hwnd = setupStruct->hwndDlg;
}
//菜单回调事件
void CBMENUENTRY(CBTYPE cbType, PLUG_CB_MENUENTRY* info) {
    
    HWND  hDia = CreateDialog(g_dllMod, MAKEINTRESOURCE(DLG_X64), g_Hwnd, X64DialogProc);
    ShowWindow(hDia, SW_SHOW);
    UpdateWindow(hDia);

}



//DbgGetProcessHandle()
//BRIDGE_IMPEXP const char* DbgInit();
//BRIDGE_IMPEXP void DbgExit();
//BRIDGE_IMPEXP bool DbgMemRead(duint va, void* dest, duint size);
//BRIDGE_IMPEXP bool DbgMemWrite(duint va, const void* src, duint size);
//BRIDGE_IMPEXP duint DbgMemGetPageSize(duint base);
//BRIDGE_IMPEXP duint DbgMemFindBaseAddr(duint addr, duint* size);