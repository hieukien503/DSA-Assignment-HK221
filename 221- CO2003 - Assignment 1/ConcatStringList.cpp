#include "ConcatStringList.h"

ConcatStringList::ReferencesList ConcatStringList::refList = ConcatStringList::ReferencesList();
ConcatStringList::DeleteStringList ConcatStringList::delStrList = ConcatStringList::DeleteStringList();

ConcatStringList::ConcatStringList()
{
    this->dHead = this->dTail = nullptr;
    this->list_len = 0;
    this->SameNode = false;
}

ConcatStringList::ConcatStringList(const char* str)
{
    this->dHead = this->dTail = new CharALNode(str, 1);
    this->list_len = (int)dHead->str.length();
    SameNode = true;
    refList.addRefNode(this->dHead);
    refList.addRefNode(this->dTail);
}

ConcatStringList ConcatStringList::concat(const ConcatStringList& otherS) const
{
    union S
    {
        ConcatStringList List;
        S()
        {
            List = ConcatStringList();
        }
        ~S()
        {

        }
    } myUnion;
    this->dTail->next = otherS.dHead;
    myUnion.List = *this;
    myUnion.List.dTail = otherS.dTail;
    myUnion.List.SameNode = (myUnion.List.dHead == myUnion.List.dTail ? true : false);
    myUnion.List.list_len = this->list_len + otherS.list_len;
    myUnion.List.dHead->numOfRef = 1;
    myUnion.List.dTail->numOfRef = 1;
    refList.addRefNode(myUnion.List.dHead);
    refList.addRefNode(myUnion.List.dTail);
    return myUnion.List;
}

int ConcatStringList::length() const
{
    return this->list_len;
}

char ConcatStringList::get(int index) const
{
    if (index >= this->length() || index < 0)
    {
        throw out_of_range("Index of string is invalid!");
    }
    CharALNode* p = dHead;
    while (index >= (int)p->str.length())
    {
        index -= (int)p->str.length();
        p = p->next;
    }
    return p->str[index];
}

int ConcatStringList::indexOf(char c) const
{
    CharALNode* p = dHead;
    int index = 0;
    int sum_length = 0;
    while (p != nullptr)
    {
        for (int i = 0; i < (int)p->str.length(); i++)
        {
            if (p->str[i] == c)
            {
                index = sum_length + i;
                return index;
            }
        }
        sum_length += (int)p->str.length();
        p = p->next;
    }
    return -1;
}

string ConcatStringList::toString() const
{
    string ans = "";
    CharALNode* p = dHead;
    while (p != dTail)
    {
        ans += p->str;
        p = p->next;
    }
    ans += p->str;
    return ans;
}

ConcatStringList ConcatStringList::subString(int from, int to) const
{
    if (from < 0 || from >= this->length() || to < 0 || to >= this->length() + 1)
    {
        throw out_of_range("Index of string is invalid!");
    }
    if (from >= to)
    {
        throw logic_error("Invalid range");
    }
    int length_string = to - from;
    CharALNode* p = dHead;
    int sum = 0;
    while (from >= sum + (int)p->str.length())
    {
        sum += (int)p->str.length();
        p = p->next;
    }
    sum += (int)p->str.length();
    union S
    {
        ConcatStringList List;
        S()
        {
            List = ConcatStringList();
        }
        ~S()
        {

        }
    } myUnion;
    string s = "";
    if (p->next == nullptr)
    {
        while (from < to)
        {
            s += get(from);
            from++;
        }
        myUnion.List.dHead = myUnion.List.dTail = new CharALNode(s.c_str());
    }
    else
    {
        while (from < min(sum, to))
        {
            s += get(from);
            from++;
        }
        myUnion.List.dHead = myUnion.List.dTail = new CharALNode(s.c_str());
        CharALNode* myPtr = nullptr;
        p = p->next;
        if (p != nullptr)
        {
            while (from + (int)p->str.length() < to)
            {
                s = p->str;
                myPtr = new CharALNode(s.c_str());
                myUnion.List.dTail->next = myPtr;
                myUnion.List.dTail = myPtr;
                from += (int)p->str.length();
                p = p->next;
            }
            s = "";
            while (from < to)
            {
                s += get(from);
                from++;
            }
            if (s != "")
            {
                myPtr = new CharALNode(s.c_str());
                myUnion.List.dTail->next = myPtr;
                myUnion.List.dTail = myPtr;
            }
        }
    }
    myUnion.List.list_len = length_string;
    myUnion.List.SameNode = (myUnion.List.dHead == myUnion.List.dTail ? true : false);
    myUnion.List.dHead->numOfRef = 1;
    myUnion.List.dTail->numOfRef = 1;
    refList.addRefNode(myUnion.List.dHead);
    refList.addRefNode(myUnion.List.dTail);
    return myUnion.List;
}

