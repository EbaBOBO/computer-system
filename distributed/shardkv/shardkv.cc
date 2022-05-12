#include <grpcpp/grpcpp.h>

#include "shardkv.h"

/**
 * This method is analogous to a hashmap lookup. A key is supplied in the
 * request and if its value can be found, we should either set the appropriate
 * field in the response Otherwise, we should return an error. An error should
 * also be returned if the server is not responsible for the specified key
 *
 * @param context - you can ignore this
 * @param request a message containing a key
 * @param response we store the value for the specified key here
 * @return ::grpc::Status::OK on success, or
 * ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "<your error message
 * here>")
 */
::grpc::Status ShardkvServer::Get(::grpc::ServerContext* context,
                                  const ::GetRequest* request,
                                  ::GetResponse* response) {
  //if get all_users
  // mtx.lock();
  std::cout<<"get "<<request->key()<<std::endl;
  if(request->key().rfind("all") != std::string::npos)
  {
    std::string resp = "";
    for(auto &it: mapp)
    {
      std::vector<std::string> key_parse_all = parse_value(it.first, "_");
      int key_size = key_parse_all.size();
      if(key_size == 2)
      {
        if(it.first.rfind("user",0) != std::string::npos)
        {
          resp += it.first + ",";
        }
      }
    }
    response->set_data(resp);
    // mtx.unlock();
    return ::grpc::Status::OK;
  }
  //check id range, not in the range, return error
  int id = extractID(request->key());
  int find = 0;
  // for(auto &range:server_map)
  // {
  //   if(id <= range.second.upper && id >= range.second.lower)
  //   {
  //     find = 1;
  //   }
  // }
  // if(find == 0)
  // {
  //   mtx.unlock();
  //   return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "<error: shardkv get not in the range>");
  // }
  //in the range
  std::vector<std::string> key_parse = parse_value(request->key(), "_");
  std::string put_name = key_parse[0];
  // int id = extractID(request->key());
  //if user_id_posts
  if(key_parse.size() == 3)
  {
    std::cout<<request->key()<<std::endl;
    // key in the map
    if(mapp.find("user_" + std::to_string(id) + "_posts") != mapp.end())
    {
      std::cout<<mapp[request->key()]<<std::endl;
      response->set_data(mapp["user_" + std::to_string(id) + "_posts"]);
      // mtx.unlock();
      return ::grpc::Status::OK;
    }
    // key not in the map
    else
    {
      // mtx.unlock();
      return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "<error: shardkv get, user_id_post not in the map>");
    }
  }
  //not user_id_posts
  else
  {
    //get user_id
    if(put_name == "user")
    {
      // key in the map
      if(mapp.find(request->key()) != mapp.end())
      {
        response->set_data(mapp[request->key()]);
        // mtx.unlock();
        return ::grpc::Status::OK;
      }
      // key not in the map
      else
      {
        // mtx.unlock();
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "<error: shardkv get, user_id not in the map>");
      }
    }
    //get post_id
    if(put_name == "post")
    {
      // key in the map
      if(mapp.find(request->key()) != mapp.end())
      {
        response->set_data(mapp[request->key()]);
        // mtx.unlock();
        return ::grpc::Status::OK;
      }
      // key not in the map
      else
      {
        // mtx.unlock();
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "<error: shardkv get, post_id not in the map>");
      }
    }
  }
  // mtx.unlock();
  return ::grpc::Status::OK;
  // return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "not implemented");
}

/**
 * Insert the given key-value mapping into our store such that future gets will
 * retrieve it
 * If the item already exists, you must replace its previous value.
 * This function should error if the server is not responsible for the specified
 * key.
 *
 * @param context - you can ignore this
 * @param request A message containing a key-value pair
 * @param response An empty message, as we don't need to return any data
 * @return ::grpc::Status::OK on success, or
 * ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "<your error message
 * here>")
 */
