/*
    Copyright (c) 2010-2017 Xavier Leclercq

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/

#include "DomainNameTests.h"
#include "Ishiko/DNS/DNSCore.h"
#include "Ishiko/FileSystem/Utilities.h"
#include <fstream>

void AddDomainNameTests(TestHarness& theTestHarness)
{
    TestSequence& domainNameTestSequence = theTestHarness.appendTestSequence("DomainName tests");

    new HeapAllocationErrorsTest("Creation test 1", DomainNameCreationTest1, domainNameTestSequence);

    new HeapAllocationErrorsTest("initializeFromBuffer test 1", DomainNameInitializeFromBufferTest1, domainNameTestSequence);
    new HeapAllocationErrorsTest("initializeFromBuffer test 2", DomainNameInitializeFromBufferTest2, domainNameTestSequence);
    new HeapAllocationErrorsTest("initializeFromBuffer test 3", DomainNameInitializeFromBufferTest3, domainNameTestSequence);
    new HeapAllocationErrorsTest("initializeFromBuffer test 4", DomainNameInitializeFromBufferTest4, domainNameTestSequence);

    new FileComparisonTest("write test 1", DomainNameWriteTest1, domainNameTestSequence);
    new FileComparisonTest("write test 2", DomainNameWriteTest2, domainNameTestSequence);

    new HeapAllocationErrorsTest("swap test 1", DomainNameSwapTest1, domainNameTestSequence);
}

TestResult::EOutcome DomainNameCreationTest1()
{
    Ishiko::DNS::DomainName domainName("www.dummy.com.");
    if (domainName == "www.dummy.com.")
    {
        return TestResult::ePassed;
    }
    else
    {
        return TestResult::eFailed;
    }
}

TestResult::EOutcome DomainNameInitializeFromBufferTest1(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "DomainNameInitializeFromStreamTest1.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r > 0)
    {
        Ishiko::DNS::DomainName domainName;
        const char* currentPos = buffer;
        if (domainName.initializeFromBuffer(buffer, buffer + r, &currentPos).succeeded())
        {
            if (domainName == "www.dummy.com.")
            {
                result = TestResult::ePassed;
            }
        }
    }
    
    return result;
}

// Read a domain name from a stream that has been truncated. Check
// than an error is returned.
TestResult::EOutcome DomainNameInitializeFromBufferTest2(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "DomainNameInitializeFromStreamTest2.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r > 0)
    {
        Ishiko::DNS::DomainName domainName;
        const char* currentPos = buffer;
        if (domainName.initializeFromBuffer(buffer, buffer + r, &currentPos).failed())
        {
            if (domainName == "")
            {
                result = TestResult::ePassed;
            }
        }
    }

    return result;
}

// Read a domain name where one of the labels is longer than the maximum allowed
// size of 63. Check that an error is returned.
TestResult::EOutcome DomainNameInitializeFromBufferTest3(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "DomainNameInitializeFromStreamTest3.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r > 0)
    {
        Ishiko::DNS::DomainName domainName;
        const char* currentPos = buffer;
        if (domainName.initializeFromBuffer(buffer, buffer + r, &currentPos).failed())
        {
            if (domainName == "")
            {
                result = TestResult::ePassed;
            }
        }
    }

    return result;
}

// Read a domain name where the total length is longer than the maximum allowed
// size of 253. Check that an error is returned.
TestResult::EOutcome DomainNameInitializeFromBufferTest4(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "DomainNameInitializeFromStreamTest4.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r > 0)
    {
        Ishiko::DNS::DomainName domainName;
        const char* currentPos = buffer;
        if (domainName.initializeFromBuffer(buffer, buffer + r, &currentPos).failed())
        {
            if (domainName == "")
            {
                result = TestResult::ePassed;
            }
        }
    }

    return result;
}

TestResult::EOutcome DomainNameWriteTest1(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "DomainNameWriteTest1.bin");
    std::ofstream stream(outputPath.c_str(), std::ios::binary);

    Ishiko::DNS::DomainName domainName("www.dummy.com");
    domainName.writeBinary(stream);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "DomainNameWriteTest1.bin");

    return TestResult::ePassed;
}

TestResult::EOutcome DomainNameWriteTest2(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "DomainNameWriteTest2.bin");
    std::ofstream stream(outputPath.c_str(), std::ios::binary);

    Ishiko::DNS::DomainName domainName("www.dummy.com.");
    domainName.writeBinary(stream);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "DomainNameWriteTest2.bin");

    return TestResult::ePassed;
}

TestResult::EOutcome DomainNameSwapTest1()
{
    TestResult::EOutcome result = TestResult::eFailed;

    Ishiko::DNS::DomainName domainName1("www.dummy1.com.");
    Ishiko::DNS::DomainName domainName2("www.dummy2.com.");

    swap(domainName1, domainName2);

    if ((domainName1 == "www.dummy2.com.") &&
        (domainName2 == "www.dummy1.com."))
    {
        result = TestResult::ePassed;
    }

    return result;
}
