#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <lmcons.h>

// func that takes a string representing C:\\ and returns void
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

        printf("Drive: %s\n", drive);
        printf("Free Space: %llu bytes\n", totalFreeBytes);
        printf("Disk Space: %llu bytes\n", totalDiskBytes);
    }
    else
    {
        printf("Failed to get free space on disk for %s. Error code: %lu\n", drive, GetLastError());
    }
}

int main()
{
    checkDiskSpace("C:\\");
    checkDiskSpace("D:\\");
    return 0;
}