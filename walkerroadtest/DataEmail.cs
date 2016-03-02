using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KaseyaEpicorValidationVerification
{
    public class DataEmail
    {

        public const string email1 = @"From: Support_Desk [mailto:kaseya@rgsupport.com] 
Sent: Thursday, January 28, 2016 10:44 AM
To: Test Test <test@company.com>
Subject: FOH006410 Closed

Ticket FOH006410 has now been closed. If this issue has not been resolved to your satisfaction, please contact us so we can further assist you with this issue.
 
Summary: Test Ticket Submitted
Created on: 1/28/2016 10:20:54 AM 
Status: Closed
Organization: test-passport
Machine Id: osi-laptop.test-passport
Submitter Name: Test Test 
Submitter Email: test@company.com 
Assignee: rfunk  
 
Notes:The station was not able to communicate over the Internet.  We rebooted the cell modem and communications were restored.
________________________________________

Updated ticket fields within kaseya.
________________________________________

";

        public const string email2 = @"hqsmail2.company.com (10.33.140.22) with Microsoft SMTP Server (TLS) id
 15.0.1076.9 via Mailbox Transport; Tue, 1 Mar 2016 10:36:50 -0800
Received: from hqsmail2.company.com (11.56.89.56) by
 hqsmail1.company.com (10.45.67.21) with Microsoft SMTP Server (TLS) id
 15.0.1076.9; Tue, 1 Mar 2016 10:36:50 -0800
Received: from hqsmail2.company.com ([fe80::99e:ec93:2797:faf0]) by
 hqsmail2.company.com ([fe80::99e:ec93:2797:faf0%12]) with mapi id
 15.00.1076.000; Tue, 1 Mar 2016 10:36:50 -0800
From: Ben Gonzales <bgonzales@company.com>
To: Test <test@company.com>
Subject: FOH006411 Closed
Thread-Topic: FOH006411 Closed
Thread-Index: AdFz6UxAK2q7GYo6TJK9Mb4/OhWD1Q==
Date: Tue, 1 Mar 2016 10:36:49 -0800
Message-ID: <10568c37818148668ab8e1e7e8ff6ddb@hqsmail2.company.com>
Accept-Language: en-US
Content-Language: en-US
X-MS-Exchange-Organization-AuthAs: Internal
X-MS-Exchange-Organization-AuthMechanism: 04
X-MS-Exchange-Organization-AuthSource: hqsmail2.company.com
X-MS-Has-Attach:
X-MS-Exchange-Organization-SCL: -1
X-MS-TNEF-Correlator:
Content-Type: text/plain; charset=""us-ascii""
MIME-Version: 1.0

Ticket FOH006411 has now been closed. If thReceived: from hqsmail1.company.com (10.33.140.21) by
is issue has not been resolved to your satisfaction, please contact us so we can further assist you with this issue.

Summary: Kaseya Ticket Submitted
Created on: 1/28/2016 10:20:54 AM
EpicorCaseNum: 21355
Status: Closed
Organization: company-passport
Machine Id: osi-laptop.company-passport
Submitter Name: Test2 User2
Submitter Email: testuser2@company.com
Assignee: rfunk

Notes:The station was not able to communicate over the Internet.  We rebooted the cell modem and communications were restored.
________________________________________

Updated ticket fields within kaseya.
________________________________________";

    }
}
