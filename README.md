#  CSono
## A small library of C++ utilities, created mostly for learning purposes.

This library contains various utility functions, classes and templates, such as
a module that manages BSD Sockets as objects.
<br/>
The workspace is divided in modules, which can be compiled indipendently from
one another.
<br/>
Most of the modules are intended to be fully portable (compliant to C++17),
though portability is not guaranteed outside of POSIX-compliant compilers.


## Licenses

[ w.i.p. ]<br/>
MIT License (included in the repo), and other non-commercial licenses for all
the tools and resources used -
<i>which I don't know if / where / how to track and list</i>.


## Dependencies

[ w.i.p. ]<br/>
G++, GNU Make, git.


## Compiling and building

All the module object files (and test programs) can be compiled with the
<code>all</code> GNU Make default target, i.e. by using the <code>make</code>
shell command.
<br/>
To compile a single module, run
<code>make build/<i>[name]</i>.o</code>.
<br/>
To link a single test program, run
<code>make bin/<i>name</i></code>.
