#include <stdio.h>


//--------------------------------------------------------------------
//  関数宣言
//--------------------------------------------------------------------

void init(int count[13][5]);                        // Countの初期化
void count_sum(int count[5][13], int sum[13]);      // Countの合計値
void two_pair(int sum[13], int the);        // ツーペアの実装
void straight(int numCount[13][5], int the);        // ストレートの実装
void arr_order(int arr[5]);                         // 配列の順番を降順にする
int arr_max(int arr[5]);

int main(void) {
    int arr1[13] = {0};
    int arr2[5] = {0, 1, 1, 2, 0};
    int count[5][13] = { {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0} };

    int k;

    int n = arr_max(arr2);
    // count_sum(count, arr1);
    // for ( k = 0; k < 5; k++ ) {
    //     printf("%d ", arr2[k]);
    // }
    printf("%d ", n);
    return 0;
}

void arr_order(int arr[5]) {
    int k, i;
    int tmp;

    for ( k = 0; k < 5; k++ ) {
        for ( i = 4; i > k; i-- ) {
            if ( arr[i] < arr[i-1] ) {
                tmp = arr[i];
                arr[i] = arr[i-1];
                arr[i-1] = tmp;
            }
        }
    }
}

void count_sum(int count[5][13], int sum[13]) {
    int i, j; 
    for ( i = 0; i < 5; i++ ) {
        for ( j = 0; j < 13; j++ ) {
            sum[j] += count[i][j];
        }
    }
}

int arr_max(int arr[5]) {
    int i, n = 0;
    int max = arr[0];

    for ( i = 1; i < 5; i++ ) {
        if ( max < arr[i] ) {
            max = arr[i];
            n = i;
        }    
    }
    return n;
}