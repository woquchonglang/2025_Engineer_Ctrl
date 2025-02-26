#include "kalman.h"

//初始化卡尔曼滤波器
void KalmanFilter_Init(KalmanFilter* kf, double initial_value, double process_noise, double measurement_noise) {
    kf->x = initial_value;
    kf->p = 1;
    kf->q = process_noise;
    kf->r = measurement_noise;
}

//卡尔曼滤波器更新预测
double KalmanFilter_Update(KalmanFilter* kf, double measurement) {
    kf->p = kf->p + kf->q;

    double kg = kf->p / (kf->p + kf->r);
    kf->x = kf->x + kg * (measurement - kf->x);
    kf->p = (1 - kg) * kf->p;

    return kf->x;
}
