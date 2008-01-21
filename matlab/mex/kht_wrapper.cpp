/*
 * Copyright (C) 2008 Leandro A. F. Fernandes and Manuel M. Oliveira
 *
 * author   : Fernandes, Leandro A. F.
 * e-mail   : laffernandes@gmail.com
 * home page: http://www.inf.ufrgs.br/~laffernandes
 *
 *
 * The complete description of the implemented techinique can be found at
 *
 *      Leandro A. F. Fernandes, Manuel M. Oliveira
 *      Real-time line detection through an improved Hough transform voting scheme
 *      Pattern Recognition (PR), Elsevier, 41:1, 2008, 299-314.
 *      DOI: http://dx.doi.org/10.1016/j.patcog.2007.04.003
 *      Project Page: http://www.inf.ufrgs.br/~laffernandes/kht.html
 *
 * If you use this implementation, please reference the above paper.
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://www.gnu.org/licenses/.
 */

#include "mex.h"
#include "kht.h"

// The MEX-file gateway function.
void
mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	// Read input arguments (the image was transposed).
	size_t image_width = static_cast<size_t>( mxGetM( prhs[0] ) );
	size_t image_height = static_cast<size_t>( mxGetN( prhs[0] ) );
	unsigned char *binary_image = static_cast<unsigned char*>( mxGetData( prhs[0] ) );
	
	size_t string_min_size = static_cast<size_t>( mxGetScalar( prhs[1] ) );
	double string_min_deviation = mxGetScalar( prhs[2] );
	double delta = mxGetScalar( prhs[3] );
	double kernel_min_height = mxGetScalar( prhs[4] );
	double n_sigmas = mxGetScalar( prhs[5] );
	
	// Execute the proposed Hough transform voting scheme and the peak detection procedure.
	static lines_list_t lines;

	kht( lines, binary_image, image_width, image_height, string_min_size, string_min_deviation, delta, kernel_min_height, n_sigmas );
	
	// Create and populate the resulting array of detected lines.
	size_t lines_count = lines.size();

	plhs[0] = mxCreateDoubleMatrix( lines_count, 2, mxREAL );
	double *dbuffer = mxGetPr( plhs[0] );

	for (size_t i=0; i!=lines_count; ++i)
	{
		line_t &line = lines[i];
		
		dbuffer[i            ] = line.rho;
		dbuffer[i+lines_count] = line.theta;
	}
}
