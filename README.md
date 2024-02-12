# pudo
**Package Use Dir Organizer** for Gentoo Linux.


# <span style="color:red">Project moved to [codeberg.](https://codeberg.org/v01z/pudo) </span>


<br>
The application aims to help organize configuration settings located inside */etc/portage/package.use* directory. It removes all commented lines, sorts use-flags on each package and sorts all packages list, reorganizes filenames bringing them to the form of category names.

## Building
`mkdir build && cd $_`
<br>
`cmake ..`
<br>
`cmake --build .`
<br>

Alternatively it is possible to install **pudo** system wide using [ready ebuild](https://github.com/cyberhoffman/portage/tree/main/app-portage/pudo).

## Usage
`sudo ./pudo`
<br><br>
If you want to prevent some config files from being get modified, then add a keyword *"--exclude"* and list all of them separated by space, for example:
<br>
`sudo ./pudo --exclude steam libre`



## TODO in the future
Removing duplicates and managing lines that contain the same package name but have different version info will be implemented further and not included in the current release.
