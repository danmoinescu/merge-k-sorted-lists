#include "merge-sorted-lists.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>

#define TEST_SUCCESS 0
#define TEST_FAILURE 1


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


static int list_length(const ListNode *head)
{
    int len = 0;
    for(; head != nullptr; ++len, head = head->next);
    return len;
}


static void delete_list(ListNode *head)
{
    for(; head != nullptr;)
    {
        ListNode *goner = head;
        head = goner->next;
        delete goner;
    }
}


static ListNode* random_sorted_list(
        int size, int starting_point=-1, int max_increment=10)
{
    ListNode anchor, *tail = &anchor;

    std::minstd_rand rng(std::random_device{}());
    std::uniform_int_distribution<> dist(0, max_increment+1);

    int val = starting_point<=0? dist(rng) : starting_point;
    for(int i = 0; i<size; ++i)
    {
        val += dist(rng);
        tail = new ListNode(val, tail, true);
    }

    return anchor.next;
}


class TestRunner
{
    private:
        ListVector lists;
        std::vector<std::vector<int>> witnesses;

    public:
        void add_list(ListNode *newlist)
        {
            lists.push_back(newlist);
            witnesses.emplace_back();
            auto &witness = witnesses.back();
            for(; newlist != nullptr; newlist = newlist->next)
            {
                witness.push_back(newlist->val);
            }
        }

        int run()
        {
            int total_length = 0;
            std::ranges::for_each(
                    lists,
                    [&total_length](auto l){total_length += list_length(l);});

            Solution s;
            ListNode *result = s.mergeKLists(lists);

            int test_result = TEST_SUCCESS;
            int result_length = list_length(result);
            if(result_length != total_length)
            {
                std::cerr << "Size mismatch: expected " << total_length
                    << ", got " << result_length << "\n";
                test_result = TEST_FAILURE;
            }
            else if(!is_list_sorted(result))
            {
                std::cerr << "Not sorted!\n";
                test_result = TEST_FAILURE;
            }
            delete_list(result);
            if(test_result == TEST_FAILURE)
            {
                std::cerr << "Values:\n";
                for(auto &witness: witnesses)
                {
                    std::ranges::copy(
                            witness,
                            std::ostream_iterator<int>(std::cerr, ","));
                    std::cerr << "\n";
                }
            }
            return test_result;
        }
};


int random_test()
{
    std::minstd_rand rng(std::random_device{}());
    std::uniform_int_distribution<> max_increment(10, 99);
    std::uniform_int_distribution<> size(400, 501);

    TestRunner runner;
    runner.add_list(nullptr); // empty list
    for(int remaining=10'000; remaining > 0; --remaining)
    {
        runner.add_list(
                random_sorted_list(size(rng), -1, max_increment(rng)));
    }
    return runner.run();
}


int test_with_no_lists()
{
    Solution s;
    ListVector lists;
    ListNode *result = s.mergeKLists(lists);
    return (result == nullptr? TEST_SUCCESS : TEST_FAILURE);
}
