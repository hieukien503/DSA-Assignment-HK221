#ifndef __CONCAT_STRING_TREE_H__
#define __CONCAT_STRING_TREE_H__

#include "main.h"

extern int cst_id;

extern int str_len(const char* s);
extern void reverseString(char* s);
extern bool compareString(char* s1, char* s2);
extern void subStr(char*& s, int from, int len);

class ConcatStringNode
{
public:
    class ParentsTree;
public:
    char* s;
    int id;
    int leftLength;
    int realLength;
    ConcatStringNode* left, * right;
    ParentsTree* avl;
public:
    ConcatStringNode(const char* s = nullptr, ConcatStringNode* left = nullptr, ConcatStringNode* right = nullptr, int id = 0)
    {
        if (s == nullptr)
        {
            this->s = nullptr;
        }
        else
        {
            int len = str_len(s);
            this->s = new char[len + 1];
            for (int i = 0; i < len; i++)
            {
                this->s[i] = s[i];
            }
            this->s[len] = '\0';
        }
        if (cst_id > 10000000)
        {
            throw overflow_error("Id is overflow!");
        }
        this->id = id;
        this->leftLength = 0;
        this->realLength = str_len(this->s);
        this->left = left;
        this->right = right;
        this->avl = new ParentsTree;
    }
    ~ConcatStringNode()
    {

    }
public:
    class ParentsTree
    {
    public:
        class Node;
    protected:
        Node* root;
        int numNodes;
    public:
        ParentsTree()
        {
            root = nullptr;
            numNodes = 0;
        }
        ~ParentsTree()
        {
            cleanUp();
        }
    protected:
        int height(Node* root)
        {
            if (root == nullptr)
            {
                return 0;
            }
            int leftHeight = 1 + height(root->left), rightHeight = 1 + height(root->right);
            return max(leftHeight, rightHeight);
        }
        int getBalanceFactor(Node* x)
        {
            if (x == nullptr)
            {
                return 0;
            }
            return height(x->left) - height(x->right);
        }
        Node* rightRotate(Node* x)
        {
            Node* y = x->left;
            x->left = y->right;
            y->right = x;
            y->bal_factor = getBalanceFactor(y);
            x->bal_factor = getBalanceFactor(x);
            return y;
        }
        Node* leftRotate(Node* x)
        {
            Node* y = x->right;
            x->right = y->left;
            y->left = x;
            y->bal_factor = getBalanceFactor(y);
            x->bal_factor = getBalanceFactor(x);
            return y;
        }
        Node* LeftRightRotate(Node* x)
        {
            x->left = leftRotate(x->left);
            return rightRotate(x);
        }
        Node* RightLeftRotate(Node* x)
        {
            x->right = rightRotate(x->right);
            return leftRotate(x);
        }
        Node* balanced(Node* root)
        {
            if (root == nullptr)
            {
                return nullptr;
            }
            root->bal_factor = getBalanceFactor(root);
            if (root->bal_factor > 1)
            {
                if (getBalanceFactor(root->left) > 0)
                {
                    root = rightRotate(root);
                }
                else
                {
                    root = LeftRightRotate(root);
                }
            }
            else if (root->bal_factor < -1)
            {
                if (getBalanceFactor(root->right) > 0)
                {
                    root = RightLeftRotate(root);
                }
                else
                {
                    root = leftRotate(root);
                }
            }
            return root;
        }
        Node* addAVL(Node* root, int key)
        {
            if (root == nullptr)
            {
                root = new Node(key);
                numNodes++;
            }
            else
            {
                if (key < root->id)
                {
                    root->left = addAVL(root->left, key);
                }
                else if (key > root->id)
                {
                    root->right = addAVL(root->right, key);
                }
                else
                {
                    root->num_repeat += 1;
                }
                root = balanced(root);
            }
            return root;
        }
        Node* findMaxValue(Node* x)
        {
            Node* temp = x;
            while (temp->right != nullptr)
            {
                temp = temp->right;
            }
            return temp;
        }
        Node* deleteAVL(Node* root, int key)
        {
            if (root == nullptr)
            {
                return nullptr;
            }
            if (key < root->id)
            {
                root->left = deleteAVL(root->left, key);
            }
            else if (key > root->id)
            {
                root->right = deleteAVL(root->right, key);
            }
            else
            {
                if (root->num_repeat == 1)
                {
                    if (root->left == nullptr || root->right == nullptr)
                    {
                        Node* temp = (root->left == nullptr ? root->right : root->left);
                        if (temp == nullptr)
                        {
                            temp = root;
                            delete temp;
                            root = nullptr;
                        }
                        else
                        {
                            *root = *temp;
                            delete temp;
                            temp = nullptr;
                        }
                        numNodes--;
                    }
                    else
                    {
                        Node* temp = findMaxValue(root->left);
                        swap(root->id, temp->id);
                        swap(root->num_repeat, temp->num_repeat);
                        root->left = deleteAVL(root->left, temp->id);
                    }
                }
                else
                {
                    root->num_repeat -= 1;
                }
            }
            return balanced(root);
        }
        string toStringPreOrder(Node* root, string ans = "") const
        {
            if (root == nullptr)
            {
                return "";
            }
            ans = ("(id=" + to_string(root->id) + ");");
            ans += toStringPreOrder(root->left, ans);
            ans += toStringPreOrder(root->right, ans);
            return ans;
        }
        void cleanUp(Node** root)
        {
            if (*root != nullptr)
            {
                cleanUp(&(*root)->left);
                cleanUp(&(*root)->right);
                delete* root;
                *root = nullptr;
            }
        }
    public:
        void addAVLTree(int key)
        {
            root = addAVL(root, key);
        }
        void deleteAVLTree(int key)
        {
            root = deleteAVL(root, key);
        }
        int size() const
        {
            return this->numNodes;
        }
        string toStringPreOrder() const
        {
            string ans = "ParentsTree[";
            int len = (int)ans.length();
            ans += toStringPreOrder(root, ans);
            if (len != (int)ans.length())
            {
                ans[ans.length() - 1] = ']';
            }
            else
            {
                ans += "]";
            }
            return ans;
        }
        void cleanUp()
        {
            cleanUp(&this->root);
        }
    public:
        class Node
        {
        private:
            int id, num_repeat;
            Node* left, * right;
            int bal_factor;
            friend class ParentsTree;
        public:
            Node(int id = 1, Node* left = nullptr, Node* right = nullptr, int bal_fac = 0)
            {
                this->id = id;
                this->num_repeat = 1;
                this->left = left;
                this->right = right;
                this->bal_factor = bal_fac;
            }
        };
    };
};

