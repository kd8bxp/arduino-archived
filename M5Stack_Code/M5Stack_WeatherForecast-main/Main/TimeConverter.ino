#include <stdio.h>
#include <time.h>

String timeConvert(int inputTime){
    time_t rawtime = inputTime;
    struct tm  ts;
    char       buf[80];
    // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
    ts = *localtime(&rawtime);
    strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
    return buf;
}
