import numpy as np
from numpy.fft import fft
import matplotlib.pyplot as plt
import scipy.io.wavfile as spwav
#from mpldatacursor import datacursor
import sys

plt.style.use('ggplot')

# Note: this epoch list only holds for "test_vector_all_voiced.wav"
epoch_marks_orig = np.load("test_vector_all_voiced_epochs.npy")
F_s, audio_data = spwav.read("test_vector_all_voiced.wav")
N = len(audio_data)

######################## YOUR CODE HERE ##############################

F_new = 420
new_epoch_spacing = F_s//F_new

#print (epoch_marks_orig)


audio_out = np.zeros(N)
# Suggested loop


#since the mapped epoch on original(index) is non-decreasing
#we only need to start there to find the next map for the next new epoch
def find_map (new_epoch, epoc_org, epoch_mark):
    itr = epoch_mark
    for k in range (epoch_mark,len(epoc_org)):
        
        if (new_epoch == epoch_mark):
            delta_min = abs(new_epoch - epoch_marks_orig[k])
        else:
            delta_new = abs(new_epoch - epoch_marks_orig[k])
            if (delta_new <= delta_min):
                delta_min = delta_new
                itr = k
            else:
                print('triggered')
                break
    return itr



epoch_mark = 0
epoch_mark_array = []
itr = 0


for i in range(0, N, new_epoch_spacing):

    # https://courses.engr.illinois.edu/ece420/lab5/lab/#overlap-add-algorithm
    # Your OLA code here
    itr = find_map(i,epoch_marks_orig,epoch_mark)
    epoch_mark = itr
    print (itr)

    window = np.hanning(epoch_marks_orig[((itr)-1)]-epoch_marks_orig[itr+1]+1)
    windowed_sample = audio_data[epoch_marks_orig[itr-1]:epoch_marks_orig[itr+1]+1] * window

    audio_out[(i-new_epoch_spacing):(i+new_epoch_spacing+1)] += windowed_sample

plt.plot(audio_out)

audio_out = audio_out.astype('int16')
spwav.write('audio_out.wav',F_s,audio_out)




    
    
           
    
