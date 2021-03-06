# XYOSCPlotter

XYOSCPlotter converts monocrome image into XY indexed array in order to be used in oscilloscopes XY mode.

 - two DAC channels (from the STM32F3_DISCO Board) are used simultaneously to parse both x & Y pixel data onto the screen.
 - high baud rate (3Mbaud) is used to send the images fluently.

![Continuous demo](https://github.com/ademkaya/XYOSCPlotter/blob/master/img/osc.cont.gif?raw=true)   ![Single Image](https://github.com/ademkaya/XYOSCPlotter/blob/master/img/2020-10-25%2014_30_15-20201025_142507.jpg%20%E2%80%8E-%20Photos.png?raw=true) ![Single Image](https://github.com/ademkaya/XYOSCPlotter/blob/master/img/2020-10-25%2014_30_49-20201025_142542.jpg%20%E2%80%8E-%20Photos.png?raw=true)

### Connections

The Oscillator probes must be connected on dual dac output which is shown on the image. Uart pins also must be placed as shown.

![Board Image](https://github.com/ademkaya/XYOSCPlotter/blob/master/img/boardImage.jpg?raw=true)