::grpc::Status ShardkvServer::Put(::grpc::ServerContext* context,
                                  const ::PutRequest* request,
                                  Empty* response) {
  // mtx.lock();
  std::cout<<"put "<<request->key()<<" "<<request->user()<<std::endl;
  //check id range, not in the range, return error
  int id = extractID(request->key());
  int find = 0;
  // for(auto &range:server_map)
  // {
  //   if(id <= range.second.upper && id >= range.second.lower)
  //   {
  //     find = 1;
  //   }
  // }
  // if(find == 0)
  // {
  //   mtx.unlock();
  //   return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,  "<error: shardkv put not in the range>");
  // }
  //in the range
  std::vector<std::string> key_name = parse_value(request->key(), "_");
  std::string put_name = key_name[0];
  //put user_id
  if(put_name.rfind("user") != std::string::npos)
  {
    //exist, replace
    if(mapp.find(request->key()) != mapp.end())
    {
      mapp[request->key()] = request->data();
    }
    //don't exist, insert
    else
    {
      //insert user_id
      mapp.insert(std::pair<std::string, std::string>(request->key(), request->data()));
      //insert all_users
      if(mapp.find("all_users") == mapp.end())
      {
        mapp.insert(std::pair<std::string, std::string>("all_users",request->key()));
      }
      else
      {
        mapp["all_users"] = mapp["all_users"] + request->key() +",";
      }
      
    }

  }
  //put post_id
  else
  {
    //check post's user_id is in the range
    int post_user_id = extractID(request->user());
    int find = 0;
    // for(auto &range:server_map)
    // {
    //   if(post_user_id <= range.second.upper && post_user_id >= range.second.lower)
    //   {
    //     find = 1;
    //   }
    // }
    // if(find == 0)
    // {
    //   mtx.unlock();
    //   return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,  "<error: shardkv put not in the range>");
    // }
    //exist, replace
    // std::cout<<request->data()<<request->user()<<std::endl;
    // std::cout<<post_user_id<<std::endl;
    if(mapp.find(request->key()) != mapp.end())
    {
      // std::cout<<post_user_id<<std::endl;
      mapp[request->key()] = request->data();
    }
    //don't exist, insert
    else
    {
      //insert post_id
      // std::cout<<post_user_id<<std::endl;
      mapp.insert(std::pair<std::string, std::string>(request->key(), request->data()));
      //insert user_id_posts
      
      if(mapp.find("user_" + std::to_string(post_user_id) + "_posts") == mapp.end())
      {
        mapp.insert(std::pair<std::string, std::string>("user_" + std::to_string(post_user_id) + "_posts",request->key() + ","));
      }
      else
      {
        mapp["user_" + std::to_string(post_user_id) + "_posts"] = mapp["user_" + std::to_string(post_user_id) + "_posts"] + request->key() +",";
      }
    }

  }
  // mtx.unlock();
  return ::grpc::Status::OK;

                          

  //server is not responsible for the specified key, return error

  // return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "not implemented");
}

/**
 * Appends the data in the request to whatever data the specified key maps to.
 * If the key is not mapped to anything, this method should be equivalent to a
 * put for the specified key and value. If the server is not responsible for the
 * specified key, this function should fail.
 *
 * @param context - you can ignore this
 * @param request A message containngi a key-value pair
 * @param response An empty message, as we don't need to return any data
 * @return ::grpc::Status::OK on success, or
 * ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "<your error message
 * here>"
 */
::grpc::Status ShardkvServer::Append(::grpc::ServerContext* context,
                                     const ::AppendRequest* request,
                                     Empty* response) {
  //check id range, not in the range, return error
  // int id = extractID(request->key());
  // int find = 0;
  // for(auto &range:server_map)
  // {
  //   if(id <= range.second.upper && id >= range.second.lower)
  //   {
  //     find = 1;
  //   }
  // }
  // if(find == 0)
  // {
  //   return ::grpc::StatusCode::INVALID_ARGUMENT, "<error: shardkv put not in the range>");
  // }
  
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "not implemented");
}

/**
 * Deletes the key-value pair associated with this key from the server.
 * If this server does not contain the requested key, do nothing and return
 * the error specified
 *
 * @param context - you can ignore this
 * @param request A message containing the key to be removed
 * @param response An empty message, as we don't need to return any data
 * @return ::grpc::Status::OK on success, or
 * ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "<your error message
 * here>")
 */
::grpc::Status ShardkvServer::Delete(::grpc::ServerContext* context,
                                           const ::DeleteRequest* request,
                                           Empty* response) {
  std::vector<std::string> key_name = parse_value(request->key(), "_");
  //delete user_id
  if(request->key().rfind("user") != std::string::npos)
  {
    //if exist
    if(mapp.find(request->key()) != mapp.end())
    {
      //delete user_id
      mapp.erase(request->key());
      //delete all_users
      std::vector<std::string> all_users_v = parse_value(mapp["all_users"], "_");
      std::string new_all_users = "";
      //construct new_all_users
      for(int i = 0; i < all_users_v.size(); i++)
      {
        if(all_users_v[i] != request->key())
        {
          new_all_users = new_all_users + all_users_v[i] + ",";
        }
      }
    }
    //don't exist, return error
    else
    {
      return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "<error: shardkv delete, user_id not in the map>");
    }
  }
  //delete post_id  
  else
  {
    //if exist
    if(mapp.find(request->key()) != mapp.end())
    {
      //delete post_id
      mapp.erase(request->key());
      //delete user_id_post
      for(auto &it: mapp)
      {
        std::vector<std::string> key_name = parse_value(it.first, "_");
        // is user_id_posts
        if(key_name.size() == 3)
        {
          //contain post_id
          if(it.second.rfind(request->key()) != std::string::npos)
          {
            std::vector<std::string> new_user_posts_v = parse_value(it.second, "_");
            std::string new_user_posts = "";
            //construct new_user_posts
            for(int i = 0; i < new_user_posts_v .size(); i++)
            {
              if(new_user_posts_v[i] != request->key())
              {
                new_user_posts = new_user_posts + new_user_posts_v[i] + ",";
              }
            }
          }
        }
      }

    }
    //don't exist, return error
    else
    {
      return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "<error: shardkv delete, user_id not in the map>");
    }

  }
  return ::grpc::Status::OK;
                                  
  
  // return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "not implemented");
}

