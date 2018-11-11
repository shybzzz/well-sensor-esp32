#ifndef __DATA_MEDIAN__
#define __DATA_MEDIAN__

#include <random>

#define SIZE 16u

int data[SIZE];
uint8_t current_sample = 0;

void count(double* a, double* inter, double* key, size_t size_a, size_t size_inter)
{
    for (size_t i = 0; i < size_a; ++i)
    {
        for (size_t j = 0; j < size_inter; ++j)
        {
            if (j != 0) {
                if (a[i] < inter[j] && a[i] > inter[j-1]){
                    key[j]++;
                }
            }
            else {
                if (a[i] < inter[j]){
                    key[0]++;
                }
            }
        }
    }
}

void genVector(double* x, double* ri, double* inter, double* out, size_t size_ri, size_t size_inter)
{
    for (size_t i = 0; i < size_ri; ++i)
    {
        for (size_t j = 0; j < size_inter; ++j)
        {
            if (j != 0)
            {
                if (ri[i] < inter[j] && ri[i] > inter[j-1]){
                    out[i] = x[j];
                }
            }
            else{
                if (ri[i] < inter[j]){
                    out[i] = x[j];
                }
            }
        }
    }
}
size_t getSeed()
{
    return time(0);
}
void genRi(int k, double* ri)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0, 1);
    for (int i = 0; i < k; ++i)
    {
        ri[i] = dist(gen);
    }
}

#endif
