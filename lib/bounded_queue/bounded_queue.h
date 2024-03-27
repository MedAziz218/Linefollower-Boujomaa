#ifndef BOUNDED_QUEUE_H
#define BOUNDED_QUEUE_H
template <class T, int Capacity>
class BoundedQueue {
private:
    T* arr;
    int front;
    int rear;
    int size;

public:
    BoundedQueue() : front(0), rear(0), size(0) {
        arr = new T[Capacity];
    }

    ~BoundedQueue() {
        delete[] arr;
    }

    void enqueue(const T& item) {
        if (size == Capacity) {
            return;
        }
        arr[rear] = item;
        rear = (rear + 1) % Capacity;
        size++;
    }

    void dequeue() {
        if (isEmpty()) {
            return;
        }
        front = (front + 1) % Capacity;
        size--;
    }

    T peek() const {
        // if (isEmpty()) {
        // let the caller to deal with it.    
        // }
        return arr[front];
    }

    bool isEmpty() const {
        return size == 0;
    }

    bool isFull() const {
        return size == Capacity;
    }
};
#endif