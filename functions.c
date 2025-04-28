#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

// Utility function to test parser
void testingParser(int arg1, char *arg2) {
    printf("The parser was called with arguments: %d and %s\n", arg1, arg2);
}

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
void print_mutual_friends(User** friends); // prints mutual friends' user_names in acsending order
Message* create_message(User* sender, User* receiver, const char* content); // int message_id is auto-generated to be unique
void print_message(Message* message);
void display_chat(User* user1, User* user2); // print messages in FIFO
Post* new_post(User* user, const char* content); // post id is auto-generated to be unique
void add_like(Post* post, User* user); // user is the individual who liked the post
void display_feed(User* user1);

// initialized variables from functions.h
User* userHashTable[MAX_USERS] = {NULL};

/*
References used:
Lecture slides
https://www.w3schools.com/c/c_structs.php
https://www.geeksforgeeks.org/bubble-sort-algorithm/
https://www.geeksforgeeks.org/hash-table-data-structure/
https://www.youtube.com/watch?v=2Ti5yvumFTU&t=686s&ab_channel=JacobSorber
https://www.programiz.com/dsa/linked-list
https://www.youtube.com/watch?v=VOpjAHCee7c&t=757s&ab_channel=JacobSorber
https://www.geeksforgeeks.org/traversal-of-singly-linked-list/
https://www.geeksforgeeks.org/queue-linked-list-implementation/
https://www.geeksforgeeks.org/message-queues-system-design/
*/

//Hashing function to create a unique ID for each user and everytime the hashing function is used
unsigned int idGeneratorFunction(const char* name) {

    int length = strlen(name); //Use the length of the name to add to randomness
    unsigned int hashVal = 5731; //start with a prime number to increase randomness

    //Use more prime numbers and operations to make the hash value random
    for (int i = 0; i < length; i++) {
        hashVal = hashVal * 79 + hashVal + name[i];
        hashVal *= name[i] + 31 * hashVal;
        //Use the Modulus to make sure id isn't higher than 100000
        hashVal = hashVal % MAX_USERS;
    }
    return hashVal;
}


//Function that creates a user by adding it to an open hash table
User* create_user(const char* name, const char* email) {

    //Use the idGeneratorFunction to get a randomly generated id for the user
    unsigned int id= idGeneratorFunction(name);

    //dynamically allocate memory for the new User
    User* newUser = (User*)malloc(sizeof(User));

    //copy the name and email to the User
    strcpy(newUser->name,name);
    strcpy(newUser->email,email);

    //make the User assigned to the head of its bucket in its buckets linked list to prevent collsions
    newUser->next = userHashTable[id];
    userHashTable[id] = newUser;

    //return the new user
    return newUser;
}


//Function that adds a friend to a user
void add_friend(User* user1, User* user2) {

    //traverse user1's linked list of friends to see if user2 is already in the list or not
    Friend* current = user1->friends;
    while (current != NULL) {
        //check to see if user1 and user2 are already friends
        if (current->user == user2) {
            break;
        }
        current = current->next; //move to the next node
    }

    //if user2 isn't found as a friend then create a new friend set to the head of the friends list for user1
    if (current == NULL) {
        //allocate memory to friend node
        Friend* newFriend = (Friend*)malloc(sizeof(Friend));

        // make the new friend node point to user2
        newFriend->user = user2;

        //link the friend node to be a part of user1's friends list
        newFriend->next = user1->friends;

        //make user1's friends list include this new node
        user1->friends = newFriend;
    }

    //traverse user2's linked list of friends to see if user1 is already in the list or not
    current = user2->friends;
    while (current != NULL) {
        //check to see if user1 and user2 are already friends
        if (current->user == user1) {
            break;
        }
        current = current->next; //move to the next node
    }

    //allocate memory to friend node
    Friend* newFriend = (Friend*)malloc(sizeof(Friend));

    // make the new friend node point to user1
    newFriend->user = user1;

    //link the friend node to be a part of user2's friends list
    newFriend->next = user2->friends;

    //make user2's friends list include this new node
    user2->friends = newFriend;

}




