#ifndef smrt_ptrs
#define smrt_ptrs
#include <algorithm>
#include <cstdint>

using u16 = uint16_t;

template <class T> class unique_ptr
{
private:
    T * ptr;
public:
    unique_ptr() : ptr(nullptr) {}
    unique_ptr(T * raw_ptr) : ptr(raw_ptr) {}
    ~unique_ptr() { delete ptr; }
    unique_ptr(const unique_ptr & a) = delete;
    unique_ptr & operator=(const unique_ptr & a) = delete;
    unique_ptr(unique_ptr && a) {
        ptr = a.ptr;
        a.ptr = nullptr;
    }
    void operator=(unique_ptr && a) {
        if (&a == this)
        {
            return;
        }
        ptr = a.ptr;
        a.ptr = nullptr;
    }
    void operator=(T * a) {
        delete ptr;
        ptr = a;
    }
    T & operator*() {
        return *ptr;
    }
    T * operator->() {
        return ptr;
    }
};

struct counters
{
    u16 s_count;
    u16 w_count;
    counters() : s_count(1), w_count(0) {}
};


template <class T> class weak_ptr;
template <class T> class shared_ptr
{
    private:
    counters * cnt;
    T * ptr;
    public:
    template <class U> friend class weak_ptr;
    shared_ptr() : ptr(nullptr), cnt(nullptr) {}
    shared_ptr(T * a) : ptr(a), cnt(new counters) {}
    shared_ptr(T * a, counters * c) : ptr(a), cnt(c) {}
    shared_ptr(const shared_ptr & p) {
        ptr = p.ptr;
        cnt = p.cnt;
        cnt->s_count += 1;
    }
    shared_ptr & operator=(const shared_ptr & p) {
        ptr = p.ptr;
        cnt = p.cnt;
        cnt->s_count += 1;
        return *this;
    }
    shared_ptr(shared_ptr && p) {
        ptr = p.ptr;
        cnt = p.cnt;
        p.cnt = nullptr;
        p.ptr = nullptr;
    }
    shared_ptr & operator=(shared_ptr && p) {
        if (&p == this)
        {
            return *this;
        }
        ptr = p.ptr;
        cnt = p.cnt;
        p.cnt = nullptr;
        p.ptr = nullptr;
        return *this;
    }

    shared_ptr(const weak_ptr<T> & p) {
        ptr = p.ptr_;
        cnt = p.cnt_;
        cnt->w_count += 1;
    }
    T & operator*() {
        return *ptr;
    }
    T * operator->() {
        return ptr;
    }
    ~shared_ptr() {
        cnt->s_count -= 1;
        if (!cnt->s_count && !cnt->w_count)
        {
            delete ptr;
            if (!cnt->w_count)
            {
                delete cnt;
            }
            
        }
        
    }
};

template <class T> class weak_ptr
{
private:
    T * ptr_;
    counters * cnt_;
public:
    template <class U> friend class shared_ptr;
    weak_ptr(const shared_ptr<T> & p): ptr_(p.ptr), cnt_(p.cnt) {
        cnt_->w_count += 1;
    };

    weak_ptr(const weak_ptr & p) {
        cnt_ = p.cnt_;
        cnt_->w_count += 1;
        ptr_ = p.ptr_;
    }

    weak_ptr & operator=(const weak_ptr & p) {
        cnt_ = p.cnt_;
        cnt_->w_count += 1;
        ptr_ = p.ptr_;
        return *this;
    }

    weak_ptr(weak_ptr && p) {
        cnt_ = p.cnt_;
        ptr_ = p.ptr_;
        p.cnt_ = nullptr;
        p.ptr_ = nullptr;
    }

    weak_ptr & operator=(weak_ptr && p) {
        if (p == *this)
        {
            return *this;
        }
        cnt_ = p.cnt_;
        ptr_ = p.ptr_;
        p.cnt_ = nullptr;
        p.ptr_ = nullptr;
        return *this;
    }

    ~weak_ptr() {
        cnt_->w_count -= 1;
        if (!(cnt_->w_count) && !(cnt_->s_count))
        {
            delete ptr_;
            delete cnt_;
        }
        
    };
    
    weak_ptr<T> & lock() {
        return *this;
    }
};

#endif