= KERNEL-BASED HOUGH TRANSFORM FOR DETECTING STRAIGHT LINES IN IMAGES ===========


 Copyright (C) 2008 Leandro A. F. Fernandes and Manuel M. Oliveira

 author   : Fernandes, Leandro A. F.
 e-mail   : laffernandes@gmail.com
 home page: http://www.inf.ufrgs.br/~laffernandes


 The complete description of the implemented techinique can be found at

     Leandro A. F. Fernandes, Manuel M. Oliveira
     Real-time line detection through an improved Hough transform voting scheme
     Pattern Recognition (PR), Elsevier, 41:1, 2008, 299-314.
     DOI: http://dx.doi.org/10.1016/j.patcog.2007.04.003
     Project Page: http://www.inf.ufrgs.br/~laffernandes/kht.html

 If you use this implementation, please reference the above paper.



= LICENCE =======================================================================


 All code is released under the GNU General Public License, version 3.



= PLATFORMS =====================================================================


 We have compiled and tested the sample application and the MATLAB wrapper
 function on Windows. The source code is compatible with GCC, so we believe
 that it works on Linux, too. We compiled the sample application using Visual
 Studio 2005 and Visual Studio 2008 Express Edition (projects files are in the
 'sample_app' directory). We compiled the MEX-file using Visual Studio 2005 and
 MATLAB 7 (M-files and MEX -functions are in the 'matlab' directory).



= REQUIRED LIBRARIES ============================================================


 The reference implementation (in the 'kht_source' directory) does not require
 any external library. However, the sample application uses DevIL, GLUT or
 freeglut and GLEW.



= RUNNING THE SAMPLE APPLICATION ================================================


 On Visual Studio, simply open the project file and run it (CTRL+F5).

 Tip: compile the application using Release configuration in order to achieve
      better frame rates.



= RUNNING THE MATLAB WRAPPER FUNCTION ===========================================


 First, you need to compile the MEX-file used by the wrapper function:

     1. Open MATLAB and set the 'matlab' directory of the downloaded source
        code as current directory;

     2. Choose a compiler for building external interface (MEX) files by
        calling 'mex -setup' in MATLAB command window. If you do not know how
        to use the 'mex' command, call 'help mex' or 'doc mex';

     3. Call the 'kht_compiler' function and wait the MEX-file compilation.

 After the MEX-file compilation, the wrapper function can be used. The
 'main.m' file in 'matlab' directory provides a simple example of how to use
 the 'kht' function. Call 'help kht' to see the function usage.

 Tip: by adding the 'matlab' directory to the MATLAB search path (see
      'help addpath') you can call the 'kht' function from any current
      directory.



= BUILDING YOUR OWN C++ APPLICATION =============================================


 To use the reference implementation of the proposed Hough transform, you
 should include the files from the 'kht_source' directory into your project and
 then call the 'kht' function declared in the 'kht.h' header file.

