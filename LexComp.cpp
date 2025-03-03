// We Are Given n-letter word composed of lowercase letters of the English alphabet. We will deal with subwords of this word. 
// Input: n - word length, m - number of queries, following m lines: a, b, x, y where a is start and b is end of the first subword and x is start
// and y is end of second subword. Output: sign < , >, = which reprezents lexigraphicall relation between subwords.

// Solve - standard lexigraphic comparator algorythm with bucket sort

#include <bits/stdc++.h>
using namespace std;

const int N = 300000;

int nazwa[N];
int tymczasowa[N];
int klucz[N];
int licznik[N+1];

void KubSort(int next, int n) {
    memset(licznik, 0, sizeof(int)*(n+1));
    for(int i = 0; i < n; i++){
        int pozycja = nazwa[i] + next;
        int r = (pozycja < n ? klucz[pozycja] : -1) + 1;
        licznik[r]++;
    }
    for(int i = 1; i <= n; i++){
        licznik[i] += licznik[i-1];
    }
    for(int i = n-1; i >= 0; i--){
        int pozycja = nazwa[i] + next;
        int r = (pozycja < n ? klucz[pozycja] : -1) + 1;
        tymczasowa[--licznik[r]] = nazwa[i];
    }
    for(int i = 0; i < n; i++){
        nazwa[i] = tymczasowa[i];
    }
}
    
int tab[20][N];
int nowy[N];

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(NULL);
    cout.tie(NULL);

    int n, m;
    string s;
    cin >> n >> m;
    cin >> s;

    
    int pot;

    for(int i = 0; i < n; i++){
        klucz[i] = s[i] - 'a';
        nazwa[i] = i;
    }

    pot = 0; 
    while((1 << pot) <= n) pot++;
    KubSort(0, n);
    {
        int r = 0; 
        tab[0][nazwa[0]] = 0;
        for(int i = 1; i < n; i++){
            if(s[nazwa[i]] != s[nazwa[i-1]]) r++;
            tab[0][nazwa[i]] = r;
        }
        for(int i = 0; i < n; i++){
            klucz[i] = tab[0][i];
        }
    }

    int length = 1;
    for(int k = 1; k < pot; k++){
        int next = length;
        KubSort(next, n);
        KubSort(0, n);

        nowy[nazwa[0]] = 0;
        int r = 0;
        for(int i = 1; i < n; i++){
            int a1 = klucz[nazwa[i]];
            int a2 = (nazwa[i] + next < n ? klucz[nazwa[i] + next] : -1);
            int o1 = klucz[nazwa[i-1]];
            int o2 = (nazwa[i-1] + next < n ? klucz[nazwa[i-1] + next] : -1);
            if(a1 != o1 || a2 != o2) r++;
            nowy[nazwa[i]] = r;
        }

        for(int i = 0; i < n; i++){
            tab[k][i] = nowy[i];
        }
        for(int i = 0; i < n; i++){
            klucz[i] = nowy[i];
        }
        length <<= 1;
        if(length >= n) break;
    }


    for(int x = 0; x < m; x++){
        int a, b, c, d;
        cin >> a >> b >> c >> d;

        int w1_len = b - a + 1;
        int w2_len = d - c + 1;

        int w1_poz = a - 1;
        int w2_poz = c - 1;

        int result = 0;
        int check = std::min(w1_len, w2_len);

        for(int q = pot - 1; q >= 0; q--) {
            int length = (1 << q);
            if(length <= check) {
                if(tab[q][w1_poz] != tab[q][w2_poz]) {
                    if(tab[q][w1_poz] < tab[q][w2_poz]){
                        result = -1;
                    }
                    else{
                        result = 1;
                    }
                    break;
                }
                w1_poz += length;
                w2_poz += length;
                check -= length;
                if(check == 0) {
                    break;
                }
            }
        }
        if(result == 0){
            if(w1_len == w2_len){
                cout << '=' << "\n";
                continue;
            }
            else if(w1_len < w2_len){
                cout << '<' << "\n";
                continue;
            }
            else{
                cout << '>' << "\n";
                continue;
            }
        }

        if(result == 0){
            cout << '=' << "\n";
        }
        else if(result < 0){
            cout << '<' << "\n";
        }
        else{
            cout << '>' << "\n";
        }
    }


    return 0;
}
