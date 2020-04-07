#include <algorithm>
#include <iostream>
#include <thread>
#include <mutex>
#include <omp.h>

class SharedArray {
private:
    char *array;
    int index;
    int size;
public:
    explicit SharedArray(int n) : size(n), index(0) {
        array = new char[size];
        std::fill(array, array + size, '-');
    }

    ~SharedArray() {
        delete[] array;
    }

    void addChar(char c) {
        #pragma omp critical
        array[index++] = c;
        spendSomeTime();
    }

    int countOccurrences(char c) {
        return (int) std::count(array, array + size, c);
    }

    std::string toString() {
        return std::string(array, (unsigned long) size);
    }

private:
    void spendSomeTime() {
        for (int i = 0; i < 10000; i++) {
            for (int j = 0; j < 100; j++) {
                // These loops shouldn't be removed by the compiler
            }
        }
    }
};


class ArrayFiller {
private:
    static const int nThreads = 3;
    static const int nTimes = 20;
    SharedArray *array;
    omp_sched_t schedule;
    int chunk;
public:
    ArrayFiller(omp_sched_t schedule, int chunk) : schedule(schedule), chunk(chunk) {
        array = new SharedArray(nThreads * nTimes);
    }

    explicit ArrayFiller(omp_sched_t schedule) : schedule(schedule), chunk(0) {
        array = new SharedArray(nThreads * nTimes);
    }

    void fillArrayConcurrently() {
        omp_set_schedule(schedule, chunk);
        omp_set_num_threads(nThreads);
        #pragma omp parallel for default(none) schedule(runtime)
        for (int i = 0; i < nThreads * nTimes; ++i) {
            array->addChar((char)('A' + omp_get_thread_num()));
        }
    }

    void printStats() {
        std::cout << array->toString() << std::endl;
        for (int i = 0; i < nThreads; ++i)
            std::cout << (char) ('A' + i) << "="
                      << array->countOccurrences((char)('A' + i)) << " ";
        std::cout << "\n" << std::endl;
    }

    ~ArrayFiller() {
        delete array;
    }

};


int main() {
    std::cout << "Each thread should add its char to the array n times (n=20)\n" << std::endl;
    std::cout << "Case 1.a: static without chunk" << std::endl;
    ArrayFiller m1(omp_sched_static);
    m1.fillArrayConcurrently();
    m1.printStats();

    std::cout << "Case 1.b: static with chunk of 2" << std::endl;
    ArrayFiller m2(omp_sched_static, 2);
    m2.fillArrayConcurrently();
    m2.printStats();

    std::cout << "Case 1.c: static with chunk of 4" << std::endl;
    ArrayFiller m3(omp_sched_static, 4);
    m3.fillArrayConcurrently();
    m3.printStats();

    std::cout << "Case 2.a: dynamic without chunk" << std::endl;
    ArrayFiller m4(omp_sched_dynamic);
    m4.fillArrayConcurrently();
    m4.printStats();

    std::cout << "Case 2.b: dynamic with chunk of 2" << std::endl;
    ArrayFiller m5(omp_sched_dynamic, 2);
    m5.fillArrayConcurrently();
    m5.printStats();

    std::cout << "Case 2.c: dynamic with chunk of 4" << std::endl;
    ArrayFiller m6(omp_sched_dynamic, 4);
    m6.fillArrayConcurrently();
    m6.printStats();

    std::cout << "Case 2.d: dynamic with chunk of 8" << std::endl;
    ArrayFiller m7(omp_sched_dynamic, 8);
    m7.fillArrayConcurrently();
    m7.printStats();

    std::cout << "Case 3.a: guided without chunk" << std::endl;
    ArrayFiller m8(omp_sched_guided);
    m8.fillArrayConcurrently();
    m8.printStats();

    std::cout << "Case 3.b: guided with chunk of 2" << std::endl;
    ArrayFiller m9(omp_sched_guided, 2);
    m9.fillArrayConcurrently();
    m9.printStats();

    std::cout << "Case 3.c: guided with chunk of 4" << std::endl;
    ArrayFiller m10(omp_sched_guided, 4);
    m10.fillArrayConcurrently();
    m10.printStats();

    std::cout << "Case 4: auto" << std::endl;
    ArrayFiller m11(omp_sched_auto);
    m11.fillArrayConcurrently();
    m11.printStats();
}
