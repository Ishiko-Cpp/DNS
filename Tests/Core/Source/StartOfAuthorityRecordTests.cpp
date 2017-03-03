/*
    Copyright (c) 2015 Xavier Leclercq

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

#include "StartOfAuthorityRecordTests.h"
#include "Ishiko/DNS/DNSCore.h"
#include "Ishiko/FileSystem/Utilities.h"
#include <fstream>

void AddStartOfAuthorityRecordTests(TestHarness& theTestHarness)
{
    TestSequence& soaRecordTestSequence = theTestHarness.appendTestSequence("StartOfAuthorityRecord tests");

    new HeapAllocationErrorsTest("Creation test 1", StartOfAuthorityRecordCreationTest1, soaRecordTestSequence);

    new HeapAllocationErrorsTest("initializeFromBuffer test 1", StartOfAuthorityRecordInitializeFromBufferTest1, soaRecordTestSequence);
    new HeapAllocationErrorsTest("initializeFromBuffer test 2", StartOfAuthorityRecordInitializeFromBufferTest2, soaRecordTestSequence);
    new HeapAllocationErrorsTest("initializeFromBuffer test 3", StartOfAuthorityRecordInitializeFromBufferTest3, soaRecordTestSequence);

    new FileComparisonTest("write test 1", StartOfAuthorityRecordWriteBinaryTest1, soaRecordTestSequence);
}

TestResult::EOutcome StartOfAuthorityRecordCreationTest1()
{
    Ishiko::DNS::StartOfAuthorityRecord soaRecord("example.org.", Ishiko::DNS::ResourceRecord::CLASS_IN,
        86400, "ns1.example.org.", "hostmaster.example.org.", 1485619377, 1200, 120, 604800, 3600);
    return TestResult::ePassed;
}

TestResult::EOutcome StartOfAuthorityRecordInitializeFromBufferTest1(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "StartOfAuthorityRecordInitializeFromBufferTest1.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r > 0)
    {
        Ishiko::DNS::StartOfAuthorityRecord soaRecord;
        const char* currentPos = buffer;
        if (soaRecord.initializeFromBuffer(buffer, buffer + r, &currentPos).succeeded())
        {
            if ((soaRecord.mname() == "ns1.example.org.") &&
                (soaRecord.rname() == "hostmaster.example.org.") &&
                (soaRecord.serial() == 1485619377) &&
                (soaRecord.refresh() == 1200) &&
                (soaRecord.retry() == 120) &&
                (soaRecord.expire() == 604800) &&
                (soaRecord.minimum() == 3600))
            {
                result = TestResult::ePassed;
            }
        }
    }

    return result;
}

// Make sure we return an error if the size of the data is incorrect
TestResult::EOutcome StartOfAuthorityRecordInitializeFromBufferTest2(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "StartOfAuthorityRecordInitializeFromBufferTest2.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r > 0)
    {
        Ishiko::DNS::StartOfAuthorityRecord soaRecord("example.org.", Ishiko::DNS::ResourceRecord::CLASS_IN,
            86401, "ns2.example.org.", "hostmaster2.example.org.", 1485619378, 1201, 121, 604801, 3601);
        const char* currentPos = buffer;
        if (soaRecord.initializeFromBuffer(buffer, buffer + r, &currentPos).failed())
        {
            if ((soaRecord.mname() == "ns2.example.org.") &&
                (soaRecord.rname() == "hostmaster2.example.org.") &&
                (soaRecord.serial() == 1485619378) &&
                (soaRecord.refresh() == 1201) &&
                (soaRecord.retry() == 121) &&
                (soaRecord.expire() == 604801) &&
                (soaRecord.minimum() == 3601))
            {
                result = TestResult::ePassed;
            }
        }
    }

    return result;
}

// Corrupt RNAME in input
TestResult::EOutcome StartOfAuthorityRecordInitializeFromBufferTest3(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "StartOfAuthorityRecordInitializeFromBufferTest3.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r > 0)
    {
        Ishiko::DNS::StartOfAuthorityRecord soaRecord("example.org.", Ishiko::DNS::ResourceRecord::CLASS_IN,
            86401, "ns2.example.org.", "hostmaster2.example.org.", 1485619378, 1201, 121, 604801, 3601);
        const char* currentPos = buffer;
        if (soaRecord.initializeFromBuffer(buffer, buffer + r, &currentPos).failed())
        {
            if ((soaRecord.mname() == "ns2.example.org.") &&
                (soaRecord.rname() == "hostmaster2.example.org.") &&
                (soaRecord.serial() == 1485619378) &&
                (soaRecord.refresh() == 1201) &&
                (soaRecord.retry() == 121) &&
                (soaRecord.expire() == 604801) &&
                (soaRecord.minimum() == 3601))
            {
                result = TestResult::ePassed;
            }
        }
    }

    return result;
}

TestResult::EOutcome StartOfAuthorityRecordWriteBinaryTest1(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "StartOfAuthorityRecordWriteBinaryTest1.bin");
    std::ofstream stream(outputPath.c_str(), std::ios::binary);

    Ishiko::DNS::StartOfAuthorityRecord soaRecord("example.org.", Ishiko::DNS::ResourceRecord::CLASS_IN,
        86400, "ns1.example.org.", "hostmaster.example.org.", 1485619377, 1200, 120, 604800, 3600);
    soaRecord.writeBinary(stream);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "StartOfAuthorityRecordWriteBinaryTest1.bin");

    return TestResult::ePassed;
}
