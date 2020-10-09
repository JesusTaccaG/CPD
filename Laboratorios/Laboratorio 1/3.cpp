#include <bits/stdc++.h>
using namespace std;

int main()
{
    int max=100;
    unsigned t0, t1;
        srand(time(NULL));
        int A[max][max], B[max][max], C[max][max];
        for(int i=0; i<max;++i){
            for(int j=0; j<max; ++j){
                A[i][j]= i+j;
            }
        }
        for(int i=0; i<max; ++i){
            for(int j=0; j<max; ++j){
                B[i][j]= i+j;
            }
        }
        int tam_block=2;
        t0=clock();
        for(int bi=0; bi<max; bi+=tam_block){
            for(int bj=0; bj<max; bj+=tam_block){
                for(int bk=0; bk<max; bk+=tam_block){
                    for(int i=0; i<tam_block; i++){
                        for(int j=0; j<tam_block; j++){
                            for(int k=0; k<tam_block; k++){
                                C[i+bi][j+bj] += A[i+bi][k+bk]*B[k+bk][j+bj];
                            }
                        }
                    }
                }
            }
        }
        t1 = clock();
        float time = (float(t1-t0)/CLOCKS_PER_SEC);
}