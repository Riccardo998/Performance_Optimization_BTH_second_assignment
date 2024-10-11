# c++ stuff
**auto**: c++ keyword, assumes the type of whatever I happen to assign to it!

- std::vector<> is a **container class** in the C++ Standard Library that implements a **dynamic array**.
- std::vector<> allows for fast random access to its elements using the subscript operator ([]), just like arrays. This makes it convenient to use when you need to quickly access individual elements.
- *std::vector* is a template class, meaning it can store any type of data. You need to specify the type of elements the vector will hold when you declare it.
- **namespaces** are packages (sets of functions)

## compiler flags
- add **Ofast** to the CXXFLAGS (https://stackoverflow.com/questions/45685487/what-does-clangs-ofast-option-do-in-practical-terms-especially-for-any-diffe)


# PPM images
https://netpbm.sourceforge.net/doc/ppm.html

**Each PPM image consists of the following:**

- A "magic number" for identifying the file type. A ppm image's magic number is the two characters "P6".
- Whitespace (blanks, TABs, CRs, LFs).
- A width, formatted as ASCII characters in decimal.
- Whitespace.
- A height, again in ASCII decimal.
- Whitespace.
- The maximum color value (Maxval), again in ASCII decimal. Must be less than 65536 and more than zero.
- A single whitespace character (usually a newline).
- A raster of *Height* rows, in order from top to bottom. Each row consists of *Width* **pixels**, in order from left to right. Each **pixel** is a triplet of red, green, and blue samples, in that order. Each sample is represented in pure binary by either 1 or 2 bytes. If the Maxval is less than 256, it is 1 byte. Otherwise, it is 2 bytes. The most significant byte is first.
- A row of an image is horizontal. A column is vertical. The pixels in the image are square and contiguous.

# Blurring

The primary goal of Gaussian blur is to create a blurring effect that makes an image appear softer. It's commonly used in photography, graphic design, and image editing to reduce noise, detail, and sharpness.

The blur is achieved using a Gaussian function, which mathematically defines the weight of neighboring pixels based on their distance from the target pixel. The Gaussian function has a bell-shaped curve, meaning pixels closer to the center (the target pixel) have a higher weight, while those further away have less influence.

*Kernel Creation:*
A Gaussian kernel (or mask) is generated based on the specified radius. This kernel contains weights calculated using the Gaussian function, where the center of the kernel has the highest weight, and the weights decrease as you move away from the center.

*Convolution Process:*
The Gaussian blur is applied through a process called convolution. For each pixel in the image:
- The algorithm retrieves the neighboring pixels within the radius.
- It multiplies each neighboring pixel's value by its corresponding weight from the Gaussian kernel.
- The results are summed up and normalized (divided by the sum of the weights) to get the new pixel value.

*Two Passes:*
In the provided code, the blur is applied in two passes:
- **Horizontal Pass**: The algorithm processes each pixel row by row, applying the Gaussian weights to the horizontal neighbors.
- **Vertical Pass**: After the horizontal blur, it processes the image column by column, applying the Gaussian weights to the vertical neighbors.

### Important:
every pixel in the result is a weighted average of itself and its surrounding pixels. The weights follow a Gaussian (bell-shaped) distribution, where:
- The center pixel (the one being processed) gets the highest weight
- Pixels further away get progressively smaller weights
The weights are symmetric around the center


# Pearson coefficient
- datasets = **dimension** x **dimension** matrix = std::vector<Vector>
- a dataset is a row of the matrix
- the Pearson coefficient is calculated among different rows of the dataset

for the 128 dataset, the pearson coeffincient files had 129 rows --> first line contains the dimension of the dataset

```c++
for (auto sample1{0}; sample1 < dataset.size() - 1; sample1++){
    for (auto sample2 { sample1 + 1 }; sample2 < datasets.size(); sample2++) {
}
```