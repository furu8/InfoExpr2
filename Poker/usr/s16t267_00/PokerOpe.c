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
#include <stdbool.h>
#include <math.h>

#include "Poker.h"

//--------------------------------------------------------------------
//  関数宣言
//--------------------------------------------------------------------

// void init(int count[5][13]);                        // Countの初期化
// void count_sum(int count[][], int sum[], int n);      // Countの合計値
int two_pair(int numCount[5][13], int n, int m);        // ツーペアの実装
int one_pair(int ud[], int us, int hd[], int n);
int straight(int hd[]);        // ストレートの実装
int flash(int hd[], int shdcCount[5][4], int n);            // フラッシュの実装
void arr_order(int arr[5]);                         // 配列の順番を降順にする
int arr_max(int arr[5]);        // 最大値の配列の要素を返却



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
    int shdc;               // スペード、ハート、ダイヤ、クラブ のどれか判断
    int num;                // 手札の数字を判断
    int arr[5];             // 手札の数字を配列に格納
    int shdcSum[4] = {0};   // 手札の種類の合計値を格納
    int numSum[13] = {0};   // 手札の数字の合計値を格納
    int shdcCount[5][4] = {{0}};    // 手札の種類の枚数
    int numCount[5][13] = {{0}};    // 手札の数字の枚数
    _Bool flag = false;
   
    // init(shdcCount);
    // init(numCount);

     for ( k = 0; k < HNUM; k++ ) { myhd[k] = hd[k]; }
    // card_show(myhd, HNUM);
    // printf(" ?  ");  scanf("%d", &the);

    // /*  カードの種類    カードの数字
    //       s h d c     A 2 3 4 5 6 7 8 9 T J Q K
    //     0           0
    //     1           1
    //     2           2
    //     3           3
    //     4           4

    //     tmp[k] : 0 1 2 3 4
    //              4 3 6 7 2
    //              1 2 3 4 6
    // */

    // ---- 手札の判定
    // -- 判定値を計算
    for ( k = 0; k < HNUM; k++ ) { 
        shdc = myhd[k] / 13;
        num = myhd[k] % 13;
        arr[k] = num;
        // shdcCount[k][shdc]++;
        numCount[k][num]++;
        shdcSum[shdc]++;
        numSum[num]++;
    }

    // -- 判定値の合計値を求める
    // count_sum(numCount, numSum, 13);
    // count_sum(shdcCount, shdcSum, 4);

    // -- 合計値を元に分類
    for ( k = 0; k < 13; k++ ) {
        if ( numSum[k] == 4 ) {                        // フォーカード
            the = -1;
            break;
        } else if ( poker_point(myhd) == P6 ) {         // フルハウス
            the = -1;
            break;
        } else if ( numSum[k] == 3 ) {                 // スリーカード
            the =  -1;
            break;
        } else if ( numSum[k] == 2 ) {                  
            if ( k == 12 ) {                        // ワンペア
                the = one_pair(ud, us, myhd, k);
            } else {                                
                for ( i = k+1; i < 13; i++ ) {
                    if ( numSum[i] == 2 ) {            // ツーペア
                        the = two_pair(numCount, k, i);  
                        break; 
                    } else {                        // ワンペア
                        the = one_pair(ud, us, myhd, k);
                        break;
                    }
                }
                break;
            }  
        } else if ( numSum[k] == 1 || numSum[k] == 0 ) {  // ノーペア
            for ( i = 0; i < 4; i++ ) {
                if ( shdcSum[i] == 5 ) {
                    the = -1;
                    flag = true;
                    break;
                } else if ( shdcSum[i] == 4 ) {
                    the = flash(myhd, shdcCount, i);
                    break;
                }
            }
            the = straight(arr);                        // ストレート
            if ( the != -1 || flag == true ) { break; }
        }
    }
    
    /*
    if ( poker_point(myhd) == P7 ) {
        the = -1;
    } else if ( poker_point(myhd) == P6 ) {
        the = -1;
    } else if ( poker_point(myhd) == P5 ) {
        the = flash(myhd, shdcCount, i);
    } else if ( poker_point(myhd) == P4 ) {
        the = straight(arr); 
    } else if ( poker_point(myhd) == P3 ) {

    } else if ( poker_point(myhd) == P2 ) {
        the = two_pair(numCount, k, i);  
    } else if ( poker_point(myhd) == P1 ) {
        the = one_pair(ud, us, myhd, k);
    }
    
    card_show(hd, HNUM);
    printf(" %d\n", the);
    for ( k = 0; k < 13; k++ ) { 
        printf("%d ", numSum[k]);
    }
    puts("\n");
    */
    // for ( k = 0; k < HNUM; k++) { printf("my %d hd %d\n", myhd[k], hd[k]); }
    // printf("cg %d us %d\n", cg, us);
    // for ( k = 0; k < us; k++) { printf("fd %d ud %d\n", fd[k], ud[k]); }
    

    // if ( the < 0 || the > 4 ) { the = -1; }
    return the;
}


