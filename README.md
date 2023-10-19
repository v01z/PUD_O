# PUD_O
**Package Use Dir Organizer** for Gentoo Linux.
<br>
This application aims to help organize configuration settings located inside */etc/portage/package.use* directory. It removes all commented lines, sorts use-flags on each package and sorts all packages list, reorganizes filenames bringing them to the form of category names.

## Building
`mkdir build && cd $_`
<br>
`cmake ..`
<br>
`cmake --build .`
<br>

## Usage
`sudo ./pudo`
<br><br>
If you want to left some confg files not get modified, then add a keyword "--exclude" and list them separated by space, for example:
<br>
`sudo ./pudo --exclude steam libre`