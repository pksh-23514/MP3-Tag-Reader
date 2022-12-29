# MP3-Tag-Reader

## Abstract
The MP3 Tag reader is a Software which will read and display MP3 (ID3) tag information from MP3 files. The software will be Desktop based. The project will be useful to individuals who wish to View and Collect MP3 tag data. This project can be extended to implement a Tag editor, wherein users can Modify MP3 tag information.

## A Brief Introduction to ID3 Standard and Tags
ID3 is a Metadata Container most often used in conjunction with the MP3 Audio File format. It allows information such as the Title, Artist, Album, Track number and other informations about the File to be stored in the file itself. There are two unrelated versions of ID3: ID3v1 and ID3v2. You will have to first detect the ID3 version before getting the tag details.

Total 128 bytes are used for storing this Metadata. If you sum the Size of all these fields we see that (30+30+30+4+30+1) equals 125 bytes and not 128 bytes. The missing 3 bytes can be found at the very end of the tag before the song title. These 3 bytes are always “TAG” and is the identification that this is indeed an ID3 tag. As all artists doesn’t have a 30 character name, it is said that if there are some bytes left after the information is entered in the field and those bytes should be filled with the binary value '0'.

An ID3v2 tag starts with a Tag header followed by one or more Frames. Each frame in turn consists of a Header and (usually) some kind of content. The ID3v2 tag is more flexible and hence more difficult to work with. An ID3v2 tag has a signature code of “ID3x” where x is the sub-version number of the tag. Typically, ID3v2 tags are found at the beginning of an MP3 file but this is not an absolute restriction. What happens next depends on the subversion of the ID3v2 tag. As far as we can tell, there have been three versions so far: 2,3 and 4. Some important details about the headers are given below.

**ID3v2 Frame header:**<br>
ID3v2/file identifier “ID3”<br>
ID3v2 version $03 00 (2, 3 or 4)<br> 
ID3v2 flags %abc00000<br>
ID3v2 size 4 * %0xxxxxxx (Total size of ID3 tag)<br>

**ID3v2.2 Frame header:**<br>
Frame ID $xx xx xx(Three characters) [e.g.: TAL Album / Movie / Show title]<br>
TOA Original artist(s) / performer(s)<br>
Size $xx xx xx(Three characters)<br>

**ID3v2.3 or ID3v2.4 Frame header:**<br>
Frame ID $xx xx xx xx (Four characters) [e.g.: TAL Album/Movie/Show title]<br>
TIT2 Title / song name /content description<br>
Size $xx xx xx xx (Four Characters)<br>
Flags $xx xx<br>