//function that deletes a friend from the friends linked list
void delete_friend(User* user1, User* user2) {

    // assign the curr pointer to the head of user1's friends list
    Friend *curr = user1->friends;

    //assign the prev pointer to the previous node to help while traversing the friends list
    Friend *prev = NULL;

    //traverse user1's friends list
    while (curr != NULL) {
        //check if the current friend in the list is user2
        if (curr->user == user2) {
            //check to see if previous node is NULL, meaning curr is the first node in the friends list
            if (prev == NULL) {
                //set the friends pointer of user1 to point to the next node
                user1->friends = curr->next;
            }
            else {
                //change the next pointer to the previous node and skip over the current node
                prev->next = curr->next;
            }
            //free the memory of the node since it is removed from the list
            free(curr);
            break;
        }
        //move to the next node by updating curr and prev
        prev = curr;
        curr = curr->next;
    }

    //update the curr and prev pointers to be associated with user2
    curr = user2->friends;
    prev = NULL;

    //traverse user2's friends list
    while (curr != NULL) {
        //check to see if previous node is NULL, meaning curr is the first node in the friends list
        if (curr->user == user1) {
            //set the friends pointer of user2 to point to the next node
            if (prev == NULL) {

                user2->friends = curr->next;
            }
            else {
                //change the next pointer to the previous node and skip over the current node
                prev->next = curr->next;
            }
            //free the memory of the node since it is removed from the list
            free(curr);
            break;
        }
        //move to the next node by updating curr and prev
        prev = curr;
        curr = curr->next;
    }
}


//function that deletes a user from the open hash table
void delete_user(User* user) {
    // loop through all buckets in the hash table
    for (int i = 0; i < MAX_USERS; i++) {
        User* currUser = userHashTable[i];
        //check when the user isn't NULL
        while (currUser != NULL) {
            //delete the current users friends
            delete_friend(currUser, user);

            //move to the next node
            currUser = currUser->next;
        }
    }

    // free the memory of each of the user's friends
    Friend* currFriend = user->friends;
    //loop to iterate through the user's friends
    while (currFriend != NULL) {
        //store the current friend in temp
        Friend* temp = currFriend;

        //move the current friend to the next node
        currFriend = currFriend->next;

        //free the memory of the friend
        free(temp);
    }


    //first find the hash of the user to find their id
    unsigned int id = idGeneratorFunction(user->name);

    //make a curr pointer to traverse the linked list
    User* curr = userHashTable[id];

    //create a prev pointer to keep track of the previous node
    User* prev = NULL;

    //traverse the list to find the user
    while (curr != NULL) {
        //check to see if the curr is the same as the user
        if (curr == user) {
            //this means that the user is at the head of the linked list
            if (prev == NULL) {
                userHashTable[id] = curr->next;
            }
                //if not head then just bypass the current node
            else {
                prev->next = curr->next;
            }
            //free the memory of the user
            free(user);
            return;
        }
        //move to the next node in the list
        prev = curr;
        curr = curr->next;
    }
}


//function that prints out the names of all the users in alphabetical order
void print_users() {
    //make a temporary array to store the names of user's
    char* nameList[MAX_USERS];
    //counter to keep track of user count
    int counter = 0;

    //traverse the hash table to add the user's to the list
    for (int i = 0; i < MAX_USERS; i++) {
        //make a curr pointer to iterate through users in each bucket
        User* curr = userHashTable[i];
        //when the user is found, store it in the list and increment the counter
        while (curr != NULL) {
            nameList[counter++] = curr->name;
            curr = curr->next;
        }
    }

    //return if the counter is 0, because that means there are no users
    if (counter == 0) {
        return;
    }

    //use bubble sort to sort the user's names in alphabetical order
    for (int i = 0; i < counter - 1; i++) {
        for (int j = 0; j < counter - i - 1; j++) {
            if (strcmp(nameList[j], nameList[j + 1]) > 0) {
                char* temp = nameList[j];
                nameList[j] = nameList[j + 1];
                nameList[j + 1] = temp;
            }
        }
    }

    // print out the names of the users
    for (int i = 0; i < counter; i++) {
        printf("%s", nameList[i]);
        if (i < counter - 1) {
            printf(",");
        }
    }
    printf("\n");
}


