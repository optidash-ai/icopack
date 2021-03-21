Icon Tool
---------
A simple command line tool to create multi-frame ICO files from PNG source images.<br>

The ICO file format allows either uncompressed or PNG compressed images. The PNG images are stored as-is inside the file and accessed through a simple file header plus frame index. The PNG files can be palette or full color, but must be no larger than 256x256.<br>

This tool simply creates a valid ICO header and "glues" together the PNG files provided. Any other types of images will be rejected.