class ConcatStringTree {
public:
    class DeleteTreeList;
    class RootList;
public:
    ConcatStringNode* root;
public:
    static DeleteTreeList delTreeList;
    static RootList rootList;
protected:
    void cleanUp(ConcatStringNode** temp)
    {
        if (*temp != nullptr)
        {
            if ((*temp)->left == (*temp)->right)
            {
                cleanUp(&(*temp)->left);
            }
            else
            {
                cleanUp(&(*temp)->left);
                cleanUp(&(*temp)->right);
            }
            (*temp)->avl->~ParentsTree();
            delete (*temp)->avl;
            delete[](*temp)->s;
            delete (*temp);
            *temp = nullptr;
        }
    }
    ConcatStringNode* cloneConcatStringTree(ConcatStringNode* temp) const
    {
        if (temp == nullptr)
        {
            return nullptr;
        }
        ConcatStringNode* newRoot = new ConcatStringNode(temp->s, nullptr, nullptr, cst_id++);
        newRoot->leftLength = temp->leftLength;
        newRoot->realLength = temp->realLength;
        newRoot->left = cloneConcatStringTree(temp->left);
        newRoot->right = cloneConcatStringTree(temp->right);
        return newRoot;
    }
    string toStringPreorder(ConcatStringNode* temp, string ans) const
    {
        if (temp == nullptr)
        {
            return "";
        }
        ans = ("(LL=" + to_string(temp->leftLength) + ",L=" + to_string(temp->realLength) + "," + (temp->s == nullptr ? "<NULL>" : "\"" + string(temp->s) + "\"") + ");");
        ans += toStringPreorder(temp->left, ans);
        ans += toStringPreorder(temp->right, ans);
        return ans;
    }
    string toStringPreorder2(ConcatStringNode* temp, string ans) const
    {
        if (temp == nullptr)
        {
            return "";
        }
        ans = (temp->s == nullptr ? "" : string(temp->s));
        ans += toStringPreorder2(temp->left, ans);
        ans += toStringPreorder2(temp->right, ans);
        return ans;
    }
    void invertBT(ConcatStringNode* temp) const
    {
        if (temp == nullptr)
        {
            return;
        }
        invertBT(temp->left);
        invertBT(temp->right);
        ConcatStringNode* x = temp->left;
        temp->left = temp->right;
        temp->right = x;
    }
    void updateTree(ConcatStringNode* temp, bool isReversedString = false)
    {
        if (temp == nullptr)
        {
            return;
        }
        updateTree(temp->left, isReversedString);
        updateTree(temp->right, isReversedString);
        if (temp->left == nullptr && temp->right == nullptr)
        {
            temp->leftLength = 0;
            temp->realLength = str_len(temp->s);
        }
        else
        {
            if (temp->left == nullptr)
            {
                temp->leftLength = 0;
                temp->realLength = temp->right->realLength;
            }
            else
            {
                temp->leftLength = temp->left->realLength;
                temp->realLength = temp->left->realLength + (temp->right == nullptr ? 0 : temp->right->realLength);
            }
        }
        if (isReversedString == true)
        {
            reverseString(temp->s);
        }
    }
    void AVLInsertPreOrder(ConcatStringNode* prev, ConcatStringNode* temp)
    {
        if (temp == nullptr)
        {
            return;
        }
        temp->avl->addAVLTree(prev == nullptr ? temp->id : prev->id);
        AVLInsertPreOrder(temp, temp->left);
        AVLInsertPreOrder(temp, temp->right);
    }
    void AVLDeletePostOrder(ConcatStringNode* prev, ConcatStringNode* temp)
    {
        if (temp == nullptr)
        {
            return;
        }
        AVLDeletePostOrder(temp, temp->left);
        AVLDeletePostOrder(temp, temp->right);
        temp->avl->deleteAVLTree(prev == nullptr ? temp->id : prev->id);
    }
    void cutLink(ConcatStringNode* prev_root, ConcatStringNode** root, ConcatStringNode* prev, ConcatStringNode* curr)
    {
        if (*root == nullptr)
        {
            return;
        }
        if ((*root)->left == (*root)->right)
        {
            cutLink((*root), &(*root)->left, prev, curr);
        }
        else
        {
            cutLink((*root), &(*root)->left, prev, curr);
            cutLink((*root), &(*root)->right, prev, curr);
        }
        if (*root == curr)
        {
            if (prev_root != prev)
            {
                *root = nullptr;
                return;
            }
            else
            {
                if (prev == nullptr)
                {
                    return;
                }
                if (prev->left == prev->right)
                {
                    prev->right = nullptr;
                }
                else
                {
                    if (prev->left == curr && prev->right != nullptr)
                    {
                        cutLink(prev_root, &prev_root->right, prev, curr);
                    }
                    else if (prev_root->right == curr && prev_root->left != nullptr)
                    {
                        cutLink(prev_root, &prev_root->left, prev, curr);
                    }
                }
            }
        }
        else
        {
            if ((*root)->s == curr->s)
            {
                (*root)->s = nullptr;
            }
        }
    }
public:
    ConcatStringTree() { root = nullptr; }
    ConcatStringTree(const char* s);
    int length() const;
    char get(int index) const;
    int indexOf(char c) const;
    string toStringPreOrder() const;
    string toString() const;
    ConcatStringTree concat(const ConcatStringTree& otherS) const;
    ConcatStringTree subString(int from, int to) const;
    ConcatStringTree reverse() const;
    void invertBST() const;
    void update(bool isReversedString = false);
    void AVLInsertPreOrder()
    {
        AVLInsertPreOrder(nullptr, this->root);
    }
    void cleanUp()
    {
        cleanUp(&this->root);
    }
    void AVLDelete()
    {
        AVLDeletePostOrder(nullptr, this->root);
    }
    void cutLinkTotal(ConcatStringNode* root, ConcatStringNode* prev, ConcatStringNode* temp)
    {
        if (temp == nullptr)
        {
            return;
        }
        cutLink(nullptr, &root, prev, temp);
        cutLinkTotal(root, temp, temp->left);
        cutLinkTotal(root, temp, temp->right);
    }
    int getParTreeSize(const string& query) const;
    string getParTreeStringPreOrder(const string& query) const;
    ~ConcatStringTree();
public:
    class DeleteTreeList
    {
    public:
        class Node;
    public:
        Node* head, * tail;
        int size = 0;
        friend class ConcatStringTree;
    public:
        DeleteTreeList()
        {
            this->head = this->tail = nullptr;
            this->size = 0;
        }
        ~DeleteTreeList()
        {
            while (this->head != nullptr)
            {
                Node* temp = this->head;
                this->head = this->head->next;
                if (this->head == nullptr)
                {
                    this->tail = nullptr;
                }
                delete temp;
                temp = nullptr;
            }
        }
    public:
        void sort()
        {
            Node* temp = this->head;
            while (temp != nullptr)
            {
                if (temp->next == nullptr)
                {
                    break;
                }
                Node* curr = temp->next;
                while (curr != nullptr)
                {
                    if (temp->data->id < curr->data->id)
                    {
                        ConcatStringNode* x = temp->data;
                        temp->data = curr->data;
                        curr->data = x;
                    }
                    curr = curr->next;
                }
                temp = temp->next;
            }
        }
        void insert(ConcatStringNode* x)
        {
            if (this->head == nullptr)
            {
                this->head = this->tail = new Node(x);
            }
            else
            {
                Node* temp = new Node(x);
                this->tail->next = temp;
                this->tail = temp;
            }
            this->size++;
            sort();
        }
        int getSize()
        {
            return this->size;
        }
    public:
        class Node
        {
        public:
            ConcatStringNode* data;
            Node* next;
            friend class DeleteTreeList;
            friend class ConcatStringTree;
        public:
            Node(ConcatStringNode* x = nullptr)
            {
                this->data = x;
                this->next = nullptr;
            }
        };
    };
public:
    class RootList
    {
    public:
        class Node;
    private:
        Node* head, * tail;
        int size = 0;
        friend class ConcatStringTree;
    public:
        RootList()
        {
            this->head = this->tail = nullptr;
            this->size = 0;
        }
        ~RootList()
        {
            while (this->head != nullptr)
            {
                Node* temp = this->head;
                this->head = this->head->next;
                if (this->head == nullptr)
                {
                    this->tail = nullptr;
                }
                delete temp;
                temp = nullptr;
            }
        }
    public:
        void insert(int x)
        {
            if (this->head == nullptr)
            {
                this->head = this->tail = new Node(x);
            }
            else
            {
                Node* temp = new Node(x);
                this->tail->next = temp;
                this->tail = temp;
            }
            this->size++;
        }
        int getSize()
        {
            return this->size;
        }
        void remove(int id)
        {
            Node* x = nullptr, * y = head;
            while (y != nullptr)
            {
                if (y->root_id == id)
                {
                    break;
                }
                x = y;
                y = y->next;
            }
            if (y != nullptr)
            {
                if (x == nullptr)
                {
                    this->head = this->head->next;
                    if (this->head == nullptr)
                    {
                        this->tail = nullptr;
                    }
                    delete y;
                    y = nullptr;
                }
                else
                {
                    x->next = y->next;
                    this->tail = (x->next == nullptr ? x : tail);
                    y->next = nullptr;
                    delete y;
                    y = nullptr;
                }
                this->size--;
            }
        }
        bool search(int id)
        {
            Node* p = this->head;
            while (p != nullptr)
            {
                if (p->root_id == id)
                {
                    return true;
                }
                p = p->next;
            }
            return false;
        }
    public:
        class Node
        {
        private:
            int root_id;
            Node* next;
            friend class RootList;
            friend class ConcatStringTree;
        public:
            Node(int id = 0)
            {
                this->root_id = id;
                this->next = nullptr;
            }
        };
    };
};

