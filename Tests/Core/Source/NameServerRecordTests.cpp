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

#include "NameServerRecordTests.h"
#include "Ishiko/DNS/DNSCore.h"
#include <fstream>

void AddNameServerRecordTests(TestHarness& theTestHarness)
{
    TestSequence& nsRecordTestSequence = theTestHarness.appendTestSequence("NameServerRecord tests");

    new HeapAllocationErrorsTest("Creation test 1", NameServerRecordCreationTest1, nsRecordTestSequence);
    new FileComparisonTest("write test 1", NameServerRecordWriteBinaryTest1, nsRecordTestSequence);
}

TestResult::EOutcome NameServerRecordCreationTest1()
{
    Ishiko::DNS::NameServerRecord nsRecord("example.org.", Ishiko::DNS::ResourceRecord::CLASS_IN, 86400);
    return TestResult::ePassed;
}

TestResult::EOutcome NameServerRecordWriteBinaryTest1(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "NameServerRecordWriteBinaryTest1.bin");
    std::ofstream stream(outputPath.c_str());

    Ishiko::DNS::NameServerRecord nsRecord("example.org.", Ishiko::DNS::ResourceRecord::CLASS_IN, 86400);
    nsRecord.writeBinary(stream);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "NameServerRecordWriteBinaryTest1.bin");

    return TestResult::ePassed;
}
