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

#include "TextRecordTests.h"
#include "Ishiko/DNS/DNSCore.h"
#include "Ishiko/FileSystem/Utilities.h"
#include <fstream>

void AddTextRecordTests(TestHarness& theTestHarness)
{
    TestSequence& textRecordTestSequence = theTestHarness.appendTestSequence("TextRecord tests");

    new HeapAllocationErrorsTest("Creation test 1", TextRecordCreationTest1, textRecordTestSequence);

    new HeapAllocationErrorsTest("initializeFromBuffer test 1", TextRecordInitializeFromBufferTest1, textRecordTestSequence);
    new HeapAllocationErrorsTest("initializeFromBuffer test 2", TextRecordInitializeFromBufferTest2, textRecordTestSequence);

    new FileComparisonTest("write test 1", TextRecordWriteBinaryTest1, textRecordTestSequence);
}

TestResult::EOutcome TextRecordCreationTest1()
{
    Ishiko::DNS::TextRecord textRecord("example.org.", 86400, "data");
    return TestResult::ePassed;
}

TestResult::EOutcome TextRecordInitializeFromBufferTest1(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "TextRecordInitializeFromBufferTest1.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r > 0)
    {
        Ishiko::DNS::TextRecord textRecord;
        const char* currentPos = buffer;
        if (textRecord.initializeFromBuffer(buffer, buffer + r, &currentPos).succeeded())
        {
            if (textRecord.text() == "data")
            {
                result = TestResult::ePassed;
            }
        }
    }

    return result;
}

// Make sure we return an error if the size of the text is incorrect
TestResult::EOutcome TextRecordInitializeFromBufferTest2(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "TextRecordInitializeFromBufferTest2.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r > 0)
    {
        Ishiko::DNS::TextRecord textRecord("example.org.", 86400, "data2");
        const char* currentPos = buffer;
        if (textRecord.initializeFromBuffer(buffer, buffer + r, &currentPos).failed())
        {
            if (textRecord.text() == "data2")
            {
                result = TestResult::ePassed;
            }
        }
    }

    return result;
}

TestResult::EOutcome TextRecordWriteBinaryTest1(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "TextRecordWriteBinaryTest1.bin");
    std::ofstream stream(outputPath.c_str(), std::ios::binary);

    Ishiko::DNS::TextRecord textRecord("example.org.", 86400, "data");
    textRecord.writeBinary(stream);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "TextRecordWriteBinaryTest1.bin");

    return TestResult::ePassed;
}
