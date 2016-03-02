using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Text;
using System.Collections.Generic;

using jacobsenroad;

namespace KaseyaEpicorValidationVerification
{
    [TestClass]
    public class PopControllerTest
    {
        private struct attr
        {
            public string name;
            public string value;
        }

        private List<attr> props = new List<attr>();

        [TestMethod]
        public void TestKaseyaEpicorController()
        {
            PopClientController controller;
            controller = new PopClientController();
            controller.Run();
        }

        [TestMethod]
        public void TestScanner_AcceptRun1()
        {
            Scanner scanner = new Scanner();

            scanner.Input(DataEmail.email2);

            List<byte> token;
            string tok1;
            attr atr;

            token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes(".0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));
            tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
            if(tok1.Equals("hqsmail2.eidpassport.com"))
            {
                token = scanner.AcceptUntil(ASCIIEncoding.ASCII.GetBytes("\r\n"));
                tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());

                bool found = false;
                while (!found) 
                {
                    scanner.SkipEmpty();

                    token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));
                    tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());

                    if (tok1.Equals("From"))
                    {
                        found = true;
                    }
                    else
                    {
                        token = scanner.AcceptUntil(ASCIIEncoding.ASCII.GetBytes("\r\n"));
                        string tok2 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                    }
                }

                token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes(":"));
                tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                if (tok1.Equals(":"))
                {
                    scanner.SkipEmpty();
                    token = scanner.AcceptUntil(ASCIIEncoding.ASCII.GetBytes("<"));
                    tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                    scanner.SkipEmpty();
                    token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes("<"));
                    tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                    scanner.SkipEmpty();
                    token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes("@.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));
                    tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                    scanner.SkipEmpty();
                    token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes(">"));
                    tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                    scanner.SkipEmpty();
                }
                bool inLoop = true;
                while (inLoop)
                {
                    scanner.SkipEmpty();
                    token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes("-ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz "));
                    tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                    if (tok1.Equals("To"))
                    {
                        atr = new attr();
                        string tokenName = tok1;
                        atr.name = tokenName;
                        scanner.SkipEmpty();
                        token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes(":"));
                        tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                        scanner.SkipEmpty();
                        token = scanner.AcceptUntil(ASCIIEncoding.ASCII.GetBytes("<"));
                        tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                        scanner.SkipEmpty();
                        token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes("<"));
                        tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                        scanner.SkipEmpty();
                        token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes("@.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));
                        tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                        atr.value = tok1;
                        props.Add(atr);
                        scanner.SkipEmpty();
                        token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes(">"));
                        tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                        scanner.SkipEmpty();
                    }
                    else if (tok1.Equals("Subject"))
                    {
                        atr = new attr();
                        string tokenName = tok1;
                        atr.name = tokenName;
                        scanner.SkipEmpty();
                        token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes(":"));
                        tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                        scanner.SkipEmpty();
                        token = scanner.AcceptUntil(ASCIIEncoding.ASCII.GetBytes("\r\n"));
                        tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                        atr.value = tok1;
                        props.Add(atr);
                        scanner.SkipEmpty();
                    }
                    else if (tok1.Equals("MIME-Version"))
                    {
                        atr = new attr();
                        string tokenName = tok1;
                        atr.name = tokenName;
                        scanner.SkipEmpty();
                        token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes(":"));
                        tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                        scanner.SkipEmpty();
                        token = scanner.AcceptUntil(ASCIIEncoding.ASCII.GetBytes("\r\n"));
                        tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                        atr.value = tok1;
                        props.Add(atr);
                        scanner.SkipEmpty();
                        //inLoop = false;
                    }
                    else if (tok1.Equals("Content-Type"))
                    {
                        atr = new attr();
                        string tokenName = tok1;
                        atr.name = tokenName;
                        scanner.SkipEmpty();
                        token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes(":"));
                        tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                        scanner.SkipEmpty();
                        token = scanner.AcceptUntil(ASCIIEncoding.ASCII.GetBytes("\r\n"));
                        tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                        atr.value = tok1;
                        props.Add(atr);
                        scanner.SkipEmpty();
                    }
                    else if (tok1.Length == 0)
                    {
                        inLoop = false;
                    }
                    else
                    {
                        atr = new attr();
                        string tokenName = tok1;
                        atr.name = tokenName;
                        scanner.SkipEmpty();
                        token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes(":"));
                        tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                        scanner.SkipEmpty();
                        token = scanner.AcceptUntil(ASCIIEncoding.ASCII.GetBytes("\r\n"));
                        string tokenValue = ASCIIEncoding.ASCII.GetString(token.ToArray());
                        atr.value = tokenValue;
                        scanner.SkipEmpty();
                        props.Add(atr);
                    }
                }
            }

        }

        [TestMethod]
        public void TestScanner_AcceptRun2()
        {
            Scanner scanner = new Scanner();

            scanner.Input(DataEmail.email1);

            List<byte> token;
            string tok1;
            attr atr;

            for (int i = 0; i < 4; i++)
            {
                atr = new attr();

                token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));
                tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());

                atr.name = tok1;

                scanner.SkipEmpty();
                token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes(":"));
                tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());

                scanner.SkipEmpty();
                token = scanner.AcceptUntil(ASCIIEncoding.ASCII.GetBytes("\r\n"));
                tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                atr.value = tok1;

                props.Add(atr);

                token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes("\r"));
                tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes("\n"));
                tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                scanner.SkipEmpty();
            }
            token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes("\r"));
            tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
            token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes("\n"));
            tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
            //token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,;_$% "));
            token = scanner.AcceptUntil(ASCIIEncoding.ASCII.GetBytes("\r\n"));
            tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
            scanner.SkipEmpty();

            //------------------

            for (int i = 0; i < 9; i++)
            {
                atr = new attr();
                token = scanner.AcceptUntil(ASCIIEncoding.ASCII.GetBytes(":"));
                tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());

                atr.name = tok1;

                scanner.SkipEmpty();
                token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes(":"));
                tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());

                scanner.SkipEmpty();
                token = scanner.AcceptUntil(ASCIIEncoding.ASCII.GetBytes("\r\n"));
                tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                atr.value = tok1;

                props.Add(atr);
                scanner.SkipEmpty();
            }
        }
    }
}
