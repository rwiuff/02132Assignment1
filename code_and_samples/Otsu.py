import numpy as np
import matplotlib.pyplot as plt
import math

img = plt.imread('example.bmp')
# img = plt.imread('7EASY.bmp')
img = img.astype(np.float64)
wimg = np.zeros((950, 950), dtype=np.longdouble)
for i in range(950):
    for j in range(950):
        wimg[i, j] = (img[i, j, 0] + img[i, j, 1] + img[i, j, 2]) / 3
hist, bins = np.histogram(wimg, bins=255, range=(0, 256))
# plt.stairs(hist, bins)
# plt.show()
numpixels = 950 * 950
p0 = np.zeros(256)
p1 = np.zeros(256)
C = np.zeros(256)
C[0] = hist[0] / numpixels
for t in range(1, 254):
    C[t] = C[t-1] + (hist[t]/numpixels)
# plt.plot(bins, C)
# plt.show()
var = np.zeros(256)
for t in range(1, 255):
    w0 = 0.000001
    w1 = 1
    for i in range(0, t-1):
        w0 = w0 + C[i]
    for i in range(t, 255):
        w1 = w1 + C[i]
    m0 = 0
    for i in range(0, t-1):
        m0 = m0 + i * C[i]
    m0 = m0 / w0
    m1 = 0
    for i in range(t, 255):
        m0 = m0 + i * C[i]
    m1 = m1 / w1
    var[t] = w0*w1*pow(m0-m1, 2)
print(np.max(var))
print(np.argmax(var))
plt.plot(bins, var)
plt.show()
