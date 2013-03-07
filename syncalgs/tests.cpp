#include <relacy/relacy_std.hpp>
//#include <relacy/windows.h>
#include "peterson_mutex.h"

struct peterson_mutex_test : rl::test_suite<peterson_mutex_test, 2>
{
    std::atomic<int> flag0;
    std::atomic<int> flag1;
    std::atomic<int> turn;

    rl::var<int> data;

    void before()
    {
        flag0($) = 0;
        flag1($) = 0;
        turn($) = 0;
    }

    void thread(unsigned index)
    {
        if (0 == index)
        {
            flag0($).store(1);
            turn($).store(1);

            while (flag1($).load()
                && 1 == turn($).load());

            data($) = 1;

            flag0($).store(0);
        }
        else
        {
            flag1($).store(1);
            turn($).store(0);

            while (flag0($).load()
                && 0 == turn($).load());

            data($) = 2;

            flag1($).store(0);
        }
    }
};

struct peterson_mutex_test2 : rl::test_suite<peterson_mutex_test2, 2>
{
    std::atomic<int> flag0;
    std::atomic<int> flag1;
    std::atomic<int> turn;

    rl::var<int> data;

    void before()
    {
        flag0($) = 0;
        flag1($) = 0;
        turn($) = 0;
    }

    void thread(unsigned index)
    {
        if (0 == index)
        {
            flag0.store(1, rl::memory_order_relaxed);
            turn.exchange(1, rl::memory_order_acq_rel);

            while (flag1.load(rl::memory_order_acquire)
                && 1 == turn.load(rl::memory_order_relaxed))
                rl::yield(1, $);

            data($) = 1;

            flag0.store(0, rl::memory_order_release);
        }
        else
        {
            flag1.store(1, rl::memory_order_relaxed);
            turn.exchange(0, rl::memory_order_acq_rel);

            while (flag0.load(rl::memory_order_acquire)
                && 0 == turn.load(rl::memory_order_relaxed))
                rl::yield(1, $);

            data($) = 2;

            flag1.store(0, rl::memory_order_release);
        }
    }
};

struct peterson_mutex_test3 : rl::test_suite<peterson_mutex_test3, 2>
{
    std::atomic<int> flag0;
    std::atomic<int> flag1;
    std::atomic<int> turn;

    rl::var<int> data;

    void before()
    {
        flag0($) = 0;
        flag1($) = 0;
        turn($) = 0;
    }

    void thread(unsigned index)
    {
        if (0 == index)
        {
            flag0.store(1, std::memory_order_relaxed);
            std::atomic_thread_fence(std::memory_order_seq_cst);
            turn.store(1, std::memory_order_relaxed);
            std::atomic_thread_fence(std::memory_order_seq_cst);

            while (flag1.load(std::memory_order_acquire)
                && 1 == turn.load(std::memory_order_relaxed));

            data($) = 1;

            flag0.store(0, std::memory_order_release);
        }
        else
        {
            flag1.store(1, std::memory_order_relaxed);
            std::atomic_thread_fence(std::memory_order_seq_cst);
            turn.store(0, std::memory_order_relaxed);
            std::atomic_thread_fence(std::memory_order_seq_cst);

            while (flag0.load(std::memory_order_acquire)
                && 0 == turn.load(std::memory_order_relaxed));

            data($) = 2;

            flag1.store(0, std::memory_order_release);
        }
    }
};

// FAILS WITH DATA RACE
struct peterson_mutex_test4 : rl::test_suite<peterson_mutex_test4, 2>
{
    std::atomic<int> flag0;
    std::atomic<int> flag1;
    std::atomic<int> turn;

    rl::var<int> data;

    void before()
    {
        flag0($) = 0;
        flag1($) = 0;
        turn($) = 0;
    }

    void thread(unsigned index)
    {
        if (0 == index)
        {
            flag0.exchange(1, rl::memory_order_acq_rel);
            turn.store(1, rl::memory_order_release);

            while (flag1.load(rl::memory_order_acquire)
                && 1 == turn.load(rl::memory_order_acquire))
                rl::yield(1, $);

            data($) = 1;

            flag0.store(0, rl::memory_order_release);
        }
        else
        {
            flag1.exchange(1, rl::memory_order_acq_rel);
            turn.store(0, rl::memory_order_release);

            while (flag0.load(rl::memory_order_acquire)
                && 0 == turn.load(rl::memory_order_relaxed))
                rl::yield(1, $);

            data($) = 2;

            flag1.store(0, rl::memory_order_release);
        }
    }
};

struct peterson_mutex_test5 : rl::test_suite<peterson_mutex_test5, 2>
{
    peterson_mutex mutex;
    rl::var<int> data;

    void before()
    {
        data($) = 0;
    }

    void thread(unsigned index)
    {
    	mutex.lock(index);
    	data($)++;
    	//printf("%d thread chan")
    	mutex.unlock(index);
    }
};


int main()
{
    rl::test_params p;
    p.search_type = rl::sched_bound;
    rl::simulate<peterson_mutex_test>();
    rl::simulate<peterson_mutex_test2>(p);
    rl::simulate<peterson_mutex_test3>();
    rl::simulate<peterson_mutex_test5>();
    //rl::simulate<peterson_mutex_test4>(p);
}