ConcatStringList ConcatStringList::reverse() const
{
    union S
    {
        ConcatStringList List;
        S()
        {
            List = ConcatStringList();
        }
        ~S()
        {

        }
    } myUnion;
    CharALNode* p = dHead;
    while (p != nullptr)
    {
        if (p == this->dTail)
        {
            string s = p->str;
            int i = 0, j = (int)s.length() - 1;
            while (i < j)
            {
                char c = s[i];
                s[i] = s[j];
                s[j] = c;
                i++;
                j--;
            }
            CharALNode* myPtr = new CharALNode(s.c_str());
            if (myUnion.List.dHead == nullptr)
            {
                myUnion.List.dHead = myUnion.List.dTail = myPtr;
            }
            else
            {
                myPtr->next = myUnion.List.dHead;
                myUnion.List.dHead = myPtr;
            }
            break;
        }
        else
        {
            string s = p->str;
            int i = 0, j = (int)s.length() - 1;
            while (i < j)
            {
                char c = s[i];
                s[i] = s[j];
                s[j] = c;
                i++;
                j--;
            }
            CharALNode* myPtr = new CharALNode(s.c_str());
            if (myUnion.List.dHead == nullptr)
            {
                myUnion.List.dHead = myUnion.List.dTail = myPtr;
            }
            else
            {
                myPtr->next = myUnion.List.dHead;
                myUnion.List.dHead = myPtr;
            }
        }
        p = p->next;
    }
    myUnion.List.list_len = this->length();
    myUnion.List.SameNode = (myUnion.List.dHead == myUnion.List.dTail ? true : false);
    myUnion.List.dHead->numOfRef = 1;
    myUnion.List.dTail->numOfRef = 1;
    refList.addRefNode(myUnion.List.dHead);
    refList.addRefNode(myUnion.List.dTail);
    return myUnion.List;
}

int ConcatStringList::ReferencesList::size() const
{
    return this->list_len;
}

