# c++ stuff
- **auto**: c++ keyword, assumes the type of whatever I happen to assign to it!
- std::vector<> is a **container class** in the C++ Standard Library that implements a **dynamic array**.
- std::vector<> allows for fast random access to its elements using the subscript operator ([]), just like arrays. This makes it convenient to use when you need to quickly access individual elements.
- *std::vector* is a template class, meaning it can store any type of data. You need to specify the type of elements the vector will hold when you declare it.
- **namespaces** are packages (sets of functions)

# Performance metrics

## Execution time
- max precision: ms
- TIMEFORMAT='%4R'; time (./blur/blur 15 ./blur/data/im3.ppm blurred_im3.ppm)


## Flamegraphs
- perf record --call-graph dwarf -- ./blur 15 data/im4.ppm data/blurred_im4.ppm
- perf record --call-graph dwarf -- ./pearson data/128.data data/128_pearson.data;
- hotspot perf.data

## Pidstat
- pidstat -u -d -r -h -I 1 -e ./blur/blur 15 blur/data/im1.ppm blur/data/blurred_im1.ppm > pidstat_output/pidstat_blur_im1_15.txt

> pidstat non mostra i dati di lettura / scrittura
> stiamo scrivendo dati nell'ordine dei MB, è probabile che la lettura e scrittura di questi files sia talmente veloce che sia in between intervals


# Optimization
## compiler flags
- add **Ofast** to the CXXFLAGS 
- https://gcc.gnu.org/onlinedocs/gcc-3.4.6/gcc/Optimize-Options.html
- https://stackoverflow.com/questions/45685487/what-does-clangs-ofast-option-do-in-practical-terms-especially-for-any-diffe
- -Ofast adds to the -O3 optimizations: 
    -fno-signed-zeros 
    -freciprocal-math 
    -ffp-contract=fast 
    -menable-unsafe-fp-math 
    -menable-no-nans 
    -menable-no-infs 

## loop unrolling Riccardo
## m256d-registers
- https://ark.intel.com/content/www/us/en/ark/products/92986/intel-xeon-processor-e5-2620-v4-20m-cache-2-10-ghz.html
- https://www.intel.com/content/www/us/en/products/sku/149091/intel-core-i78565u-processor-8m-cache-up-to-4-60-ghz/specifications.html
- the processor must be compatible with **AVX2 Instruction set extension**
- https://stackoverflow.com/questions/12875325/using-m256d-registers


This optimized version uses AVX2 intrinsics to perform SIMD operations on 4 double-precision floating-point values at once. Here are the key optimizations:

We use __m256d vectors to process 4 pixels simultaneously in both horizontal and vertical passes.
The _mm256_fmadd_pd intrinsic is used for fused multiply-add operations, which can be faster than separate multiply and add instructions on many processors.
We've separated the horizontal and vertical passes to improve cache locality.
The inner loops now process 8 pixels at a time (4 in each SIMD vector for r, g, and b).
We use aligned memory access where possible (alignas(32) for the weights array) to improve memory access efficiency.

Note that this optimization assumes that the Matrix class allows direct access to its data via the r(), g(), and b() methods. If this is not the case, you may need to modify the code to work with your specific Matrix implementation.
**Also, keep in mind that this optimization may require additional error handling for edge cases, such as when the image dimensions are not multiples of 8. You might need to add cleanup code to handle these edge cases.**
Lastly, remember to compile with AVX2 support enabled (e.g., -mavx2 flag for GCC or Clang) to use these intrinsics.
Would you like me to explain any part of this optimization in more detail?

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




