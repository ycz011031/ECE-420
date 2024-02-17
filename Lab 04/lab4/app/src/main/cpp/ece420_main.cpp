//
// Created by daran on 1/12/2017 to be used in ECE420 Sp17 for the first time.
// Modified by dwang49 on 1/1/2018 to adapt to Android 7.0 and Shield Tablet updates.
//

#include "ece420_main.h"
#include "ece420_lib.h"
#include "kiss_fft/kiss_fft.h"

// JNI Function
extern "C" {
JNIEXPORT float JNICALL
Java_com_ece420_lab4_MainActivity_getFreqUpdate(JNIEnv *env, jclass);
}

// Student Variables
#define F_S 48000
#define FRAME_SIZE 1024
#define VOICED_THRESHOLD (1800000000/2048)*FRAME_SIZE  // Find your own threshold
float lastFreqDetected = -1;


void ece420ProcessFrame(sample_buf *dataBuf) {
    // Keep in mind, we only have 20ms to process each buffer!
    struct timeval start;
    struct timeval end;
    gettimeofday(&start, NULL);

    // Data is encoded in signed PCM-16, little-endian, mono
    float bufferIn[FRAME_SIZE];
    for (int i = 0; i < FRAME_SIZE; i++) {
        int16_t val = ((uint16_t) dataBuf->buf_[2 * i]) | (((uint16_t) dataBuf->buf_[2 * i + 1]) << 8);
        bufferIn[i] = (float) val;
    }

    // ********************** PITCH DETECTION ************************ //
    // In this section, you will be computing the autocorrelation of bufferIn
    // and picking the delay corresponding to the best match. Naively computing the
    // autocorrelation in the time domain is an O(N^2) operation and will not fit
    // in your timing window.
    //
    // First, you will have to detect whether or not a signal is voiced.
    // We will implement a simple voiced/unvoiced detector by thresholding
    // the power of the signal.
    //
    // Next, you will have to compute autocorrelation in its O(N logN) form.
    // Autocorrelation using the frequency domain is given as:
    //
    //  autoc = ifft(fft(x) * conj(fft(x)))
    //
    // where the fft multiplication is element-wise.
    //
    // You will then have to find the index corresponding to the maximum
    // of the autocorrelation. Consider that the signal is a maximum at idx = 0,
    // where there is zero delay and the signal matches perfectly.
    //
    // Finally, write the variable "lastFreqDetected" on completion. If voiced,
    // write your determined frequency. If unvoiced, write -1.
    // ********************* START YOUR CODE HERE *********************** //
    //float threshold = (1800000000/2048)*FRAME_SIZE;


    float E = 0;
    for (int i = 0; i<FRAME_SIZE; i++){
        E += bufferIn[i] * bufferIn[i];
    }

    if (E < VOICED_THRESHOLD){
        lastFreqDetected = -1;

    }
    else {

        kiss_fft_cpx fin[FRAME_SIZE];
        kiss_fft_cpx fout[FRAME_SIZE];
        kiss_fft_cpx fmulti[FRAME_SIZE];
        kiss_fft_cpx output[FRAME_SIZE];

        for (int k = 0; k < FRAME_SIZE; k++) {
            fin[k].r = bufferIn[k];
            fin[k].i = 0;
        }
        kiss_fft_cfg cfg = kiss_fft_alloc(FRAME_SIZE, 0, NULL, NULL);
        kiss_fft(cfg, fin, fout);
        for (int j = 0; j < FRAME_SIZE; j++) {
            fmulti[j].r = fout[j].r * fout[j].r - (-1 * fout[j].i) * fout[j].i;
            fmulti[j].i = fout[j].r * fout[j].i + (-1 * fout[j].i) * fout[j].r;
        }
        kiss_fft_cfg cfg_ = kiss_fft_alloc(FRAME_SIZE, 1, NULL, NULL);
        kiss_fft(cfg_, fmulti, output);

        float stuff[FRAME_SIZE];
        for (int itr = 0; itr < FRAME_SIZE; itr++) {
            stuff[itr] = output[itr].r;
        }

        int maxIdx = findMaxArrayIdx(stuff, int(F_S / 270), int(F_S / 60));

        lastFreqDetected = F_S / maxIdx;
    }







    // ********************* END YOUR CODE HERE ************************* //
    gettimeofday(&end, NULL);
    LOGD("Time delay: %ld us",  ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
}

JNIEXPORT float JNICALL
Java_com_ece420_lab4_MainActivity_getFreqUpdate(JNIEnv *env, jclass) {
    return lastFreqDetected;
}