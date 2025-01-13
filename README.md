# Chat Rooms

## Overview

This project is a terminal-based chat application built in C. It uses sockets
for real-time communication, allowing users to join chat rooms, set nicknames
and exchange messages seamlessly.

### Features

- Multiple chat rooms for users to join and interact.

- Customizable usernames

- Real-time messaging.

- Minimalistic terminal-based interface

## Requirements

- A C compiler like GCC

- Make (if you're using the provided Makefile)

- A terminal

## Installation

#### Clone the repo

```bash
git clone https://github.com/felipemdutra/chat-rooms.git
cd chat-rooms
```

#### Build the project
```bash
make
```

## Running the application

#### Start the server

```bash
./chat_app
```

##### The server will listen on the default port 8080

#### Simulate a client

Open a new terminal window and type

```bash
nc localhost 8080
```

Open multiple terminal windows to simulate different clients.

# List of commands

After running the server and connecting as a client you try the following commands:

- Set a nickname:

```bash
\name <your-nickname>
```

- Create a chat room:

```bash
\create <room-name>
```

- List available rooms:

```bash
\list
```

- Join a chat room:

```bash
\join <room-name>
```

- Leave a room:

```bash
\leave
```

- Send a message:

Type and press Enter.

# Known Issues

- When a client disconnects, the client array doesn't change, causing errors

- When a room is deleted, the rooms array doesn't change, causing errors

- When a client leaves a room, the room's clients array does't change, causing errors

