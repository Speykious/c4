# My experience making this game from absolute scratch in C

> 2024-03-20

It's been a long time since I wrote any amount of C code. The last time I did that was in university, where we had to write our own implementations of a linked list, hashmap and quad tree, and compare the performance of all of them in various scenarios. And they were all implemented with extensive use of `malloc` and `free`. Naturally I got a few segfaults here and there, and I expected that.

I come mainly from Rust, which is the one memory-safe language without a garbage collector that continues to gain traction more and more in all corners of the software industry (so much so that it got all the way to the White House). I've been coding in Rust regularly for about 4 years and I'd say I'm very productive with it. At my software development job, which mainly has an old Java tech stack, I've been advocating for using it as much as possible and have managed to make my colleagues interested in the language, so now we use it here and there for the main project's configuration scripts and for some cronjobs, and it's only going to grow from there.

Why am I doing this game in C then? As a Rust developer, am I not supposed to absolutely *despise* and *tremble in fear* at the idea of writing C code and having it littered with potential security vulnerabilities and way more bugs in general? Well, to a less exaggerated extent, sort of. After all, Rust is designed in such a way that you just tend to make APIs where the types force you to handle them in the way that you intended when you wrote them, to a much greater extent than C, while being in the same performance ballpark. Indeed, I'm losing the power of Rust's type system when doing this.

