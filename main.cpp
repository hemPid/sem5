#include <iostream>
#include "smrt_ptrs.h"

struct person
{
    int age;
    int height;
};


int main() {
    do
    {
        shared_ptr<person> a = new person;
        a->age = 40;
        std::cout << a->age << "\n";
    } while (false);
    shared_ptr<person> a = new person;
    a->age = 50;
    a->height = 180;
    std::cout << a->age << "\n";
    weak_ptr<person> aw = a;
    do
    {
        shared_ptr<person> b(aw.lock());
        std::cout << b->age << " " << b->height << "\n";
        b->age = 35;
    } while (false);
    std::cout << a->age << " " << a->height << "\n";
    weak_ptr<person> c = std::move(aw);
    shared_ptr<person> d = c.lock();
    std::cout << d->age << " " << d->height << "\n";
    return 0;
}