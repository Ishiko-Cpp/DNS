/*
    Copyright (c) 2017 Xavier Leclercq

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

#include "MailExchangeRecordTests.h"
#include "Ishiko/DNS/DNSCore.h"
#include "Ishiko/FileSystem/Utilities.h"
#include <fstream>

void AddMailExchangeRecordTests(TestHarness& theTestHarness)
{
    TestSequence& mailExchangeRecordTestSequence = theTestHarness.appendTestSequence("MailExchangeRecord tests");

    new HeapAllocationErrorsTest("Creation test 1", MailExchangeRecordCreationTest1, mailExchangeRecordTestSequence);

    new HeapAllocationErrorsTest("initializeFromBuffer test 1", MailExchangeRecordInitializeFromBufferTest1, mailExchangeRecordTestSequence);
    new HeapAllocationErrorsTest("initializeFromBuffer test 2", MailExchangeRecordInitializeFromBufferTest2, mailExchangeRecordTestSequence);
    new HeapAllocationErrorsTest("initializeFromBuffer test 3", MailExchangeRecordInitializeFromBufferTest3, mailExchangeRecordTestSequence);

    new FileComparisonTest("write test 1", MailExchangeRecordWriteBinaryTest1, mailExchangeRecordTestSequence);
}

TestResult::EOutcome MailExchangeRecordCreationTest1()
{
    Ishiko::DNS::MailExchangeRecord mailExchangeRecord("example.org.", 86400, 20, "mx.example.org");
    return TestResult::ePassed;
}

TestResult::EOutcome MailExchangeRecordInitializeFromBufferTest1(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "MailExchangeRecordInitializeFromBufferTest1.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r > 0)
    {
        Ishiko::DNS::MailExchangeRecord mailExchangeRecord;
        const char* currentPos = buffer;
        if (mailExchangeRecord.initializeFromBuffer(buffer, buffer + r, &currentPos).succeeded())
        {
            if ((mailExchangeRecord.preference() == 20) &&
                (mailExchangeRecord.exchange() == "mx.example.org."))
            {
                result = TestResult::ePassed;
            }
        }
    }

    return result;
}

// Make sure we return an error if the size of the data is incorrect
TestResult::EOutcome MailExchangeRecordInitializeFromBufferTest2(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "MailExchangeRecordInitializeFromBufferTest2.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r > 0)
    {
        Ishiko::DNS::MailExchangeRecord mailExchangeRecord("example.org.", 86400, 10, "mx2.example.org.");
        const char* currentPos = buffer;
        if (mailExchangeRecord.initializeFromBuffer(buffer, buffer + r, &currentPos).failed())
        {
            if ((mailExchangeRecord.preference() == 10) &&
                (mailExchangeRecord.exchange() == "mx2.example.org."))
            {
                result = TestResult::ePassed;
            }
        }
    }

    return result;
}

// Make sure we return an error if the data is truncated
TestResult::EOutcome MailExchangeRecordInitializeFromBufferTest3(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "MailExchangeRecordInitializeFromBufferTest3.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r > 0)
    {
        Ishiko::DNS::MailExchangeRecord mailExchangeRecord("example.org.", 86400, 10, "mx2.example.org.");
        const char* currentPos = buffer;
        if (mailExchangeRecord.initializeFromBuffer(buffer, buffer + r, &currentPos).failed())
        {
            if ((mailExchangeRecord.preference() == 10) &&
                (mailExchangeRecord.exchange() == "mx2.example.org."))
            {
                result = TestResult::ePassed;
            }
        }
    }

    return result;
}

TestResult::EOutcome MailExchangeRecordWriteBinaryTest1(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "MailExchangeRecordWriteBinaryTest1.bin");
    std::ofstream stream(outputPath.c_str(), std::ios::binary);

    Ishiko::DNS::MailExchangeRecord mailExchangeRecord("example.org.", 86400, 20, "mx.example.org");
    mailExchangeRecord.writeBinary(stream);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "MailExchangeRecordWriteBinaryTest1.bin");

    return TestResult::ePassed;
}
