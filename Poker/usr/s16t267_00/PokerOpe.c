//====================================================================
//  工学部「情報環境実験２」(富永)  C言語プログラミング  例題
//  ポーカーゲームの戦略
//--------------------------------------------------------------------
//  Poker  usr/s13t200_00/  PokerOpe.c
//  Linux CentOS 6.7  GCC 4.4.7
//--------------------------------------------------------------------
//  喜田研究室 furuhama Naoki 古濵尚樹
//  2018.11.27
//====================================================================


//====================================================================
//  仕様
//====================================================================

/*--------------------------------------------------------------------

手札、場札、チェンジ数、テイク数、捨札を引数とし、捨札を決める。
返却値は、捨札の位置である。-1のときは、交換しないで、手札を確定させる。
関数 strategy() は、戦略のインタフェースであり、この関数内で、
実際の戦略となる関数を呼び出す。手札と捨札は、不正防止のため、
変更不可なので、局所配列にコピーして、整列などの処理を行う。
複数の戦略を比較したり、パラメタを変化させて、より強い戦略を目指す。

---------------------------------------------------------------------*/


//====================================================================
//  前処理
//====================================================================

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Poker.h"

//--------------------------------------------------------------------
//  関数宣言
//--------------------------------------------------------------------

void init(int count[5][13]);                        // Countの初期化
void count_sum(int count[5][13], int sum[13]);      // Countの合計値
int two_pair(int numCount[5][13], int n, int m);        // ツーペアの実装
void one_pair(int numCount[5][13], int ud[], int us, int the);
void straight(int shdcCount[5][13], int the);        // ストレートの実装
void arr_order(int arr[5]);                         // 配列の順番を降順にする



//====================================================================
//  戦略
//====================================================================

/*--------------------------------------------------------------------
//  ユーザ指定
//--------------------------------------------------------------------

最初の手札のまま交換しない。

hd : 手札配列
fd : 場札配列(テイク内の捨札)
cg : チェンジ数
tk : テイク数
ud : 捨札配列(過去のテイクも含めた全ての捨札)
us : 捨札数

--------------------------------------------------------------------*/

int strategy( int hd[],  int fd[], int cg, int tk,  int ud[], int us) {
    int myhd[HNUM];
    int the;
    int k, i;
    int sum[13] = {0};
    int shdc;               // スペード、ハート、ダイヤ、クラブ のどれか判断
    int num;                // 手札の数字を判断
    int shdcCount[5][13];    // 手札の種類の枚数
    int numCount[5][13];    // 手札の数字の枚数
   

    for ( k = 0; k < HNUM; k++ ) { myhd[k] = hd[k]; }
    init(shdcCount);
    init(numCount);
    
    //card_show(myhd, HNUM);
    // printf(" ?  ");  scanf("%d", &the);

    /*  カードの種類    カードの数字
          s h d c     A 2 3 4 5 6 7 8 9 T J Q K
        0           0
        1           1
        2           2
        3           3
        4           4

        tmp[k] : 0 1 2 3 4
                 4 3 6 7 2
                 1 2 3 4 6
    */

    // -- 手札の判定
    for ( k = 0; k < HNUM; k++ ) { 
        shdc = hd[k] / 13;
        num = hd[k] % 13;
        shdcCount[k][shdc]++;
        numCount[k][num]++;
    }
    count_sum(numCount, sum);

    for ( k = 0; k < 13; k++ ) {
        if ( sum[k] == 4 ) {                        // フォーカード
            the = -1;
            break;
        } else if ( sum[k] == 3 ) {                 // スリーカード
            the = -1;
            break;
        } else if ( sum[k] == 2 ) {                  
            if ( k == 12 ) {                        // ワンペア
                the = -1;
                break;
            } else {                                
                for ( i = k+1; i < 13; i++ ) {
                    if ( sum[i] == 2 ) {            // ツーペア
                        the = two_pair(numCount, k, i);  
                        break; 
                    } else {                        // ワンペア
                        the = -1;
                        break;
                    }
                }
                break;
            }  
        } else if ( sum[k] == 1 || sum[k] == 0 ) {  // ノーペア
            the = -1;
        } 
    }
    //printf(" %d\n", the);
    

    /*
    for ( k = 0; k < HNUM; k++) { printf("my %d hd %d\n", myhd[k], hd[k]); }
    printf("cg %d us %d\n", cg, us);
    for ( k = 0; k < HNUM; k++) { printf("fd %d ud %d\n", fd[k], ud[k]); }
    */

    // if ( the < 0 || the > 4 ) { the = -1; }
    return -1;
}


//====================================================================
//  補助関数
//====================================================================

void init(int count[5][13]) {
    int i, j;
    for ( i = 0; i < HNUM; i++ ) {
        for ( j = 0; j < 13; j++ ) {
            count[i][j] = 0;
        }
    }
}

void count_sum(int count[5][13], int sum[13]) {
    int i, j; 
    for ( i = 0; i < HNUM; i++ ) {
        for ( j = 0; j < 13; j++ ) {
            sum[j] += count[i][j];
        }
    }
}

int two_pair(int numCount[5][13], int n, int m) {
    int i, j;
    int the;
    for ( i = 0; i < HNUM; i++ ) {
        for ( j = 0; j < 13; j++ ) { 
            if ( numCount[i][j] == 1 && j != n && j != m ) {
                the = i;
                //printf(" %d", the);
                return the;
            }
        }
    }
}

void one_pair(int numCount[5][13], int ud[], int us, int the) {
    int k, num;
    int count[5][13], sum[13] = {0};

    init(count);
    for ( k = 0; k < us; k++ ) {
        num = ud[k] % 13;
        count[k][num]++;
    }
    count_sum(count, sum);
    for( k = 0; k < us; k++ ) {
        
    }
}

// void straight(int shdcCount[5][13], int the) {
//     int k, i;
//     int tmp[5];

//     for ( k = 0; k < HNUM; k++ ) {
//         for ( i = 0; i < 13; i++ ) {
//             if ( numCount[i][k] == 1 ) {
//                 tmp[k] = i;
//             } else {
//                 break;
//             }
//         }
//     }

//     for ( k = 0; k < HNUM; k++ ) {
//         for ( i = k+1; i < 5; i++ ) {
//             if ( tmp[k] == tmp[i] + 1  ) {
                
//             }
//         }
        
//     }   

    
// }

// void arr_order(int arr[5]) {
//     int k, i;
//     int tmp;

//     for ( k = 0; k < HNUM; k++ ) {
//         for ( i = 4; i > k; i-- ) {
//             if ( arr[i] < arr[i-1] ) {
//                 tmp = arr[i];
//                 arr[i] = arr[i-1];
//                 arr[i-1] = tmp;
//             }
//         }
//     }
    
// }
