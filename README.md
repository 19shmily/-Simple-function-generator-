# -Simple-Signal-generator-
基于stm32f407的信号发生器，需要连上串口后调试使用，内置有正弦波、三角波和正弦波。
频率和幅度都可以通过串口调节。
频率范围为100Hz--200kHz。
幅度最多为3.3v。
还可以自定义波形，通过输入50个采样点从而得到自定义波形。
不足之处：不能脱离串口单独使用。


The signal generator based on STM32F407 needs to be connected to the serial port and debugged, and has built-in sine wave, triangle wave and sine wave. 
Both the frequency and amplitude can be adjusted through the serial port. 
The frequency range is 100Hz--200kHz.
The amplitude is up to 3.3V. 
You can also customize the waveform, which can be obtained by entering 50 sample points.
Disadvantages: It cannot be used separately from the serial port.
