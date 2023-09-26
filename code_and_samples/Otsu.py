import numpy as np
import matplotlib.pyplot as plt
import math

img = plt.imread('6HARD.bmp')
# img = img.astype(np.float64)
# wimg = np.zeros((950, 950), dtype=np.longdouble)
# for i in range(950):
#     for j in range(950):
#         wimg[i, j] = (img[i, j, 0] + img[i, j, 1] + img[i, j, 2]) / 3
# hist, bins = np.histogram(wimg, bins=255, range=(0, 256))
# plt.stairs(hist, bins)
# plt.show()
# numpixels = 950 * 950
# C = np.zeros(256)
# C[0] = hist[0] / numpixels
# for t in range(1, 255):
#     C[t] = C[t-1] + (hist[t]/numpixels)
# plt.plot(bins, C)
# plt.show()
# var = np.zeros(256)

# for t in range(0, 256):
#     w0 = 0.1
#     w1 = 0.1
#     for i in range(0, t-1):
#         w0 = w0 + C[i]
#     for i in range(t, 255):
#         w1 = w1 + C[i]
#     m0 = 0
#     for i in range(0, t-1):
#         m0 = m0 + i * C[i]
#     m0 = m0 / w0
#     m1 = 0
#     for i in range(t, 255):
#         m1 = m1 + i * C[i]
#     m1 = m1 / w1
#     var[t] = w0*w1*math.pow(m0-m1, 2)
# print(np.max(var))
# print(np.argmax(var))
# plt.plot(bins, var)
# plt.show()

def compute_otsu_criteria(im, th):
    """Otsu's method to compute criteria."""
    # create the thresholded image
    thresholded_im = np.zeros(im.shape)
    thresholded_im[im >= th] = 1

    # compute weights
    nb_pixels = im.size
    nb_pixels1 = np.count_nonzero(thresholded_im)
    weight1 = nb_pixels1 / nb_pixels
    weight0 = 1 - weight1

    # if one of the classes is empty, eg all pixels are below or above the threshold, that threshold will not be considered
    # in the search for the best threshold
    if weight1 == 0 or weight0 == 0:
        return np.inf

    # find all pixels belonging to each class
    val_pixels1 = im[thresholded_im == 1]
    val_pixels0 = im[thresholded_im == 0]

    # compute variance of these classes
    var1 = np.var(val_pixels1) if len(val_pixels1) > 0 else 0
    var0 = np.var(val_pixels0) if len(val_pixels0) > 0 else 0

    return weight0 * var0 + weight1 * var1

# testing all thresholds from 0 to the maximum of the image
threshold_range = range(np.max(img)+1)
criterias = [compute_otsu_criteria(img, th) for th in threshold_range]

# best threshold is the one minimizing the Otsu criteria
best_threshold = threshold_range[np.argmin(criterias)]
print(best_threshold)