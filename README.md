# File-Infector-Virus
Created a self-replicating file-infector virus that will copy itself to a new file within the directory it is executed in, then whenever that file is run, it will happen again.

# Introduction

A virus is a program that reproduces its own code by attaching itself to other executable files in such a way that the virus code is
executed when the infected executable file is executed. Although thousands of viruses exist today, only a fraction of them are really
a serious threat at a given time. Many viruses only exist in the databases of well-known anti-virus companies and are obsolete by now 
(i.e., when was the last time you saw a boot-sector virus?). Nevertheless, there exist enough worms and viruses that continue to cause 
significant damages to unsuspecting users and careless administrators. Hence, the virus threat has to be taken seriously by any
security specialist. The aim of this (final) challenge is to give the student a little practical insight into the techniques viruses
may use to infect other programs in order to propagate and survive. We aim to show the student that viruses, in fact, are nothing
magical (wake up Hollywood! ;-)) and that it is no problem for any advanced programmer to write a simple virus in a short period of
time.

# Storyline

You have smashed the stack, you have reverse engineered applications, and you have spent many hours debugging C code by now. You've 
been there and you've done it all. It is time for a new challenge for you, so you simply quit your job and move to Russia to work for 
a small anti-virus company. The "company" actually consists of 3 guys who are really into virus detection. They know the business and 
are making a lot of money by now. They are somewhat impressed by what you have to offer. Not many people can claim that they have 
experience with buffer overflows, reverse engineering, GOT exploits, and remote attacks. However, you have had no experience with 
viruses until now :-( The "virus guys" know this of course, and want to find out how good you really are. They tell you to write a 
simple Linux virus as fast as possible. Go ahead. Show them what you're made of. Hmmm... what was that thing you had to program back 
in your university days? Was the course called SeftVolSoc2? Oh well, whatever ;-) Life moves on.

# Detailed Description

Your task is to write a simple Linux virus that infects one Linux ELF executable at a time in the directory that it is started in. 
Your virus may be coded in any language that is available on the lab computers (e.g., scripting languages, C, etc.). Your virus may
also make use of any temporary files in order to achieve its purpose. However, if you use temporary files, then make sure that you
cleanup afterwards and be sure to use the /tmp directory(!).

Your virus will at first be embedded in a "bootstrap" C application (i.e., that is in fact a virus dropper) called virus.c. Once 
compiled and started, the virus application will infect the first Linux ELF executable that it finds in the current directory (in any
order). Note that your virus should only infect ELF executables and not executable scripts or similar. Remember, this is a virus, so 
it has to reproduce itself. That is, whenever an infected program is executed, the virus should find another file and should infect
it. Furthermore, files that have already been infected should not be infected again.

The "payload" of your virus is a simple message: Every time an application that is infected is started, it should print "Hello! I am a
simple virus!\n" to <stdout> before or after it completes its task. For example, suppose that your simple virus has infected the 
  executable /bin/echo. After the infection, if echo is started as:

echo Dude

it should display:

Dude
Hello! I am a simple virus!

or...

Hello! I am a simple virus!
Dude

Of course, needless to say, the original functionality of the host application (i.e., the program that got infected) should not 
change. All command-line arguments should work. However, for the sake of simplicity, you can ignore environment variables (we will not
test this). If a file is write protected, your virus should not crash or display weird messages (i.e., think of possible errors that
can happen and catch them).

Please note that whatever you write, it should work in our lab environment. Test your virus on the lab machines and make sure that it
works there before you submit it to us. You can hardcode any environment-relevant information into your virus to make it work on the
lab machines.