ConcatStringList::~ConcatStringList()
{
    ReferencesList::RefNode* r_node = refList.rHead, * r_node1 = refList.rHead;
    int sum1 = 0, sum2 = 0;
    while (r_node != nullptr)
    {
        if (r_node->ptr == this->dHead)
        {
            r_node->numOfRef -= (r_node->numOfRef == 0 ? 0 : 1);
            sum1 = r_node->numOfRef;
            break;
        }
        r_node = r_node->next;
    }
    while (r_node1 != nullptr)
    {
        if (r_node1->ptr == this->dTail)
        {
            r_node1->numOfRef -= (r_node1->numOfRef == 0 ? 0 : 1);
            sum2 = r_node1->numOfRef;
            break;
        }
        r_node1 = r_node1->next;
    }
    delStrList.addDeletedString(this->dHead, this->dTail, this->SameNode, (this->SameNode == true ? sum2 : sum1), sum2);
    refList.sortList();
    DeleteStringList::DelNode* d_head1 = delStrList.dHead;
    ReferencesList::RefNode* r_node3 = refList.rHead;
    while (r_node3 != nullptr)
    {
        if (r_node3->numOfRef == 0)
        {
            while (d_head1 != nullptr)
            {
                if (d_head1->dHead == r_node3->ptr)
                {
                    d_head1->numHeadRef = 0;
                }
                if (d_head1->dTail == r_node3->ptr)
                {
                    d_head1->numTailRef = 0;
                }
                d_head1 = d_head1->next;
            }
        }
        r_node3 = r_node3->next;
    }
    ReferencesList::RefNode* r_node2 = refList.rHead;
    while (r_node2 != nullptr)
    {
        if (r_node2->numOfRef == 0)
        {
            if (r_node2 == refList.rHead)
            {
                DeleteStringList::DelNode* d_head2 = delStrList.dHead, * d_head3 = nullptr;
                while (d_head2 != nullptr)
                {
                    while (d_head2->dHead != nullptr)
                    {
                        CharALNode* Node1 = d_head2->dHead;
                        ReferencesList::RefNode* r_node3 = refList.rHead;
                        while (r_node3 != nullptr)
                        {
                            if (r_node3->ptr == Node1)
                            {
                                break;
                            }
                            r_node3 = r_node3->next;
                        }
                        d_head2->dHead = d_head2->dHead->next;
                        if (d_head2->dHead == nullptr)
                        {
                            d_head2->dTail = nullptr;
                        }
                        d_head3 = delStrList.dHead;
                        while (d_head3 != nullptr)
                        {
                            CharALNode* Node2 = d_head3->dHead, * pre_Node2 = nullptr;
                            while (Node2 != nullptr)
                            {
                                if (Node2 == Node1)
                                {
                                    break;
                                }
                                pre_Node2 = Node2;
                                Node2 = Node2->next;
                            }
                            if (Node2 != nullptr)
                            {
                                if (pre_Node2 == nullptr)
                                {
                                    d_head3->dHead = d_head3->dHead->next;
                                    if (d_head3->dHead == nullptr)
                                    {
                                        d_head3->dTail = nullptr;
                                    }
                                }
                                else
                                {
                                    pre_Node2->next = Node2->next;
                                }
                            }
                            d_head3 = d_head3->next;
                        }
                        Node1->next = nullptr;
                        delete Node1;
                        Node1 = nullptr;
                        if (r_node3 != nullptr)
                        {
                            r_node3->ptr = nullptr;
                        }
                    }
                    d_head2 = d_head2->next;
                }
                delStrList.~DeleteStringList();
                refList.~ReferencesList();
                break;
            }
        }
        r_node2 = r_node2->next;
    }
}

void ConcatStringList::ReferencesList::addRefNode(CharALNode* h)
{
    RefNode* myPtr = nullptr;
    bool inc_list_len = true;
    if (rHead == nullptr)
    {
        rHead = rTail = new RefNode(h);
        rHead->numOfRef = rTail->numOfRef = h->numOfRef;
    }
    else
    {
        RefNode* p = rHead;
        while (p != nullptr)
        {
            if (p->ptr == h)
            {
                inc_list_len = false;
                break;
            }
            p = p->next;
        }
        if (p == nullptr)
        {
            myPtr = new RefNode(h);
            myPtr->next = this->rHead;
            this->rHead = myPtr;
            this->rHead->numOfRef = myPtr->ptr->numOfRef;
        }
        else
        {
            p->numOfRef += h->numOfRef;
        }
    }
    list_len += (inc_list_len == true ? 1 : 0);
    sortList();
    RefNode* ptr = this->rHead;
}

void ConcatStringList::ReferencesList::swap(ReferencesList::RefNode* a, ReferencesList::RefNode* b)
{
    CharALNode* ptr = a->ptr;
    a->ptr = b->ptr;
    b->ptr = ptr;
    int num = a->numOfRef;
    a->numOfRef = b->numOfRef;
    b->numOfRef = num;
}

