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
#define N_TAPS 101
// TODO: Change myfilter to contain the coefficients of your designed filter.
double myfilter[N_TAPS] = {-0.0025946353538358826 ,
                           -0.005066523700642792 ,
                           -0.00787823843267169 ,
                           -0.01031058851278237 ,
                           -0.011580461142652154 ,
                           -0.01113096852002554 ,
                           -0.008890047656415401 ,
                           -0.005380114289940278 ,
                           -0.0016045964714621825 ,
                           0.0012788235710980724 ,
                           0.0023944437052455286 ,
                           0.0015267071889856606 ,
                           -0.0007278040817824968 ,
                           -0.0031303090431338853 ,
                           -0.004263296477467854 ,
                           -0.0031298898528997216 ,
                           0.00034760361539617503 ,
                           0.005155026993111039 ,
                           0.009455856729048745 ,
                           0.011247693896716633 ,
                           0.0091819520462772 ,
                           0.0032284917934017215 ,
                           -0.005105491439253691 ,
                           -0.013142191464794572 ,
                           -0.01790163195147312 ,
                           -0.017176868339881277 ,
                           -0.01041080833198116 ,
                           0.000985760075858028 ,
                           0.014041150946273053 ,
                           0.025244778519503228 ,
                           0.03182216399583356 ,
                           0.03277709285957583 ,
                           0.02927812436276814 ,
                           0.024203065080395547 ,
                           0.020979935588927502 ,
                           0.022150676758119513 ,
                           0.028216714620828184 ,
                           0.037242205673912385 ,
                           0.045414023637213 ,
                           0.04838885375176881 ,
                           0.04293999197838835 ,
                           0.02827795102978977 ,
                           0.006521421677617363 ,
                           -0.01789361408247961 ,
                           -0.03967494236347559 ,
                           -0.05447347073142796 ,
                           -0.060347667328368534 ,
                           -0.058337173794161014 ,
                           -0.05192454399403468 ,
                           -0.04558902812044614 ,
                           0.9569933615385475 ,
                           -0.04558902812044614 ,
                           -0.05192454399403468 ,
                           -0.058337173794161014 ,
                           -0.060347667328368534 ,
                           -0.05447347073142796 ,
                           -0.03967494236347559 ,
                           -0.01789361408247961 ,
                           0.006521421677617363 ,
                           0.02827795102978977 ,
                           0.04293999197838835 ,
                           0.04838885375176881 ,
                           0.045414023637213 ,
                           0.037242205673912385 ,
                           0.028216714620828184 ,
                           0.022150676758119513 ,
                           0.020979935588927502 ,
                           0.024203065080395547 ,
                           0.02927812436276814 ,
                           0.03277709285957583 ,
                           0.03182216399583356 ,
                           0.025244778519503228 ,
                           0.014041150946273053 ,
                           0.000985760075858028 ,
                           -0.01041080833198116 ,
                           -0.017176868339881277 ,
                           -0.01790163195147312 ,
                           -0.013142191464794572 ,
                           -0.005105491439253691 ,
                           0.0032284917934017215 ,
                           0.0091819520462772 ,
                           0.011247693896716633 ,
                           0.009455856729048745 ,
                           0.005155026993111039 ,
                           0.00034760361539617503 ,
                           -0.0031298898528997216 ,
                           -0.004263296477467854 ,
                           -0.0031303090431338853 ,
                           -0.0007278040817824968 ,
                           0.0015267071889856606 ,
                           0.0023944437052455286 ,
                           0.0012788235710980724 ,
                           -0.0016045964714621825 ,
                           -0.005380114289940278 ,
                           -0.008890047656415401 ,
                           -0.01113096852002554 ,
                           -0.011580461142652154 ,
                           -0.01031058851278237 ,
                           -0.00787823843267169 ,
                           -0.005066523700642792 ,
                           -0.0025946353538358826 , };



// Circular Buffer
int16_t circBuf[N_TAPS] = {};
int16_t circBufIdx = 0;


int ready = 0;
int16_t overflow(int16_t a){
    //a = a+1;
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
        if (circBufIdx == N_TAPS-1){
            ready = 1;}
        output = 0;
    }
    if (ready == 1){
        for (int i = 0; i < N_TAPS; i++){
            output = output+myfilter[i]*circBuf[overflow(circBufIdx+i)];
            //output = output + 1;
        }
        output = sample;
    }
    circBufIdx = overflow(circBufIdx+1);

    // ********************* END YOUR CODE HERE ********************* //
    return output;
}

