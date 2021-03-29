<p align="center"><a href="https://optidash.ai"><img src="media/logotype.png" alt="Optidash" width="143" height="45"/></a></p>

<p align="center">
Optidash is a modern, AI-powered image optimization and processing API.<br>We will drastically speed-up your websites and save you money on bandwidth and storage.
</p>

---
<p align="center">
<strong>A command line tool to create multi-frame ICO files from PNG source images.</strong><br>
<br>
<img src="https://img.shields.io/github/issues-raw/optidash-ai/icopack?style=flat&color=success"/>
</p>

---

### Intro
This tool simply creates a valid ICO header and "glues" together the PNG files provided. Any other types of images will be rejected. `icopack` is the most efficient way to create Favicons.

### Background
The ICO file format allows either uncompressed or PNG compressed images. The PNG images are stored as-is inside the file and accessed through a simple file header plus frame index. The PNG files can be palette or full color, but must be no larger than 256x256.

### Building
```bash
$ make
```

### Usage
```bash
$ icopack output.ico icon-1.png icon-2.png icon-N.png
```

### License
This software is distributed under the GNU General Public License, Version 3. See the [LICENSE](LICENSE) file for more information.