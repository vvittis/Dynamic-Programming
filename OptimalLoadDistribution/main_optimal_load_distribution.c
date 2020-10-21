#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float minValue(float b[],size_t arr_size);
#define FLOAT_TO_INT(x) ((int)((x)+0.5))

int main(){

    // Read K
    int K;
    printf("Please enter the number of products(K) : ");
    scanf(" %d", &K);

    //Read B
    float B;
    printf("\nPlease enter the total_weight of products(B) :  ");
    scanf(" %f", &B);

    // Read b_k and w_k
    float b[K];
    float w[K];
    for (int i=0; i<K; ++i){
        printf("\nPlease enter the weight and value of product (b%d,w%d) :  ",i,i);
        scanf(" %f,%f", &b[i],&w[i]);
    }
    printf("\n");


    float minV = minValue(b,(sizeof(b)/sizeof(b[0]))); // min value
    int maxDisPoint = (int)B/minV+1;
    float X_K[K+1][maxDisPoint]; // x[k]
    float V_K[K+1][maxDisPoint]; // V[x(k),k]
    float U_K[K][maxDisPoint]; // optimal control rule , u[x(k),k]
    float step=0.0;

    // Find discrete valid x(k)
    for (int i=0; i<=K; ++i){

        step=0.0;
        if(i!=0 && i!=K){
            step=b[i-1];
        }

        for (int j=0; j<maxDisPoint; ++j) {

            if(j==0) { X_K[i][j]=0; }
            else{
                float point = j*step;
                if(point<=B && point!=0){ X_K[i][j]=point; }
                else { X_K[i][j]=-1.0f; }
            }
            if(i==K && j==0){ X_K[i][j] = B; } // last level
            V_K[i][j] = 0; // initialize V_K
        }
    }

    // Discrete Dynamic Programming
    for(int i=K-1;i>=0;i--){
        for(int j=0;j<maxDisPoint;j++){

            if( X_K[i][j]== -1.0 ){
                V_K[i][j] = -1.0;
                U_K[i][j] = -1.0;
            }
            else{

                //Find valid u_k
                float u_k[maxDisPoint];
                for (int k = 0; k < maxDisPoint; k++) {
                    u_k[k] = (X_K[i + 1][k] - X_K[i][j]) / b[i];
                }

                //Find the V[x_k](max)
                float v_k = 0;
                float u_k_max = 0;
                for (int l = 0; l < maxDisPoint; l++) {
                    if (v_k <= u_k[l] * w[i] + V_K[i + 1][l]) {
                        v_k = u_k[l] * w[i] + V_K[i + 1][l];
                        u_k_max = u_k[l];
                    }
                }

                U_K[i][j] = u_k_max;
                V_K[i][j] = v_k;
            }
        }
    }

    // Print solution
    float total_value=V_K[0][0];
    float total_value_rounded=0.0;

    for (int i=0; i<K; ++i) {
        for (int j=0; j<maxDisPoint; ++j){
            if(V_K[i][j] != -1.0){
                if( fabs(V_K[i][j]-total_value)<=0.001){
                    printf("At level %d : u(%d) = %d\n",i,i,FLOAT_TO_INT(U_K[i][j]));
                    total_value_rounded=total_value_rounded+FLOAT_TO_INT(U_K[i][j])*w[i];
                    total_value=total_value-U_K[i][j]*w[i];
                    break;
                }
            }
        }
    }

    printf("Total value = %.2f\n",total_value_rounded);
    return 0;

}

float minValue(float b[],size_t arr_size){

    float minV = b[0];

    for (int i=1; i<arr_size-1; ++i) {
        if(b[i]<=minV){
            minV=b[i];
        }
    }
    return minV;
}
