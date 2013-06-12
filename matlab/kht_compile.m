function kht_compile()
%KHT_COMPILE	Kernel-based Hough transform wrapper function compilator.
%   KHT_COMPILE compiles the MEX-file used by the wrapper function LINES = KHT(IMAGE,...).
%   Such MEX-file calls the C++ implementation of the improved Hough transform voting
%   scheme proposed by Fernandes and Oliveira in
%
%      Leandro A. F. Fernandes, Manuel M. Oliveira
%      Real-time line detection through an improved Hough transform voting scheme
%      Pattern Recognition (PR), Elsevier, 41:1, 2008, 299-314. <a href="matlab:web('http://dx.doi.org/10.1016/j.patcog.2007.04.003')">DOI</a> <a href="matlab:web('http://www.inf.ufrgs.br/~laffernandes/kht.html')">Project Page</a>
%
%   The complete description of the implemented techinique can be found at the
%   above paper. If you use this implementation, please reference the above paper.
%
%   See also <a href="matlab:help('kht')">KHT</a>

% Copyright (C) 2008 Leandro A. F. Fernandes and Manuel M. Oliveira

base_path = fileparts([mfilename('fullpath') '.m']);

kht_source_path = [base_path filesep '..' filesep 'kht_source'];
mex_source_path = [base_path filesep 'mex'];

mex_command = ['mex ' ...
    '-O ' ...
    '-I''' kht_source_path ''' ' ...
    '-outdir ''' base_path ''' ' ...
    '''' mex_source_path filesep 'kht_wrapper.cpp'' ' ...
	'''' kht_source_path filesep 'buffer_2d.cpp'' ' ...
	'''' kht_source_path filesep 'eigen.cpp'' ' ...
	'''' kht_source_path filesep 'kht.cpp'' ' ...
	'''' kht_source_path filesep 'linking.cpp'' ' ...
	'''' kht_source_path filesep 'peak_detection.cpp'' ' ...
	'''' kht_source_path filesep 'subdivision.cpp'' ' ...
	'''' kht_source_path filesep 'voting.cpp'' '];

eval(mex_command)

disp(' ')
disp('The KHT MEX-file was compiled.')
disp('Now you can use the LINES = KHT(IMAGE,...) function.')
disp(' ')