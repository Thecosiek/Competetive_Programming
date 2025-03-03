// We are given n - array filled with ones. There are two types of actions: N (a, b, k) - increase segment [a;b] by k , C (a,b) - give
// the longest contiguous non-decreasing sequence in segment [a;b]
// input: n - arrat lenght, m - amount of actions, in each of m lines either 'N', a, b, k for action N or 'C', a, b for action C. 

//Maksymilian Piątek
#include<bits/stdc++.h>
using namespace std;

struct Node {
    int ciag;   //Długość ciagu dla danego zakresu
    int prefix; //Najdluzszy prefix niemalejacy dla zakresu
    int suffix; //Analogicznie suffix
    int lewy; // Wartosc najbardziej na lewo przedzialu
    int prawy; // Wartosc najbardziej na prawo przedzialu
    int ans; // Miejsce na odpowiedz na zapytanie
};

vector<Node> SegTree; // Drzewo
vector<int> lazy; // Wartosc do lazy propagation
vector<int> left_ans;   // Składowa odpowiedz dla wezła z lewa
vector<int> right_ans;  // Analogicznie z prawa

int N = 0;
int p = 1;

void build(int node, int start, int end){   // Inicjacja drzewa, na poczatku wszystkie wartosci to 1
    if(start == end){
        SegTree[node] = {1, 1, 1, 1, 1, -1};
    }
    else{
        int mid = (start + end) / 2;
        build(2*node, start, mid);
        build(2*node + 1, mid + 1, end);

        SegTree[node] = {SegTree[2*node].ciag + SegTree[2*node + 1].ciag, SegTree[2*node].prefix + SegTree[2*node + 1].prefix, 
                                    SegTree[2*node].suffix + SegTree[2*node + 1].suffix, SegTree[2*node].lewy, SegTree[2*node + 1].prawy, -1};
    }
}

void propagate(int node){
    if(lazy[node] != 0){

        int v = lazy[node];  //Przenoszenie wartośći do synów

        lazy[2 * node] += v;
        lazy[2 * node + 1] += v;

        SegTree[2*node].lewy += v;
        SegTree[2*node].prawy += v;

        SegTree[2*node + 1].lewy += v;
        SegTree[2*node + 1].prawy += v;

        lazy[node] = 0;
    }
}

void updateRange(int node, int start, int end, int a, int b, int k){
    if(a > end || b < start || start > end){ //Poza zakresem
        return;
    }

    if(start >= a && end <= b){ // w pelni w zakresie
        lazy[node] += k;
        SegTree[node].lewy += k;
        SegTree[node].prawy += k;
        return;
    }

    propagate(node);

    int mid = (start + end) / 2;
    updateRange(node * 2, start, mid , a, b, k);
    updateRange(node * 2 + 1, mid + 1, end, a, b, k);

    SegTree[node].lewy = SegTree[node * 2].lewy; // Aktualizujemy wartosci na podstawie synów
    SegTree[node].prawy = SegTree[node * 2 + 1].prawy; 

    SegTree[node].prefix = SegTree[node * 2].prefix;
    SegTree[node].suffix = SegTree[node * 2 + 1].suffix;

    SegTree[node].ciag = max(SegTree[node * 2].ciag, SegTree[node * 2 + 1].ciag);


    if(SegTree[2*node].prawy <= SegTree[2*node + 1].lewy){ // Jesli miedzy prawym i lewym synem jest ciag niemalejacys
        if(SegTree[2 * node].prefix == mid - start + 1){ // Sprawdzamy czy wydłużyć prefix
            SegTree[node].prefix += SegTree[2*node + 1].prefix;
        }

        if(SegTree[2 * node + 1].suffix == end - mid){ // Sprawdzamy czy wydłużyć suffix
            SegTree[node].suffix += SegTree[2*node].suffix;
        }

        int q = SegTree[2*node].suffix + SegTree[2*node + 1].prefix;

        if(q > SegTree[node].ciag){ // Sprawdzamy czy nowy ciąg jest dłuższy niż maksimum z dotychczasowych ciągów
            SegTree[node].ciag = q;
        }
    }

}

