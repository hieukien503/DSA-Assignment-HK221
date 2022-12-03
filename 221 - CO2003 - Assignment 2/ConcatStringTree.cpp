#include "ConcatStringTree.h"

ConcatStringTree::DeleteTreeList ConcatStringTree::delTreeList = ConcatStringTree::DeleteTreeList();
ConcatStringTree::RootList ConcatStringTree::rootList = ConcatStringTree::RootList();
LitStringHash ReducedConcatStringTree::litStringHash = LitStringHash();
LitStringHash* ReducedConcatStringTree::copyLitStringHash = nullptr;

int cst_id = 1;

int str_len(const char* str)
{
    if (str == nullptr)
    {
        return 0;
    }
    int len = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    return len;
}

void reverseString(char* str)
{
    int i = 0, j = str_len(str) - 1;
    while (i < j)
    {
        swap(str[i], str[j]);
        i++;
        j--;
    }
}

void subStr(char*& str, int from, int len)
{
    char* newStr = new char[len + 1];
    int j = 0;
    for (int i = from; i < from + len; i++)
    {
        newStr[j] = str[i];
        j++;
    }
    newStr[len] = '\0';
    delete[] str;
    str = newStr;
}

bool compareString(char* str1, char* str2)
{
    int len1 = str_len(str1), len2 = str_len(str2);
    if (len1 != len2)
    {
        return false;
    }
    for (int i = 0; i < len1; i++)
    {
        if (str1[i] != str2[i])
        {
            return false;
        }
    }
    return true;
}

ConcatStringTree::ConcatStringTree(const char* s)
{
    try
    {
        this->root = new ConcatStringNode(s, nullptr, nullptr, cst_id++);
        rootList.insert(root->id);
        AVLInsertPreOrder();
    }
    catch (exception& e)
    {
        throw runtime_error(e.what());
    }
}

int ConcatStringTree::length() const
{
    return this->root->realLength;
}

ConcatStringTree ConcatStringTree::concat(const ConcatStringTree& otherS) const
{
    union S
    {
        ConcatStringTree newTree;
        S()
        {
            newTree = ConcatStringTree();
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
        s.newTree.AVLInsertPreOrder();
        rootList.insert(s.newTree.root->id);
        return s.newTree;
    }
    catch (exception& e)
    {
        throw runtime_error(e.what());
    }
}

char ConcatStringTree::get(int index) const
{
    if (index >= this->length() || index < 0)
    {
        throw out_of_range("Index of string is invalid!");
    }
    ConcatStringNode* temp = this->root;
    while (temp->s == nullptr)
    {
        if (index < temp->leftLength)
        {
            temp = temp->left;
        }
        else
        {
            index -= temp->leftLength;
            temp = temp->right;
        }
    }
    return temp->s[index];
}

int ConcatStringTree::indexOf(char c) const
{
    for (int i = 0; i < this->length(); i++)
    {
        if (this->get(i) == c)
        {
            return i;
        }
    }
    return -1;
}

string ConcatStringTree::toStringPreOrder() const
{
    string ans = "ConcatStringTree[";
    ans += toStringPreorder(this->root, ans);
    ans[ans.length() - 1] = ']';
    return ans;
}

string ConcatStringTree::toString() const
{
    string ans = "ConcatStringTree[\"";
    ans += toStringPreorder2(this->root, "");
    ans += "\"]";
    return ans;
}

void ConcatStringTree::update(bool isReversedString)
{
    updateTree(this->root, isReversedString);
    rootList.insert(this->root->id);
}

void ConcatStringTree::invertBST() const
{
    invertBT(this->root);
}

ConcatStringTree ConcatStringTree::reverse() const
{
    union S
    {
        ConcatStringTree newTree;
        S()
        {
            newTree = ConcatStringTree();
        }
        ~S()
        {

        }
    } s;
    try
    {
        s.newTree.root = cloneConcatStringTree(this->root);
        invertBT(s.newTree.root);
        s.newTree.update(true);
        return s.newTree;
    }
    catch (exception& e)
    {
        throw e.what();
    }
}

ConcatStringTree ConcatStringTree::subString(int from, int to) const
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
        ConcatStringTree newTree;
        S()
        {
            newTree = ConcatStringTree();
        }
        ~S()
        {

        }
    } s;
    try
    {
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
        return s.newTree;
    }
    catch (exception& e)
    {
        throw e.what();
    }
}

int ConcatStringTree::getParTreeSize(const string& query) const
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

string ConcatStringTree::getParTreeStringPreOrder(const string& query) const
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

ConcatStringTree::~ConcatStringTree()
{
    AVLDelete();
    if (this->root != nullptr)
    {
        if (rootList.search(this->root->id))
        {
            delTreeList.insert(this->root);
            rootList.remove(this->root->id);
            if (rootList.getSize() == 0)
            {
                DeleteTreeList::Node* x = delTreeList.head;
                while (x != nullptr)
                {
                    DeleteTreeList::Node* y = delTreeList.head;
                    while (y != nullptr)
                    {
                        cutLinkTotal(x->data, nullptr, y->data);
                        y = y->next;
                    }
                    cleanUp(&x->data);
                    x = x->next;
                }
                delTreeList.~DeleteTreeList();
                rootList.~RootList();
                cst_id = 1;
            }
        }
    }
}