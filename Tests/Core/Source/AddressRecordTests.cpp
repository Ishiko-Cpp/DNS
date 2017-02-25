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

#include "AddressRecordTests.h"
#include "Ishiko/DNS/DNSCore.h"
#include <fstream>

void AddAddressRecordTests(TestHarness& theTestHarness)
{
    TestSequence& addressRecordTestSequence = theTestHarness.appendTestSequence("AddressRecord tests");

    new HeapAllocationErrorsTest("Creation test 1", AddressRecordCreationTest1, addressRecordTestSequence);
    new FileComparisonTest("write test 1", AddressRecordWriteBinaryTest1, addressRecordTestSequence);
}

TestResult::EOutcome AddressRecordCreationTest1()
{
    Ishiko::DNS::AddressRecord addressRecord("example.org.", 86400, "127.0.0.1");
    return TestResult::ePassed;
}

TestResult::EOutcome AddressRecordWriteBinaryTest1(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "AddressRecordWriteBinaryTest1.bin");
    std::ofstream stream(outputPath.c_str(), std::ios::binary);

    Ishiko::DNS::AddressRecord addressRecord("example.org.", 86400, "127.0.0.1");
    addressRecord.writeBinary(stream);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "AddressRecordWriteBinaryTest1.bin");

    return TestResult::ePassed;
}
