# File-Infector-Virus
Created a self-replicating file-infector virus that will copy itself to a new file within the directory. Then, whenever the infected  file is run, it will execute its normal functionality and infect another file in the directory. 

## Introduction

A virus is a program that reproduces its own code by attaching itself to other executable files in such a way that the virus code is
executed when the infected executable file is executed. Although thousands of viruses exist today, only a fraction of them are really
a serious threat at a given time. Many viruses only exist in the databases of well-known anti-virus companies and are obsolete by now 
(i.e., when was the last time you saw a boot-sector virus?). Nevertheless, there exist enough worms and viruses that continue to cause 
significant damages to unsuspecting users and careless administrators. Hence, the virus threat has to be taken seriously by any
security specialist. The aim of this (final) challenge was to give the student a little practical insight into the techniques viruses
may use to infect other programs in order to propagate and survive. Viruses, in fact, are nothing magical (wake up Hollywood! ;-)) and  
it is no problem for any advanced programmer to write a simple virus in a short period of time.

## Detailed Description

Wrote a simple Linux virus that infects one Linux ELF executable at a time in the directory that it is started in.
Remember, this is a virus, so it has to reproduce itself. That is, whenever an infected program is executed, the virus will find another file and should infect it. Furthermore, files that have already been infected should not be infected again.

The "payload" of this virus is a simple message: Every time an application that is infected is started, it will print "Hello! I am a
simple virus!\n" to <stdout> before or after it completes its task. For example, suppose that the virus has infected the 
  executable /bin/echo. After the infection, if echo is started as:

echo Dude

it should display:

Dude
Hello! I am a simple virus!

or...

Hello! I am a simple virus!
Dude

Of course, needless to say, the original functionality of the host application (i.e., the program that got infected) will not 
change. All command-line arguments work. However, for the sake of simplicity, I ignored environment variables. If a file is write protected, my virus does not crash or display weird messages.
