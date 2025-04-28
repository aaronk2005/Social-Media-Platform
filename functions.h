// functions.h
#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#define MAX_LENGTH 49
#define MAX_USERS 10000
#define MAX_MESSAGE_LENGTH 256
#define MAX_POST_LENGTH 256

typedef struct User User;
typedef struct Friend Friend;
typedef struct Message Message;
typedef struct Chat Chat;
typedef struct Post Post;

struct Friend {
    User* user;
    Friend* next;
};

struct Message {
    unsigned int messageID;
    char content[MAX_MESSAGE_LENGTH];
    User* sender;
    User* receiver;
    Message* next;
};

struct Chat {
    User* otherUser;
    Message* chatHead;
    Message* chatTail;
    int numMessages;
    Chat* next;
};
typedef struct Post {
    unsigned int postID;
    char content[MAX_POST_LENGTH];
    struct User *user;
    int likes;
    struct User **likedBy;
    struct Post *next;
} Post;


struct User {
    char name[MAX_LENGTH];
    char email[MAX_LENGTH];
    User* next;
    Friend* friends;
    Chat* chats;
    Post* posts;
};

static unsigned int idMessageCounter = 0;
static unsigned int idPostCounter = 0;
extern User* userHashTable[MAX_USERS];

User* create_user(const char* name, const char* email); // int user_id is auto-generated to be unique
void add_friend(User* user1, User* user2); // users user1 and user2 are now friends
void delete_friend(User* user1, User* user2); // users user1 and user2 are no longer friends
void delete_user(User* user); // user is deleted
void print_users(); // prints all user names in ascending order
void change_user_name(User* user, char* new_name);
void change_user_email(User* user, char* new_email);
void print_friends(User* user); // prints user's friends in ascending order
User* search_user_by_name(const char* name);
User* search_user_by_email(const char* email);
User** mutual_friends(User* user1, User* user2); // returns an array of pointers to the mutual friends
void print_mutual_friends(User** friends); // prints mutual friends' user names in acsending order
Message* create_message(User* sender, User* receiver, const char* content); // int message_id is auto-generated to be unique
void print_message(Message* message);
void display_chat(User* user1, User* user2); // print messages in FIFO
Post* new_post(User* user, const char* content); // post id is auto-generated to be unique
void add_like(Post* post, User* user); // user is the individual who liked the post
void display_feed(User* user1);
#endif
