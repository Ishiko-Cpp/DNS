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

#include "QuestionMessageSectionEntryTests.h"
#include "Ishiko/DNS/DNSCore.h"
#include <fstream>

void AddQuestionMessageSectionEntryTests(TestHarness& theTestHarness)
{
	TestSequence& entryTestSequence = theTestHarness.appendTestSequence("QuestionMessageSectionEntry tests");

	new HeapAllocationErrorsTest("Creation test 1", QuestionMessageSectionEntryCreationTest1, entryTestSequence);
	new FileComparisonTest("write test 1", QuestionMessageSectionEntryWriteTest1, entryTestSequence);
}

TestResult::EOutcome QuestionMessageSectionEntryCreationTest1()
{
	Ishiko::DNS::QuestionMessageSectionEntry entry("www.dummy.com", 
		Ishiko::DNS::QuestionMessageSectionEntry::QTYPE_A,
		Ishiko::DNS::QuestionMessageSectionEntry::QCLASS_IN);
	return TestResult::ePassed;
}

TestResult::EOutcome QuestionMessageSectionEntryWriteTest1(FileComparisonTest& test)
{
	boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "QuestionMessageSectionEntryWriteTest1.bin");
	std::ofstream stream(outputPath.c_str());

	Ishiko::DNS::QuestionMessageSectionEntry entry("www.dummy.com",
		Ishiko::DNS::QuestionMessageSectionEntry::QTYPE_A,
		Ishiko::DNS::QuestionMessageSectionEntry::QCLASS_IN);
	entry.write(stream);

	test.setOutputFilePath(outputPath);
	test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "QuestionMessageSectionEntryWriteTest1.bin");

	return TestResult::ePassed;
}
