The ppm_creator creates a ppm file based off a supplied input file. It checks for any errors such as
negative numbers, missing data, and incorrect number of arguments and provides feedback.

Command line arguments:
input.txt output.ppm

input.txt is an input file that starts with the line "imsize width height" where width and height are
positive numbers. output.ppm is an output file name for the final image.