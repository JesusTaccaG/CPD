#include <iostream>
#include <ctime>
using namespace std;
void func1(int MAX){
    double A[MAX][MAX], x[MAX], y[MAX];
    for (int i = 0; i < MAX; i++)
        for (int j = 0; j < MAX; j++)
            y[i] += A[i][j]*x[j];
}
void func2(int MAX){
    double A[MAX][MAX], x[MAX], y[MAX];
    for (int j = 0; j < MAX; j++)
        for (int i = 0; i < MAX; i++)
            y[i] += A[i][j]*x[j];
}
int main()
{
    unsigned t0, t1, tt0, tt1;
    int MAX=100;
    for(int k=0; k<5; k++){
        t0=clock();
        func1(MAX);
        t1 = clock();
        double time1 = (double(t1-t0)/CLOCKS_PER_SEC);
        cout << "Execution1 Time: " << time1 << endl;
        tt0=clock();
        func2(MAX);
        tt1 = clock();
        double time2 = (double(tt1-tt0)/CLOCKS_PER_SEC);
        cout << "Execution2 Time: " << time2 << endl;
        MAX+=100;
    }
}