The reason is that I want to learn several new coding patterns that are alien to Rust, and I want to call this paradigm "performance-aware programming". These include extensive usage of various types of allocators instead of using `malloc` and `free`, intrusive data structures, zero-initialization, *maybe* using nil-objects sometimes, and no generic code apart from a handful of macros here and there. These are patterns promoted by game developers such as [Casey Muratori](https://www.youtube.com/@MollyRocket) and [Ryan Fleury](https://www.rfleury.com/), who tend to have quite controversial opinions. Examples include [this video titled "Clean Code, Horrible Performance"](https://youtu.be/tD5NrevFtbU) and [this article on handling errors by not having them](https://www.rfleury.com/p/the-easiest-way-to-handle-errors) (they both bring interesting points but are quite inflammatory).

Going further, the reason I want to learn these coding patterns in the first place is because I really care about performance of software. I'm tired of having all my apps either be the ugliest shit I've ever seen or be written with bloated web technologies on Electron, taking several times more memory than it actually needs, takes several seconds to load when it should take *no* time, and lacks animations because it would somehow take too many resources. And when I first saw [Casey's first video on refterm](https://youtu.be/hxM8QmyZXtg), I saw a light of hope that maybe we *can* write software that is both simple and performant from the get go without actively trying to optimize, at least at first. And that's how I eventually fell in the "performance-aware programming" rabbit hole.

Sure, I could do all of this in Rust: I could use `#[no_std]`, then try to implement some allocators, try to think about using intrusive data structures instead of making generic ones, and try to avoid generic code in general — in fact, I partially did some of that when I implemented an arena allocator in Rust in [this repo](https://github.com/Speykious/hardcore-rust). But much like Rust's safety features force you to code in a safe way, C's lack of features force you to code in a way that you don't use what you want to avoid. I want to experience for myself what coding like this feels like, how it turns out in practice, what kinds of problems I encounter, so that I can reflect on these practices, and also write my thoughts here as I code along.

&nbsp;

## Intermediary rant

I've heard a lot of stuff from hardcore C developers like this. They tend to argue that Rust is actually "not memory-safe for sensible definitions of memory safety". Turns out it includes memory leaks, which let's be honest, is just a disingenuous way to throw the baby with the bathwater and disregard every memory safety feature of the language. And the irony of the matter is that now these people are using *[my own fucking joke of a project](https://github.com/Speykious/cve-rs)* to further this already deeply flawed reasoning. [I'm not even kidding](https://twitter.com/Speykious/status/1762951606620786782), it's as if they don't realize it's a compiler bug.

I can understand that from the point of view of some of these developers who don't program in what Casey calls ["individual element thinking"](https://youtu.be/dcdbPsrdX90), that Rust's abstractions are very tied to that in particular and that we can do better than this. While I have no relevant experience in their alternative yet (hence why I'm doing this project), I think this is fair. But that is completely unrelated to providing memory safety guarantees, and to use this argument in any way to claim that Rust isn't safe is just dishonest and delusional. We're at this point in time where there is [sizable](https://security.googleblog.com/2022/12/memory-safe-languages-in-android-13.html), [empirical](https://youtu.be/8T6ClX-y2AE?t=2703) [evidence](https://twitter.com/LinaAsahi/status/1583498896847667216) of Rust's undeniably tangible benefits, so let's stop pretending that they don't exist or that this kind of memory safety doesn't matter. Let's also stop pretending that simply getting more skill fixes the problem. You don't get to not wear a seatbelt just because you're the top 1 driver in the world, **you get more skills within a safe environment because it matters**, period.

### Tangential intermediary rant

the C community (at least that particular one) is very unwelcoming. The gatekeeping mentality is very strong, something I'd expect from people who use "skill issue" as a serious argument. They think that people who can't "code from first principles" or who "need" guarantees like what Rust provides shouldn't be programming at all, and they will literally mention *mental retardation* if you try to shift the line to something actually realistic. They simultaneously argue that people who code in Rust and criticize C for being unsafe are arrogant and think of themselves as geniuses, and that this same language holds the "philosophy" that the programmer is dumb, doesn't know what they're doing and therefore needs some kind of training wheels. It's all very toxic and quite overwhelmingly repulsive to me at times. And I'm basically learning from them despite that.

Forgive me for any overgeneralizations, I just wanted to express my anger and frustrations on full display. If you're a C developer reading this, you don't need to tell me what specific thing from this rant is a misrepresentation of your personal opinion. I'm not targetting you specifically.

&nbsp;

## The beginnings: making a platform layer

> 2024-03-20

I've encountered two segfaults so far.

1. I don't remember the first one very well, but it mostly had to do with me being confused as to how to properly initialize a heap-allocated variable in C. It's been a long time since I wrote any C code after all. It's mostly my fault for not being used to the language here of course, although it just wouldn't have compiled in a safe language. It's just a minor inconvenience though, a segfault is not as dramatic of an issue as a buffer overflow issue that can be exploited. It's not that different from a panic in Rust, only that in C the kernel catches the problem instead of your own program.

2. When working on the windowing part of the platform layer (the `app` module), I got a segfault immediately while testing my window spawning code. Turns out I had forgotten to call `app_init` just a few minutes after I had made that function. I had wondered if I should go with global state or do the somewhat more tedious thing of having a struct and pass it around as a context, which would've been the safer option. I opted for simplicity and it immediately bit me right in the ass. So what I did was of course, at first, to add the missing call to `app_init()` which made it work, but second, I made `app_open_window()` explicitly fail if the global variables it uses aren't initialized. That way I keep the simplicity of the API and make sure I know what happens if it crashes.

Other than that, I have another quite frustrating thing to share: ALSA has memory leaks that are completely out of my control, and apparently [it's been known for 16 years](https://alsa-devel.alsa-project.narkive.com/n3Pl3SxS/alsa-leaking-memory). (Thank you Valerie for finding this)

I tried to make ASAN ignore these leaks, but so far I failed, both custom attributes and a blacklist seem to just be ignored even for the simplest program. It's a lot of noise to filter out when I'm trying to know if I fucked anything up in my program. If I find a solution later maybe I'll activate ASAN again, but for now I think I'll only use it if I'm not doing anything related to audio.

***

> 2024-03-21

I implemented my own non-generic circular buffer for an event queue system, it was quite refreshingly simple to implement. I think in Rust it would've been something generic and with tons of helper functions I wouldn't need.

Anyways, I've encountered by 3rd and 4th segfaults today.

The first one, for some random reason, triggered when *exiting the `main()` function*. And then it disappeared as fast as it appeared. I have no clue why it happened.

The second one is still not fixed right now because I also have no clue what's going on: it triggers upon calling `XCreateSimpleWindow`. And it was working perfectly fine beforehand. What kind of thing am I doing that warrants such unpredictable behavior??