//function that lets a user change their user name
void change_user_name(User* user, char* new_name) {

    //find the original id of the user
    unsigned int originalID = idGeneratorFunction(user->name);

    //set up curr and prev pointers to traverse the linked list
    User* curr = userHashTable[originalID];
    User* prev = NULL;

    //traverse the linked list in the buckets in the hash table
    while (curr != NULL) {
        //check if the wanted user is found
        if (curr == user) {
            //check if the is user is at the head of the bucket
            if (prev == NULL) {
                //move the head to the next node in the bucket
                userHashTable[originalID] = curr->next;
            }
                //otherwise bypass curr and link the pervious node
            else {
                prev->next = curr->next;
            }
            break;//exit the loop since the user has already been removed from their bucket
        }

        //move to the next node
        prev = curr;
        curr = curr->next;
    }

    // copy the user's new name in the name part of the user's structure
    strncpy(user->name, new_name, MAX_LENGTH - 1);//ensure there is no overflow
    //make the string end with the null-terminate
    user->name[MAX_LENGTH - 1] = '\0';

    // calculate the new id for the user using the hash function
    unsigned int newID = idGeneratorFunction(user->name);

    //put the new user's name into their new bucket in the hash table
    user->next = userHashTable[newID];
    userHashTable[newID] = user;
}


//function to change the user's email
void change_user_email(User* user, char* new_email) {

    //copy the user's new email into the user
    strcpy(user->email, new_email);
    //prevent overflow and add the null-terminate to the new email string
    user->email[MAX_LENGTH - 1] = '\0';

}


//function to print out all the user's friends
void print_friends(User* user) {

    //make a curr pointer for the head of the user's friends linked list
    Friend* currFriend = user->friends;
    //make a counter to keep track of the user's friends
    int counter = 0;
    //make a pointer array to store the names of all the user's friends
    char* friendsList[MAX_USERS];

    //traverse the linked list of the user's friends
    while (currFriend != NULL) {
        //increment the counter and store the friends name in the array
        friendsList[counter++] = currFriend->user->name;
        //go to the next friend
        currFriend = currFriend->next;
    }

    //if the counter is 0, then there are no friends
    if (counter == 0) {
        return;//return since no friends needed to print out
    }

    // use bubble sort to sort the friends names in alphabetical order
    for (int i = 0; i < counter - 1; i++) {
        for (int j = 0; j < counter - i - 1; j++) {
            if (strcmp(friendsList[j], friendsList[j + 1]) > 0) {
                char* temp = friendsList[j];
                friendsList[j] = friendsList[j + 1];
                friendsList[j + 1] = temp;
            }
        }
    }

    // print out the names of all the users
    for (int i = 0; i < counter; i++) {
        printf("%s", friendsList[i]);
        if (i < counter - 1) {
            printf(",");
        }
    }
    printf("\n");
}


//function to search for and find the user's name
User* search_user_by_name(const char* name) {
    //get the user's id by using the hash function
    unsigned int userID = idGeneratorFunction(name);

    //set the head of the user id hash bucket to currUser
    User* currUser = userHashTable[userID];

    //traverse the linked list to find the user
    while(currUser!=NULL) {
        //check to see if the user's name in the bucket is the same as the name entered
        if(strcmp(currUser->name, name)==0) {
            return currUser;
        }
        currUser = currUser->next;//move to the next node
    }

    return NULL;//return NULL since no user found

}

//function to find the user's email
User* search_user_by_email(const char* email) {

    //iterate through the buckets to check every email
    for (int i = 0; i < MAX_USERS; i++) {
        //make a curr pointer to get the head of the linked list
        User* currUser = userHashTable[i];

        //traverse the linked list
        while (currUser != NULL) {
            //check to see if the user's email is the same as the one at the specific index
            if (strcmp(currUser->email, email) == 0) {
                return currUser;//return the user since their email was found
            }
            currUser = currUser->next;//move to the next node
        }
    }
    return NULL;// return NULL since the email wasn't found
}


