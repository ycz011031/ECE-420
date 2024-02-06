//
// Created by daran on 1/12/2017 to be used in ECE420 Sp17 for the first time.
// Modified by dwang49 on 1/1/2018 to adapt to Android 7.0 and Shield Tablet updates.
//

#include "ece420_main.h"

// Student Variables
#define FRAME_SIZE 128

// FIR Filter Function Defined here located at the bottom
int16_t firFilter(int16_t sample);

void ece420ProcessFrame(sample_buf *dataBuf) {
    // Keep in mind, we only have a small amount of time to process each buffer!
    struct timeval start;
    gettimeofday(&start, NULL);

    // Using {} initializes all values in the array to zero
    int16_t bufferIn[FRAME_SIZE] = {};
    int16_t bufferOut[FRAME_SIZE] = {};

    // Your buffer conversion (unpacking) here
    // Fetch data sample from dataBuf->buf_[], unpack and put into bufferIn[]
    // ******************** START YOUR CODE HERE ******************** //

    for (int i = 0; i< FRAME_SIZE; i++){
        bufferIn[i] = (int16_t)dataBuf->buf_[2*i]+(int16_t)dataBuf->buf_[2*i+1]*256;
    }



    // ********************* END YOUR CODE HERE ********************* //

    // Loop code provided as a suggestion. This loop simulates sample-by-sample processing.
    for (int sampleIdx = 0; sampleIdx < FRAME_SIZE; sampleIdx++) {
        // Grab one sample from bufferIn[]
        int16_t sample = bufferIn[sampleIdx];
        // Call your filFilter funcion
        int16_t output = firFilter(sample);
        // Grab result and put into bufferOut[]
        bufferOut[sampleIdx] = output;
    }

    // Your buffer conversion (packing) here
    // Fetch data sample from bufferOut[], pack them and put back into dataBuf->buf_[]
    // ******************** START YOUR CODE HERE ******************** //
    for (int j=0 ; j < FRAME_SIZE; j++){
        auto temp = bufferOut[j];
        dataBuf -> buf_[2*j+1] = (uint8_t)(temp/256);
        temp=temp*256;
        dataBuf -> buf_[2*j] = (uint8_t)(temp/256);
    }


    // ********************* END YOUR CODE HERE ********************* //

	// Log the processing time to Android Monitor or Logcat window at the bottom
    struct timeval end;
    gettimeofday(&end, NULL);
    LOGD("Loop timer: %ld us",  ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));

}

// TODO: Change N_TAPS to match your filter design
#define N_TAPS 45
// TODO: Change myfilter to contain the coefficients of your designed filter.
double myfilter[N_TAPS] = {0.009484189519882358 ,
                           0.01511762943375239 ,
                           0.02121561332719457 ,
                           0.027342760421280924 ,
                           0.03297430386723425 ,
                           0.037552702093525 ,
                           0.04055468230137409 ,
                           0.04155847492157301 ,
                           0.04030053700470133 ,
                           0.03671253542439821 ,
                           0.030932533666379772 ,
                           0.023288633486990912 ,
                           0.01425797311578193 ,
                           0.004408110340458147 ,
                           -0.005669351573772391 ,
                           -0.015422033132267844 ,
                           -0.02438708163178623 ,
                           -0.03222144487538158 ,
                           -0.038709094766853414 ,
                           -0.04374667324388587 ,
                           -0.047313100307905614 ,
                           -0.049431536288688745 ,
                           0.9498668179269525 ,
                           -0.049431536288688745 ,
                           -0.047313100307905614 ,
                           -0.04374667324388587 ,
                           -0.038709094766853414 ,
                           -0.03222144487538158 ,
                           -0.02438708163178623 ,
                           -0.015422033132267844 ,
                           -0.005669351573772391 ,
                           0.004408110340458147 ,
                           0.01425797311578193 ,
                           0.023288633486990912 ,
                           0.030932533666379772 ,
                           0.03671253542439821 ,
                           0.04030053700470133 ,
                           0.04155847492157301 ,
                           0.04055468230137409 ,
                           0.037552702093525 ,
                           0.03297430386723425 ,
                           0.027342760421280924 ,
                           0.02121561332719457 ,
                           0.01511762943375239 ,
                           0.009484189519882358 ,};

// Circular Buffer
int16_t circBuf[N_TAPS] = {};
int16_t circBufIdx = 0;


int ready = 0;
int16_t overflow(int16_t a){
    a = a+1;
    if (a<45){
        return a;
    }
    else return 0;
}

// FirFilter Function
int16_t firFilter(int16_t sample) {
    // This function simulates sample-by-sample processing. Here you will
    // implement an FIR filter such as:
    //
    // y[n] = a x[n] + b x[n-1] + c x[n-2] + ...
    //
    // You will maintain a circular buffer to store your prior samples
    // x[n-1], x[n-2], ..., x[n-k]. Suggested initializations circBuf
    // and circBufIdx are given.
    //
    // Input 'sample' is the current sample x[n].
    // ******************** START YOUR CODE HERE ******************** //
    int16_t output = 0;
    circBuf[circBufIdx] = sample;


    if (ready == 0 && circBufIdx == 44){
        ready = 1;
        circBufIdx = 0;
    }
    if (ready == 1){
        for (int i = 0; i < 45; i++){
            output = output+myfilter[i]*circBuf[overflow(circBufIdx+i)];
        }
    }
    circBufIdx = overflow(circBufIdx);

    // ********************* END YOUR CODE HERE ********************* //
    return output;
}

