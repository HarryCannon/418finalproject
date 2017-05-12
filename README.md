# Parallel Risk AI
## By Felipe Archondo, Harry Cannon

# Final Report

### Summary
We developed and parallelized a Risk AI using an expectiminimax game tree with alpha-beta pruning. We used OpenMP and wrote in C++ to target the GHC machines.

### Background

#### Key data structures

The main data structure is the `risk_game`, which includes the `board`, `player`s, and other meta-information needed to play the game, such as turn count, whose turn it is, etc. The `player` data structure simply keeps track of the AI's stats, such as how aggressive they are, what heuristics they use to play, and what version of our algorithms they use. The most interesting structure is the `board`, which contains the topology of the risk board as well as the current state of play. The current state includes who owns which territories, as well as how many troops are each position. The `graph` data structure is responsible for keeping track of all of these operations, and we built a solid library to make these operations as quick and efficient as possible. This also allowed us to iterate quickly on strategies since all the helper functions were already built.

#### Key operations on these data structures

The most important operation is the `execute_move` operation on a `risk_game`, which takes the game from Player i's turn to Player i+1. The game runner simply calls on the function pointer passed to it by the player which modifies the `board` structure in place, after the player AI calculates the best moves to take. Other than this, most data structures never or infrequently change, such as the strategies of each `player` for example. 

#### What is the part that computationally expensive and could benefit from parallelization?

Given that the `graph` functions are as efficient as can be, the main part that benefits of parallelization is the calculation of the best moves to make. Because the game tree has a very high branching factor, from every given game state there are dozens if not hundreds of different direct children. So not only do we want to evaluate these child nodes in parallel, but ideally we would only explore the children states worth exploring. This is the main part of our project that we parallelized. 

#### Workload Features
Break down the workload. Where are the dependencies in the program? How much parallelism is there? Is it data-parallel? Where is the locality? Is it amenable to SIMD execution?
Given any game state in the tree, we can call this the root and it is entirely independent of any of its parents. That is to say, given that we have arrived at a certain state of play, it does not matter how we got there. There are no dependencies between children of the same node, so the game tree can be viewed as a DAG of dependencies. There is a lot of parallelism in this approach because we can map over the children independently, however doing this naively leads to a lot of wasted computation. This is why we used alpha-beta pruning in order to prune off game branches that are not fruitful. Because each game state needs its own memory, there is no data-parallelism in the main game tree branching. However, in calculating the value of the game in each state, there is data-parallelism. However because boards tend to be very small, usually with less than 50 territories, there is not a lot of potential for parallelism because of the overhead of spinning up new threads. In the future this part could be augmented with SIMD execution however.

### Approach

### Results

### References

### Division of Work
Work was completed evenly by both team members.

# Proposal/Checkpoint

### Summary

We are going to implement a sequential Risk AI with minimax/alpha-beta pruning and then parallelize it on the GHC machines with pthreads in C++. We will measure increases in speedup in the parallel AI as well as it's winrate against the sequential AI.

### Background

Risk is a board game where the goal is to conquer the world. The board is a map of the world divided into 42 territories, each of which is occupied by armies belonging to a player. Players that have armies in a territory can use them to attack neighboring territories belonging to other players, winning these battles (which are decided by dice rolls) allows a player to conquer territories and occupy them with their armies. The goal of Risk is to occupy every territory. Risk can be played with up to 6 players and an immense amount of moves are possible at a given state of the game. To make Risk more computationally feasible for us, we're going to limit it to two players and make other simplications.

To make an AI for Risk we're going to use minimax with alpha-beta pruning. Minimax is a classic algorithm used to decide moves in two player games that works as follows. We construct a tree where the root node is the current game state, and child nodes are all the game states that can result from a legal move from the current game state. We explore this tree to a set depth and based on a heuristic quantify how good each move branch is, deciding the move that leads to the best heuristic.

Alpha-beta pruning is a technique used to reduce the number of braches we explore in the game tree. We keep track of the best guaranteed score for both players and stop evaluating branches that would lead to worse scores for one of the players. In this manner we don't waste time computing branches that won't be used.

### Challenge

Minimax by itself is extremely parallel, each branch of the game tree can be explored in parallel. The workload is computation intensive, memory isn't much of an issue. The challenge of parallelizing will be largely due to alpha-beta pruning. Different threads can explore different branches of the game tree in parallel, but if threads can't communicate their alpha-beta values with each other, threads will waste a lot of time evaluating suboptimal branches of the game tree. In addition, if a thread exhausts the subtree it is assigned because it is suboptimal according to the global alpha-beta values, it will sit idle unless we can find a way for it to get work from non-idle threads.

### Resources

We will be implementing our AI on the GHC machines. Our codebase will be from scratch. To help implement the sequential minimax algorithm with alpha-beta pruning we'll likely use Wikipedia pseudo-code.

### Goals and Deliverables

We plan to achieve the following -

* Implement a sequential minimax with alpha-beta pruning based AI that plays a simplified version of Risk (only 2 players, deterministic battle outcomes and reinforcing strategies, can only attack with one territory per turn, etc.)

* Evenly distribute game tree branches among threads and show resultant speedup of board evaluations

* Allow communication of alpha-beta values between threads and show resultant speedup of board evaluations

* Implement work sharing so that threads that exhaust their branch of the game tree don't sit idle, show resultant speedup of board evaluations

* Have our final parallelized AI play against our sequential AI several times where each move is time-constrained. Measure the parallelized AI's winrate. Specifically we'd like our optimized AI to have at least a 60% win rate.

We hope to achieve the following -

* Extend our sequential algorithm to more complete versions of Risk (more than 2 players, consider reinforcing strategies, etc.) and parallelize it in the same manner as our simplified Risk AI.

During our presentation we'll show our speedup graphs and present our parallel AI's winrate against the sequential AI.

### Platform Choice

C++ is a good language for us because it's high performance and can exploit parallelism. The GHC machines are good for us because they have a lot of cores and are available to us all the time, so we'll be able to iterate quickly.

### Schedule

Week 1:

* Make sequential minimax with alpha-beta pruning based AI that plays a simplified version of Risk, make framework for AIs to play against each other.
* Start parallelizing the AI by distributing exploration of game tree branches among multiple threads
* Implement communication of alpha-beta values between threads

Week 2:

* Implement work sharing between threads
* Measure winrate of parallel AI vs sequential AI
* Work on writeup
