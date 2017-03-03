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

#include "QuestionMessageSectionTests.h"
#include "Ishiko/DNS/DNSCore.h"
#include "Ishiko/FileSystem/Utilities.h"
#include <fstream>

void AddQuestionMessageSectionTests(TestHarness& theTestHarness)
{
    TestSequence& questionTestSequence = theTestHarness.appendTestSequence("QuestionMessageSection tests");

    new HeapAllocationErrorsTest("Creation test 1", QuestionMessageSectionCreationTest1, questionTestSequence);
    new HeapAllocationErrorsTest("Creation test 2", QuestionMessageSectionCreationTest2, questionTestSequence);

    new HeapAllocationErrorsTest("initializeFromBuffer test 1", QuestionMessageSectionInitializeFromBufferTest1, questionTestSequence);
    new HeapAllocationErrorsTest("initializeFromBuffer test 2", QuestionMessageSectionInitializeFromBufferTest2, questionTestSequence);
    new HeapAllocationErrorsTest("initializeFromBuffer test 3", QuestionMessageSectionInitializeFromBufferTest3, questionTestSequence);

    new FileComparisonTest("write test 1", QuestionMessageSectionWriteTest1, questionTestSequence);
    new FileComparisonTest("write test 2", QuestionMessageSectionWriteTest2, questionTestSequence);
}

TestResult::EOutcome QuestionMessageSectionCreationTest1()
{
    Ishiko::DNS::QuestionMessageSection question;
    return TestResult::ePassed;
}

TestResult::EOutcome QuestionMessageSectionCreationTest2()
{
    Ishiko::DNS::QuestionMessageSection question("www.dummy.com");
    return TestResult::ePassed;
}

TestResult::EOutcome QuestionMessageSectionInitializeFromBufferTest1(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "QuestionMessageSectionInitializeFromBufferTest1.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r >= 0)
    {
        Ishiko::DNS::QuestionMessageSection question("www.dummy.com");
        const char* currentPos = buffer;
        if (question.initializeFromBuffer(0, buffer, buffer + r, &currentPos).succeeded())
        {
            if (question.entries().size() == 0)
            {
                result = TestResult::ePassed;
            }
        }
    }

    return result;
}

TestResult::EOutcome QuestionMessageSectionInitializeFromBufferTest2(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "QuestionMessageSectionInitializeFromBufferTest2.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r >= 0)
    {
        Ishiko::DNS::QuestionMessageSection question;
        const char* currentPos = buffer;
        if (question.initializeFromBuffer(1, buffer, buffer + r, &currentPos).succeeded())
        {
            if (question.entries().size() == 1)
            {
                const Ishiko::DNS::QuestionMessageSectionEntry& entry = *question.entries()[0];
                if ((entry.qname() == "www.dummy.com.") &&
                    (entry.qtype() == Ishiko::DNS::QuestionMessageSectionEntry::QTYPE_A) &&
                    (entry.qclass() == Ishiko::DNS::QuestionMessageSectionEntry::QCLASS_IN))
                {
                    result = TestResult::ePassed;
                }
            }
        }
    }

    return result;
}

TestResult::EOutcome QuestionMessageSectionInitializeFromBufferTest3(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "QuestionMessageSectionInitializeFromBufferTest3.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r >= 0)
    {
        Ishiko::DNS::QuestionMessageSection question;
        const char* currentPos = buffer;
        if (question.initializeFromBuffer(1, buffer, buffer + r, &currentPos).failed())
        {
            if (question.entries().size() == 0)
            {
                result = TestResult::ePassed;
            }
        }
    }

    return result;
}

TestResult::EOutcome QuestionMessageSectionWriteTest1(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "QuestionMessageSectionWriteTest1.bin");
    std::ofstream stream(outputPath.c_str());

    Ishiko::DNS::QuestionMessageSection question;
    question.write(stream);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "QuestionMessageSectionWriteTest1.bin");

    return TestResult::ePassed;
}

TestResult::EOutcome QuestionMessageSectionWriteTest2(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "QuestionMessageSectionWriteTest2.bin");
    std::ofstream stream(outputPath.c_str());

    Ishiko::DNS::QuestionMessageSection question("www.dummy.com");
    question.write(stream);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "QuestionMessageSectionWriteTest2.bin");

    return TestResult::ePassed;
}
