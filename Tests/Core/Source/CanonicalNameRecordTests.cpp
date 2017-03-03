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

#include "CanonicalNameRecordTests.h"
#include "Ishiko/DNS/DNSCore.h"
#include "Ishiko/FileSystem/Utilities.h"
#include <fstream>

void AddCanonicalNameRecordTests(TestHarness& theTestHarness)
{
    TestSequence& cnameRecordTestSequence = theTestHarness.appendTestSequence("CanonicalNameRecord tests");

    new HeapAllocationErrorsTest("Creation test 1", CanonicalNameRecordCreationTest1, cnameRecordTestSequence);

    new HeapAllocationErrorsTest("initializeFromBuffer test 1", CanonicalNameRecordInitializeFromBufferTest1, cnameRecordTestSequence);
    new HeapAllocationErrorsTest("initializeFromBuffer test 2", CanonicalNameRecordInitializeFromBufferTest2, cnameRecordTestSequence);
    new HeapAllocationErrorsTest("initializeFromBuffer test 3", CanonicalNameRecordInitializeFromBufferTest3, cnameRecordTestSequence);

    new FileComparisonTest("write test 1", CanonicalNameRecordWriteBinaryTest1, cnameRecordTestSequence);
}

TestResult::EOutcome CanonicalNameRecordCreationTest1()
{
    Ishiko::DNS::CanonicalNameRecord cnameRecord("www.example.org.", 86400, "example.org.");
    return TestResult::ePassed;
}

TestResult::EOutcome CanonicalNameRecordInitializeFromBufferTest1(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "CanonicalNameRecordInitializeFromBufferTest1.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r > 0)
    {
        Ishiko::DNS::CanonicalNameRecord cnameRecord;
        const char* currentPos = buffer;
        if (cnameRecord.initializeFromBuffer(buffer, buffer + r, &currentPos).succeeded())
        {
            if (cnameRecord.cname() == "example.org.")
            {
                result = TestResult::ePassed;
            }
        }
    }

    return result;
}

// Make sure we return an error if the size of the data is incorrect
TestResult::EOutcome CanonicalNameRecordInitializeFromBufferTest2(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "CanonicalNameRecordInitializeFromBufferTest2.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r > 0)
    {
        Ishiko::DNS::CanonicalNameRecord cnameRecord("www.example.org.", 86400, "www2.example.org.");
        const char* currentPos = buffer;
        if (cnameRecord.initializeFromBuffer(buffer, buffer + r, &currentPos).failed())
        {
            if (cnameRecord.cname() == "www2.example.org.")
            {
                result = TestResult::ePassed;
            }
        }
    }

    return result;
}

// Make sure we return an error if the domain name is corrupt
TestResult::EOutcome CanonicalNameRecordInitializeFromBufferTest3(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "CanonicalNameRecordInitializeFromBufferTest3.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r > 0)
    {
        Ishiko::DNS::CanonicalNameRecord cnameRecord("www.example.org.", 86400, "www2.example.org.");
        const char* currentPos = buffer;
        if (cnameRecord.initializeFromBuffer(buffer, buffer + r, &currentPos).failed())
        {
            if (cnameRecord.cname() == "www2.example.org.")
            {
                result = TestResult::ePassed;
            }
        }
    }

    return result;
}

TestResult::EOutcome CanonicalNameRecordWriteBinaryTest1(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "CanonicalNameRecordWriteBinaryTest1.bin");
    std::ofstream stream(outputPath.c_str(), std::ios::binary);

    Ishiko::DNS::CanonicalNameRecord cnameRecord("www.example.org.", 86400, "example.org.");
    cnameRecord.writeBinary(stream);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "CanonicalNameRecordWriteBinaryTest1.bin");

    return TestResult::ePassed;
}