//====================================================================
//  補助関数
//====================================================================

// void init(int count[5][13]) {
//     int i, j;
//     for ( i = 0; i < HNUM; i++ ) {
//         for ( j = 0; j < 13; j++ ) {
//             count[i][j] = 0;
//         }
//     }
// }

// void count_sum(int count[][], int sum[], int n) {
//     int i, j; 
//     for ( i = 0; i < HNUM; i++ ) {
//         for ( j = 0; j < n; j++ ) {
//             sum[j] += count[i][j];
//         }
//     }
// }

int two_pair(int numCount[5][13], int n, int m) {
    int i, j;
    for ( i = 0; i < HNUM; i++ ) {
        for ( j = 0; j < 13; j++ ) { 
            if ( numCount[i][j] == 1 && j != n && j != m ) {
                return i;
            }
        }
    }
}

int one_pair(int ud[], int us, int hd[], int n) {
    int i, j;
    int num;     // num1 = 捨てた数字
    int sum[5] = {0};
    
    for ( i = 0; i < us; i++ ) {
        num = ud[i] % 13;
        for ( j = 0; j < HNUM; j++ ) {
            if ( num == (hd[j] % 13) && j != n ) {
                sum[j]++;
            }
        }
    }
    return arr_max(sum);
}

int straight(int hd[]) {
    int k, num, count = 0;
    
    //arr_output(hd, 5);
    arr_order(hd);
    //arr_output(hd, 5);
    //puts("");
    for ( k = 0; k < HNUM-1; k++ ) {
        if ( hd[4] - hd[0] == 4 ) {
            if ( hd[k] == hd[k+1] - 1 ) {
                count++;
            } else {
                num = k;
            }
        } else if ( hd[k] == hd[k+1] - 1 ) {
            count++;
        } else {
            num = k;
        }
    }

    if ( count == 3 || count == 2 ) {
        return num;
    } else {        // count == 4 も含む
        return -1;
    }
}

int flash(int hd[], int shdcCount[5][4], int n) {
    int i, j;

    for ( i = 0; i < HNUM; i++ ) {
        for ( j = 0; j < HNUM-1; j++ ) {
            if ( shdcCount[i][j] == 1 && j != n ) {
                return i;
            }
        }
    }
}

void arr_order(int arr[5]) {
    int i, j;
    int tmp;
    
    for ( i = 0; i < HNUM; i++ ) {
        for ( j = 4; j > i; j-- ) {
            if ( arr[j] < arr[j-1] ) {
                tmp = arr[j];
                arr[j] = arr[j-1];
                arr[j-1] = tmp;
            }
        }
    }
}

int arr_max(int arr[5]) {
    int i, n = 0;
    int max = arr[0];

    for ( i = 1; i < HNUM; i++ ) {
        if ( max < arr[i] ) {
            max = arr[i];
            n = i;
        }    
    }
    return n;
}
