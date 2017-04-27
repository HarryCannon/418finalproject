# Parallel Risk AI
## By Felipe Archondo, Harry Cannon

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