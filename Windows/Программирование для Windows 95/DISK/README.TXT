===================================================
    "PROGRAMMING WINDOWS 95" BY CHARLES PETZOLD
                  COMPANION DISC
       COPYRIGHT (C) 1996 BY CHARLES PETZOLD
===================================================

This companion CD-ROM contains the sample source code 
and executable files for the examples presented in 
"Programming Windows 95" by Charles Petzold. We recommend 
that you store this disc in a safe place after you install 
the files.

All sample source code and executable files are copyrighted
by Charles Petzold -- except those in Chapters 12 and 20,
which are copyrighted by Paul Yao.


INSTALLATION

The files on this disc are not compressed and can be 
copied to your hard disk by running the Setup program. 
You must have at least 2.7 megabytes of free space on 
your hard disk to install the files.


DISC CONTENTS

The sample source code and executable files for each 
chapter are stored under \PETZOLD\CHAPXX, where XX is 
the chapter number. For example, all files for Chapter 2 
are in the \PETZOLD\CHAP02 directory.


COMPILING THE PROGRAMS

To compile the sample programs, you need to have installed 
Microsoft Visual C++ version 4.0. You need to include the 
following statement in your AUTOEXEC.BAT file:

     CALL \MSDEV\BIN\VCVARS32.BAT

This calls a file included with VC++ that sets up DOS environment 
variables for compiling programs from the MS-DOS command line. 
You also need to include the MSC.BAT file (located in the 
\PETZOLD\CHAP01 directory) in your AUTOEXEC.BAT file.

To compile one of the sample programs from the MS-DOS command 
line, change to the appropriate directory and type:

     NMAKE <program name>.MAK


IF THERE ARE PROBLEMS WITH THIS DISC OR THE SAMPLES

Every effort was made to ensure the accuracy of the book 
and this disc. Should you encounter a problem, please 
contact us at the following address:

    Microsoft Press
    Attn: "Programming Windows 95" Editor
    One Microsoft Way
    Redmond, WA  98052-6399

You can also send feedback to Microsoft Press via electronic mail
at msinput@microsoft.com.
