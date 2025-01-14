#include "merge-sorted-lists.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>

std::ostream& operator<<(std::ostream &out, const ListNode *list_head);


static ListNode* make_list_from_string(const char *list_str)
{
    ListNode anchor, *tail = &anchor;
    std::stringstream ss(list_str);
    std::string number_str;
    while(std::getline(ss, number_str, ','))
    {
        char *end;
        int number = (int) strtol(number_str.c_str(), &end, 10);
        if(*end != 0)
        {
            throw std::invalid_argument(
                    std::string("Invalid character: ") + *end);
        }
        tail = new ListNode(number, tail, true);
    }
    return anchor.next;
}


static std::vector<ListNode*> read_from_stream(std::istream &in)
{
    std::vector<ListNode*> lists;
    std::string list_str;
    while(in >> list_str)
    {
        lists.push_back(make_list_from_string(list_str.c_str()));
    }
    return lists;
}


static std::vector<ListNode*> get_from_args(int argc, const char *argv[])
{
    std::vector<ListNode*> lists;
    for(; argc>0; --argc, ++argv)
    {
        lists.push_back(make_list_from_string(argv[0]));
    }
    return lists;
}


static bool is_list_sorted(const ListNode *head)
{
    if(head == nullptr) { return true; }
    int prev = head->val;
    int cnt = 1;
    for(head = head->next; head != nullptr; head = head->next, ++cnt)
    {
        if(head->val < prev)
        {
            std::cerr << "Out of order! "
                << head->val << " (@ " << cnt+1 << ") < "
                << prev << " (@ " << cnt << ")\n";
            return false;
        }
        prev = head->val;
    }
    return true;
}


int main(int argc, const char *argv[])
{
    std::vector<ListNode*> lists;

    if(argc == 1)
    {
#ifdef DEBUG
        std::cout << "Expecting arguments on stdin "
            << "(space-separated lists, comma-separated list elements)\n";
#endif
        lists = read_from_stream(std::cin);
    }
    else
    {
        lists = get_from_args(argc-1, argv+1);
    }
#ifdef DEBUG
    std::cout << "Have " << lists.size() << " lists:\n";
    for(ListNode *list_head: lists)
    {
        std::cout << list_head << "\n";
    }
#endif

    Solution sol;
    ListNode *sorted_list = sol.mergeKLists(lists);
    if(is_list_sorted(sorted_list))
    {
        std::cout << "OK\n";
        return 0;
    }
    else
    {
        std::cout << "Oops\n";
        return 1;
    }
}
