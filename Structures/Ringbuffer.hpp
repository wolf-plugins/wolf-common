#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdexcept>
#include <cassert>

START_NAMESPACE_DISTRHO

namespace wolf
{

template <class T>
class Ringbuffer
{
  public:
    explicit Ringbuffer(const int size);
    ~Ringbuffer();

    void add(const T item);
    T get();

    int count();
    bool full();
    bool empty();

  private:
    int fCount;
    T *fItems;
    const int fCapacity;
    int fStart;
    int fEnd;
};

template <class T>
Ringbuffer<T>::Ringbuffer(const int capacity) : fCount(0),
                                                fItems(new T[capacity]),
                                                fCapacity(capacity),
                                                fStart(0),
                                                fEnd(-1)
{
}

template <class T>
Ringbuffer<T>::~Ringbuffer()
{
    delete[] fItems;
}

template <class T>
void Ringbuffer<T>::add(const T value)
{
    assert(!full());

    ++fCount;
    fItems[++fEnd % fCapacity] = value;
}

template <class T>
T Ringbuffer<T>::get()
{
    assert(!empty());

    --fCount;
    return fItems[fStart++ % fCapacity];
}

template <class T>
int Ringbuffer<T>::count()
{
    return fCount;
}

template <class T>
bool Ringbuffer<T>::full()
{
    return fCount == fCapacity;
}

template <class T>
bool Ringbuffer<T>::empty()
{
    return fCount == 0;
}

} // namespace wolf

END_NAMESPACE_DISTRHO

#endif