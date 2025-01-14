#pragma once
#include <vector>


struct ListNode {
    int val;
    ListNode *next;

    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *other, bool insert_after_other=false): val(x)
    {
        if(insert_after_other)
        {
            next = other->next;
            other->next = this;
        }
        else // insert before "other"
        {
            next = other;
        }
    }
};


typedef std::vector<ListNode*> ListVector;


class Solution {
    private:
        ListVector listHeads;
    public:
        ListNode* mergeKLists(ListVector &lists);

    private:
        ListNode* addToResult(ListNode *result);
        void insertListHead(ListNode *newNode);
        void sortListHeads();
};