//function to determine if two users are mutual friends
User** mutual_friends(User* user1, User* user2) {

    // set the max amount of friends to the amount of user's -1
    int maxFriends = (MAX_USERS-1);
    //make an array of pointers to store user1's friends
    User* user1Friends[maxFriends];
    //make a counter to keep track of user1's friends
    int counter1 = 0;

    //make curr pointer to point to the head of the linked list
    Friend* currFriend = user1->friends;

    //traverse the linked list to get all of user1's friends
    while (currFriend != NULL) {
        //when a friend is found increase the counter and add it to the array
        user1Friends[counter1++] = currFriend->user;
        //move to the next friend in the linked list
        currFriend = currFriend->next;
    }

    // allocate memory for the  mutual friends array
    User** mutualFriendsArray = (User**)malloc(sizeof(User*) * counter1);
    //make a counter to keep track of the amount of mutual friends
    int mutualsCounter = 0;

    // traverse user2's friends to see if any of them are the same as user1's friends
    currFriend = user2->friends;
    while (currFriend != NULL) {

        for (int i = 0; i < counter1; i++) {
            //check to see if they have mutuals
            if (currFriend->user == user1Friends[i]) {
                //increment the mutualsCounter and add this to the arrau
                mutualFriendsArray[mutualsCounter++] = currFriend->user;
                break;
            }
        }
        currFriend = currFriend->next;//move to the next node
    }

    // reallocate the memory of the mutuals array based off of the mutuals counter
    mutualFriendsArray = (User**)realloc(mutualFriendsArray, sizeof(User*) * (mutualsCounter + 1));

    //set the last element of the array to NULL
    mutualFriendsArray[mutualsCounter] = NULL;

    //return the mutual friends array
    return mutualFriendsArray;
}



//function to print the mutual friends in alphebetical order
void print_mutual_friends(User** friends) {

    // Count the number of mutual friends
    int counter= 0;

    //find the amount of mutual friends
    while (friends[counter] != NULL) {
        counter++;
    }

    //if the counter is 0, it means no mutual friends were found
    if (counter == 0) {
        return;// return since not mutual friends were found
    }

    // allocate memory to store the names of the mutual friends
    char** names = (char**)malloc(counter * sizeof(char*));

    //copy the names from the user's array
    for (int i = 0; i < counter; i++) {
        names[i] = friends[i]->name;
    }

    // use bubble sort to sort the names in alphabetical order
    for (int i = 0; i < counter - 1; i++) {
        for (int j = 0; j < counter - i - 1; j++) {
            if (strcmp(names[j], names[j + 1]) > 0) {
                char* temp = names[j];
                names[j] = names[j + 1];
                names[j + 1] = temp;
            }
        }
    }

    // print out all the names of all the mutual friends
    for (int i = 0; i < counter; i++) {
        printf("%s", names[i]);
        if (i < counter - 1) {
            printf(",");
        }
    }
    printf("\n");

}


//helper function to add a message to the chat between two users
void addMessage(Chat* chat, Message* message) {

    //move current messages to the end of the queue
    if (chat->chatTail != NULL) {
        chat->chatTail->next = message;

    }
    //if the queue is empty then make the message the head
    else {
        chat->chatHead = message;
    }

    //make the tail pointer for the new message
    chat->chatTail = message;

    //increase the number of messages counter
    chat->numMessages++;

    // traverse the queue to make sure there aren't more than 50 recent messages
    while (chat->numMessages > 50) {

        // make pointer to the head of the queue which is the oldest message
        Message* lastMessage = chat->chatHead;
        //move the last message to the next message in the queue
        chat->chatHead = lastMessage->next;

        //free the last message and decrease the message counter
        free(lastMessage);
        chat->numMessages--;
    }
}


