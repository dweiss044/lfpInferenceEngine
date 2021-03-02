#include <lfpRatiometer.h>

int main () {
    int N = 10;
    double sampling = 200;
    lfpRatiometer lfpratiometer(N, sampling);
    lfpratiometer.execute();

    return 0;

}