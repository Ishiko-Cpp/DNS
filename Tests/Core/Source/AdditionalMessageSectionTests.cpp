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

#include "AdditionalMessageSectionTests.h"
#include "Ishiko/DNS/DNSCore.h"
#include "Ishiko/FileSystem/Utilities.h"
#include <fstream>

void AddAdditionalMessageSectionTests(TestHarness& theTestHarness)
{
    TestSequence& additionalTestSequence = theTestHarness.appendTestSequence("AdditionalMessageSection tests");

    new HeapAllocationErrorsTest("Creation test 1", AdditionalMessageSectionCreationTest1, additionalTestSequence);

    new HeapAllocationErrorsTest("initializeFromBuffer test 1", AdditionalMessageSectionInitializeFromBufferTest1, additionalTestSequence);
    new HeapAllocationErrorsTest("initializeFromBuffer test 2", AdditionalMessageSectionInitializeFromBufferTest2, additionalTestSequence);
    new HeapAllocationErrorsTest("initializeFromBuffer test 3", AdditionalMessageSectionInitializeFromBufferTest3, additionalTestSequence);

    new FileComparisonTest("write test 1", AdditionalMessageSectionWriteTest1, additionalTestSequence);
    new FileComparisonTest("write test 2", AdditionalMessageSectionWriteTest2, additionalTestSequence);
}

TestResult::EOutcome AdditionalMessageSectionCreationTest1()
{
    Ishiko::DNS::AdditionalMessageSection additional;
    return TestResult::ePassed;
}

TestResult::EOutcome AdditionalMessageSectionInitializeFromBufferTest1(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "AdditionalMessageSectionInitializeFromBufferTest1.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r >= 0)
    {
        Ishiko::DNS::AdditionalMessageSection additional;
        std::shared_ptr<Ishiko::DNS::TextRecord> txtRecord = std::make_shared<Ishiko::DNS::TextRecord>("example.org.", 86400, "data");
        additional.appendResourceRecord(txtRecord);
        const char* currentPos = buffer;
        if (additional.initializeFromBuffer(0, buffer, buffer + r, &currentPos).succeeded())
        {
            if (additional.resourceRecords().size() == 0)
            {
                result = TestResult::ePassed;
            }
        }
    }

    return result;
}

TestResult::EOutcome AdditionalMessageSectionInitializeFromBufferTest2(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "AdditionalMessageSectionInitializeFromBufferTest2.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r >= 0)
    {
        Ishiko::DNS::AdditionalMessageSection additional;
        const char* currentPos = buffer;
        if (additional.initializeFromBuffer(1, buffer, buffer + r, &currentPos).succeeded())
        {
            if (additional.resourceRecords().size() == 1)
            {
                std::shared_ptr<Ishiko::DNS::TextRecord> record = std::dynamic_pointer_cast<Ishiko::DNS::TextRecord>(additional.resourceRecords()[0]);
                if (record && (record->text() == "data"))
                {
                    result = TestResult::ePassed;
                }
            }
        }
    }

    return result;
}

TestResult::EOutcome AdditionalMessageSectionInitializeFromBufferTest3(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "AdditionalMessageSectionInitializeFromBufferTest3.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r >= 0)
    {
        Ishiko::DNS::AdditionalMessageSection additional;
        const char* currentPos = buffer;
        if (additional.initializeFromBuffer(1, buffer, buffer + r, &currentPos).failed())
        {
            if (additional.resourceRecords().size() == 0)
            {
                result = TestResult::ePassed;
            }
        }
    }

    return result;
}

TestResult::EOutcome AdditionalMessageSectionWriteTest1(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "AdditionalMessageSectionWriteTest1.bin");
    std::ofstream stream(outputPath.c_str());

    Ishiko::DNS::AdditionalMessageSection additional;
    additional.write(stream);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "AdditionalMessageSectionWriteTest1.bin");

    return TestResult::ePassed;
}

TestResult::EOutcome AdditionalMessageSectionWriteTest2(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "AdditionalMessageSectionWriteTest2.bin");
    std::ofstream stream(outputPath.c_str());

    Ishiko::DNS::AdditionalMessageSection additional;
    std::shared_ptr<Ishiko::DNS::TextRecord> txtRecord = std::make_shared<Ishiko::DNS::TextRecord>("example.org.", 86400, "data");
    additional.appendResourceRecord(txtRecord);
    additional.write(stream);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "AdditionalMessageSectionWriteTest2.bin");

    return TestResult::ePassed;
}
