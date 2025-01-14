#include "merge-sorted-lists.hpp"
#include <algorithm>
#include <functional>

#include <iostream>
#include <iterator>


std::ostream& operator<<(std::ostream &out, const ListNode *list_head)
{
    out << "[";
    while(list_head)
    {
        out << " " << list_head->val;
        list_head = list_head->next;
    }
    out << " ]";
    return out;
}


ListNode* Solution::mergeKLists(std::vector<ListNode*>& lists)
{
    listHeads.clear();
    // populate listHeads with the head element of each list
    std::ranges::for_each(
            lists,
            [this](ListNode *head) {
                if(head != nullptr) listHeads.push_back(head);
            });
    sortListHeads();
    ListNode retvalAnchor, *retvalCrt = &retvalAnchor;
    while(listHeads.size() > 1)
    {
        retvalCrt = addToResult(retvalCrt);
#ifdef DEBUG
        retvalCrt->next = nullptr;
        std::cout << "Result so far: " << retvalAnchor.next << "\n";
#endif
    }
    // add the remaining list
    if(!listHeads.empty())
    {
#ifdef DEBUG
        std::cout << "Adding final list: " << listHeads.front() << "\n";
#endif
        retvalCrt->next = listHeads.front();
    }
    return retvalAnchor.next;
}


ListNode* Solution::addToResult(ListNode *result)
{
    ListNode *list1Head = listHeads.back();
    result->next = list1Head; // link list1 at the end of the result list
    result = list1Head; // advance result
    list1Head = list1Head->next; // advance the head of list1

    auto listHeadIter = listHeads.rbegin();
    listHeadIter++;
    int start2Val = (*listHeadIter)->val; // first value in 2nd list
    /* Iterate through list1 until we reach a value
       greater than start2Val
    */
#ifdef DEBUG
    std::cout << "\n";
    std::cout << "First list: " << result << "\n";
    std::cout << "2nd list: " << (*listHeadIter) << "\n";
#endif
    while((list1Head != nullptr) && (list1Head->val <= start2Val))
    {
        result = list1Head;
        list1Head = list1Head->next;
    }
    listHeads.pop_back();
    if(list1Head != nullptr) // list1 not yet exhausted
    {
#ifdef DEBUG
        std::cout << "list1 leftover: " << list1Head << "\n";
#endif
        insertListHead(list1Head);
    }
#ifdef DEBUG
    std::cout << "Lists after rearranging:\n";
    std::ranges::for_each(
            listHeads,
            [](const ListNode *head) { std::cout << head << '\n'; });
#endif
    return result;
}


void Solution::insertListHead(ListNode *newNode)
{
    if(newNode->val >= listHeads.front()->val)
    {
        listHeads.insert(listHeads.begin(), newNode);
    }
    else
    {
        // Perform a binary search for the insertion point
        ListVector::iterator start = listHeads.begin();
        ListVector::iterator end = listHeads.end();
        while(end-start > 2)
        {
            ListVector::iterator median = start + (end-start)/2;
            if((*median)->val < newNode->val)
            {
                // maybe we're lucky and we found our insertion point
                if((*(median-1))->val >= newNode->val)
                {
                    listHeads.insert(median, newNode);
                    return;
                }
                else
                {
                    end = median; // try [start, median) next time
                }
            }
            else if((*median)->val > newNode->val)
            {
                // maybe we're lucky and we found our insertion point
                if((*(median+1))->val <= newNode->val)
                {
                    listHeads.insert(median+1, newNode);
                    return;
                }
                else
                {
                    start = median; // try [median, end) next time
                }
            }
            else // median->val == newNode->val
            {
                // we found our insertion point
                listHeads.insert(median, newNode);
                return;
            }
        }
        // if we got here, our listHeads is short. Do a sequential search
        listHeads.insert(
                std::find_if(
                    start, end,
                    [this, newNode](ListNode *node){
                        return node->val < newNode->val;}),
                newNode);
    }
}


void Solution::sortListHeads()
{
    // sort the list heads in decreasing order according to the val field
    std::ranges::sort(
            listHeads,
            std::ranges::greater{},
            [](auto p_list_node) { return p_list_node->val; });
#ifdef DEBUG
    std::cout << "Lists after sorting:\n";
    std::ranges::for_each(
            listHeads,
            [](const ListNode *head) { std::cout << head << '\n'; });
#endif
}
