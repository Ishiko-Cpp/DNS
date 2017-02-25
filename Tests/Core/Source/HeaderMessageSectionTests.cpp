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

#include "HeaderMessageSectionTests.h"
#include "Ishiko/DNS/DNSCore.h"
#include "Ishiko/FileSystem/Utilities.h"
#include <fstream>

void AddHeaderMessageSectionTests(TestHarness& theTestHarness)
{
    TestSequence& headerTestSequence = theTestHarness.appendTestSequence("HeaderMessageSection tests");

    new HeapAllocationErrorsTest("Creation test 1", HeaderMessageSectionCreationTest1, headerTestSequence);
    new HeapAllocationErrorsTest("initializeFromBuffer test 1", HeaderMessageSectionInitializeFromBufferTest1, headerTestSequence);
    new HeapAllocationErrorsTest("initializeFromBuffer test 2", HeaderMessageSectionInitializeFromBufferTest2, headerTestSequence);

    new FileComparisonTest("write test 1", HeaderMessageSectionWriteTest1, headerTestSequence);
    new FileComparisonTest("write test 2", HeaderMessageSectionWriteTest2, headerTestSequence);
}

TestResult::EOutcome HeaderMessageSectionCreationTest1()
{
    Ishiko::DNS::HeaderMessageSection header(false);
    return TestResult::ePassed;
}

TestResult::EOutcome HeaderMessageSectionInitializeFromBufferTest1(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "HeaderMessageSectionCreationTest2.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r > 0)
    {
        Ishiko::DNS::HeaderMessageSection header;
        const char* currentPos = buffer;
        if (header.initializeFromBuffer(buffer, buffer + r, &currentPos).succeeded())
        {
            if (!header.isResponse())
            {
                result = TestResult::ePassed;
            }
        }
    }
    
    return result;
}

TestResult::EOutcome HeaderMessageSectionInitializeFromBufferTest2(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "HeaderMessageSectionCreationTest3.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r > 0)
    {
        Ishiko::DNS::HeaderMessageSection header;
        const char* currentPos = buffer;
        if (header.initializeFromBuffer(buffer, buffer + r, &currentPos).succeeded())
        {
            if (header.isResponse())
            {
                result = TestResult::ePassed;
            }
        }
    }

    return result;
}

TestResult::EOutcome HeaderMessageSectionWriteTest1(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "HeaderMessageSectionWriteTest1.bin");
    std::ofstream stream(outputPath.c_str());

    Ishiko::DNS::HeaderMessageSection header(false);
    header.write(stream);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "HeaderMessageSectionWriteTest1.bin");

    return TestResult::ePassed;
}

TestResult::EOutcome HeaderMessageSectionWriteTest2(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "HeaderMessageSectionWriteTest2.bin");
    std::ofstream stream(outputPath.c_str());

    Ishiko::DNS::HeaderMessageSection header(true);
    header.write(stream);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "HeaderMessageSectionWriteTest2.bin");

    return TestResult::ePassed;
}
