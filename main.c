#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int readMeminfo(void)
{
    FILE *input = fopen("/proc/meminfo", "r");

    if (input == NULL)
    {
        perror("/proc/meminfo");
        return 1;
    }

    char line[256];

    unsigned long memTotal = 0;
    unsigned long memAvailable = 0;
    unsigned long memFree = 0;
    unsigned long buffers = 0;
    unsigned long cached = 0;
    unsigned long swapTotal = 0;
    unsigned long swapFree = 0;

    while (fgets(line, sizeof(line), input) != NULL)
    {
        if (strncmp(line, "MemTotal:", 9) == 0)
            sscanf(line, "MemTotal: %lu kB", &memTotal);

        else if (strncmp(line, "MemAvailable:", 13) == 0)
            sscanf(line, "MemAvailable: %lu kB", &memAvailable);

        else if (strncmp(line, "MemFree:", 8) == 0)
            sscanf(line, "MemFree: %lu kB", &memFree);

        else if (strncmp(line, "Buffers:", 8) == 0)
            sscanf(line, "Buffers: %lu kB", &buffers);

        else if (strncmp(line, "Cached:", 7) == 0)
            sscanf(line, "Cached: %lu kB", &cached);

        else if (strncmp(line, "SwapTotal:", 10) == 0)
            sscanf(line, "SwapTotal: %lu kB", &swapTotal);

        else if (strncmp(line, "SwapFree:", 9) == 0)
            sscanf(line, "SwapFree: %lu kB", &swapFree);
    }

    fclose(input);

    double totalGb = memTotal / (1024.0 * 1024.0);
    double availableGb = memAvailable / (1024.0 * 1024.0);
    double usedGb = totalGb - availableGb;
    double usagePercent = (usedGb / totalGb) * 100.0;

    printf("===== MEMORY =====\n");
    printf("Total     : %.2f GB\n", totalGb);
    printf("Used      : %.2f GB\n", usedGb);
    printf("Available : %.2f GB\n", availableGb);
    printf("Usage     : %.1f%%\n", usagePercent);
    printf("==================\n\n");

    return 0;
}

int timeKeep(void)
{
    FILE *input = fopen("/proc/uptime", "r");

    if (input == NULL)
    {
        perror("/proc/uptime");
        return 1;
    }

    double uptimeSeconds = 0;

    fscanf(input, "%lf", &uptimeSeconds);

    fclose(input);

    unsigned long hours = (unsigned long)(uptimeSeconds / 3600);
    unsigned long minutes = ((unsigned long)uptimeSeconds / 60) % 60;

    printf("===== SYSTEM =====\n");
    printf("Uptime : %luh %lum\n", hours, minutes);
    printf("==================\n\n");

    return 0;
}

int loadAvg(void)
{
    FILE *input = fopen("/proc/loadavg", "r");

    if (input == NULL)
    {
        perror("/proc/loadavg");
        return 1;
    }

    double load1 = 0;
    double load5 = 0;
    double load15 = 0;

    fscanf(input, "%lf %lf %lf", &load1, &load5, &load15);

    fclose(input);

    printf("===== LOAD =====\n");
    printf("1 min  : %.2f\n", load1);
    printf("5 min  : %.2f\n", load5);
    printf("15 min : %.2f\n", load15);
    printf("================\n\n");

    return 0;
}

int cpuUsage(void)
{
    unsigned long user1, nice1, system1, idle1;
    unsigned long iowait1, irq1, softirq1, steal1;

    unsigned long user2, nice2, system2, idle2;
    unsigned long iowait2, irq2, softirq2, steal2;

    FILE *input = fopen("/proc/stat", "r");

    if (input == NULL)
    {
        perror("/proc/stat");
        return 1;
    }

    fscanf(input,
           "cpu %lu %lu %lu %lu %lu %lu %lu %lu",
           &user1, &nice1, &system1, &idle1,
           &iowait1, &irq1, &softirq1, &steal1);

    fclose(input);

    usleep(200000); // 200 ms

    input = fopen("/proc/stat", "r");

    if (input == NULL)
    {
        perror("/proc/stat");
        return 1;
    }

    fscanf(input,
           "cpu %lu %lu %lu %lu %lu %lu %lu %lu",
           &user2, &nice2, &system2, &idle2,
           &iowait2, &irq2, &softirq2, &steal2);

    fclose(input);

    unsigned long idleDelta =
        (idle2 + iowait2) - (idle1 + iowait1);

    unsigned long total1 =
        user1 + nice1 + system1 + idle1 +
        iowait1 + irq1 + softirq1 + steal1;

    unsigned long total2 =
        user2 + nice2 + system2 + idle2 +
        iowait2 + irq2 + softirq2 + steal2;

    unsigned long totalDelta = total2 - total1;

    double cpuPercent =
        100.0 * (totalDelta - idleDelta) / totalDelta;

    printf("===== CPU =====\n");
    printf("Usage : %.1f%%\n", cpuPercent);
    printf("================\n\n");

    return 0;
}

double thermals(void)
{
    FILE *input =
        fopen("/sys/class/thermal/thermal_zone0/temp", "r");

    if (input == NULL)
    {
        perror("thermal_zone0/temp");
        return -1.0;
    }

    int tempMilliC = 0;

    fscanf(input, "%d", &tempMilliC);

    fclose(input);

    double tempC = tempMilliC / 1000.0;

    printf("===== THERMALS =====\n");
    printf("CPU Temp : %.1f°C\n", tempC);
    printf("====================\n\n");

    return tempC;
}
int network(void){
    FILE *input = fopen("/proc/net/dev", "r");
    unsigned long rx=0;
    unsigned long tx =0;
    char line[1000];
    if(input == NULL){
        printf("error!");
        return 1;
    }
    while(fgets(line,sizeof(line),input)!=NULL){
        if(strncmp(line,"wlan0:",6)==0){
            sscanf(line,"wlan0: %lu %*u %*u %*u %*u %*u %*u %*u %lu",&rx ,&tx);
            break;
        }
    }

    printf("===== NETWORK =====\n");
    printf("RX Bytes : %lu\n", rx);
    printf("TX Bytes : %lu\n", tx);
    printf("===================\n");

    fclose(input);

    return 0;
}

int main(void)
{
    readMeminfo();
    timeKeep();
    loadAvg();
    cpuUsage();
    thermals();

    return 0;
}