//helper function to make a chat or look for the pre-existing chat
Chat* makeChat(User* user, User* otherUser) {
    //make a current pointer to initialize the beginning of the user's chat list
    Chat* currChat = user->chats;

    //allocate memory fot the chat
    Chat* newChat = (Chat*)malloc(sizeof(Chat));

    //link the new chat to the other user
    newChat->otherUser = otherUser;
    //make the chat head and tail NULL since no messages have been added yet
    newChat->chatHead = NULL;
    newChat->chatTail = NULL;

    //make the number of messages zero since none have been added yet
    newChat->numMessages = 0;

    //make the chat set to the beginning of the user's chat list
    newChat->next = user->chats;
    //make the head of the chat list set to the head
    user->chats = newChat;
    //return the pointer to the new chat
    return newChat;
}


//helper function that checks to see if the sender and receiver are friends
int areFriends(User* user1, User* user2) {
    //make a curr pointer to point to the head of user1's head
    Friend* curr = user1->friends;

    //traverse user1's friends to see if user2 is their
    while (curr != NULL) {
        if (curr->user == user2) {
            return 1;//return 1 meaning they are friends
        }
        curr = curr->next;//move to the next node
    }
    return 0;//return 0 meaning they are not friends
}

//function to find if the chat exists
Chat* findChat(User* user, User* otherUser) {
    //make a curr pointer to point to the head of the user's linked list
    Chat* currChat = user->chats;

    //traverse the linked list of chats
    while (currChat != NULL) {
        //check that th curr chat involves the other user
        if (currChat->otherUser == otherUser) {
            return currChat;//return the currChat since it was found
        }
        currChat = currChat->next;//move to the next node
    }
    return NULL;//return NULL since the chat was not found
}


//function makes a message between a sender to a receiver that makes sure it's added to both of their chat lists
Message* create_message(User* sender, User* receiver, const char* content) {

    // use the arFriends helper function to check if the sender and receiver are friends
    if (!areFriends(sender, receiver)) {
        return NULL;//return NULL if they aren't friends
    }

    // check if the message is greater than the maximum length
    if (strlen(content) >= MAX_MESSAGE_LENGTH) {
        return NULL;// return NULL because can't send message that exceeds the character limit
    }

    // create chat for sender
    Chat* senderChat = findChat(sender, receiver);

    //make chat for sender if it does not exist by calling the makeChat helper function
    if (senderChat == NULL) {
        senderChat = makeChat(sender, receiver);
    }
    // create chat for receiver
    Chat* receiverChat = findChat(receiver, sender);
    //make chat for receiver if it does not exist by calling the makeChat helper function
    if (receiverChat == NULL) {
        receiverChat = makeChat(receiver, sender);
    }

    // allocate memory to create the new message
    Message* newMessage = (Message*)malloc(sizeof(Message));

    // use a counter to make a unique id for the message
    newMessage->messageID = ++idMessageCounter;

    // add the content into the new message
    strncpy(newMessage->content, content, MAX_MESSAGE_LENGTH - 1);
    //add the NULL terminator to the end of the mnessage to prevent overflow
    newMessage->content[MAX_MESSAGE_LENGTH - 1] = '\0';
    //assign the sender and the receiver to the new message
    newMessage->sender = sender;
    newMessage->receiver = receiver;
    //make the next message in the queue set to NULL
    newMessage->next = NULL;

    // add the message to both the sender and receivers chats using the addMessage helper function
    addMessage(senderChat, newMessage);
    addMessage(receiverChat, newMessage);

    //return the new message
    return newMessage;
}

//function used to print out the message that was crated
void print_message(Message* message) {
    //check to see if the message is empty (NULL)
    if (message == NULL) {
        return;//return since the message has no content
    }
    //print out the message in the right format
    printf("[%s]: %s\n", message->sender->name, message->content);

}


