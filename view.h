#ifndef VIEW_H
#define VIEW_H

#include "types.h"      //Contains User-defined Data Types.

/*
 * Structure to store information required for storing the various MP3 Tag details
 * Information about Output data is also stored
 */

typedef struct _TagInfo
{
    /* Source MP3 file Info */
    FILE* fptr_mp3;
    char frame_Id [4];

    /* Title Info */
    char* title_tag;
    uint title_tag_size;

    /* Artist Info */
    char* artist_tag;
    uint artist_tag_size;

    /* Album Info */
    char* album_tag;
    uint album_tag_size;

    /* Year of Release Info */
    char* year;
    uint year_size;

    /* Content Type Info */
    char* content_type;
    uint content_type_size;

    /* Comments Info */
    char* comments;
    uint comment_size;

} TagInfo;

/* Printing MP3 Tag details Function Prototype */

/* Check Operation type */
OperationType check_operation (char* argv[]);

/* Perform Validation of the Arguments passed and store in the Structure */
Status read_and_validate_mp3_file (char* argv[], TagInfo* mp3tagInfo);

/* Perform the Viewing Tag details Operation */
Status view_tag (char* argv[], TagInfo* mp3tagInfo);

/* Common function to display the various Frame contents of the MP3 file */
Status get_and_display_data (const char* str_frame, const char* str_Id, char* frame_Id, uint* tag_size, char* tag, FILE* fptr);

#endif
