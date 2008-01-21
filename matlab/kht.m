function lines = kht(image,varargin)
%KHT	Kernel-based Hough transform for detecting straight lines in images.
%   LINES = KHT(IMAGE,...) is a wrapper function for the C++ implementation of the
%   improved Hough transform voting scheme proposed by Fernandes and Oliveira in
%
%      Leandro A. F. Fernandes, Manuel M. Oliveira
%      Real-time line detection through an improved Hough transform voting scheme
%      Pattern Recognition (PR), Elsevier, 41:1, 2008, 299-314. <a href="matlab:web('http://dx.doi.org/10.1016/j.patcog.2007.04.003')">DOI</a> <a href="matlab:web('http://www.inf.ufrgs.br/~laffernandes/kht.html')">Project Page</a>
%
%   The complete description of the implemented techinique can be found at the
%   above paper. If you use this implementation, please reference the above paper.
%
%   LINES = KHT(IMAGE,'PropertyName1',PropertyValue1,'PropertyName2',PropertyValue2,...)
%   performs the KHT procedure over a given RGB or gray scale image and returns a Nx2 
%   array with the [rho theta] parameters (rho in pixels and theta in degrees) of the
%   N detected lines. The lines are in the form
%
%       rho = x * cosd(theta) + y * sind(theta)
%
%   and we assume that the origin of the image coordinate system is at the center
%   of the image, with the x-axis growing to the right and the y-axis growing down.
%   The resulting lines are sorted in descending order of relevance.
%
%	The expected input properties are:
%
%        'cluster_min_size' : Minimum number of pixels in the clusters of approximately
%                             collinear feature pixels. The default value is 10.
%
%   'cluster_min_deviation' : Minimum accepted distance between a feature pixel and the
%                             line segment defined by the end points of its cluster.
%                             The default value is 2.
%
%                   'delta' : Discretization step for the parameter space. The default
%                             value is 0.5.
%
%       'kernel_min_height' : Minimum height for a kernel pass the culling operation.
%                             This property is restricted to the [0,1] range. The
%                             default value is 0.002.
%
%                'n_sigmas' : Number of standard deviations used by the Gaussian kernel
%                             The default value is 2.
%
%   See also <a href="matlab:help('kht_compile')">KHT_COMPILE</a>

% Copyright (C) 2008 Leandro A. F. Fernandes and Manuel M. Oliveira

% Check input arguments.
if nargin == 0
    error('Not enough input arguments.')
end

if rem(length(varargin),2) ~= 0
    error('Invalid number of input arguments.')
end

% Set input parameters.
if ndims(image) == 3
    gray = rgb2gray(image);
else
    gray = image;
end
binary = uint8(edge(gray,'canny')');

cluster_min_size = uint32(10);
cluster_min_deviation = 2;
delta = 0.5;
kernel_min_height = 0.002;
n_sigmas = 2;

for i=1:2:length(varargin)
    switch lower(char(varargin{i}))
        case 'cluster_min_size'
            cluster_min_size = uint32(varargin{i+1});
        case 'cluster_min_deviation'
            cluster_min_deviation = double(varargin{i+1});
        case 'delta'
            delta = double(varargin{i+1});
        case 'kernel_min_height'
            kernel_min_height = double(varargin{i+1});
        case 'n_sigmas'
            n_sigmas = double(varargin{i+1});
        otherwise
            error('The argument ''%s'' is unknown.',char(varargin{i}))
    end
end

% Call the MEX-File.
try
    lines = kht_wrapper(binary,cluster_min_size,cluster_min_deviation,delta,kernel_min_height,n_sigmas);
catch
    err = lasterror;
    switch err.identifier
        case 'MATLAB:UndefinedFunction'
            error('The KHT MEX-file was not compiled yet. Please, call KHT_COMPILE and try again.')
        otherwise
            rethrow(err);
    end
end