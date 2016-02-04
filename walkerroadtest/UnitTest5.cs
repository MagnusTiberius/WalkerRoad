using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;

using util_email;

namespace walkerroadtest
{
    [TestClass]
    public class UnitTest5
    {
        [TestMethod]
        public void TestMethod1()
        {
            //SureIdEmail.Login("sys_k2e@exchange.eidpassport.com", "test");

            SureIdEmail.Connect();
        }
    }
}
