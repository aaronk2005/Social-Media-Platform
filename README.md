# Social Media Platform

## Description
A basic social media platform implemented in C that supports:
- User management (create, search, modify, delete users)
- Friendship system (add friends, remove friends, list friends, find mutual friends)
- Messaging system (send messages between friends, view chat history)
- Posts and feed system (create posts, like posts, display a user’s feed)

The platform uses hash tables, linked lists, and queues for efficient data management — designed as a final project for ELEC 278: Data Structures and Algorithms.

## Features
- Create users with unique names and emails
- Add/Delete friends and view mutual friends
- Send and receive messages between users (FIFO chat queue)
- Create posts, like posts, and display feeds sorted by likes
- Handles up to 10,000 users efficiently using open hashing (separate chaining)

## Core Data Structures
| Feature   | Data Structure Used | Notes |
|-----------|----------------------|-------|
| Users     | Open Hash Table       | Fast O(1) insertion and search |
| Friends   | Singly Linked List    | Each user maintains a dynamic friend list |
| Messages  | Queue (Linked List)   | Messages are stored in FIFO order |
| Posts     | Singly Linked List + Dynamic Array (likes) | Fast post creation and dynamic likes |

## How It Works
1. Users are created and stored in an open hash table.
2. Friendships are managed using singly linked lists.
3. Messages are queued and displayed FIFO (First In, First Out).
4. Posts can be created and liked by friends, and feeds are displayed sorted by likes.

## References
- W3Schools: C Structs
- GeeksforGeeks: Bubble Sort Algorithm
- GeeksforGeeks: Hash Table Data Structure
- Programiz: Linked Lists
- GeeksforGeeks: Queue Linked List Implementation
- Jacob Sorber: YouTube - Understanding Data Structures