/**
 * This method is called in a separate thread on periodic intervals (see the
 * constructor in shardkv.h for how this is done). It should query the shardmaster
 * for an updated configuration of how shards are distributed. You should then
 * find this server in that configuration and look at the shards associated with
 * it. These are the shards that the shardmaster deems this server responsible
 * for. Check that every key you have stored on this server is one that the
 * server is actually responsible for according to the shardmaster. If this
 * server is no longer responsible for a key, you should find the server that
 * is, and call the Put RPC in order to transfer the key/value pair to that
 * server. You should not let the Put RPC fail. That is, the RPC should be
 * continually retried until success. After the put RPC succeeds, delete the
 * key/value pair from this server's storage. Think about concurrency issues like
 * potential deadlock as you write this function!
 *
 * @param stub a grpc stub for the shardmaster, which we use to invoke the Query
 * method!
 */
void ShardkvServer::QueryShardmaster(Shardmaster::Stub* stub) {
  Empty query;
  QueryResponse response;
  ::grpc::ClientContext cc;

  auto status = stub->Query(&cc, query, &response);
  if (status.ok()) {
    // TODO: figure out what to do here!
  } else {
    // TODO: here too!
  }
}



// #include <grpcpp/grpcpp.h>

// #include "shardkv.h"

// /**
//  * This method is analogous to a hashmap lookup. A key is supplied in the
//  * request and if its value can be found, we should either set the appropriate
//  * field in the response Otherwise, we should return an error. An error should
//  * also be returned if the server is not responsible for the specified key
//  *
//  * @param context - you can ignore this
//  * @param request a message containing a key
//  * @param response we store the value for the specified key here
//  * @return ::grpc::Status::OK on success, or
//  * ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "<your error message
//  * here>")
//  */
// ::grpc::Status ShardkvServer::Get(::grpc::ServerContext* context,
//                                   const ::GetRequest* request,
//                                   ::GetResponse* response) {
//   return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "not implemented");
// }

// /**
//  * Insert the given key-value mapping into our store such that future gets will
//  * retrieve it
//  * If the item already exists, you must replace its previous value.
//  * This function should error if the server is not responsible for the specified
//  * key.
//  *
//  * @param context - you can ignore this
//  * @param request A message containing a key-value pair
//  * @param response An empty message, as we don't need to return any data
//  * @return ::grpc::Status::OK on success, or
//  * ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "<your error message
//  * here>")
//  */
// ::grpc::Status ShardkvServer::Put(::grpc::ServerContext* context,
//                                   const ::PutRequest* request,
//                                   Empty* response) {
//   return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "not implemented");
// }

// /**
//  * Appends the data in the request to whatever data the specified key maps to.
//  * If the key is not mapped to anything, this method should be equivalent to a
//  * put for the specified key and value. If the server is not responsible for the
//  * specified key, this function should fail.
//  *
//  * @param context - you can ignore this
//  * @param request A message containngi a key-value pair
//  * @param response An empty message, as we don't need to return any data
//  * @return ::grpc::Status::OK on success, or
//  * ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "<your error message
//  * here>"
//  */
// ::grpc::Status ShardkvServer::Append(::grpc::ServerContext* context,
//                                      const ::AppendRequest* request,
//                                      Empty* response) {
//   return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "not implemented");
// }

// /**
//  * Deletes the key-value pair associated with this key from the server.
//  * If this server does not contain the requested key, do nothing and return
//  * the error specified
//  *
//  * @param context - you can ignore this
//  * @param request A message containing the key to be removed
//  * @param response An empty message, as we don't need to return any data
//  * @return ::grpc::Status::OK on success, or
//  * ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "<your error message
//  * here>")
//  */
// ::grpc::Status ShardkvServer::Delete(::grpc::ServerContext* context,
//                                            const ::DeleteRequest* request,
//                                            Empty* response) {
//   return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "not implemented");
// }

// /**
//  * This method is called in a separate thread on periodic intervals (see the
//  * constructor in shardkv.h for how this is done). It should query the shardmaster
//  * for an updated configuration of how shards are distributed. You should then
//  * find this server in that configuration and look at the shards associated with
//  * it. These are the shards that the shardmaster deems this server responsible
//  * for. Check that every key you have stored on this server is one that the
//  * server is actually responsible for according to the shardmaster. If this
//  * server is no longer responsible for a key, you should find the server that
//  * is, and call the Put RPC in order to transfer the key/value pair to that
//  * server. You should not let the Put RPC fail. That is, the RPC should be
//  * continually retried until success. After the put RPC succeeds, delete the
//  * key/value pair from this server's storage. Think about concurrency issues like
//  * potential deadlock as you write this function!
//  *
//  * @param stub a grpc stub for the shardmaster, which we use to invoke the Query
//  * method!
//  */
// void ShardkvServer::QueryShardmaster(Shardmaster::Stub* stub) {
//   Empty query;
//   QueryResponse response;
//   ::grpc::ClientContext cc;

//   auto status = stub->Query(&cc, query, &response);
//   if (status.ok()) {
//     // TODO: figure out what to do here!
//   } else {
//     // TODO: here too!
//   }
// }