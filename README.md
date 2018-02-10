# Contents

- [Introduction and Overview](#introduction-and-overview)
- [Why am I writing this?](#why-am-i-writing-this)
- [On who's authority?](#on-whos-authority)
- [Starting definitions - Threads and Procesess](#starting-definitions---threads-and-procesess)
- [Processors](#processors)
- [Back to threads](#back-to-threads)
    - [My first Thread!](#my-first-thread)
    - [My first program with more than one thread](#my-first-program-with-more-than-one-thread)
- [When would I use threads? Why would I want to write a multi-threaded program?](#when-would-i-use-threads-why-would-i-want-to-write-a-multi-threaded-program)
    - [A shift towards parallel programming as a standard?](#a-shift-towards-parallel-programming-as-a-standard)
- [Why is it often difficult to write multi-threaded programs?](#why-is-it-often-difficult-to-write-multi-threaded-programs)
    - [Race Conditions](#race-conditions)
    - [Shared mutable state](#shared-mutable-state)
- [Green Threads](#green-threads)
- [Concurrency Models (CSP, Actor Model, STM)](#concurrency-models-csp-actor-model-stm)
    - [Actor Model](#actor-model)
    - [CSP](#csp)
    - [Software Transactional Memory](#software-transactional-memory)
- [Concurrency Vs Paralellism](#concurrency-vs-paralellism)
- [Research and resources](#research-and-resources)
- [Footnotes](#footnotes)

# Introduction and Overview

This doc is an attempt to chart my own deep dive into understanding concurrent and parallel programming. This document will be a constant work and progress. Lots to learn. Lots to cover.

# Why am I writing this?

I started programming by learning Javascript, outside of a more "traditional" comp-sci/comp-eng setting; I followed online resources, courses, and built projects in my spare time (I still do!). In the last few months I've been learning new languages; languages where words like `threads` `concurrency` `parallelism` `futures` `race-conditions` and so on, tend to show up. And me? I'm like a leaf in the wind, floating by and circling around these topics &#x2013; picking up little notions of what they mean as I go. These ancillary understandings were enough for me to get by while I built my projects, went to work, read articles, and listened to podcasts about new programming languages.

However! I can only exist like a ghost on the edge of understanding for so long before I become frustrated; I have been building a puzzle for this new knowledge, but I have only found the borders (the easiest pieces to find!) to frame a new picture, yet I have not filled out the middle.

What I'm trying to say, is that I want to do a deep dive into this topic; this doc is my attempt to fill in the center of the puzzle that is this world of understanding concurrency, parallel programming, and *whatever-you-call-its* (surely by the end of my research I'll have a better umbrella term for all this; for now, you'll have to bear with me.) While I'm a firm advocate of learning by building; I also enjoy doing these deep-dives on occaision (especially when I'm circling around a topic but not engaging directly with it)

NB: *A javascript-first education provides a strange and intriguing position to approach these topics. In my discussions I've often struggled to relate these new topics to my perceived knowledge in the world of programming, framed by having learned javascript <span class="underline">first</span>. Is there anything wrong with this? No! But, I'm writing this novella here because I've found that my position has not aligned as well as I'd hoped to understand some of these concepts.*

NB: *While I do claim this doc is for my own purposes, I've also tried to make it more interesting (tone wise) to others than myself. Let's keep it light and have some fun.*

# On who's authority?

**Q:** Why should I listen / heed / pay attention to you / this article / all the / slashes you use / ?

**A:** Good question! It's really up to you. This doc is a personal chronicle&#x2013;an auto-didactic log to fill in my own knowledge gaps. I am publishing this online to share my findings / my take on things.  I would be very pleased if this doc proved useful for people who find themselves in a similar situation as I've described above.

**Q:** Speaking of being wrong, you made a mistake!

**A:** Let's fix that! Feel free to make a PR / open an issue if I've misquoted something (or not properly attributed something), or have a fact wrong. With your help, the vaccuum room of my brain and this outputted mud-doc can shape itself into a gleaming mountain, with resilient flowers in the windy valleys below.

NB: *Since this is a personal doc charting my own understanding, I will gladly consider pull requests, but may incorporate contributions with adaptation in my own writing. Don't let that stop you from making PR's and issues, though.*

**Q:** Do you have any other disclaimers for us?

Oh yeah! *Nobody* is paying me to include *any* of the links or references to books or whatever. If you want to pay me, FORGET IT! Go and look at some flowers, pet a puppy or paint a painting. I'll know and we can mentally high five across space and time.

Furthermore! If I've incorrectly cited your work *please* let me know and I can make changes / remove it as you like!

# Starting definitions - Threads and Procesess

A lot of the words and terms I've been throwing around are related to multi-threaded programming. So, let's get started by working up to figure out what a <span class="underline">thread</span> is.

At first I thought we could start with this [definition from wikipedia](https://en.wikipedia.org/wiki/Thread_(computing)).

> In computer science, a thread of execution is the smallest sequence of programmed instructions that can be managed independently by a scheduler, which is typically a part of the operating system.[1] The implementation of threads and processes differs between operating systems, but in most cases a thread is a component of a process. Multiple threads can exist within one process, executing concurrently and sharing resources such as memory, while different processes do not share these resources. In particular, the threads of a process share its executable code and the values of its variables at any given time.

Isn't that quite a bit of information at once though? Let's dig into it.

Let's do something practical. If you open your \`terminal\` application (and are running unix) you can type in the following to see some processes:

`$ top`

Here's the result of mine:

![img](/images/top-output.png)

Woop! That's a lot of information. But you know what it reminds me of? The activity monitor on my mac &#x2013; which I have seen many times before:

![img](/images/activity-monitor.png)

There's a lot of information we can glean from these screenshots (especially if you know what to look for!). Looking closer at my `top` output, I can see the following:

`Processes: 391 total, 2 running, 389 sleeping, 2309 threads`

Hmm, I just quit my twitter client and now I have this: 

`Processes: 389 total, 3 running, 386 sleeping, 2296 threads`.

Hmm (pt 2!): I just quit a project I was working on where I had a terminal pane opened that was running a clojure/clojurescript project (which uses Java). Now the following:

`Processes: 372 total, 2 running, 370 sleeping, 2203 threads`

Alright, alright, contrived / heavy handed examples aside! We have deduced that the things running on your computer are *Processes* &#x2013; your browser, your twitter client, your code editor, your terminal, everything you interact with &#x2013; All processes! As Wikipedia puts it:

> A computer program is a passive collection of instructions, while a process is the actual execution of those instructions.
> 
> -   [Process (computing)](https://en.wikipedia.org/wiki/Process_(computing))

We're talking about LIVING, BREATHING, PROCESSES PEOPLE. It's beautiful. Processes are identified by a `PID` &#x2013; a process id. Have you ever had to force quit a program? Maybe your browser froze, so you opened your task manager or activity monitor and forced a process to end. The equivalent of doing that in the terminal is to send a message to kill a process. If I type `kill 93835` it's going to kill something &#x2013; Google Chrome to be exact (or at least, *a tab in google chrome* becasue each tab lives in it's *own* process).

By the way, I DID type in `kill 93835` WITHOUT knowing what it was (because I live recklessly). Then this notification showed up:

![img](/images/kill-chrome-process.png)

My beloved vimium extension crashed. Chrome runs each extension as it's own process too, it seems. Neat. Maybe you *should not* do this though.

Anyway, from these processes we can also see that each one contains a certain amount of running *threads*. On my machine, two instances of Java have 28 threads. What are those 28 threads doing? I don't know! I don't even know if I can figure it out. JAVA! What are you DOING. Maybe I will know at the end of all this research. Now, if we were to return to the above wikipedia quote on threads, it might make a bit more sense.

> Multiple threads can exist within one process, executing concurrently and sharing resources such as memory

That's where things start to get tricky. I think we're about to get into the material.

But first a quick preface:

# Processors

The *Processor* is the brain in your computer makin' stuff happen. These days, computers are usually *multi-core-processors* &#x2013; computers with more than one brain, in a sense.

Two brains you say? Or yet, 4, or maybe even 8? "How can a computer work with more than one brain?" you ask. "I can barely operate with 1!" I yell loudly into the void, not answering your question. I bow to the 8 core computer, who is 8x more than me in everyway possible. But here's the ticket (I say, whispering), the 2, 4, 8, 16, whatever-many-brain-ed computer is only as smart as the programs fed to it; and how they make use of multi-core-threading, concurrency, and all those other cool terms I buzzed on about before.


# Back to threads

I think it's about time we looked at some code so that we can take a practical approach to learning how to use threads. Why don't we create a thread?

NB: *Please note that the following examples were built to run on **my machine**. I'd recommend you treat them as reading material.*

## My first Thread!
```C
#include <stdio.h>

int main() {
    printf("Lucky me, lucky mud.");
    return 0;
}
```

"Hmmm that's strange" You say, "I don't see anything about threads in that program". I look back at you, grinning like a fool. I've laid some trick code before you (like a trick question, but with code right?). The above code is a C program. After compiling it and running it in the command line it prints "Lucky me, lucky mud." While there is no explicit mention of *threads* the `main()` function in this program *is* a thread; a single default thread. If you want to make more threads you have to do it manually. So let's *actually* do that (sorry).


## My first program with more than one thread
```C
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *takeNap(void *threadid) {
    // pull the thread name off of the function param
    int tid;
    tid = (int)threadid;

    printf("'Allo It me, thread #%d!\nI like napping, so I'm going to go sleep for 2 seconds \n", tid);
    sleep(2);
    printf("THREAD 1 HERE, I'm BACK, SICK nap, I'M like, gonna STOP EXISTING NOW\n");
    pthread_exit(NULL);
}

int main () {
    pthread_t myThread; // look! A name for our beautiful child to be. So proud.
    int rdata;          // our thread might return an error so we should capture that too.
    int counter;        // we use this for a FOR loop below.


    // Let's make that cake. I mean thread.
    rdata = pthread_create(&myThread, NULL, takeNap, (void *) 1);

    // Let's check in case our new thread returns an error code.
    if (rdata) {
        printf("ERROR: pthread_create() returned an error code of %d\n", rdata);
        exit(-1);
    }

    // Let's make the main function do some work tho right? Let's count some HATS OK?
    for(counter=0; counter<5; counter++){
        sleep(1);
        printf("I see %d cool hats\n", counter);
    }

    // we should exit all our threads I guess.
    pthread_exit(NULL);
```

Here's our output:

```
Hello World! It's me, thread #1!
I like napping, so I'm going to go sleep for 2 seconds
I see 0 cool hats
THREAD 1 HERE, I'm BACK,  SICK nap, I'M like, gonna STOP EXISTING NOW
I see 1 cool hats
I see 2 cool hats
I see 3 cool hats
I see 4 cool hats
```

Wow, so beautiful. Can't stop crying at this. Do you see how two things are happening in parallel? Our `main()` function (which is the default single thread of a program) is counting how many cool hats it sees. (4! Four C00L hats believe it or not!). While counting away at some sickening hats, a thread has shot off from the main function and is doing it's own thing &#x2013; running the function `takeNap`.

I've peppered in a bunch of `sleep()` functions everywhere to make things more evident if you were to run this on your machine. `sleep` pauses the execution of *a thread* for a certain duration.

Let's address some possible questions from the code above.

**What's up with "pthread"?**

Pthreads are POSIX threads &#x2013; threads that adhere to the POSIX standard. I think the following [in-depth tutorial](https://computing.llnl.gov/tutorials/pthreads/#Pthread) does a good job summarizing what that means:

> "Historically, hardware vendors have implemented their own proprietary versions of threads. These implementations differed substantially from each other making it difficult for programmers to develop portable threaded applications. [&#x2026;] Most hardware vendors now offer Pthreads in addition to their proprietary API's."
> 
> -   [POSIX Threads Programming](https://computing.llnl.gov/tutorials/pthreads/#Pthread)

**What are the params for `pthread` create doing?**

If you go to your terminal and type `man pthread` you can see a detailed manual for `pthread`. If we look for the pthread create function we find:

    
    Thread Routines
       int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg)
               Creates a new thread of execution.

To me, that doesn't read super easily. Let's dig in. Things inbetween `<` `>` are arguments to the `pthread create` function:


`<pthread_t *thread>`
This was the unique thread we declared ("myThread")

`<const pthread_att_t *attr>`
An object housing configuration details for the creation of our thread.

`<start_routine>`
The function we want the thread to start once it's created.

`<arg>`
We can pass a single argument to the starting routine (or NULL)

**So like, what IS a thread?**

So you noticed that I haven't given you my definition of a thread. I'm not going to lie; putting the concept of a thread into [simplified and concrete](https://stackoverflow.com/questions/5201852/what-is-a-thread-really) terms is difficult. Currently, at my level of understanding, I don't think I can describe the literal and physical characteristics of a thread; for now  analogy and metaphor serves me better for my own understanding.

I like the idea of Google Docs as a metaphor for threads. In a Google Doc there is the ability for multiple people to edit a document's contents simulataneously (what a feat!). One person may have created the doc (the main thread from the program itself), but they invited 6 other people (created 6 threads) to read and **edit** the document (and in real-time!). Each person has access to the contents (data) of the Google Doc, and can make changes.

I see threads as being closely linked with functions. When we created a thread above, we tied it's execution *to* a function. Kicking off a thread resulted in a function being invoked; it's just that *that* function was able to run at the same time as the execution of other code. This thread-function can run in parallel to other code, and like other functions, is able to access it's own local state; global state, and do all the other cool things that functions do.


# When would I use threads? Why would I want to write a multi-threaded program?

Generally, you'd use threads to build faster programs.

Threads enable you to do parallel programming. You can make programs that run faster on hardware with multiple cores. Picking when you do / don't need to take advantages of threads is a case by case scenario, and generally something you want to carefully consider (see "why is it often difficult to write multi-threaded programs?" ahead).

Here are a handful of places you might find threads being used:

-   handling lots of I/O (input/output) &#x2013; maybe you're reading or writing large amounts of data to disk and want to simultaneously perform other operations.
-   handling http requests in a web server.
-   you want to prioritize work in order of importance.

## A shift towards parallel programming as a standard?

In general, multi-core programming has been gaining a lot of interest (as well as languages that purport to make it easier to write / maintain parallel programs than it has been in the past). In 2005 Herb Sutter published "The Free Lunch Is Over: A Fundamental Turn Toward Concurrency in Software" which details a prescient acknowledgement of coming changes:

> "If you’re a software developer, chances are that you have already been riding the “free lunch” wave of desktop computer performance. Is your application’s performance borderline for some local operations? “Not to worry,” the conventional (if suspect) wisdom goes; “tomorrow’s processors will have even more throughput, and anyway today’s applications are increasingly throttled by factors other than CPU throughput and memory speed (e.g., they’re often I/O-bound, network-bound, database-bound).” Right?
> Right enough, in the past. But dead wrong for the foreseeable future.
> 
> The good news is that processors are going to continue to become more powerful. The bad news is that, at least in the short term, the growth will come mostly in directions that do not take most current applications along for their customary free ride.
> 
> [&#x2026;]
> 
> Starting today, the performance lunch isn’t free any more. Sure, there will continue to be generally applicable performance gains that everyone can pick up, thanks mainly to cache size improvements. But if you want your application to benefit from the continued exponential throughput advances in new processors, it will need to be a well-written concurrent (usually multithreaded) application. And that’s easier said than done, because not all problems are inherently parallelizable and because concurrent programming is hard.
> 
> -   <https://www.cs.utexas.edu/~lin/cs380p/Free_Lunch.pdf>

Stutter's paper was written in 2005; it's interesting to read [discussions](https://softwareengineering.stackexchange.com/questions/212916/is-the-free-lunch-over) and [reflections](http://henrikeichenhardt.blogspot.ca/2013/06/free-lunch-for-programers.html) on it, especially years later. Katherine Kox's recently publised book "Concurrency in Go" launches right into these topics within the first chapter with the title "Moore’s Law, Web Scale, and the Mess We’re In". The chapter provides a succinct and descriptive summary of the history of Moore's law and how it relates to concurrency and parallel programming today.

# Why is it often difficult to write multi-threaded programs?

We've gotten a few terms covered (threads, processes, parallel programming) and now I want to talk about *why* multicore programming can be challenging. We'll start with the concrete example of a "race condition" and then talk a little more abstractly about *shared mutable state*.


## Race Conditions

Let's check in with our friend Wikipedia to see how digestible the [entry on "Race conditions"](https://en.wikipedia.org/wiki/Race_condition) is:

> A race condition or race hazard is the behavior of an electronics, software, or other system where the output is dependent on the sequence or timing of other uncontrollable events. It becomes a bug when events do not happen in the order the programmer intended.

Not bad, not bad! (I say inquisitevly, looking into the distance, my hand on my chin 🤔). I scroll down a little further to the section on race conditions in [software](https://en.wikipedia.org/wiki/Race_condition#Software):

> Critical race conditions often happen when the processes or threads depend on some shared state. Operations upon shared states are critical sections that must be mutually exclusive. Failure to obey this rule opens up the possibility of corrupting the shared state.

Well, well, well. What do we have here? SHARED STATE. I wasn't going to talk about *YOU* for another section, but here you are. For now, let's look at an example of a potential race condition, this time in the Go programming language:

```go
package main

import (
    "fmt"
)

var num_rings int = 0

func main() {
    go incrementRings()
    fmt.Print("Number of rings to rule them all: ", num_rings)
}

func incrementRings () {
    num_rings++
}
```

The program above, outputs the following when run:
```sh
❯ go run main.go
Number of rings to rule them all: 0
```

NB: *the above Go snippet is a program that can execute in parallel, thanks to Golang's "goroutines" (which I won't get into here). I think it looks a lot cleaner and simpler than working with threads in our C example, but I'm not as familiar with C as Go (and that's not saying much)&#x2026; so! Bias acknowledged - let's move on to discuss this race condition.*

There's a race condition here! In the small program above, we have some global, mutable state - `num_rings`. When I compile and run the golang program, it executes the main funciton. If we didn't know anything about Go's syntax, we might think that when reading `main()` line by line, the expression `go incrementRings()` would literally, *go and increment the number of rings*. when I ran it, it did not. Will it ever ? It could, because there's a race condition! This example is very arbitrary - and race conditions usually are not &#x2013; they are often hiding deep in layers of code. But our example will do for now.

## Shared mutable state

From what I've gathered reading and listening, shared mutable state is often at the root of the difficulties surrounding concurrent programming. I think of "shared mutable state" as data structures that can be overwritten, changed, or altered from anywhere in your program. In contrast, immutable data is data that will never be changed &#x2013; any "alterations" would involve creating "clones" of data with whatever changes made in the new copy. Your definition may vary. I like this [brief post](https://benmccormick.org/2016/06/04/what-are-mutable-and-immutable-data-structures-2/) describing the differences between mutable and immutable objects in Javascript.

In the example above, we have `num_rings` &#x2013; a piece of data that stores a number intended to increment. That piece of *state* (or data) *mutates* (or changes) over time. Data and how it changes is what makes our programs non-trivial, and furthermore, the introduction of making and instructing change to happen in *parallel* is where things become difficult.

Many programming languages offer abstractions over handling parallel programming. You've already seen syntactic difference in how languages like Go handles concurrency compared to C; but behind these special grammers are powerful ideas and abstractions. Some popular abstractions/methodologies for performing parallel programming include CSP (communicating sequential processing) the Actor model, Software Transactional Memory. We'll talk about these shortly.

# Green Threads

Before we start looking at different concurrency models and approaches to abstracting multi-thread programming, we should talk about *Green Threads*. Several higher level languages implement multi-thread programming using this notion of "Green Threads". Let's return again to the wikipedia definition of a thread and analyze it more closely:

> &#x2026;a thread of execution is the smallest sequence of programmed instructions that can be managed independently by a scheduler, which is typically a part of the operating system.

So it turns out that threads aren't always associated with an operating system: green threads are threads that are not managed by the operating system, instead they are managed by a library or a virtual machine<sup><a id="fnr.1" class="footref" href="#fn.1">1</a></sup>. As far as I can tell, green threads appeared quite a while ago, and "solved" the problem of performing *concurrent* tasks (not parallel, necessarily), especially in cases where a language couldn't be counted on to make use of the OS's threading mechanisms (recall that different hardware had different threading implementations?). By using green threads, you could be assured that as long as your VM (ie, the JVM, or the Erlang virtual machine) could run on your platform, then you could make use of threading.

Green threads appeared as a library developed by Sun Microsystems. This [thread](https://stackoverflow.com/questions/5713142/green-threads-vs-non-green-threads) on stack overflow discusses the difference between green threads and native threads; one commenter posits that green threads are out of date and a thing of the past (contextually, in the world of Java perhaps &#x2013; more on this shortly). I followed this (stack overflow) thread to an interesting [doc](https://docs.oracle.com/cd/E19455-01/806-3461/6jck06gqe/index.html) from Oracle. The doc chronicles some history of green threads, and also provides some great definitions of multithreading. I'm actively trying not to quote the entire thing right here. But for now, I will cite some interesting notes from the section on the "Many-to-one" model of multi-threading.

In this context, these terms *many-to-one*, *one-to-one*, *many-to-many* are describing the relationship between threads at the user space and the kernel space. When we're discussing green threads, we're talking about threads that are handled in the [user space](https://en.wikipedia.org/wiki/User_space) (I think this means inside a virtual machine &#x2013; at least for Java). Green threads as a "many-to-one" relationship describes having several user space threads which then connect with a single kernel thread.

Let's look at an excerpt from the docs:

> Implementations of the many-to-one model (many user threads to one kernel thread) allow the application to create any number of threads that can execute concurrently. In a many-to-one (user-level threads) implementation, all threads activity is restricted to user space. Additionally, only one thread at a time can access the kernel, so only one schedulable entity is known to the operating system. As a result, this multithreading model provides limited concurrency and does not exploit multiprocessors. The initial implementation of Java threads on the Solaris system was many-to-one, as shown in the following figure.
> 
> [source](https://docs.oracle.com/cd/E19455-01/806-3461/6jck06gqk/index.html#ch2mt-41)

So, I think that when the previously linked [stack overflow](https://stackoverflow.com/questions/5713142/green-threads-vs-non-green-threads) discussion referred to green threads as "out of date", they were perhaps referring to this original many-to-one implementation, which didn't actaully take advantage of <span class="underline">multiple processors</span>. It's important to note that the many-to-one model enabled  concurrency but not parallelism<sup><a id="fnr.2" class="footref" href="#fn.2">2</a></sup>. Regardless, it appears that Solaris moved away from many-to-one towards many-to-many<sup><a id="fnr.3" class="footref" href="#fn.3">3</a></sup>.

I've heard the words "green threads" thrown around especially in newer languages like Go, Rust and Elixir (ok, Erlang isn't new). I'm still sorting out this history, but from what I can tell, the notion of green threads is <span class="underline">not</span> old, or out-dated&#x2026; it's definitely in use&#x2013;I think it's merely that as of late, newer languages have been built to take advantage of the many-to-many model of green threads; I think that what was outdated was the "many-to-one" threading relationship.

I've found this territory particularly confusing, especially when I hear people talking about "spawning MILLIONS (holy shit!) of threads in Erlang"" and I'm just too darn embarassed (having spit out my coffee in disbelief) to shout back "BUT I THOUGHT IT WAS LIKE, JUST ONE THREAD PER CORE?!" and "DO COMPUTERS RUNNING ERLANG HAVE MILLIONS OF CORES? WHY HAVEN'T THESE COMPUTERS TAKEN OVER YET?", and so on.

Hopefully that's enough of a preface / history to take foward into discussing concurrency models.


# Concurrency Models (CSP, Actor Model, STM)

Each of these topics could probably warrant a whole book written on them, and so I think I can only afford myself a shallow dive into some examples of these concurrency models (for now).

At the very least, let's look at a) some brief history/descriptions of each concurrency model and b) find some basic code examples.


## Actor Model

Abstractly: I've mostly heard of the actor model in reference to Erlang / Elixir and Scala. I think Erlang is probably the most common / popular example of the Actor concurrency model and seems to be lauded as a powerful solution for building distributed and fault tolerant systems. But these are just things that I've heard! I have never written an erlang or elixir program, let alone a parallel one.

**Background Info**

The wikipedia page for the [Actor Model](https://en.wikipedia.org/wiki/Actor_model) hits us with this as it's introduction:

> The actor model in computer science is a mathematical model of concurrent computation that treats "actors" as the universal primitives of concurrent computation. In response to a message that it receives, an actor can: make local decisions, create more actors, send more messages, and determine how to respond to the next message received. Actors may modify their own private state, but can only affect each other through messages (avoiding the need for any locks).

Neat. I've just noticed the mention of the word "lock" now, which seems to be something I've gotten by without understanding until now. It will remain in a bit of a fuzzy state and perhaps be visited later in an advanced section. For now let's continue with Actors.

This post ["The actor model in 10 minutes"](http://www.brianstorti.com/the-actor-model/) kicks off by stating that:

> An actor is the primitive unit of computation. It’s the thing that receives a message and do some kind of computation based on it.
> 
> The idea is very similar to what we have in object-oriented languages: An object receives a message (a method call) and do something depending on which message it receives (which method we are calling).
> The main difference is that actors are completely isolated from each other and they will never share memory. It’s also worth noting that an actor can maintain a private state that can never be changed directly by another actor.

This idea of actors as "primitive" units of &#x2026; *something*, makes me feel like they are a core "thing", native to a programming language like a STRING or an INT.

Here's an intriguing excerpt from the elixir docs on getting started with [processes and concurrency](https://elixir-lang.org/getting-started/processes.html):

> In Elixir, all code runs inside processes. Processes are isolated from each other, run concurrent to one another and communicate via message passing. Processes are not only the basis for concurrency in Elixir, but they also provide the means for building distributed and fault-tolerant programs.
> 
> Elixir’s processes should not be confused with operating system processes. Processes in Elixir are extremely lightweight in terms of memory and CPU (unlike threads in many other programming languages). Because of this, it is not uncommon to have tens or even hundreds of thousands of processes running simultaneously

It's particularly interesting that we are not working with concurrency by using something directly corresponding to the hardware of a computer (ie; posix threads). These "processes", which although they sound similar to thread, are existing only inside the Erlang virtual machine. It's easy to mix up processes in Elixir vs the processes discussed earlier, so I'll try my best not to do so.

In Elixir the simplest way to work with concurrency primitives is to use the `spawn` function. The following is directly from the [Elixir docs](https://elixir-lang.org/getting-started/processes.html):

```Elixir
spawn fn -> 1 + 2 end
# PID<0.43.0>
```

The following example is intended to be executed in the shell, but I've removed the shell prompt to keep code highlighting working. In this example, `spawn` is a function that takes one argument &#x2013; a function to execute in a new process. The *act* of applying `spawn` also returns a PID - a process id (again, just like processes running on your computer.)

Elixir provides further abstractions for interfacing with processes; from what I've gathered it's more likely you'd be operating with things like "Agents" (a tool for managing state in processes easier) and "Tasks" which sort of wrap around `spawn` to make it easier to report errors and manage the call-response between actors.

Let's keep things pretty low level and look at just spawn'd processes for now.

```elixir
# Spawn a process, storing it's PID in a var

myFirstProcess = spawn(fn ->
    IO.puts "I'm a processing waiting around for a message."
    receive do
    msg -> IO.puts "Got a message! It says: #{inspect msg}"
    end
end)

send(myFirstProcess, "Hello Process!")
```

In this example, we are passing an anonymous function to execute in a new process &#x2013; it waits around for a message and then prints it; then it ceases to exist.

To keep processes alive, we'd need the function above to recur so that it can continue to wait for, and receive new messages. This post by [codeship](https://blog.codeship.com/concurrency-in-elixir/) covers that idea nicely. That's all for now!

**Further reading:**

- [The Actor Model](http://www.brianstorti.com/the-actor-model/)
- [The Hitchhiker's Guide to Concurrency](http://learnyousomeerlang.com/the-hitchhikers-guide-to-concurrency)
- [Actors in erlang and scala](https://rocketeer.be/articles/concurrency-in-erlang-scala/)
- [Concurrency in Elixir](https://blog.codeship.com/concurrency-in-elixir/)


## CSP

TODO


## Software Transactional Memory

TODO

# Concurrency Vs Paralellism

TODO

- Talk about the difficulty of distinguising between the two; different definitions etc.

> The definitions of "concurrency" and "parallelism" sometimes get mixed up, but they are not the same.
> 
> A concurrent system is one that can be in charge of many tasks, although not necessarily it is executing them at the same time. You can think of yourself being in the kitchen cooking: you chop an onion, put it to fry, and while it's being fried you chop a tomato, but you are not doing all of those things at the same time: you distribute your time between those tasks. Parallelism would be to stir fry onions with one hand while with the other one you chop a tomato.
> 
> At the moment of this writing, Crystal has concurrency support but not parallelism: several tasks can be executed, and a bit of time will be spent on each of these, but two code paths are never executed at the same exact time.
> 
> -   <https://crystal-lang.org/docs/guides/concurrency.html>


# Research and resources

-   diff between [process and a thread](https://stackoverflow.com/questions/200469/what-is-the-difference-between-a-process-and-a-thread?rq=1)
-   analogy of [a friend reading a book](https://stackoverflow.com/a/5201906/5378196) for describing a thread
-   [Excellent pthread tutorial](https://computing.llnl.gov/tutorials/pthreads/#Thread)


# Footnotes

<sup><a id="fn.1" href="#fnr.1">1</a></sup> <https://en.wikipedia.org/wiki/Green_threads>

<sup><a id="fn.2" href="#fnr.2">2</a></sup> While at this point in the doc I still have not defined the difference between concurrency and parallelism (still avoiding the wrath of being wrong on the internet.), I've personally found some that these concepts are unraveling (ha!) and revealing themselves through other related topics (in this case through these concepts of many-to-one, and many-to many relationships between a user space and a kernel space). Particularly, the notion that one can have threading on a single core machine (concurrency via [time-slicing](http://web.mit.edu/6.031/www/fa17/classes/19-concurrency/#processes_threads_time-slicing)) becomes more apparent in this case.

<sup><a id="fn.3" href="#fnr.3">3</a></sup> The detailed history of Java and Solaris is beyond the scope of this doc (and my interests, currently) so I'll just leave a link to this doc [here](https://docs.oracle.com/cd/E19455-01/806-3461/6jck06gqe/index.html).