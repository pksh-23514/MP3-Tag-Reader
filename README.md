# MP3-Tag-Reader
## Abstract
The MP3 tag reader is a software which will read and display MP3 (ID3) tag information from MP3 files. The software will be desktop based and not web based. The project will be useful to individuals who wish to view and collect MP3 tag data. This project can be extended to implement a tag editor, wherein users can modify mp3 tag information.
A Brief Introduction to ID3 Standard and Tags

ID3 is a metadata container most often used in conjunction with the MP3 audio file format. It allows information such as the title, artist, album, track number, and other information about the file to be stored in the file itself. There are two unrelated versions of ID3: ID3v1 and ID3v2. You will have to first detect the ID3 version before getting the tag details.

Total 128 bytes used for storing this metadata. If you one sum the size of all these fields we see that 30+30+30+4+30+1 equals 125 bytes and not 128 bytes. The missing three bytes can be found at the very end of the tag, before the song title. These three bytes are always “TAG” and is the identification that this is indeed an ID3 tag. As all artists doesn’t have a 30 character name it is said that if there are some bytes left after the information is entered in the field, those bytes should be filled with the binary value 0.

An ID3v2 tag starts with a tag header followed by one or more frames. Each frame in turn consists of a header and (usually) some kind of content. The ID3v2 tag is more flexible and hence more difficult to work with. An ID3v2 tag has a signature code of “ID3x” where x is the sub-version number of the tag. Typically, ID3v2 tags are found at the beginning of an MP3 file, but this is not an absolute restriction. What happens next depends on the subversion of the ID3v2 tag. As far as we can tell, there have been three versions so far: 2,3 and 4. Some important details about the headers are given below.
