//
// Created by daran on 1/12/2017 to be used in ECE420 Sp17 for the first time.
// Modified by dwang49 on 1/1/2018 to adapt to Android 7.0 and Shield Tablet updates.
//

#include "ece420_main.h"

// Student Variables
#define FRAME_SIZE 256

// FIR Filter Function Defined here located at the bottom
int16_t firFilter(int16_t sample);

void ece420ProcessFrame(sample_buf *dataBuf) {
    // Keep in mind, we only have a small amount of time to process each buffer!
    struct timeval start;
    gettimeofday(&start, NULL);

    // Using {} initializes all values in the array to zero
    int16_t bufferIn[FRAME_SIZE] = {};
    int16_t bufferOut[FRAME_SIZE] = {};
    uint8_t buffer[FRAME_SIZE*2] = {};
    uint8_t buffer_o[FRAME_SIZE*2] ={};
    for (int k=0; k<FRAME_SIZE*2-1;k++){
        buffer[k] = dataBuf -> buf_ [k];
    }
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
        //int16_t output = sample;
        // Grab result and put into bufferOut[]
        bufferOut[sampleIdx] = output;
    }

    // Your buffer conversion (packing) here
    // Fetch data sample from bufferOut[], pack them and put back into dataBuf->buf_[]
    // ******************** START YOUR CODE HERE ******************** //
    for (int j=0 ; j < FRAME_SIZE; j++){
        uint8_t temp = bufferOut[j];
        dataBuf -> buf_[2*j] = bufferOut[j];
        dataBuf -> buf_[2*j+1] = (((uint16_t)bufferOut[j]-temp)>>8);

    }
    for (int k=0 ; k < FRAME_SIZE; k++){
        buffer_o[k] = dataBuf -> buf_[k];
    }


    // ********************* END YOUR CODE HERE ********************* //

	// Log the processing time to Android Monitor or Logcat window at the bottom
    struct timeval end;
    gettimeofday(&end, NULL);
    LOGD("Loop timer: %ld us",  ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));

}

// TODO: Change N_TAPS to match your filter design
#define N_TAPS 51
// TODO: Change myfilter to contain the coefficients of your designed filter.
double myfilter[N_TAPS] = {0.008180792539787974 ,
                           0.00337409314563651 ,
                           -0.002733153283563368 ,
                           -0.008069113761747742 ,
                           -0.009925032654075648 ,
                           -0.005858383893969195 ,
                           0.005332949456855658 ,
                           0.02295701199554409 ,
                           0.04439247663267261 ,
                           0.06573995310414397 ,
                           0.08295204381726481 ,
                           0.09302773220936715 ,
                           0.09481279237136868 ,
                           0.08909449838617894 ,
                           0.07796271909842799 ,
                           0.06371523254874101 ,
                           0.047782739183163156 ,
                           0.0301496202404391 ,
                           0.009542342951647832 ,
                           -0.01567128871508223 ,
                           -0.046245571700661896 ,
                           -0.08102528482199799 ,
                           -0.11660481606651171 ,
                           -0.14789861244111951 ,
                           -0.16947488250868367 ,
                           1.8228160327457188 ,
                           -0.16947488250868367 ,
                           -0.14789861244111951 ,
                           -0.11660481606651171 ,
                           -0.08102528482199799 ,
                           -0.046245571700661896 ,
                           -0.01567128871508223 ,
                           0.009542342951647832 ,
                           0.0301496202404391 ,
                           0.047782739183163156 ,
                           0.06371523254874101 ,
                           0.07796271909842799 ,
                           0.08909449838617894 ,
                           0.09481279237136868 ,
                           0.09302773220936715 ,
                           0.08295204381726481 ,
                           0.06573995310414397 ,
                           0.04439247663267261 ,
                           0.02295701199554409 ,
                           0.005332949456855658 ,
                           -0.005858383893969195 ,
                           -0.009925032654075648 ,
                           -0.008069113761747742 ,
                           -0.002733153283563368 ,
                           0.00337409314563651 ,
                           0.008180792539787974 ,};



// Circular Buffer
int16_t circBuf[N_TAPS] = {};
int16_t circBufIdx = 0;


int ready = 0;
int16_t overflow(int16_t a){
    a = a+1;
    if (a<N_TAPS){
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

    //double myfilter[N_TAPS] = {0};
    //myfilter[0] = 1;
    if (ready == 0){
        if (circBufIdx == 44){
            ready = 1;}
        output = 0;
    }
    if (ready == 1){
        for (int i = 0; i < 45; i++){
            output = output+myfilter[i]*circBuf[overflow(circBufIdx+i)];
            //output = output + 1;
        }
        output = sample;
    }
    circBufIdx = overflow(circBufIdx);

    // ********************* END YOUR CODE HERE ********************* //
    return output;
}

