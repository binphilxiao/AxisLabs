import numpy as np
import matplotlib.pyplot as plt
from scipy.fftpack import fft, ifft
import pandas as pd

FFT_start_freq = 20
Length_data = 8192
Sample_rate = 1024
FFT_N = Sample_rate
Reference_freq = 20
Ref_or_data = 1 #1: data, 0: reference sin
File_number = 2

# Import csv file
if (File_number == 1):
    df = pd.read_csv('thinCord-A-01.csv')
elif (File_number == 2):
    df = pd.read_csv('thickCord-A-02.csv')
else:
    df = pd.read_csv('thinCord-A-01.csv')
y= np.array(df['Channel1(V)'])
print('The data in the CSV file is :')
print(df)
#plt.figure(figsize=(12,4))
#df.plot(linestyle = '-', marker = '.', color='b')
#plt.show()

# Reference Signal
t = np.arange(0,1,1.0/Length_data) # time vector
yr = np.sin(2*np.pi*Reference_freq*t)
#plt.plot(t,yr)
#plt.show()

#FFT
s=[]
if (Ref_or_data ==1):
    y_in = y
else:
    y_in = yr
for i in range(len(y_in)):
    if i % (Length_data//Sample_rate) == 0:
        s.append(y_in[i])
t = np.arange(0,1,1/FFT_N) # time vector
plt.plot(t,s)
plt.show()

#perform FFT on signal
yf = abs(fft(s))
#create new x-axis: frequency from signal
xf = np.linspace(0, FFT_N/2, FFT_N//2)
#plot results
plt.plot(xf[FFT_start_freq:FFT_N//2], yf[FFT_start_freq:FFT_N//2], label = 'signal')
plt.grid()
plt.xlabel('Frequency (Hz)')
plt.ylabel(r'Spectral Amplitude')
plt.legend(loc=1)
plt.show()




