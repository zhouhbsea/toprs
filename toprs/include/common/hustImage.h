#ifndef HUSTIMAGE_H
#define HUSTIMAGE_H

#include <fstream>
#include <iostream>
#include <string>
#include <cassert>
#include <typeinfo>

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned uint32_t;


template<class T>
class Image {
  int width, height;
  T *image, *img;
  bool localalloc;
public:
  Image();
  Image(int w, int h, T *ptr=NULL);
  ~Image() { if (localalloc) delete [] img; }
  void SetSize(int w, int h);
  void SetData(T *ptr) { image = ptr; }
  T *GetData() const { return image; }
  /// Get image width 
  int GetWidth() const { return width; }
  /// Get image height
  int GetHeight() const { return height; }
  /// Get pointer to pixel row \b i 
  T *operator[](int i) { return &image[i*width]; }
  /// Copy image data
  void operator=(Image<T> &src);

  std::ostream& print(std::ostream& out = std::cout) const;
};

template<class T>
std::ostream& Image<T>::print( std::ostream& out /*= std::cout*/ ) const
{
	out.setf(std::ios::fixed);
	for (int i = 0 ; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			out  << (float)*(image + i*width +j) << "\t";

		}
		out << "\n";
	}
	out << "\n";
	return out;
}

typedef Image<double> Img64f;
typedef Image<unsigned char> Img8u;
typedef Image<float> Img32f;

template<class T>
class Buffer {
  int width, height;
  T *image, *img;
public:
  Buffer(int w, int h);
  ~Buffer() { delete [] img; }
  T *GetData() const { return image; }
  int GetWidth() const { return width; }
  int GetHeight() const { return height; }
  T *operator[](int i) { 
    return &image[((i+height)%height)*width]; 
  }
  void Clear(T val = (T) 0);
};

typedef Buffer<double> Buf64f;
typedef Buffer<unsigned char> Buf8u;
typedef Buffer<float>	Buf32f;


template<class T>
Image<T>::Image()
{
	localalloc = false;
}

template<class T>
Image<T>::Image(int w, int h, T *ptr) : width(w), height(h)
{
  int extra = 16 / sizeof(T);
  if (ptr==NULL) {
    img = new T[w*h+extra];
    localalloc = true;
#if __WORDSIZE == 64
    image = (T *)((uint64_t)(img+extra-1) & (uint64_t)(~15));
#else 
    image = (T *)((uint32_t)(img+extra-1) & (uint32_t)(~15));
#endif
  } else {
    img = ptr;
    localalloc = false;
    image = img;
  }
}

template<class T>
void Image<T>::SetSize(int w, int h)
{
  if (w==width && h==height) 
    return;
  if (localalloc) 
    delete [] img; 
  width = w;
  height = h;
  int extra = 16 / sizeof(T);
  img = new T[w*h+extra];
  localalloc = true;
#if __WORDSIZE == 64
    image = (T *)((uint64_t)(img+extra-1) & (uint64_t)(~15));
#else 
    image = (T *)((uint32_t)(img+extra-1) & (uint32_t)(~15));
#endif
}


template <class T>
void Image<T>::operator=(Image<T> &src)
{
  memcpy(image, src.GetData(), sizeof(T) * width * height);
}

template<class T>
Buffer<T>::Buffer(int w, int h) : width(w), height(h)
{
  int extra = 16 / sizeof(T);
  img = new T[w*h+extra];
#if __WORDSIZE == 64
    image = (T *)((uint64_t)(img+extra) & (uint64_t)(~15));
#else 
    image = (T *)((uint32_t)(img+extra) & (uint32_t)(~15));
#endif
}

template<class T>
void Buffer<T>::Clear(T val)
{
  for (int i=0;i<(width*height);i++) 
    image[i] = (T) val;
}

#endif // HUSTIMAGE_H
