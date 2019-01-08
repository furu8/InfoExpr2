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

#include "Poker.h"

//--------------------------------------------------------------------
//  関数宣言
//--------------------------------------------------------------------

void judge_value(int hd[], int arr[], int shdcCount[5][4], int numCount[5][13], int shdcSum[], int numSum[]);               // 判定値を計算
int except_check(int sum[], int n, int num);                                                                                // 例外のときは-2を返す                                                                          // フラッシュを狙う
int nopair(int ud[], int us, int hd[], int myhd[], int shdcCount[5][4], int shdcSum[], int numCount[5][13], int numSum[]);  // ノーペア時の呼び出し関数
int onepair(int ud[], int us, int hd[], int myhd[], int shdcCount[5][4], int shdcSum[], int numCount[5][13], int numSum[]); // ワンペア時の呼び出し関数
int three_card(int ud[], int us, int hd[], int numSum[]);                                                                   // スリーカードからフォーカードまたはフルハウス
int two_pair(int numCount[5][13], int numSum[]);                                                                            // ツーペアからスリーカードまたはフルハウス
int one_pair(int ud[], int us, int hd[], int numSum[]);                                                                     // ワンペアからツーペア
int no_pair(int ud[], int us, int hd[]);                                                                                    // ノーペアからワンペア
int no_straight(int numCount[5][13], int numSum[]);                                                                         // ノーペア時のストレートを狙う
int one_straight(int numCount[5][13], int numSum[]);                                                                        // ワンペア時のストレートを狙う
int ace_straight(int hd[], int myhd[]);                                                                                     // Aを含むストレートを狙う
int flash(int shdcCount[5][4], int n);                                                                                      // フラッシュを狙う
void arr_order(int arr[]);                                                                                                  // 配列の順番を降順にする
int arr_max(int arr[5]);                                                                                                    // 最大値の配列の要素を返却



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

int strategy( int hd[], int fd[], int cg, int tk,  int ud[], int us) {
    int myhd[HNUM];
    int the;
    int k;
    int shdc;                       // スペード、ハート、ダイヤ、クラブ のどれか判断
    int num;                        // 手札の数字を判断
    int arr[5];                     // 手札の数字を配列に格納
    int shdcSum[4] = {0};           // 手札の種類の合計値を格納
    int numSum[13] = {0};           // 手札の数字の合計値を格納
    int shdcCount[5][4] = {{0}};    // 手札の種類の枚数
    int numCount[5][13] = {{0}};    // 手札の数字の枚数

    for ( k = 0; k < HNUM; k++ ) { myhd[k] = hd[k]; }

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

         0  1  2  3  4 
        S5 C5 C9 SK D9  0
        0 0 0 0 2 0 0 0 2 0 0 0 1

        H8 S2 C2 C7 D8  2
        0 2 0 0 0 0 1 2 0 0 0 0 0

        H8 S2 D7 C7 D8  1
        0 1 0 0 0 0 2 2 0 0 0 0 0
    */

    // ---- 手札の判定
    // -- 判定値を計算
    judge_value(myhd, arr, shdcCount, numCount, shdcSum, numSum);

    // -- poker_pointで分類
    if ( poker_point(myhd) > P3 ) {                                             // ロイヤル、フォーカード、フルハウス、ストレートフラッシュ、フラッシュ、ストレート
        the = -1;
    } else if ( poker_point(myhd) == P3 ) {                                     // スリーカード -> フルハウス、フォーカード
        the = three_card(ud, us, myhd, numSum);
    } else if ( poker_point(myhd) == P2 ) {                                     // ツーペア -> スリーカード、フルハウス
        the = two_pair(numCount, numSum); 
    } else if ( poker_point(myhd) == P1 ) {                                     // ワンペア -> ツーペア、フラッシュ、ストレート
        the = onepair(ud, us, arr, myhd, shdcCount, shdcSum, numCount, numSum);
    } else if ( poker_point(myhd) == P0 ) {                                     // ノーペア -> ワンペア、フラッシュ、ストレート
        the = nopair(ud, us, arr, myhd, shdcCount, shdcSum, numCount, numSum);
    }

    // card_show(hd, HNUM);
    // printf(" %d\n", the);
    // for ( k = 0; k < 13; k++ ) { 
    //     printf("%d ", numSum[k]);
    // }
    // puts("");
    // for ( k = 0; k < us; k++) { printf("%d ", ud[k] % 13 + 1); }
    // puts("\n");
    
    return the;
}


//====================================================================
//  補助関数
//====================================================================

 void judge_value(int hd[], int arr[], int shdcCount[5][4], int numCount[5][13], int shdcSum[], int numSum[]) {
    int k, num, shdc;

    for ( k = 0; k < HNUM; k++ ) { 
        shdc = hd[k] / 13;
        arr[k] = num = hd[k] % 13;
        shdcCount[k][shdc]++;
        numCount[k][num]++;
        shdcSum[shdc]++;
        numSum[num]++;
    }
}

int except_check(int sum[], int n, int num) {
    int i;

    for ( i = 0; i < n; i++ ) {
        if ( sum[i] == num ) { return i; } 
    }
    return -2;
}

