using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Net;
using System.Net.Security;
using System.Security.Cryptography.X509Certificates;

using System.Net.Mail;

using Microsoft.Exchange.WebServices.Data;

namespace util_email
{
    public class SureIdEmail
    {
        static private readonly ExchangeService service = new ExchangeService(ExchangeVersion.Exchange2007_SP1);

        static public void Login(string userId, string password)
        {
            service.Credentials = new WebCredentials(userId, password);
            service.UseDefaultCredentials = true;
            service.AutodiscoverUrl("sys_k2e@exchange.eidpassport.com", RedirectionCallback);
            var res = service.Url;
            EmailMessage email = new EmailMessage(service);
            email.ToRecipients.Add("user1@contoso.com ");
            email.Subject = "HelloWorld";
            email.Body = new MessageBody("This is the first email I've sent by using the EWS Managed API");
            //email.Send();
        }

        static bool RedirectionCallback(string url)
        {
            // Return true if the URL is an HTTPS URL.
            return url.ToLower().StartsWith("https://");
        }

        static public void Connect()
        {
            System.Net.Mail.SmtpClient smtp = new System.Net.Mail.SmtpClient();
            smtp.Host = "smtp.eidpassport.com";
            smtp.DeliveryMethod = SmtpDeliveryMethod.Network;
            smtp.UseDefaultCredentials = false;
            //smtp.Credentials = new System.Net.NetworkCredential("sys_k2e@exchange.eidpassport.com", "p0)c&E23@n91=p");
            smtp.Credentials = new System.Net.NetworkCredential("bgonzales@exchange.eidpassport.com", "RapidgateRocks!");
            MailMessage mail = new MailMessage("bgonzales@exchange.eidpassport.com", "bgonzales@exchange.eidpassport.com");
            mail.Subject = "this is a test email.";
            mail.Body = "this is my test email body";
            smtp.Send(mail);
        }

    }
}
