#ifndef __CONCAT_STRING_LIST_H__
#define __CONCAT_STRING_LIST_H__

#include "main.h"

class CharALNode
{
private:
    string str;
    CharALNode* next;
    int numOfRef;
    friend class ConcatStringList;
public:
    CharALNode(const char* str = "", int numOfRef = 0) : str(string(str)), next(nullptr), numOfRef(numOfRef) {}
    ~CharALNode()
    {

    }
};

class ConcatStringList {
public:
    class ReferencesList; // forward declaration
    class DeleteStringList; // forward declaration
public:
    static ReferencesList refList;
    static DeleteStringList delStrList;
protected:
    CharALNode* dHead, * dTail;
    bool SameNode;
    int list_len;
public:
    ConcatStringList();
    ConcatStringList(const char* str);
    int length() const;
    char get(int index) const;
    int indexOf(char c) const;
    std::string toString() const;
    ConcatStringList concat(const ConcatStringList& otherS) const;
    ConcatStringList subString(int from, int to) const;
    ConcatStringList reverse() const;
    ~ConcatStringList();

public:
    class ReferencesList {
    public:
        class RefNode;
    protected:
        RefNode* rHead, * rTail;
        int list_len;
        friend class ConcatStringList;
    public:
        ReferencesList()
        {
            rHead = nullptr;
            rTail = nullptr;
            list_len = 0;
        }
        ~ReferencesList()
        {
            while (this->rHead != nullptr)
            {
                RefNode* r_node1 = this->rHead;
                rHead = rHead->next;
                if (this->rHead == nullptr)
                {
                    this->rTail = nullptr;
                }
                r_node1->next = nullptr;
                delete r_node1;
                r_node1 = nullptr;
            }
            this->list_len = 0;
        }
    public:
        void addRefNode(CharALNode* h);
        int size() const;
        int refCountAt(int index) const;
        std::string refCountsString() const;
        void swap(RefNode* a, RefNode* b);
        void sortList();
    public:
        class RefNode
        {
        private:
            CharALNode* ptr;
            RefNode* next;
            int numOfRef;
            friend class ReferencesList;
            friend class ConcatStringList;
        public:
            RefNode() : ptr(nullptr), next(nullptr), numOfRef(0) {}
            RefNode(CharALNode* ptr) : ptr(ptr), next(nullptr), numOfRef(0) {}
            ~RefNode()
            {

            }
        };
    };

    class DeleteStringList {
    public:
        class DelNode;
    protected:
        DelNode* dHead, * dTail;
        int list_len;
        friend class ConcatStringList;
    public:
        DeleteStringList() : dHead(nullptr), dTail(nullptr), list_len(0) {}
        ~DeleteStringList()
        {
            while (dHead)
            {
                DelNode* ptr = dHead;
                dHead = dHead->next;
                if (!dHead)
                {
                    dTail = nullptr;
                }
                delete ptr;
            }
            this->list_len = 0;
        }
    public:
        void addDeletedString(CharALNode* h, CharALNode* t, bool SameNode, int numHeadRef, int numTailRef);
        int size() const;
        std::string totalRefCountsString() const;
    public:
        class DelNode
        {
        private:
            CharALNode* dHead, * dTail;
            bool SameNode;
            int numHeadRef, numTailRef;
            DelNode* next;
            friend class DeleteStringList;
            friend class ConcatStringList;
        public:
            DelNode(CharALNode* dHead = nullptr, CharALNode* dTail = nullptr, bool SameNode = true, int numHeadRef = 0, int numTailRef = 0)
            {
                this->dHead = dHead;
                this->dTail = dTail;
                this->SameNode = SameNode;
                this->next = nullptr;
                this->numHeadRef = numHeadRef;
                this->numTailRef = numTailRef;
            }
            ~DelNode()
            {

            }
        };
    };
};

#endif // __CONCAT_STRING_LIST_H__