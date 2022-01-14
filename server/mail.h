#ifndef MAIL_H
#define MAIL_H

#define EMAIL_ADDR_SIZE 30
#define TOPIC_SIZE 50
#define TEXT_SIZE 1500

typedef struct Mail{
  char from[EMAIL_ADDR_SIZE];  //email address or ip
  char to[EMAIL_ADDR_SIZE];
  char topic[TOPIC_SIZE];
  char text[TEXT_SIZE];
} Mail;


typedef struct Feedback{
  int feedback;
  char message[50];
} Feedback;


char* get_recipient(Mail* mail){
  char* recipient;
  recipient = strdup(mail->to);
  recipient = strsep(&recipient, "@");

  return recipient;
}

#endif // MAIL_H
