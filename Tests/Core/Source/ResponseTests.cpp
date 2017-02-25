/*
	Copyright (c) 2010-2015 Xavier Leclercq

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

#include "ResponseTests.h"
#include "Ishiko/DNS/DNSCore.h"
#include "Ishiko/FileSystem/Utilities.h"
#include <fstream>

void AddResponseTests(TestHarness& theTestHarness)
{
	TestSequence& responseTestSequence = theTestHarness.appendTestSequence("Response tests");

	new HeapAllocationErrorsTest("Creation test 1", ResponseCreationTest1, responseTestSequence);
	new HeapAllocationErrorsTest("initializeFromBuffer test 1", ResponseInitializeFromBufferTest1, responseTestSequence);
	new FileComparisonTest("write test 1", ResponseWriteTest1, responseTestSequence);
}

TestResult::EOutcome ResponseCreationTest1()
{
	Ishiko::DNS::Response response;
	return TestResult::ePassed;
}

TestResult::EOutcome ResponseInitializeFromBufferTest1(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "ResponseInitializeFromBufferTest1.bin");
    char buffer[512];
    int r = Ishiko::FileSystem::Utilities::readFile(inputPath.string().c_str(), buffer, 512);
    if (r > 0)
    {
        Ishiko::DNS::Response response;
        const char* currentPos = buffer;
        if (response.initializeFromBuffer(buffer, buffer + r, &currentPos).succeeded())
        {
            if ((response.headerSection().isResponse()) &&
                (response.questionSection().entries().size() == 0) &&
                (response.answerSection().resourceRecords().size() == 0))
            {
                result = TestResult::ePassed;
            }
        }
    }

    return result;
}

TestResult::EOutcome ResponseWriteTest1(FileComparisonTest& test)
{
	boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "ResponseWriteTest1.bin");
	std::ofstream stream(outputPath.c_str());

	Ishiko::DNS::Response response;
	response.write(stream);

	test.setOutputFilePath(outputPath);
	test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "ResponseWriteTest1.bin");

	return TestResult::ePassed;
}
