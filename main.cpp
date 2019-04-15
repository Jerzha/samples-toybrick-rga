//
// Created by Jeffery on 2019-04-15.
//
#include <rockchip_rga/rockchip_rga.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <linux/videodev2.h>
#include <cstring>

#define RBS_ALIGN(x, a) (((x)+(a)-1)&~((a)-1))

#define BUFFER_WIDTH 4096
#define BUFFER_HEIGHT 2160
#define BUFFER_SIZE BUFFER_WIDTH*BUFFER_HEIGHT*4

unsigned char srcBuffer[BUFFER_SIZE] = {0};
unsigned char dstBuffer[BUFFER_SIZE] = {0};

#define NANOTIME_PER_MSECOND 1000000L
static unsigned long long nanoTime(void) {
    struct timespec t;

    t.tv_sec = t.tv_nsec = 0;
    clock_gettime(CLOCK_MONOTONIC, &t);

    return (unsigned long long)(t.tv_sec) * 1000000000 + t.tv_nsec;
}

void randomData() {
    srand((unsigned)time(nullptr));
    for (unsigned char &i : srcBuffer) {
        i = (unsigned char)rand();
    }
}

bool checkData() {
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        if (srcBuffer[i] != dstBuffer[i]) {
            return false;
        }
    }
    return true;
}

void rga_copy() {
    RockchipRga *mRga;
    if (!mRga) {
        printf("create rga failed !\n");
        abort();
    }
    mRga->ops->initCtx(mRga);

    mRga->ops->setSrcFormat(mRga, V4L2_PIX_FMT_ABGR32, BUFFER_WIDTH, BUFFER_HEIGHT);
    mRga->ops->setDstFormat(mRga, V4L2_PIX_FMT_ABGR32, BUFFER_WIDTH, BUFFER_HEIGHT);

    mRga->ops->setSrcBufferPtr(mRga, srcBuffer);
    mRga->ops->setDstBufferPtr(mRga, dstBuffer);

    mRga->ops->go(mRga);
}

int main() {
    randomData();
    if (checkData()) {
        printf("WTF ...\n");
        return -1;
    }

    long long begintime = nanoTime();
    rga_copy();
    long long endtime = nanoTime() - begintime;
    printf("RGA copy time : %lld\n", endtime/NANOTIME_PER_MSECOND);

    if (!checkData()) {
        printf("RGA copy buffer error !");
        return -1;
    }

    begintime = nanoTime();
    memcpy(dstBuffer, srcBuffer, BUFFER_SIZE);
    endtime = nanoTime() - begintime;
    printf("Memcpy copy time : %lld\n", endtime/NANOTIME_PER_MSECOND);

    if (!checkData()) {
        printf("Memcpy copy buffer error !");
        return -1;
    }

    return 0;
}