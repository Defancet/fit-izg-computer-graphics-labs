#include "gtest/gtest.h"

namespace testing
{

class ConfigurableEventListener : public TestEventListener
{

protected:
    TestEventListener* eventListener;

public:

    /**
     * Show the names of each test case.
     */
    bool showTestCases;

    /**
     * Show the names of each test.
     */
    bool showTestNames;

    /**
     * Show each success.
     */
    bool showSuccesses;

    /**
     * Show each failure as it occurs. You will also see it at the bottom after the full suite is run.
     */
    bool showInlineFailures;

    /**
     * Show the setup of the global environment.
     */
    bool showEnvironment;

    explicit ConfigurableEventListener(TestEventListener* theEventListener) : eventListener(theEventListener)
    {
        showTestCases = true;
        showTestNames = true;
        showSuccesses = true;
        showInlineFailures = true;
        showEnvironment = true;
    }

    virtual ~ConfigurableEventListener()
    {
        delete eventListener;
    }

    virtual void OnTestProgramStart(const UnitTest& unit_test)
    {
        eventListener->OnTestProgramStart(unit_test);
    }

    virtual void OnTestIterationStart(const UnitTest& unit_test, int iteration)
    {
        eventListener->OnTestIterationStart(unit_test, iteration);
    }

    virtual void OnEnvironmentsSetUpStart(const UnitTest& unit_test)
    {
        if (showEnvironment) {
            eventListener->OnEnvironmentsSetUpStart(unit_test);
        }
    }

    virtual void OnEnvironmentsSetUpEnd(const UnitTest& unit_test)
    {
        if (showEnvironment) {
            eventListener->OnEnvironmentsSetUpEnd(unit_test);
        }
    }

    virtual void OnTestCaseStart(const TestCase& test_case)
    {
        if (showTestCases) {
            eventListener->OnTestCaseStart(test_case);
        }
    }

    virtual void OnTestStart(const TestInfo& test_info)
    {
        if (showTestNames) {
            eventListener->OnTestStart(test_info);
        }
    }

    virtual void OnTestPartResult(const TestPartResult& result)
    {
        const static std::string delimiter = "#";

        std::string message (result.message());
        auto n = message.find(delimiter);

        if (std::string::npos != n)
        {
            std::string sub = message.substr(n+1, message.length());
            std::cout << "\x1B[91m[  FAILED  ]\033[0m " << sub << std::endl;
        }
        else
        {
            eventListener->OnTestPartResult(result);
        }
    }

    virtual void OnTestEnd(const TestInfo& test_info)
    {
        if ((showInlineFailures && test_info.result()->Failed()) || (showSuccesses && !test_info.result()->Failed())) {
            eventListener->OnTestEnd(test_info);
        }
    }

    virtual void OnTestCaseEnd(const TestCase& test_case)
    {
        if (showTestCases) {
            eventListener->OnTestCaseEnd(test_case);
        }
    }

    virtual void OnEnvironmentsTearDownStart(const UnitTest& unit_test)
    {
        if (showEnvironment) {
            eventListener->OnEnvironmentsTearDownStart(unit_test);
        }
    }

    virtual void OnEnvironmentsTearDownEnd(const UnitTest& unit_test)
    {
        if (showEnvironment) {
            eventListener->OnEnvironmentsTearDownEnd(unit_test);
        }
    }

    virtual void OnTestIterationEnd(const UnitTest& unit_test, int iteration)
    {
        eventListener->OnTestIterationEnd(unit_test, iteration);
    }

    virtual void OnTestProgramEnd(const UnitTest& unit_test)
    {
        eventListener->OnTestProgramEnd(unit_test);
    }

};
}