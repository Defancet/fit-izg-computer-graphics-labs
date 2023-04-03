#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <gtest/gtest.h>
#include <memory>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

#include <tests/CustomListener.h>

GTEST_API_ int main(int argc, char **argv)
{

    int customArgc = argc + 1;
    std::shared_ptr<char*> customArgv(new char*[customArgc], std::default_delete<char*[]>());

    const char* const filter = "--gtest_filter=*";

    for (int i = 0; i < argc; ++i)
    {
        customArgv.get()[i] = argv[i];
    }
    customArgv.get()[argc] = const_cast<char*>(filter);

    testing::InitGoogleTest(&customArgc, customArgv.get());

    // Gets hold of the event listener list.
    testing::TestEventListeners& listeners =  testing::UnitTest::GetInstance()->listeners();
    auto default_printer = listeners.Release(listeners.default_result_printer());

    testing::ConfigurableEventListener * listener = new testing::ConfigurableEventListener(default_printer);
    listener->showEnvironment = true;
    listener->showTestCases = true;
    listener->showTestNames = true;
    listener->showSuccesses = true;
    listener->showInlineFailures = true;
    listeners.Append(listener);

    return RUN_ALL_TESTS();
}
