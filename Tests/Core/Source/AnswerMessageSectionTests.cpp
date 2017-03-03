/*
    Copyright (c) 2015-2017 Xavier Leclercq

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

#include "AnswerMessageSectionTests.h"
#include "Ishiko/DNS/DNSCore.h"
#include "Ishiko/FileSystem/Utilities.h"
#include <fstream>

void AddAnswerMessageSectionTests(TestHarness& theTestHarness)
{
    TestSequence& answerTestSequence = theTestHarness.appendTestSequence("AnswerMessageSection tests");

    new HeapAllocationErrorsTest("Creation test 1", AnswerMessageSectionCreationTest1, answerTestSequence);

    new HeapAllocationErrorsTest("initializeFromBuffer test 1", AnswerMessageSectionInitializeFromBufferTest1, answerTestSequence);
    new HeapAllocationErrorsTest("initializeFromBuffer test 2", AnswerMessageSectionInitializeFromBufferTest2, answerTestSequence);
    new HeapAllocationErrorsTest("initializeFromBuffer test 3", AnswerMessageSectionInitializeFromBufferTest3, answerTestSequence);

    new FileComparisonTest("write test 1", AnswerMessageSectionWriteTest1, answerTestSequence);
    new FileComparisonTest("write test 2", AnswerMessageSectionWriteTest2, answerTestSequence);
}

TestResult::EOutcome AnswerMessageSectionCreationTest1()
{
    Ishiko::DNS::AnswerMessageSection answer;
    return TestResult::ePassed;
}

TestResult::EOutcome AnswerMessageSectionInitializeFromBufferTest1(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "AnswerMessageSectionInitializeFromBufferTest1.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r >= 0)
    {
        Ishiko::DNS::AnswerMessageSection answer;
        std::shared_ptr<Ishiko::DNS::AddressRecord> addressRecord = std::make_shared<Ishiko::DNS::AddressRecord>("example.org.", 86400, "127.0.0.1");
        answer.appendResourceRecord(addressRecord);
        const char* currentPos = buffer;
        if (answer.initializeFromBuffer(0, buffer, buffer + r, &currentPos).succeeded())
        {
            if (answer.resourceRecords().size() == 0)
            {
                result = TestResult::ePassed;
            }
        }
    }

    return result;
}

TestResult::EOutcome AnswerMessageSectionInitializeFromBufferTest2(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "AnswerMessageSectionInitializeFromBufferTest2.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r >= 0)
    {
        Ishiko::DNS::AnswerMessageSection answer;
        const char* currentPos = buffer;
        if (answer.initializeFromBuffer(1, buffer, buffer + r, &currentPos).succeeded())
        {
            if (answer.resourceRecords().size() == 1)
            {
                std::shared_ptr<Ishiko::DNS::AddressRecord> record = std::dynamic_pointer_cast<Ishiko::DNS::AddressRecord>(answer.resourceRecords()[0]);
                if (record && (record->IPAddress().to_string() == "127.0.0.1"))
                {
                    result = TestResult::ePassed;
                }
            }
        }
    }

    return result;
}

TestResult::EOutcome AnswerMessageSectionInitializeFromBufferTest3(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "AnswerMessageSectionInitializeFromBufferTest3.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r >= 0)
    {
        Ishiko::DNS::AnswerMessageSection answer;
        const char* currentPos = buffer;
        if (answer.initializeFromBuffer(1, buffer, buffer + r, &currentPos).failed())
        {
            if (answer.resourceRecords().size() == 0)
            {
                result = TestResult::ePassed;
            }
        }
    }

    return result;
}

TestResult::EOutcome AnswerMessageSectionWriteTest1(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "AnswerMessageSectionWriteTest1.bin");
    std::ofstream stream(outputPath.c_str());

    Ishiko::DNS::AnswerMessageSection answer;
    answer.write(stream);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "AnswerMessageSectionWriteTest1.bin");

    return TestResult::ePassed;
}

TestResult::EOutcome AnswerMessageSectionWriteTest2(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "AnswerMessageSectionWriteTest2.bin");
    std::ofstream stream(outputPath.c_str());

    Ishiko::DNS::AnswerMessageSection answer;
    std::shared_ptr<Ishiko::DNS::AddressRecord> addressRecord = std::make_shared<Ishiko::DNS::AddressRecord>("example.org.", 86400, "127.0.0.1");
    answer.appendResourceRecord(addressRecord);
    answer.write(stream);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "AnswerMessageSectionWriteTest2.bin");

    return TestResult::ePassed;
}