int nopair(int ud[], int us, int hd[], int myhd[], int shdcCount[5][4], int shdcSum[], int numCount[5][13], int numSum[]) {
    int num, the;
    if ( (num = except_check(shdcSum, 4, 4)) != -2 ) { 
        the = flash(shdcCount, num);                                                        
    } else if ( (the = no_straight(numCount, numSum)) == -2 ) {                             
        if ( (the = ace_straight(hd, myhd)) == -2 ) { 
            the = no_pair(ud, us, myhd); 
        }
    }
    return the;
}

int onepair(int ud[], int us, int hd[], int myhd[], int shdcCount[5][4], int shdcSum[], int numCount[5][13], int numSum[]) {
    int num, the;
    
    if ( (num = except_check(shdcSum, 4, 4)) != -2 ) { 
        the = flash(shdcCount, num);                                                       
    } else if ( (the = one_straight(numCount, numSum)) == -2 ) {                            
        if ( (the = ace_straight(hd, myhd)) == -2 ) { 
            the = one_pair(ud, us, myhd, numSum); 
        }
    }
    return the;
}

int three_card(int ud[], int us, int hd[], int numSum[]) {
    int i, j, num;
    int sum[5] = {0};

    num = except_check(numSum, 13, 3);
    for ( i = 0; i < us; i++ ) {
        for ( j = 0; j < HNUM; j++ ) {
            if ( hd[j] % 13 == num ) {
                sum[j]--;
            } else {
                if ( ud[i] % 13 == hd[j] % 13 ) { sum[j]++; }
            }
        }
    }
    return arr_max(sum);
}

int two_pair(int numCount[5][13], int numSum[]) {
    int num, k;

    num = except_check(numSum, 13, 1);
    for ( k = 0; k < HNUM; k++ ) {
        if ( numCount[k][num] == 1 ) { return k; }
    }
}

int one_pair(int ud[], int us, int hd[], int numSum[]) {
    int i, j, num;
    int sum[5] = {0};

    num = except_check(numSum, 13, 2);
    for ( i = 0; i < us; i++ ) {
        for ( j = 0; j < HNUM; j++ ) {
            if ( hd[j] % 13 == num ) {
                sum[j]--;
            } else {
                if ( ud[i] % 13 == hd[j] % 13 ) { sum[j]++; }
            }
        }
    }
    return arr_max(sum);
}

int no_pair(int ud[], int us, int hd[]) {
    int i, j, num;
    int sum[5] = {0};

    for ( i = 0; i < us; i++ ) {
        for ( j = 0; j < HNUM; j++ ) {
            if ( ud[i] % 13 == hd[j] % 13 ) {
                sum[j]++;
            }
        }
    }
    return arr_max(sum);
}

int no_straight(int numCount[5][13], int numSum[]) {
    int k, num1, num2, count = 0, end = 0;
    int flag1 = 0, flag2 = 0;
    
    for ( k = 0; k < 13; k++ ) {
        if ( numSum[k] == 1 && flag1 == 0 ) {
            count++;
            flag2 = 1;
            num1 = k;
        } else if ( numSum[k] == 0 ) {
            if ( numSum[k+1] == 0 && flag2 == 1 ) {
                flag1 = 1; 
            } 
        } else if ( numSum[k] == 1 ) {
            end++;
            num2 = k;
        }
    }
    for ( k = 0; k < HNUM; k++ ) {
        if ( count == 4 && numCount[k][num2] == 1 ) {
            return k;
        } else if ( end == 4 && numCount[k][num1] == 1 ) {
            return k;
        }
    }
    return -2;
}

int one_straight(int numCount[5][13], int numSum[]) {
    int k, num, count = 0;
    int flag1 = 0, flag2 = 0;
    
    for ( k = 0; k < 13; k++ ) {
        if ( numSum[k] == 1 && flag1 == 0 ) {
            count++;
            flag2 = 1;
        } else if ( numSum[k] == 0 ) {
            if ( numSum[k+1] == 0 && flag2 == 1 ) {
                flag1 = 1; 
            } 
        } else if ( numSum[k] == 2 && flag1 == 0 ) {
            count++;
            flag2 = 1;
            num = k;
        }
    }
    for ( k = 0; k < HNUM; k++ ) {
        if ( count == 4 && numCount[k][num] == 1 ) {
            return k;
        }
    }
    return -2;
}

int ace_straight(int hd[], int myhd[]) {
    int i, num = 0;
    int n = 9, count = 0;

    arr_order(hd);
    for ( i = 1; i < HNUM; i++ ) {
        if ( hd[i] == n ) {
            count++;
        } else {
            num = i;
        }
        n++;
    }

    for ( i = 0; i < HNUM; i++ ) {
        if ( hd[num] == myhd[i] % 13 ) {
            num = i;
            break;
        }
    }
    
    if ( count == 3 && hd[0] == 0 ) {
        return num;
    } else if ( count == 4 ) {
        return num;
    } else {
        return -2;
    }
}

int flash(int shdcCount[5][4], int n) {
    int i, j;
    
    for ( i = 0; i < HNUM; i++ ) {
        for ( j = 0; j < HNUM-1; j++ ) {
            if ( shdcCount[i][j] == 1 && j != n ) {
                return i; 
            }
        }
    }
}

void arr_order(int arr[]) {
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
