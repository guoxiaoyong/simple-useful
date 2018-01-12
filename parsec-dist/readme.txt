PARSEC version 1.1

1. Contents
./license.txt: License agreement
./readme.txt : This file
./bin/       : PARSEC compiler executable (pcc)
./include/   : Header files for PARSEC
./runtime/   : Runtime libraries

2. Requirement for installation
PARSEC simulation environment requires a C compiler. It
works with Visual C++ 6.0 on Windows, Sun C compiler on
Solaris, xlc on AIX and gcc if the target system has it. You
can check the default C compiler by typing "pcc -env" after
installation. By default, the linker is set to a C++ compiler
for the convenience of linking C++ object files with the
object files compiled by pcc, but the use of C++ compiler is
not necessary. If you do not have a C++ compiler properly
installed, you can change the linker to the C compiler. See
the next section for changing the settings.

3. Installation
(For UNIX based machines)
Each package contains precompiled binaries. If you have
permission to create /usr/local/parsec, the installation is
easy; just copy the whole package under
/usr/local/parsec. If you do not have permission to create a
directory under /usr/local, create a directory anywhere you
like, and set the designated directory to an environment
variable "PCC_DIRECTORY." For instance, you can set the
variable by typing "setenv PCC_DIRECTORY /home/foo/parsec"
if you are using (t)csh.

(For Windows)
The default directory of the packages for Windows based
operating systems is c:\parsec. You can change the directory
in the same way as UNIX based machines.

4. Environment variables and compiler options
pcc checks the following environment variables when
executed:

PCC_DIRECTORY      : Directory that pcc looks up
PCC_CC             : C compiler used for preprocessing and compiling
PCC_LINKER         : Linker used for linking
PCC_PP_OPTIONS     : Options for preprocessing
PCC_CC_OPTIONS     : Options for compiling
PCC_LINKER_OPTIONS : Options for linking

You can set these variable and pcc will change the
settings. In general, the default settings should be
fine. In addition to this, version 1.1.1 has the command
line options corresponding to those environment variables:

-pcc_directory
-pcc_cc
-pcc_linker
-pcc_pp_optoins
-pcc_cc_options
-pcc_linker_options

These command line options can override the environment
variables and are useful when changing options temporarily.
There are other command line options besides those. You can
see brief descriptions of those options by typing "pcc -help"
For detailed descriptions, you can read the manual for
PARSEC on the web:
    http://pcl.cs.ucla.edu/projects/parsec
The following section describes the changes from PARSEC
version 1.0 and should be in the on-line manual soon.

5. Changes from version 1.0
5-1. New features in version 1.1
Version 1.1 supports two types of clocktype: "unsigned int"
and "long long" (the default clocktype is "unsigned int").
You can change the clocktype for the long term simulation on
the command line:
    pcc -clock longlong ... /* for long long clocktype */
Please note that execution with long long clocktype is
slower than the one with unsigned int cocktype.

Since casting a clocktype variable to a variable whose range
is shorter than clocktype causes unexpected results, we do
not support the functions ctoi() and ctof(). Instead,
version 1.1 introduces two new functions:
ctoa(clocktype, char []) and atoc(char[], clocktype).
ctoa() can alternate the role of ctoi() and ctof() if ctoi()
or ctof() was used for printing out the clock values. ctoi()
and ctof() should not have been used other than this purpose.

If you want to use clocktype in .c files, you can include
PCC_DIRECTORY/include/pc_api.h. This is useful if the file
contains non-ANSI grammar or it is C++ file. Please note
that when the clocktype needs to be long long, all the files
including pc_api.h header file must be compiled with
CLOCK_LONGLONG definition. You should use pcc for linking
those object files with .pc files.

5-2. List of incompatibilities from version 1.0
  * -pcc_* options (new)
    See Section 4.
  * -clock option (new)
    See Section 5-1.
  * ctoa() and atoc() functions (new)
    See Section 5-1.
  * pc_printf() and pc_fprintf() (new)
    These correspond to printf() and fprintf(). They print
    out node (processor) number for parallel execution.
  * pc_print_runtime(void) (new)
    It prints out the running time.
  * pc_exit(int) (new)
    It is used for aborting the program. Finalize statement
    will not be executed when this function is called.
  * nnodes() to pc_num_nodes(changed)
    For the consistency nnodes() changed its name to
    pc_num_nodes().
  * -protocol (-sync) option (changed)
    "-sync" is changed to "-protocol" for better
    understanding of the option. -sync is still available
    for backward compatibility. Because of changes of
    runtime library names in version 1.1, -protocol option
    for sequential execution is "-protocol gel," not
    "-protocol seq." However, sequential runtime library is
    chosen by default and -protocol option is not required.
  * -arch option (obsolete)
    No -arch option in version 1.1. It selects an
    appropriate architecture type automatically.
  * ctoi() and ctof() functions (obsolete)
    See Section 5-1.
  * clock*() functions (obsolete)
    Version 1.1 does not support the following clock
    operation functions to make clock operations clearer:
        clockset(a,b) => replace with ((a) = (b))
        clockadd(a,b) => replace with ((a) + (b))
        clockmpl(a,b) => replace with ((a) * (b))
        clocksub(a,b) => replace with ((a) - (b))
        clockdvd(a,b) => replace with ((a) / (b))
        clockeq(a,b)  => replace with ((a) == (b))
        clockle(a,b)  => replace with ((a) <= (b))
        clockge(a,b)  => replace with ((a) >= (b))
        clocklt(a,b)  => replace with ((a) < (b))
        clockgt(a,b)  => replace with ((a) > (b))

6. Architecture dependent issues
6-1. Parallel runtime libraries
Parallel runtime libraries are currently available for
solaris, x86-solaris, irix, aix, windows-nt, but they are
not included in the distributions. If you are interested in
using those, please contact us via email. Note that
Windows95 and SunOS do not support multi-processor
machines.

6-2. Linux
Linux (Redhat) has a compatibility problem with system()
function and pcc compiled on Redhat 5.0 does not run on
Redhat 4.x. We made two versions of packages for Linux for
this purpose, but we have not tested those on a variety of
machines running Linux (We will support only Redhat for
Linux).

6-3. SunOS
stdio.h in SunOS does not have prototypes of printf() and
fprintf() (You may find other functions whose prototypes are
supposed to be in stdio.h). Because of this, pcc will
complain those functions when the file to compile has
those. It is annoying but you can ignore those warnings.

6-4.  Windows 2000
As far as we can tell, the Windows NT 4.0 distribution works
fine on Windows 2000 Professional.
