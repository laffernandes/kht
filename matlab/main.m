% LINES = KHT(IMAGE,...) function usage. The complete description of the
% implemented techinique can be found at the paper below. If you use this
% implementation, please reference the paper.
%
%    Leandro A. F. Fernandes, Manuel M. Oliveira
%    Real-time line detection through an improved Hough transform voting scheme
%    Pattern Recognition (PR), Elsevier, 41:1, 2008, 299-314.
%    DOI: http://dx.doi.org/10.1016/j.patcog.2007.04.003
%    Project Page: http://www.inf.ufrgs.br/~laffernandes/kht.html

% Copyright (C) 2008 Leandro A. F. Fernandes and Manuel M. Oliveira

close all
clear
clc

% Sample image files and number of most relevant lines.
filename       = {'simple.jpg','chess.jpg','road.jpg','wall.jpg','board.jpg','church.jpg','building.jpg','beach.jpg'};
relevant_lines = [8 25 15 36 38 40 19 19];

for i=1:length(filename)
    % Load input image.
    I = imread(filename{i});
    
    % Call the kernel-base Hough transform function.
    lines = kht(I);
    
    % Show current image and its most relevant detected lines.
    figure('Name',sprintf('KHT - Image ''%s'' - %d most relevant lines',filename{i},relevant_lines(i)))
    
    [height width c] = size(I);
    
    axes
    set(gca,'XLim',[0 height-1])
    set(gca,'YLim',[0 width-1])
    hold on
    
    warning('off','Images:initSize:adjustingMag')
    imshow(I)
    warning('on','Images:initSize:adjustingMag')
    
    for j=1:relevant_lines(i)
        if sind(lines(j,2)) ~= 0
            x = [-width/2 width/2-1];
            y = (lines(j,1) - x*cosd(lines(j,2)))/sind(lines(j,2));
        else
            x = [lines(j,1) lines(j,1)];
            y = [-height/2 height/2-1];
        end
        patch(x+width/2,y+height/2,[1 1 0],'EdgeColor','y','LineWidth',0.5);
    end
    
    hold off
end
