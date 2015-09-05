
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

#include <boost/fiber/all.hpp>
#include <boost/preprocessor.hpp>

#include "../clock.hpp"

#ifndef JOBS
#define JOBS BOOST_PP_LIMIT_REPEAT
#endif

#define JOIN(z, n, _) \
    boost::fibers::fiber( worker).join();

void worker()
{ boost::this_fiber::yield(); }

duration_type measure( duration_type overhead)
{
    boost::fibers::fiber( worker).join();

    time_point_type start( clock_type::now() );

    BOOST_PP_REPEAT_FROM_TO(1, JOBS, JOIN, _)

    duration_type total = clock_type::now() - start;
    total -= overhead_clock(); // overhead of measurement
    total /= JOBS;  // loops

    return total;
}

int main( int argc, char * argv[])
{
    try
    {
        duration_type overhead = overhead_clock();
        std::cout << "overhead " << overhead.count() << " nano seconds" << std::endl;
        boost::uint64_t res = measure( overhead).count();
        std::cout << "average of " << res << " nano seconds" << std::endl;

        return EXIT_SUCCESS;
    }
    catch ( std::exception const& e)
    { std::cerr << "exception: " << e.what() << std::endl; }
    catch (...)
    { std::cerr << "unhandled exception" << std::endl; }
    return EXIT_FAILURE;
}