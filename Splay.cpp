// Splay tree which starts empty and allows two operations insert (j, x, k) which puts k copies of x in position j. get(j) - gives the number
//on position j.


#include <bits/stdc++.h>
using namespace std;

struct SplayNode {
    int value;
    int count;
    int size;
    SplayNode *left, *right;

    SplayNode(int val, int c)
        : value(val), count(c), size(c), left(nullptr), right(nullptr) {}
};

inline int getSize(SplayNode* node) {
    return node ? node->size : 0;
}

inline void updateSize(SplayNode* node) {
    if (node) {
        node->size = getSize(node->left) + getSize(node->right) + node->count;
    }
}

SplayNode* rotateRight(SplayNode* y) {
    if (!y || !y->left) return y; 
    SplayNode* x = y->left;
    y->left = x->right;
    x->right = y;
    updateSize(y);
    updateSize(x);
    return x;
}

SplayNode* rotateLeft(SplayNode* x) {
    if (!x || !x->right) return x; 
    SplayNode* y = x->right;
    x->right = y->left;
    y->left = x;
    updateSize(x);
    updateSize(y);
    return y;
}

SplayNode* splay(SplayNode* root, int idx) {
    if (!root) return nullptr;

    int leftSize = getSize(root->left);
    int nodeRangeStart = leftSize;
    int nodeRangeEnd = leftSize + root->count - 1;

    if (idx < nodeRangeStart) {
        if (!root->left) return root;

        int leftLeftSize = getSize(root->left->left);
        if (idx < leftLeftSize) {
            root->left->left = splay(root->left->left, idx);
            root = rotateRight(root);
        } else if (idx >= leftLeftSize + root->left->count) {
            root->left->right = splay(root->left->right, idx - (leftLeftSize + root->left->count));
            if (root->left->right) root->left = rotateLeft(root->left);
        }
        return rotateRight(root);
    } else if (idx > nodeRangeEnd) {
        if (!root->right) return root;

        idx -= (nodeRangeEnd + 1);
        int rightLeftSize = getSize(root->right->left);
        if (idx < rightLeftSize) {
            root->right->left = splay(root->right->left, idx);
            if (root->right->left) root->right = rotateRight(root->right);
        } else if (idx >= rightLeftSize + root->right->count) {
            root->right->right = splay(root->right->right, idx - (rightLeftSize + root->right->count));
            root->right = rotateLeft(root->right);
        }
        return rotateLeft(root);
    }
    return root;
}

SplayNode* mergeTrees(SplayNode* A, SplayNode* B) {
    if (!A) return B;
    if (!B) return A;
    A = splay(A, getSize(A) - 1);
    A->right = B;
    updateSize(A);
    return A;
}

pair<SplayNode*, SplayNode*> split(SplayNode* root, int idx) {
    if (!root) return {nullptr, nullptr};
    root = splay(root, idx);

    int leftSize = getSize(root->left);
    int nodeRangeStart = leftSize;
    int nodeRangeEnd = leftSize + root->count - 1;

    if (idx <= nodeRangeStart) {
        SplayNode* L = root->left;
        root->left = nullptr;
        updateSize(L);
        updateSize(root);
        return {L, root};
    }

    if (idx > nodeRangeEnd) {
        SplayNode* R = root->right;
        root->right = nullptr;
        updateSize(root);
        updateSize(R);
        return {root, R};
    }

    int A = idx - nodeRangeStart;
    int B = root->count - A;

    SplayNode* leftNode = nullptr;
    if (A > 0) leftNode = new SplayNode(root->value, A);

    root->count = B;
    updateSize(root);

    SplayNode* L = root->left;
    root->left = nullptr;
    updateSize(L);

    return {mergeTrees(L, leftNode), root};
}

SplayNode* insert(SplayNode* root, int j, int x, int k) {
    auto [L, R] = split(root, j);
    SplayNode* newBlock = new SplayNode(x, k);
    return mergeTrees(mergeTrees(L, newBlock), R);
}

int getValue(SplayNode*& root, int j) {
    if (!root) throw runtime_error("Invalid access to null tree.");
    root = splay(root, j);
    return root->value;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m;
    cin >> m;

    SplayNode* root = nullptr;
    long long lastGet = 0;
    long long n = 0;

    for (int q = 0; q < m; q++) {
        char op;
        cin >> op;

        if (op == 'i') {
            long long j1, x;
            int k;
            cin >> j1 >> x >> k;
            long long j = (j1 + lastGet) % (n + 1);

            root = insert(root, (int)j, (int)x, k);
            n += k;
        } else {
            long long j1;
            cin >> j1;
            long long j = (j1 + lastGet) % n;

            int w = getValue(root, (int)j);
            cout << w << "\n";
            lastGet = w;
        }
    }

    return 0;
}
