#  CSono
## A small library of C++ utilities, created mostly for learning purposes.

This library contains various utility functions, classes and templates, such as
a module that manages BSD Sockets as objects.
<br/>
The workspace is divided in modules, which can be compiled indipendently from
one another.
<br/>
The project is meant to be built on POSIX-compliant systems, but the code is
portable where possible.


## Licenses

[ w.i.p. ]<br/>
MIT License (included in the repo), and other non-commercial licenses for all
the tools and resources used -
<i>which I don't know if / where / how to track and list</i>.


## Dependencies

[ w.i.p. ]<br/>
G++, GNU Make, git.


## Installation

Installing the library means moving all (exposed) headers and the static
library to <code>/usr/local/...</code> - this can be done by running
<code>sudo make</code> or <code>sudo make install</code> as superuser.
<br/>
To undo the operation, run <code>sudo make uninstall</code>.


## Compiling and building

All the module object files (and test programs) can be compiled with the
<code>all</code> GNU Make target, i.e. by using the <code>make all</code>
shell command.
<br/>
To compile a single module, run <code>make build/<i>[name]</i>.o</code>;
<br/>
To link a single test program, run <code>make bin/<i>name</i></code>;
<br/>
To compile the static library, run <code>make lib/libcsono.a</code>.