//function that shows all messages displayed for 2 users
void display_chat(User* user1, User* user2) {
    //use the findChat helper function to verify a chat exists between the 2 users
    Chat* chat = findChat(user1, user2);
    //check to see if the chat has no  messages
    if (chat == NULL || chat->chatHead == NULL) {
        return;
    }
    //pointer to the first message which is the chatHead
    Message* current = chat->chatHead;

    //helps make sure there is proper comma formatting
    int commaCounter = 1;

    //traverse the queue and print out all messages in the chat
    while (current != NULL) {
        //checks to see if it is the first message so that no comma is printed in front of it
        if (!commaCounter) {
            printf(",");
        }
        //print out the messages from the users
        printf("[%s:]", current->sender->name);
        printf("%s", current->content);

        //reduce the comma counter making it not the first message
        commaCounter = 0;
        current = current->next;//move to the next node
    }
    printf("\n");

}

//function that creates a post and store's it in the user's post linked list
Post* new_post(User* user, const char* content) {

    //  make sure the post isn't greater than 256 characters
    if (strlen(content) >= MAX_POST_LENGTH) {
        return NULL; //return NULL since the post is too big
    }
    // allocate memory to the post
    Post *newPost = (Post *)malloc(sizeof(Post));

    //assign the id to the post
    newPost->postID = ++idPostCounter;

    //copy the content into the post
    strncpy(newPost->content, content, MAX_POST_LENGTH - 1);
    //set the last character to the null-terminate
    newPost->content[MAX_POST_LENGTH - 1] = '\0';

    //link the post to the user
    newPost->user = user;
    //set the likes to 0
    newPost->likes = 0;
    newPost->likedBy = NULL;
    //add the post to the post list
    newPost->next = user->posts;
    user->posts = newPost;

    //return the post to the user
    return newPost;
}


//function that adds likes to the posts
void add_like(Post* post, User* user) {

    // check that the user is friends with the user
    if (!areFriends(user, post->user)) {
        return;//return since the user is not the user or is not friends with the user
    }

    //check if the user liked the post or not already
    for (int i = 0; i < post->likes; i++) {
        //check if the post is liked by the user at this index
        if (post->likedBy[i] == user) {
            return;//return since they already liked the post
        }
    }

    // add the user to the likedBy array
    post->likedBy = (User **)realloc(post->likedBy, sizeof(User *) * (post->likes + 1));

    //update the count for likes
    post->likedBy[post->likes] = user;
    post->likes++;

}

//function that displays the feed of posts
void display_feed(User* user1) {

    //pointer array to store all the posts
    Post *totalPosts[MAX_USERS];
    //counter to see how many posts there are
    int postCount = 0;

    //curr pointer for the head of the post linked list
    Post *currPost = user1->posts;
    //traverse the linked list and add each post ot the totalposts array
    while (currPost != NULL && postCount < MAX_USERS) {
        totalPosts[postCount++] = currPost;
        currPost = currPost->next;//move to the next node
    }

    //now make a curr pointer for the friend
    Friend *currFriend = user1->friends;
    //traverse the linked list to add the friends posts to the totalposts array
    while (currFriend != NULL) {
        currPost = currFriend->user->posts;
        while (currPost != NULL && postCount < MAX_USERS) {
            totalPosts[postCount++] = currPost;
            currPost = currPost->next;
        }
        currFriend = currFriend->next;//move to the next node
    }

    //sort the totalposts based on likes and order of posts using bubble sort
    for (int i = 0; i < postCount - 1; i++) {
        for (int j = 0; j < postCount - i - 1; j++) {
            if (totalPosts[j]->likes < totalPosts[j + 1]->likes ||
                (totalPosts[j]->likes == totalPosts[j + 1]->likes && totalPosts[j]->postID > totalPosts[j + 1]->postID)) {
                Post *temp = totalPosts[j];
                totalPosts[j] = totalPosts[j + 1];
                totalPosts[j + 1] = temp;
            }
        }
    }

    //set the amount of posts to display at most 20
    int maxPosts = postCount < 20 ? postCount : 20;
    //print out the posts
    for (int i = 0; i < maxPosts; i++) {
        if (i > 0) {
            printf(",");
        }
        printf("[%s:]", totalPosts[i]->user->name);
        printf("%s", totalPosts[i]->content);
    }
    printf("\n");
}

