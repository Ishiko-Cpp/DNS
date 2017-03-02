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

#include "TTLTests.h"
#include "Ishiko/DNS/DNSCore.h"
#include "Ishiko/FileSystem/Utilities.h"
#include <fstream>

void AddTTLTests(TestHarness& theTestHarness)
{
    TestSequence& ttlTestSequence = theTestHarness.appendTestSequence("TTL tests");

    new HeapAllocationErrorsTest("Creation test 1", TTLCreationTest1, ttlTestSequence);

    new HeapAllocationErrorsTest("initializeFromBuffer test 1", TTLInitializeFromBufferTest1, ttlTestSequence);

    new FileComparisonTest("write test 1", TTLWriteBinaryTest1, ttlTestSequence);
}

TestResult::EOutcome TTLCreationTest1()
{
    Ishiko::DNS::TTL ttl(5);
    return TestResult::ePassed;
}

TestResult::EOutcome TTLInitializeFromBufferTest1(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "TTLInitializeFromBufferTest1.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r > 0)
    {
        Ishiko::DNS::TTL ttl;
        const char* currentPos = buffer;
        if (ttl.initializeFromBuffer(buffer, buffer + r, &currentPos).succeeded())
        {
            if (ttl.asUInt32() == 5)
            {
                result = TestResult::ePassed;
            }
        }
    }

    return result;
}

TestResult::EOutcome TTLWriteBinaryTest1(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "TTLWriteBinaryTest1.bin");
    std::ofstream stream(outputPath.c_str(), std::ios::binary);

    Ishiko::DNS::TTL ttl(5);
    ttl.writeBinary(stream);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "TTLWriteBinaryTest1.bin");

    return TestResult::ePassed;
}
