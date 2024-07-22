#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <lmcons.h>
#include <string.h>

void OSInfo()
{
    OSVERSIONINFOEX osInfo;
    ZeroMemory(&osInfo, sizeof(OSVERSIONINFOEX));
    osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    if (GetVersionEx((OSVERSIONINFO *)&osInfo))
    {
        HKEY hKey;
        TCHAR productName[256];
        DWORD bufferSize = sizeof(productName);

        // opening the registry
        if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                         "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
                         0, KEY_READ, &hKey) == ERROR_SUCCESS)
        {

            // query product name from registry
            if (RegQueryValueEx(hKey, "ProductName", NULL, NULL,
                                (LPBYTE)productName, &bufferSize) == ERROR_SUCCESS)
            {
                printf("OS: %s\n", productName);
            }
            else
            {
                printf("Failed to retrieve product name.\n");
            }
            RegCloseKey(hKey);
        }
        else
        {
            printf("Failed to open registry key.\n");
        }

        // print version
        printf("Version: %d.%d (Build %d)\n",
               osInfo.dwMajorVersion,
               osInfo.dwMinorVersion,
               osInfo.dwBuildNumber);
    }
    else
    {
        printf("Failed to retrieve OS version.\n");
    }
}

void GetResolution(int *horizontal, int *vertical)
{
    RECT desktop;
    HWND hDesktop = GetDesktopWindow();
    // get the size of screen to the variable desktop
    GetWindowRect(hDesktop, &desktop);
    // (horizontal, vertical)
    *horizontal = desktop.right;
    *vertical = desktop.bottom;
}

void getPCManufacturer()
{
    FILE *fp;
    char path[1035];

    // open command
    fp = _popen("systeminfo | findstr /C:\"System Manufacturer\"", "r");
    if (fp == NULL)
    {
        printf("Failed to run command\n");
        exit(1);
    }

    // read output
    while (fgets(path, sizeof(path) - 1, fp) != NULL)
    {
        printf("%s", path);
    }

    // close
    _pclose(fp);
}

void getCPUInfo()
{
    char cpuInfo[64] = {0};
    DWORD bufSize = sizeof(cpuInfo);
    HKEY hKey;

    // Open the registry
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        RegQueryValueExA(hKey, "ProcessorNameString", NULL, NULL, (LPBYTE)cpuInfo, &bufSize);
        RegCloseKey(hKey);
    }

    printf("CPU: %s\n", cpuInfo);
}

void GetGPUInfo()
{
    DISPLAY_DEVICE DD;
    DD.cb = sizeof(DISPLAY_DEVICE);
    DWORD deviceIndex = 0;

    if (EnumDisplayDevices(NULL, deviceIndex, &DD, 0))
    {
        printf("GPU Name: %s\n", DD.DeviceString);
    }
    else
    {
        printf("Couldn't get GPU information\n");
    }
}

void checkDiskSpace(LPCSTR drive)
{
    DWORD sectorsPerCluster;
    DWORD bytesPerSector;
    DWORD freeClusters;
    DWORD totalClusters;

    BOOL result = GetDiskFreeSpaceA(
        drive,
        &sectorsPerCluster,
        &bytesPerSector,
        &freeClusters,
        &totalClusters);

    if (result)
    {
        DWORDLONG totalFreeBytes = (DWORDLONG)freeClusters * sectorsPerCluster * bytesPerSector;
        DWORDLONG totalDiskBytes = (DWORDLONG)totalClusters * sectorsPerCluster * bytesPerSector;

        // convert bytes to gigabytes
        double totalFreeGB = (double)totalFreeBytes / (1024 * 1024 * 1024);
        double totalDiskGB = (double)totalDiskBytes / (1024 * 1024 * 1024);

        printf("Drive: %s\n", drive);
        printf("Free Space: %.2f GB\n", totalFreeGB);
        printf("Disk Space: %.2f GB\n", totalDiskGB);
    }
    else
    {
        printf("Failed to get free space on disk for %s. Error code: %lu\n", drive, GetLastError());
    }
}

int main()
{
    OSInfo();

    Sleep(800);

    int horizontal = 0;
    int vertical = 0;
    GetResolution(&horizontal, &vertical);
    printf("Resolution: %d x %d\n", horizontal, vertical);

    Sleep(800);

    getPCManufacturer();

    Sleep(800);

    getCPUInfo();

    Sleep(800);

    GetGPUInfo();

    Sleep(800);

    checkDiskSpace("C:\\");

    Sleep(800);

    checkDiskSpace("D:\\");
    return 0;
}
