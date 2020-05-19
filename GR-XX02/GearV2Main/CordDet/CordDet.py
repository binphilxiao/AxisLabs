import numpy as np
import matplotlib.pyplot as plt
from scipy.fftpack import fft, ifft
import pandas as pd
import math
#---------------------Design Parameter---------------------
Length_data = 8192
Sample_rate = 200
FFT_N = 256
Reference_freq = 20
Ref_or_data = 1 #1: data, 0: reference sin
File_number = 1
Log_or_Raw = 0 #1: Log y axis, 0: Raw value
#---------------------Reading data----------------------------
if (File_number == 1):
    df = pd.read_csv('thinCord-A-01.csv')
elif (File_number == 2):
    df = pd.read_csv('thickCord-A-02.csv')
else:
    df = pd.read_csv('thinCord-A-01.csv')
y_raw= np.array(df['Channel1(V)'])
#print('The data in the CSV file is :')
#print(df)
#plt.figure(figsize=(12,4))
#df.plot(linestyle = '-', marker = '.', color='b')
#plt.show()
#---------------------Reference Signal----------------
t = np.arange(0,1,1.0/Length_data) # time vector
y_ref = np.sin(2*np.pi*Reference_freq*t)
#plt.plot(t,yr)
#plt.show()
#---------------------Sampled data--------------------
sample=[]
if (Ref_or_data ==1):
    y_in = y_raw
else:
    y_in = y_ref
sum = 0.0
for i in range(len(y_in)):
    sum += y_in[i]
    if i % (Length_data//FFT_N) == Length_data//FFT_N-1:
        sample.append(sum * FFT_N / Length_data)
        sum = 0.0
t = np.arange(0,1*FFT_N/Sample_rate,1/Sample_rate) # time vector
plt.grid()
plt.xlabel('Time (Sec)')
plt.ylabel('Amplitude (V)')
plt.plot(t,sample,label = 'Sampled Signal')
plt.legend(loc=4)
plt.show()
#---------------------FFT-----------------------------
x_fft = np.linspace(0, Sample_rate/2, FFT_N//2)
if Log_or_Raw == 1:
    y_fft = np.log(abs(fft(sample)))
else:
    y_fft = abs(fft(sample))
plt.plot(x_fft[1:FFT_N//2], y_fft[1:FFT_N//2], label = 'FFT of sampled signal')
plt.grid()
plt.xlabel('Frequency (Hz)')
if Log_or_Raw == 1:
    plt.ylabel('Spectral Amplitude (dB)')
else:
    plt.ylabel('Spectral Amplitude')
plt.legend(loc=1)
plt.show()
#---------------------Filter--------------------------
#   Sample rate =   1k Hz
#   Fc =            30.5 Hz
#   Bw =            50.5 Hz
#   N_Poles =       2
#   Type =          Butterworth
#   dB_gain =       20
#   a0   1.000000000000000000
#   a1   -1.894000291715878030
#   a2   0.905724944304319823
#   b0   -0.438395206537278315
#   b1   0.000000000000000000
#   b2   0.438395206537278315
N_Poles = 2
b = (-0.438395206537278315,
     0.000000000000000000,
     0.438395206537278315)
a = (1.000000000000000000,
     -1.894000291715878030,
     0.905724944304319823)
regX = []
regY = []
y=[]
for j in range(20):
    regX.append(0.0)
    regY.append(0.0)
for j in range(FFT_N):
    y.append(0.0)
for j in range(FFT_N):
    for k in range(N_Poles,0,-1):
        regX[k] = regX[k-1]
        regY[k] = regY[k-1]
    CenterTap = 0.0
    regX[0] = sample[j]
    for k in range(0,N_Poles+1):
        CenterTap += b[k]* regX[k]
    regY[0] = CenterTap * a[0]
    for k in range(1,N_Poles+1):
        regY[0] -= a[k]* regY[k]
    y[j] = regY[0]      
x1 = np.linspace(-0.512, 0.512, FFT_N)
plt.plot(x1[100:FFT_N], y[100:FFT_N], label = 'Filtered signal')
plt.grid()
plt.xlabel('Time (Sec)')
plt.ylabel('Amplitude (V)')
plt.legend(loc=1)
plt.show()
#---------------------FFT Filtered----------------------
if Log_or_Raw == 1:
    y_fft = np.log(abs(fft(y)))
else:
    y_fft = abs(fft(y))
x_fft = np.linspace(0, Sample_rate/2, FFT_N//2)
plt.grid()
plt.xlabel('Frequency (Hz)')
if Log_or_Raw == 1:
    plt.ylabel('Spectral Amplitude (dB)')
else:
    plt.ylabel('Spectral Amplitude')
plt.plot(x_fft[0:FFT_N//2], y_fft[0:FFT_N//2], label = 'Filtered FFT')
plt.legend(loc=1)
plt.show()
    