class LitStringHash;

class HashConfig {
private:
    int p;
    double c1, c2;
    double lambda;
    double alpha;
    int initSize;
    friend class LitStringHash;
public:
    HashConfig(int p = 0, double c1 = 0.0, double c2 = 0.0, double lambda = 0.0, double alpha = 0.0, int initSize = 0)
    {
        this->p = p;
        this->c1 = c1;
        this->c2 = c2;
        this->lambda = lambda;
        this->initSize = initSize;
        this->alpha = alpha;
    }
    friend class ReducedConcatStringTree;
};

class LitString
{
public:
    char* s;
    int numOfRefs;
public:
    LitString(const char* s = nullptr, int numRefs = 0)
    {
        if (s == nullptr)
        {
            this->s = nullptr;
        }
        else
        {
            int len = str_len(s);
            this->s = new char[len + 1];
            for (int i = 0; i < len; i++)
            {
                this->s[i] = s[i];
            }
            this->s[len] = '\0';
        }
        this->numOfRefs = numRefs;
    }
    ~LitString()
    {

    }
};

class LitStringHash {
public:
    LitString* litStringTable;
    HashConfig hashConfig;
    int size;
    int max_size;
    int last_index;
    friend class ReducedConcatStringTree;
public:
    LitStringHash()
    {
        this->litStringTable = nullptr;
        this->size = this->max_size = 0;
        this->last_index = -1;
    }
    LitStringHash(const HashConfig& hashConfig)
    {
        this->hashConfig = hashConfig;
        this->litStringTable = nullptr;
        this->max_size = this->hashConfig.initSize;
        this->size = 0;
        this->last_index = -1;
    }
    ~LitStringHash()
    {

    }
    long long h(char* s)
    {
        long long int sum = 0;
        long long pow = 1;
        for (int i = 0; i < str_len(s); i++)
        {
            sum = (sum % this->max_size + (((int)(s[i]) % this->max_size) * (pow % this->max_size)) % this->max_size) % this->max_size;
            pow = ((pow % this->max_size) * (this->hashConfig.p % this->max_size)) % this->max_size;
        }
        return sum;
    }
    void reHashing(char* str)
    {
        int newSize = (int)(this->hashConfig.alpha * this->max_size);
        LitString* newLitStringTable = new LitString[newSize];
        int oldSize = this->max_size;
        this->max_size = newSize;
        for (int i = 0; i < oldSize; i++)
        {
            if (this->litStringTable[i].s != nullptr)
            {
                int j = 0;
                long long h_func = h(this->litStringTable[i].s);
                do
                {
                    int res = (int)(h_func + this->hashConfig.c1 * j + this->hashConfig.c2 * j * j) % this->max_size;
                    if (newLitStringTable[res].s == nullptr)
                    {
                        newLitStringTable[res] = this->litStringTable[i];
                        if (this->litStringTable[i].s == str)
                        {
                            this->last_index = res;
                        }
                        break;
                    }
                    j++;
                } while (j < this->max_size);
                if (j == this->max_size)
                {
                    throw runtime_error("No possible slot");
                }
            }
        }
        this->max_size = newSize;
        delete[] this->litStringTable;
        this->litStringTable = newLitStringTable;
    }
    void insert(ConcatStringNode* p)
    {
        if (this->litStringTable == nullptr)
        {
            this->litStringTable = new LitString[this->hashConfig.initSize];
            this->size = 0;
            this->max_size = this->hashConfig.initSize;
        }
        long long h_func = h(p->s);
        int* arr = new int[this->max_size]{ -1 };
        int y = 0;
        for (int x = 0; x < this->max_size; x++)
        {
            int res = (int)(h_func + this->hashConfig.c1 * x + this->hashConfig.c2 * x * x) % this->max_size;
            if (compareString(this->litStringTable[res].s, p->s) == true)
            {
                if (p->s != this->litStringTable[res].s)
                {
                    delete[] p->s;
                    p->s = this->litStringTable[res].s;
                }
                this->litStringTable[res].numOfRefs++;
                delete[] arr;
                return;
            }
            else if (this->litStringTable[res].s == nullptr)
            {
                arr[y] = res;
                y++;
            }
        }
        if (arr[0] == -1)
        {
            delete[] arr;
            throw runtime_error("No possible slot");
        }
        this->litStringTable[arr[0]].s = p->s;
        this->litStringTable[arr[0]].numOfRefs = 1;
        this->last_index = arr[0];
        this->size++;
        if ((this->size * 1.0) / this->max_size > this->hashConfig.lambda)
        {
            try
            {
                delete[] arr;
                reHashing(p->s);
            }
            catch (exception& e)
            {
                throw runtime_error(e.what());
            }
        }
        else
        {
            delete[] arr;
        }
    }
    void insertToHashPreOrder(ConcatStringNode* temp)
    {
        if (temp == nullptr)
        {
            return;
        }
        if (temp->left == nullptr && temp->right == nullptr)
        {
            this->insert(temp);
        }
        this->insertToHashPreOrder(temp->left);
        this->insertToHashPreOrder(temp->right);
    }
    void removeElement(int index)
    {
        if (index >= this->max_size)
        {
            return;
        }
        this->size--;
        if (this->size == 0)
        {
            this->last_index = -1;
            delete[] litStringTable;
            this->litStringTable = nullptr;
            this->max_size = 0;
        }
        else
        {
            this->litStringTable[index].s = nullptr;
            this->litStringTable[index].numOfRefs = 0;
        }
    }
    int getLastInsertedIndex() const
    {
        return this->last_index;
    }
    string toString() const
    {
        string ans = "LitStringHash[";
        for (int i = 0; i < this->max_size; i++)
        {
            if (this->litStringTable[i].s == nullptr)
            {
                ans += "();";
            }
            else
            {
                ans += "(litS=\"" + string(this->litStringTable[i].s) + "\");";
            }
        }
        if (ans == "LitStringHash[")
        {
            ans += "]";
        }
        else
        {
            ans[ans.length() - 1] = ']';
        }
        return ans;
    }
};

