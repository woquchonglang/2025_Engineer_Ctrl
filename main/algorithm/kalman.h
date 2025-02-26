#ifndef Kalman_filter
#define Kalman_filter

#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
  double x; // state
  double p; // eetimated error convariance
  double q; // process_noise
  double r; // measurement_noise
} KalmanFilter;

void KalmanFilter_Init(KalmanFilter *kf, double initial_value,
                       double process_noise, double measurement_noise);
double KalmanFilter_Update(KalmanFilter *kf, double measurement);
#ifdef __cplusplus
}
#endif
#endif
