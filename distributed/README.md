Project 6: Distributed Store
============================

In this project, you will build the infrastructure for a small social network. In particular, you will work on a distributed, sharded key-value store that stores the social graph and user information for the social network.

# How to Run:

Run `./install.sh` to install gRPC and its dependencies.

To build your code, run `make` from inside the build directory.

To test you code, run `./test.sh` or `make check` inside the build directory.

## Running the frontend

Inside the course container, after making all of the executables, start up a shardmaster on port 9095, shardkv servers, and a client in separate terminals.

```
./shardmaster 9095

./client <SHARDMASTER_HOST> 9095

./shardkv <PORT> <SHARDMASTER_HOST> 9095
```

Start as many shardkv servers as you would like and add them using the client's `join` command (e.g. `join <SHARDMASTER_HOST>:<PORT>`). You can verify that they've been added using the client's `query` command.
The shardmaster host name will be printed after starting up the shardmaster -- this is should be the ID of the cs300 docker container.

Outside of the course container, navigate to this directory and run `docker compose up`. The Bacefook UI should be now viewable at http://localhost:3000 in your browser!

Note: In order for the frontend to work correctly, you must enter `user_<NUM>` and `post_<NUM>` in the user/post ID fields where NUM is between 0 and 1000.

<!-- TODO: Fill this out. -->
## Design Overview:

## Collaborators:
Yiran Zhou
## Conceptual Questions:
1.Describe the roles of the controller, the key-value servers, and the clients in this distributed system. How are they involved in processing a client request for key k?
The controller masters the sharding of servers. It decides which server corresponding to which shards and if there are any changes, react to the requests. The key-value servers store the server's data and corresponding shards. The clients in this distributed system send request to the servers and get response from the servers.
A client request for a key k. The controller get the request and decides which server this key should belong to and send to the corresponding server. Then server check whether this key is belong to its shard and do the corresponding operations and then return responses.

2.Currently, the clients contact the shardmaster to get the current configuration of shards and then make RPCs to key-value servers. Clients will only re-query the shardmaster if a request to a server fails because the shard containing they key was moved away from that server. Instead of this model, the shardmaster could also be in charge of making requests to the key-value servers on clients’ behalf and pass the responses back to the clients. Why will this design result in lower system throughput?
Because if the shardmaster is in charge of making requests to the key-value servers on clients' behalf and pass the responses back to the clients, it will be very time consuming. When the shardmster is sending message and waiting for the requests, other process need to wait it. So this design will result int lower system throughput.
## Extra Credit Attempted:

## How long did it take to complete Distributed Store?
30 hours.