int queryRange(int node, int start, int end, int a, int b){
    if(start > end || start > b || end < a){ //przedzial za zakresem
        return 0;
    }

    if(start >= a && end <= b){ //Przedzial w pelni w srodku zapytania
        SegTree[node].ans = SegTree[node].ciag;

        left_ans[node] = SegTree[node].prefix;
        right_ans[node] = SegTree[node].suffix;

        return SegTree[node].ans;
    }
    
    propagate(node);


    int mid = (start + end) / 2;

    int p1 = queryRange(node * 2, start, mid, a, b);
    int p2 = queryRange(node * 2 + 1, mid + 1, end, a, b);

    if(p1 == 0){ //Jesli lewa strona jest poza zakresem to interesuje nas tylko prawa
        left_ans[node] = -1;
        right_ans[node] = right_ans[2*node + 1];

        SegTree[node].ans = SegTree[node * 2 + 1].ans; 
        return SegTree[node].ans;
    }

    if(p2 == 0){ // Analogicznie interesuje nas tylko lewa
        left_ans[node] = left_ans[2*node];
        right_ans[node] = -1;

        SegTree[node].ans = SegTree[node * 2].ans; 
        return SegTree[node].ans;
    }

    SegTree[node].ans = max(SegTree[2*node].ans, SegTree[2*node + 1].ans); // Aktualizujemy wartości na podstawie odpowiedzi z prawej i lewej

    left_ans[node] = left_ans[node * 2];
    right_ans[node] = right_ans[node * 2 + 1];

    if(SegTree[2*node].prawy <= SegTree[2*node + 1].lewy){ //Jesli miedzy przedzialami jest ciag niemalejacy
        if(left_ans[2*node] == mid - start + 1){ //Sprawdzamy czy mozna wydluzyc lewa odpowiedz o lewa z prawego syna
            left_ans[node] += left_ans[2*node + 1];
        }

        if(right_ans[2 * node + 1] == end - mid){ // Analogicznie patrzymy na prawą z lewego syna
            right_ans[node] += right_ans[2*node];
        }

        int q = right_ans[2*node] + left_ans[2*node + 1]; // Patrzymy czy poprzez polaczenie node nie stworzyla sie w srodku wieksza odpowiedz

        if(q > SegTree[node].ans){
            SegTree[node].ans = q;
        }
    }
    return SegTree[node].ans;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, m;
    cin >> n >> m;

    if(n == 1){ //edge case - ciąg długości 1
        for(int x = 0; x < m; x++){
            char c;
            cin >> c;

            if (c == 'N') {
                int a, b, k;
                cin >> a >> b >> k;
            } 

            else if (c == 'C') {
                int a, b;
                cin >> a >> b;
                cout << 1 << "\n";
            }
        }
        return 0;
    }

    N = n;

    while(p < n){ 
        p*=2;
    }

    SegTree.resize(2*p); // Resize na Vektorze
    lazy.resize(2*p, 0);
    left_ans.resize(2*p, -1);
    right_ans.resize(2*p, -1);

    build(1, 1, n);

    for(int x = 0; x < m; x++){ // Zapytania do drzewa
        char c;
        cin >> c;

        if (c == 'N') { //Zapytanie ze zwiekszaniem na przedziale
            int a, b, k;
            cin >> a >> b >> k;
            updateRange(1, 1, n, a, b, k);
            /*for(int x = p; x < 2*p; x++){
                cout << SegTree[x].lewy << " ";
            }
            cout << "\n";*/
        } 

        else if (c == 'C') { // Zapytanie z pytaniem o ciag niemalejacy
            int a, b;
            cin >> a >> b;
            cout << queryRange(1, 1, n, a, b) << "\n";
            //cout << SegTree[1].ans;
        }
    }

    return 0;
}