void ConcatStringList::ReferencesList::sortList()
{
    if (rHead == nullptr)
    {
        return;
    }
    RefNode* ptr1 = nullptr;
    int i = 0;
    do
    {
        ptr1 = rHead;
        while (ptr1->next != nullptr)
        {
            if (ptr1->numOfRef > ptr1->next->numOfRef)
            {
                swap(ptr1, ptr1->next);
            }
            ptr1 = ptr1->next;
        }
        i++;
    } while (i < this->size());
    if (rTail->numOfRef == 0)
    {
        return;
    }
    while (rHead != nullptr)
    {
        if (rHead->numOfRef != 0)
        {
            break;
        }
        RefNode* myPtr = rHead;
        rHead = rHead->next;
        myPtr->next = nullptr;
        this->rTail->next = myPtr;
        this->rTail = myPtr;
    }
}

int ConcatStringList::ReferencesList::refCountAt(int index) const
{
    if (index < 0)
    {
        throw out_of_range("Index of references list is invalid!");
    }
    else
    {
        int i = 0;
        RefNode* p = rHead;
        while (p != nullptr)
        {
            if (i == index)
            {
                break;
            }
            i++;
            p = p->next;
        }
        if (p == nullptr)
        {
            throw out_of_range("Index of references list is invalid!");
        }
        return p->numOfRef;
    }
}

std::string ConcatStringList::ReferencesList::refCountsString() const
{
    string ans = "RefCounts[";
    RefNode* p = rHead;
    while (p != nullptr)
    {
        ans += to_string(p->numOfRef);
        ans += (p->next == nullptr ? "" : ",");
        p = p->next;
    }
    ans += "]";
    return ans;
}

void ConcatStringList::DeleteStringList::addDeletedString(CharALNode* h, CharALNode* t, bool SameNode, int numHeadRef, int numTailRef)
{
    DelNode* myPtr = new DelNode(h, t, SameNode, numHeadRef, numTailRef);
    if (dHead == nullptr)
    {
        dHead = dTail = myPtr;
    }
    else
    {
        DelNode* temp = dHead;
        while (temp != nullptr)
        {
            if (myPtr->dHead == temp->dHead)
            {
                temp->numHeadRef -= (temp->numHeadRef == 0 ? 0 : (myPtr->SameNode == true ? 2 : 1));
                temp->numTailRef = (temp->SameNode == true ? temp->numHeadRef : temp->numTailRef);
            }
            if (myPtr->dTail == temp->dTail)
            {
                temp->numTailRef -= (temp->numTailRef == 0 ? 0 : (myPtr->SameNode == true ? 2 : 1));
                temp->numHeadRef = (temp->SameNode == true ? temp->numTailRef : temp->numHeadRef);
            }
            temp = temp->next;
        }
        this->dTail->next = myPtr;
        this->dTail = myPtr;
    }
    int real_length = 0;
    DelNode* x = this->dHead;
    while (x != nullptr)
    {
        if (x->numHeadRef + x->numTailRef != 0)
        {
            real_length++;
        }
        x = x->next;
    }
    this->list_len = real_length;
}

int ConcatStringList::DeleteStringList::size() const
{
    return this->list_len;
}

std::string ConcatStringList::DeleteStringList::totalRefCountsString() const
{
    string ans = "TotalRefCounts[";
    DelNode* p = dHead;
    int sumOfRef = 0;
    while (p != nullptr)
    {
        sumOfRef = (p->SameNode == true ? p->numHeadRef : p->numHeadRef + p->numTailRef);
        if (sumOfRef != 0)
        {
            ans += (to_string(sumOfRef) + ",");
        }
        p = p->next;
    }
    ans += "]";
    for (int i = 0; i < (int)ans.length() - 1; i++)
    {
        if (ans[i] == ',')
        {
            if (i == (int)ans.length() - 2)
            {
                ans.erase(ans.begin() + i);
                i--;
            }
        }
    }
    return ans;
}