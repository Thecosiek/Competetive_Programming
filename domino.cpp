//We are given k*n matrix. The task is to write the biggest sum we can get by placing domino pieces (1*2 or 2*1). Domino pieces need to be
// fully in matrix, two pieces cannot overlap on each other. 

//solve: clever dynamic with bitmask that reprezents possible placements in previous row and bool which reprezents possible placement of domino 
//in previous step.

//Maksymilian Piątek
#include <bits/stdc++.h>
using namespace std;

const int NMAX = 1e3, KMAX = 12;
const int BSMAX = (1 << (KMAX + 1));

int plansza[NMAX][KMAX];
long long dp[2][KMAX][BSMAX][2];

int main(){

    int n, k;
    cin >> n >> k;

    int bs = (1 << (k + 1));

    for(int x = 0; x < k; x++){ 
        for(int y = 0; y < n; y++){
            cin >> plansza[y][x];
        }
    }

    for(int x = 0; x < n; x++){
        for(int y = 0; y < k; y++){
            for(int b = 0; b < bs; b++){
                for(int l = 0; l < 2; l++){
                    if((b & 1) and l == 1){
                        dp[x % 2][y][b][l] = -1e9;
                        continue;
                    }
                    if( x == 0 and y == 0){
                        if((b & 1) or l == 1){
                            dp[x % 2][y][b][l] = plansza[x][y];
                        }
                        else{
                            dp[x % 2][y][b][l] = 0;
                        }
                        continue;
                    }

                    int prevX, prevY;

                    if(y != 0){
                        prevX = x;
                        prevY = y-1;
                    }
                    else{
                        prevX = x-1;
                        prevY = k-1;
                    }

                    int prevB = b >> 1;

                    if(l == 1){
                        dp[x % 2][y][b][l] = plansza[x][y] + dp[prevX % 2][prevY][prevB][0];
                    }
                    else if(b & 1){
                        dp[x % 2][y][b][l] = plansza[x][y] + dp[prevX % 2][prevY][prevB][0];
                    }
                    else{
                        long long best = dp[prevX % 2][prevY][b >> 1][0];

                        if(y != 0){
                            best = max(best, plansza[x][y] + dp[prevX % 2][prevY][prevB][1]);
                        }

                        if(x != 0){
                            int mask = 1 << (k-1);
                            prevB = prevB | mask;
                            best = max(best, plansza[x][y] + dp[prevX % 2][prevY][prevB][0]);
                        }
                        dp[x % 2][y][b][l] = best;
                    }
                }
            }
        }
    }

    cout << dp[(n-1) % 2][k-1][0][0];

    return 0;
}
