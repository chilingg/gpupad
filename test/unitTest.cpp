#include <RDebug.h>
#include <RThreadPool.h>

using namespace Redopera;

bool primeNum(unsigned n)
{
    for(unsigned i = 2; i < n; ++i)
        if(n % i == 0)
            return false;
    return true;
}

int main()
{
    RThreadPool pool;
    for(unsigned i = 0; i < 100000; ++i)
    {
        RFunction<bool(unsigned)> f([i]{
            if(primeNum(i))
                rDebug << i;
        });
        pool.submit(std::move(f));
    }

    while(!pool.isIdle())
        std::this_thread::yield();

    return 0;
}
