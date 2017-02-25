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

#include "DomainNameTests.h"
#include "TTLTests.h"
#include "NameServerRecordTests.h"
#include "StartOfAuthorityRecordTests.h"
#include "AddressRecordTests.h"
#include "HeaderMessageSectionTests.h"
#include "QuestionMessageSectionEntryTests.h"
#include "QuestionMessageSectionTests.h"
#include "AnswerMessageSectionTests.h"
#include "AuthorityMessageSectionTests.h"
#include "AdditionalMessageSectionTests.h"
#include "QueryTests.h"
#include "ResponseTests.h"
#include "ZoneFileTests.h"
#include "Ishiko/TestFramework/TestFrameworkCore.h"
#include <boost/filesystem/operations.hpp>

int main(int argc, char* argv[])
{
    Ishiko::TestFramework::TestHarness theTestHarness("IshikoDNSCore");

    theTestHarness.environment().setTestDataDirectory("../../TestData");
    theTestHarness.environment().setTestOutputDirectory("../../TestOutput");
    boost::filesystem::create_directories("../../TestOutput");
    theTestHarness.environment().setReferenceDataDirectory("../../ReferenceData");

    AddDomainNameTests(theTestHarness);
    AddTTLTests(theTestHarness);
    AddNameServerRecordTests(theTestHarness);
    AddStartOfAuthorityRecordTests(theTestHarness);
    AddAddressRecordTests(theTestHarness);
    AddHeaderMessageSectionTests(theTestHarness);
    AddQuestionMessageSectionEntryTests(theTestHarness);
    AddQuestionMessageSectionTests(theTestHarness);
    AddAnswerMessageSectionTests(theTestHarness);
    AddAuthorityMessageSectionTests(theTestHarness);
    AddAdditionalMessageSectionTests(theTestHarness);
    AddQueryTests(theTestHarness);
    AddResponseTests(theTestHarness);
    AddZoneFileTests(theTestHarness);

    return theTestHarness.run();
}
