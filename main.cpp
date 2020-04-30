#include <fstream>

using namespace std;

ifstream fin("abce.in");
ofstream fout("abce.out");

class Node{
public:
    int key;
    Node *left;
    Node *right;
    int height;
};

Node* newNode(int key)
{
    Node* node = new Node();
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return(node);
}

int height(Node *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

Node *rightrotation(Node *y)
{
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

Node *leftrotation(Node *x)
{
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(Node *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

Node* insert(Node* node, int key)
{
    if (node == NULL)
        return(newNode(key));

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && key < node->left->key)
        return rightrotation(node);

    if (balance < -1 && key > node->right->key)
        return leftrotation(node);

    if (balance > 1 && key > node->left->key)
    {
        node->left = leftrotation(node->left);
        return rightrotation(node);
    }

    if (balance < -1 && key < node->right->key)
    {
        node->right = rightrotation(node->right);
        return leftrotation(node);
    }

    return node;
}

Node * minValueNode(Node* node)
{
    Node* current = node;

    while (current->left != NULL)
        current = current->left;

    return current;
}

Node* deleteNode(Node* root, int key)
{
    if (root == NULL)
        return root;

    if ( key < root->key )
        root->left = deleteNode(root->left, key);

    else if( key > root->key )
        root->right = deleteNode(root->right, key);

    else
    {
        if( (root->left == NULL) || (root->right == NULL) )
        {
            Node *temp = root->left ?  root->left :  root->right;

            // No child case
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else
                *root = *temp;

            free(temp);
        }
        else
        {
            Node* temp = minValueNode(root->right);

            root->key = temp->key;

            root->right = deleteNode(root->right,
                                     temp->key);
        }
    }

    if (root == NULL)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightrotation(root);

    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left = leftrotation(root->left);
        return rightrotation(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftrotation(root);

    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightrotation(root->right);
        return leftrotation(root);
    }

    return root;
}

bool find(Node *root, int key)
{
    if(root == NULL)
        return false;
    if(root->key < key)
        return find(root->right, key);
    if(root->key > key)
        return find(root->left, key);
    return true;
}

void inorder(Node *root)
{
    if(root != NULL)
    {
        inorder(root->left);
        fout << root->key << " ";
        inorder(root->right);
    }
}

int findbigY(Node *root, int x)
{
    int ret = -2000000000;
    if(root != NULL)
    {
        if(root->key == x)
            ret = root->key;
        else if(root->key > x)
            ret = findbigY(root->left, x);
        else if(root->key < x)
            ret = max(root->key, findbigY(root->right, x));
    }
    return ret;
}

int findsmallY(Node *root, int x)
{
    int ret = 2000000000;
    if(root != NULL)
    {
        if(root->key == x)
            ret = root->key;
        else if(root->key < x)
            ret = findsmallY(root->right, x);
        else if(root->key > x)
            ret = min(root->key, findsmallY(root->left, x));
    }
    return ret;
}

void print(Node *root, int x, int y)
{
    if(root != NULL)
    {
        if(root->key > x)
            print(root->left, x, y);

        if(root->key >= x && root->key <= y)
            fout << root->key << ' ';

        if(root->key < y)
            print(root->right, x, y);
    }
}
int main()
{
    Node *root = NULL;
    int n, Q, x, y;
    fin >> n;
    for(int i = 1; i <= n; i++)
    {
        fin >> Q;
        switch(Q)
        {
        case 1:
            fin >> x;
            root = insert(root, x);
            break;
        case 2:
            fin >> x;
            root = deleteNode(root, x);
            break;
        case 3:
            fin >> x;
            fout << find(root, x) << '\n';
            break;
        case 4:
            fin >> x;
            fout << findbigY(root, x) << '\n';
            break;
        case 5:
            fin >> x;
            fout << findsmallY(root, x) << '\n';
            break;
        default:
            fin >> x >> y;
            print(root, x, y);
            fout << '\n';
        }

    }
    return 0;
}