class ReducedConcatStringTree : public ConcatStringTree
{
public:
    static LitStringHash litStringHash;
protected:
    static LitStringHash* copyLitStringHash;
public:
    ReducedConcatStringTree()
    {
        this->root = nullptr;
    }
    ReducedConcatStringTree(const char* s, LitStringHash* litStringHash)
    {
        try
        {
            this->root = new ConcatStringNode(s, nullptr, nullptr, cst_id++);
            litStringHash->insert(this->root);
            this->copyLitStringHash = litStringHash;
            this->litStringHash = *litStringHash;
            rootList.insert(this->root->id);
            AVLInsertPreOrder();
        }
        catch (exception& e)
        {
            throw runtime_error(e.what());
        }
    }
    int length() const
    {
        return this->root->realLength;
    }
    char get(int index) const
    {
        if (index < 0 || index >= this->length())
        {
            throw out_of_range("Index of string is invalid!");
        }
        return ConcatStringTree::get(index);
    }
    int indexOf(char c) const
    {
        return ConcatStringTree::indexOf(c);
    }
    string toStringPreOrder() const
    {
        return ConcatStringTree::toStringPreOrder();
    }
    string toString() const
    {
        return ConcatStringTree::toString();
    }
    ReducedConcatStringTree concat(const ReducedConcatStringTree& otherS) const
    {
        union S
        {
            ReducedConcatStringTree newTree;
            S()
            {
                newTree = ReducedConcatStringTree();
            }
            ~S()
            {

            }
        } s;
        try
        {
            s.newTree.root = new ConcatStringNode(nullptr, this->root, otherS.root, cst_id++);
            s.newTree.root->leftLength = this->root->realLength;
            s.newTree.root->realLength = this->root->realLength + otherS.root->realLength;
            this->litStringHash.insertToHashPreOrder(s.newTree.root);
            *this->copyLitStringHash = this->litStringHash;
            rootList.insert(s.newTree.root->id);
            s.newTree.AVLInsertPreOrder();
            return s.newTree;
        }
        catch (exception& e)
        {
            throw runtime_error(e.what());
        }
    }
    ReducedConcatStringTree reverse() const
    {
        union S
        {
            ReducedConcatStringTree newTree;
            S()
            {
                newTree = ReducedConcatStringTree();
            }
            ~S()
            {

            }
        } s;
        s.newTree.root = cloneConcatStringTree(this->root);
        invertBT(s.newTree.root);
        s.newTree.update(true);
        this->litStringHash.insertToHashPreOrder(s.newTree.root);
        *this->copyLitStringHash = this->litStringHash;
        return s.newTree;
    }
    ReducedConcatStringTree subString(int from, int to) const
    {
        if (from < 0 || from >= this->length() || to < 0 || to >= this->length() + 1)
        {
            throw out_of_range("Index of string is invalid!");
        }
        if (from >= to)
        {
            throw logic_error("Invalid range!");
        }
        union S
        {
            ReducedConcatStringTree newTree;
            S()
            {
                newTree = ReducedConcatStringTree();
            }
            ~S()
            {

            }
        } s;
        s.newTree.root = cloneConcatStringTree(this->root);
        ConcatStringNode* prev_tempFrom = nullptr, * prev_tempTo = nullptr;
        ConcatStringNode* tempFrom = s.newTree.root;
        int x = from, y = to, str_length = y - x;
        while (tempFrom->s == nullptr)
        {
            if (from < tempFrom->leftLength)
            {
                prev_tempFrom = tempFrom;
                tempFrom = tempFrom->left;
            }
            else
            {
                from -= tempFrom->leftLength;
                prev_tempFrom = tempFrom;
                tempFrom = tempFrom->right;
                s.newTree.cleanUp(&prev_tempFrom->left);
            }
        }
        ConcatStringNode* tempTo = s.newTree.root;
        while (tempTo->s == nullptr)
        {
            if (to <= tempTo->leftLength)
            {
                prev_tempTo = tempTo;
                tempTo = tempTo->left;
                s.newTree.cleanUp(&prev_tempTo->right);
            }
            else
            {
                to -= tempTo->leftLength;
                prev_tempTo = tempTo;
                tempTo = tempTo->right;
            }
        }
        if (prev_tempTo == nullptr || prev_tempFrom == nullptr || tempFrom == tempTo)
        {
            subStr(tempFrom->s, from, str_length);
        }
        else
        {
            subStr(tempFrom->s, from, min(str_length, tempFrom->realLength - from));
            tempFrom->realLength = str_len(tempFrom->s);
            int index = 0;
            subStr(tempTo->s, index, min((str_length, tempTo->realLength - index), to - index));
            tempTo->realLength = str_len(tempTo->s);
        }
        s.newTree.update();
        this->litStringHash.insertToHashPreOrder(s.newTree.root);
        *this->copyLitStringHash = this->litStringHash;
        return s.newTree;
    }
    int getParTreeSize(const string& query) const
    {
        if (root == nullptr)
        {
            return 0;
        }
        ConcatStringNode* temp = this->root;
        for (int i = 0; i < (int)query.length(); i++)
        {
            if (query[i] == 'l')
            {
                temp = temp->left;
                if (temp == nullptr)
                {
                    throw runtime_error("Invalid query: reaching NULL");
                }
            }
            else if (query[i] == 'r')
            {
                temp = temp->right;
                if (temp == nullptr)
                {
                    throw runtime_error("Invalid query: reaching NULL");
                }
            }
            else
            {
                throw runtime_error("Invalid character of query");
            }
        }
        return temp->avl->size();
    }
    string getParTreeStringPreOrder(const string& query) const
    {
        if (root == nullptr)
        {
            return "";
        }
        ConcatStringNode* temp = this->root;
        for (int i = 0; i < (int)query.length(); i++)
        {
            if (query[i] == 'l')
            {
                temp = temp->left;
                if (temp == nullptr)
                {
                    throw runtime_error("Invalid query: reaching NULL");
                }
            }
            else if (query[i] == 'r')
            {
                temp = temp->right;
                if (temp == nullptr)
                {
                    throw runtime_error("Invalid query: reaching NULL");
                }
            }
            else
            {
                throw runtime_error("Invalid character of query");
            }
        }
        return temp->avl->toStringPreOrder();
    }
    void LitStringDeletePostOrder(ConcatStringNode* prev, ConcatStringNode* temp)
    {
        if (temp == nullptr)
        {
            return;
        }
        LitStringDeletePostOrder(temp, temp->left);
        LitStringDeletePostOrder(temp, temp->right);
        if (temp->left == nullptr && temp->right == nullptr)
        {
            int index = -1;
            for (int i = 0; i < this->litStringHash.max_size; i++)
            {
                if (temp->s == this->litStringHash.litStringTable[i].s)
                {
                    index = i;
                    break;
                }
            }
            this->litStringHash.litStringTable[index].numOfRefs -= 1;
            if (this->litStringHash.litStringTable[index].numOfRefs == 0)
            {
                this->litStringHash.removeElement(index);
                *this->copyLitStringHash = this->litStringHash;
            }
        }
    }
    void LitStringDelete()
    {
        LitStringDeletePostOrder(nullptr, this->root);
    }
    ~ReducedConcatStringTree()
    {
        LitStringDelete();
    }
};

#endif // __CONCAT_STRING_TREE_H__