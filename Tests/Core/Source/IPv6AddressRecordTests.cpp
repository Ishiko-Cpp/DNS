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

#include "IPv6AddressRecordTests.h"
#include "Ishiko/DNS/DNSCore.h"
#include "Ishiko/FileSystem/Utilities.h"
#include <fstream>

void AddIPv6AddressRecordTests(TestHarness& theTestHarness)
{
    TestSequence& addressRecordTestSequence = theTestHarness.appendTestSequence("IPv6AddressRecord tests");

    new HeapAllocationErrorsTest("Creation test 1", IPv6AddressRecordCreationTest1, addressRecordTestSequence);

    new HeapAllocationErrorsTest("initializeFromBuffer test 1", IPv6AddressRecordInitializeFromBufferTest1, addressRecordTestSequence);
    new HeapAllocationErrorsTest("initializeFromBuffer test 2", IPv6AddressRecordInitializeFromBufferTest2, addressRecordTestSequence);
    new HeapAllocationErrorsTest("initializeFromBuffer test 3", IPv6AddressRecordInitializeFromBufferTest3, addressRecordTestSequence);

    new FileComparisonTest("write test 1", IPv6AddressRecordWriteBinaryTest1, addressRecordTestSequence);
}

TestResult::EOutcome IPv6AddressRecordCreationTest1()
{
    Ishiko::DNS::IPv6AddressRecord addressRecord("example.org.", 86400, "::1");
    return TestResult::ePassed;
}

TestResult::EOutcome IPv6AddressRecordInitializeFromBufferTest1(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "IPv6AddressRecordInitializeFromBufferTest1.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r > 0)
    {
        Ishiko::DNS::IPv6AddressRecord addressRecord;
        const char* currentPos = buffer;
        if (addressRecord.initializeFromBuffer(buffer, buffer + r, &currentPos).succeeded())
        {
            if (addressRecord.IPAddress().to_string() == "::1")
            {
                result = TestResult::ePassed;
            }
        }
    }

    return result;
}

// Make sure we return an error if the size of the data is incorrect
TestResult::EOutcome IPv6AddressRecordInitializeFromBufferTest2(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "IPv6AddressRecordInitializeFromBufferTest2.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r > 0)
    {
        Ishiko::DNS::IPv6AddressRecord addressRecord("example.org.", 86400, "::");
        const char* currentPos = buffer;
        if (addressRecord.initializeFromBuffer(buffer, buffer + r, &currentPos).failed())
        {
            if (addressRecord.IPAddress().to_string() == "::")
            {
                result = TestResult::ePassed;
            }
        }
    }

    return result;
}

// Make sure we return an error if the data is truncated
TestResult::EOutcome IPv6AddressRecordInitializeFromBufferTest3(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "IPv6AddressRecordInitializeFromBufferTest3.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r > 0)
    {
        Ishiko::DNS::IPv6AddressRecord addressRecord("example.org.", 86400, "::");
        const char* currentPos = buffer;
        if (addressRecord.initializeFromBuffer(buffer, buffer + r, &currentPos).failed())
        {
            if (addressRecord.IPAddress().to_string() == "::")
            {
                result = TestResult::ePassed;
            }
        }
    }

    return result;
}

TestResult::EOutcome IPv6AddressRecordWriteBinaryTest1(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "IPv6AddressRecordWriteBinaryTest1.bin");
    std::ofstream stream(outputPath.c_str(), std::ios::binary);

    Ishiko::DNS::IPv6AddressRecord addressRecord("example.org.", 86400, "::1");
    addressRecord.writeBinary(stream);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "IPv6AddressRecordWriteBinaryTest1.bin");

    return TestResult::ePassed;
}
