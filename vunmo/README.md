Project 5 - Vunmo
====================

<!-- TODO: Fill this out. -->
## Design Overview:
None
## Collaborators:

## Conceptual Questions:
1.Discuss the tradeoffs between spawning a thread per request versus using a worker pool consisting of a fixed number of threads. Consider how performance will vary as the number of users increases.

When processing a large number of threads, having a work pool will have more advantages. Because it has a large compacity and can process them on a scale. In this situation, processing a thread per request will be time-consuming. Spawning a thread per request will be better when processing threads on a small scale. Because we don't need to create and delete the work pool. And it will be more efficient. 

A popular online food ordering service, PackSnass, includes a feature which allows users to collect rewards and share them with their friends. The PackSnass infrastructure contains the user_t struct (to store user data) and send_rewards function (to transfer rewards) listed below:
typedef struct user {
    int id;           // a unique user id
    int rewards;      // the number of rewards a user has
    std::mutex mtx;   // mutex to synchronize user data 
} user_t;

void send_rewards(user_t* sender, user_t* receiver){
    sender->mtx.lock();
    receiver->mtx.lock();
    sender->rewards--;
    receiver->rewards++;
    sender->mtx.unlock();
    receiver->mtx.unlock();
}
The PackSnass servers have multiple worker threads, so user requests can be executed in parallel. As such, what might go wrong if two users, Alice and Zachary simultaneously attempt to send each other rewards? How might you change send_rewards to fix this problem?

If two users simultaneously attempt to send rewards, there may be a deadlock. The correct code is shown below. 
void send_rewards(user_t* sender, user_t* receiver){
    if(send->rewards > receiver->rewards)
    {
        receiver->mtx.lock();
        sender->mtx.lock();
        sender->rewards--;
        receiver->rewards++;
        receiver->mtx.unlock();  
        sender->mtx.unlock();
    }
    else
    {
    sender->mtx.lock();
    receiver->mtx.lock();
    sender->rewards--;
    receiver->rewards++;
    sender->mtx.unlock();
    receiver->mtx.unlock();
    }
}


Q1: Where could a supply-chain attack have occurred in Vunmo? It might help to think about what external functions or libraries you needed to use! (If you have access to the source-code, it might be worth looking at it.)
 The exchange of data conducted among various companies brings with it a certain level of risk which entails cyber theft. Sophisticated cyber criminals also realize the importance of the data held by companies and device strategies to gain access to the sensitive data. A company’s supply network usually consists of third party entities like manufacturers, suppliers, handlers, shippers, and purchasers all involved in the process of making products available to the end consumers. Because the target company may have a security system that may be impenetrable for even the sophisticated cyber criminals, supply chain attacks are carried out on the third party businesses on the chain who are deemed to have the weakest internal measures and processes in place. Once one member’s security protocols are found to be weak, the member’s vulnerabilities become the target company’s risk.

Q2: Was the developer justified in his actions? Why or why not?
 I think the developer's behavior is wrong, because it hurts the interests of many users. Many users' privacy will also be compromised. It can also damage the company's reputation. They are all innocent.

Q3: How do you think we can make open source sustainable?

Some aspects to consider:

Who do you think should be responsible for funding open source projects? Should anyone?
Besides the most crucial software directly relevant to their codebases, it’s unlikely for companies to provide funding for open source software. Should companies be obligated to financially support open source projects they use?
Do OSS contributors deserve to be compensated for their work?

I think it should be the users of open source projects who fund these open source projects. And I think that companies should be obligated to financially support open source projects they use. OSS contributors deserve to be compensated for their work. Only if these open source developers receive funding will subsequent people be willing to do open source libraries.

## Extra Credit attempted:
Consider the design of the Vunmo service, and your implementation. What types of usage patterns (e.g., transfer patterns between users) are likely to scale well to many threads, and which will scale poorly?
The work pool and accounts structure are likely to scale well to many threads. We can add other functions for accounts to convenient user's using experience. 
Some patterns such as accept clients and process requests are possible to scale well to many threads. Because they can process a queue of requests at a time. But some patterns like get accounts may be scale poorly. Because it may have conflict once there are too many request. 
## How long did it take to complete Vunmo?
10 hours
