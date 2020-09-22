#include <bits/stdc++.h>
using namespace std;
vector<vector<int> > multiplicacion(vector<vector<int> > M1, vector<vector<int> > M2){
    vector<vector<int> > resultado;
    for(int i=0;i<M1.size();i++){
        vector<int> temp;
        for(int j=0;j<M1.size();j++){
            int valor=0;
            for(int k=0;k<M1.size();k++){
                valor=valor+M1[i][k]*M2[k][j];
            }
            temp.push_back(valor);
        }
        resultado.push_back(temp);
    }
    return resultado;
}
void mostrar(vector<vector<int> > M){
    for(int i=0;i<M.size();i++){
        for(int j=0;j<M[0].size();j++){
            cout<<M[i][j]<<"\t";
        }
        cout<<endl;
    }
}
int main(){
    int max=100;
    unsigned t0, t1;
        int A[max][max], B[max][max], C[max][max];
        for(int i=0; i<max;++i){
            for(int j=0; j<max; ++j){
                A[i][j]= i*2+j*2;
            }
        }
        for(int i=0; i<max; ++i){
            for(int j=0; j<max; ++j){
                B[i][j]= i*2+j*2;
            }
        }
        // Opciones posibles de la memoria caché
        int mCOptions[5] = {64,128,256,512,1024}
        // Opciones posibles para los bloques
        int BOptions[5] = {2,4,8,16,32};
        t0=clock();
        for(int i=0;i<max;i++){
            for(int j=0;j<max;j++){
                C[i][j]=0;
                for(int k=0;k<max;k++){
                    C[i][j]=C[i][j]+A[i][k]*B[k][j];
                }
            }
        }
        t1 = clock();
        float time = (float(t1-t0)/CLOCKS_PER_SEC);
}
