#include <mail/SmtpClient.h>
#include <mail/Credentials.h>
#include <mail/Message.h>
#include <iostream>

int main(int argc, char const* argv[])
{
	try {
		mail::SmtpClient mailer("smtp.mail.foo.fr", 587);
		mail::Message message;
		message.from = "toto@foo.fr";
		message.to = { "tata.titi@bar.com", "tutu.titi@bar.com" };
		message.subject = "Webmail";
		message.body = R"!-!(Bonjour,

Cordialement,

Toto)!-!";
		message.attachments = { mail::Attachment{"D:/Somewhere/On/The/Filesystem/foo.txt"} };
		mailer.send(message, { "toto@foo.fr", "myPassword" } );
	}
	catch (std::exception const& e) {
		std::cerr << e.what() << std::endl;
	}
}